#include "McProcessor.hpp"
#include "App.hpp"
#include <set>
#include "app/Json.hpp"

// {{{ McProcessor::McProcessor()

McProcessor::McProcessor(AdServerContext* context) :
	_context(context) {
	_configure = _context->config;
}

// }}}
// {{{ McProcessor::~McProcessor()

McProcessor::~McProcessor() {
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::add()	

adbase::mc::ProtocolBinaryResponseStatus McProcessor::add(const void* key,
            uint16_t keylen,
            const void *data,
            uint32_t datalen,
            uint32_t flags,
            uint32_t exptime,
            uint64_t* cas) {
	(void)flags;
	(void)exptime;
	(void)cas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	adbase::Buffer bufferBody;
	bufferBody.append(static_cast<const char*>(data), static_cast<size_t>(datalen));
	LOG_INFO << "Mc ADD key:" << keyData;
	LOG_INFO << "Mc ADD data size:" << bufferBody.readableBytes();
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::append()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::append(const void* key,
            uint16_t keylen,
            const void *data,
            uint32_t datalen,
            uint64_t cas,
            uint64_t* resultCas) {
	(void)cas;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	adbase::Buffer bufferBody;
	bufferBody.append(static_cast<const char*>(data), static_cast<size_t>(datalen));
	LOG_INFO << "Mc APPEND key:" << keyData;
	LOG_INFO << "Mc APPEND data size:" << bufferBody.readableBytes();
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::decrement()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::decrement(const void* key,
			uint16_t keylen,
			uint64_t delta,
			uint64_t initial,
			uint32_t expiration,
			uint64_t* result,
			uint64_t* resultCas) {
	(void)delta;
	(void)initial;
	(void)expiration;
	(void)result;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	LOG_INFO << "Mc DECREMENT key:" << keyData;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::deleteOp()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::deleteOp(const void* key,
			uint16_t keylen,
			uint64_t cas) {
	(void)cas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	LOG_INFO << "Mc DELETE key:" << keyData;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::flush()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::flush(uint32_t when) {
	LOG_INFO << "Mc FLUSH when:" << when;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::get()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::get(const void* key,
			uint16_t keylen,
			adbase::Buffer *data) {
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
    rapidjson::Document documentData;

    // base64 decode
    char decodeData[static_cast<size_t>(keylen)];
    memset(decodeData, 0, static_cast<size_t>(keylen));
    int length = 0;
    adbase::base64Decode(decodeData, &length, keyData);

    documentData.Parse(decodeData);
    if (documentData.HasParseError()) {
        return adbase::mc::PROTOCOL_BINARY_RESPONSE_KEY_ENOENT;
    }

    if (!documentData.IsObject()) {
        std::string error = "pattern format is invalid.";
        LOG_ERROR << error;
        return adbase::mc::PROTOCOL_BINARY_RESPONSE_KEY_ENOENT;
    }

    if (!documentData.HasMember("dict") || !documentData.HasMember("contents")) {
        std::string error = "pattern format is invalid.";
        LOG_ERROR << error;
        return adbase::mc::PROTOCOL_BINARY_RESPONSE_KEY_ENOENT;
    }

    rapidjson::Value& contents = documentData["contents"];
    if (!contents.IsArray() || !static_cast<uint32_t>(contents.Size())) {
        std::string error = " contents not is array or empty.";
        LOG_ERROR << error;
        return adbase::mc::PROTOCOL_BINARY_RESPONSE_KEY_ENOENT;
    }

    std::string patternType = documentData["dict"].GetString();

    rapidjson::Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value result(rapidjson::kArrayType);
    for (int i = 0; i < static_cast<int>(contents.Size()); i++) {
        if (!contents[i].IsString()) {
            continue;
        }

        rapidjson::Value item(rapidjson::kObjectType);
        std::set<std::string> patternResult;
        std::string text = std::string(contents[i].GetString());
        LOG_INFO << "Pattern type: `" << patternType << "` text: " << text;
        int hit = _context->patternManager->search(patternType, text, patternResult);

        rapidjson::Value patterns(rapidjson::kArrayType);
        for (auto &t : patternResult) {
            rapidjson::Value messageValue;
            messageValue.SetString(t.c_str(), static_cast<unsigned int>(t.size()), allocator);
            patterns.PushBack(messageValue, allocator);
        }
        item.AddMember("hit", hit, allocator);
        item.AddMember("patterns", patterns, allocator);
        result.PushBack(item, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    result.Accept(writer);

	data->append(static_cast<const char*>(buffer.GetString()), static_cast<size_t>(buffer.GetSize()));
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::increment()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::increment(const void* key,
			uint16_t keylen,
			uint64_t delta,
			uint64_t initial,
			uint32_t expiration,
			uint64_t* result,
			uint64_t* resultCas) {
	(void)delta;
	(void)initial;
	(void)expiration;
	(void)result;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	LOG_INFO << "Mc INCREMENT key:" << keyData;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::prepend()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::prepend(const void* key,
			uint16_t keylen,
			const void *data,
			uint32_t datalen,
			uint64_t cas,
			uint64_t* resultCas) {
	(void)cas;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	adbase::Buffer bufferBody;
	bufferBody.append(static_cast<const char*>(data), static_cast<size_t>(datalen));
	LOG_INFO << "Mc PREPEND key:" << keyData;
	LOG_INFO << "Mc PREPEND data size:" << bufferBody.readableBytes();
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::noop()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::noop() {
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::quit()
	
adbase::mc::ProtocolBinaryResponseStatus McProcessor::quit() {
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::replace()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::replace(const void* key,
			uint16_t keylen,
			const void *data,
			uint32_t datalen,
			uint32_t flags,
			uint32_t exptime,
			uint64_t cas,
			uint64_t* resultCas) {
	(void)flags;
	(void)exptime;
	(void)cas;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	adbase::Buffer bufferBody;
	bufferBody.append(static_cast<const char*>(data), static_cast<size_t>(datalen));
	LOG_INFO << "Mc REPLACE key:" << keyData;
	LOG_INFO << "Mc REPLACE data size:" << bufferBody.readableBytes();
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::set()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::set(const void* key,
			uint16_t keylen,
			const void *data,
			uint32_t datalen,
			uint32_t flags,
			uint32_t exptime,
			uint64_t cas,
			uint64_t* resultCas) {
	(void)flags;
	(void)exptime;
	(void)cas;
	(void)resultCas;
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	adbase::Buffer bufferBody;
	bufferBody.append(static_cast<const char*>(data), static_cast<size_t>(datalen));
	LOG_INFO << "Mc SET key:" << keyData;
	LOG_INFO << "Mc SET data size:" << bufferBody.readableBytes();
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::stat()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::stat(const void* key,
			uint16_t keylen,
			adbase::Buffer *data) {
	std::string keyData(static_cast<const char*>(key), static_cast<size_t>(keylen));
	data->append(static_cast<const char*>(key), static_cast<size_t>(keylen));
	LOG_INFO << "Mc STAT key:" << keyData;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::version()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::version(adbase::Buffer *data) {
    data->append("0.1.0");
    return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::verbosity()

adbase::mc::ProtocolBinaryResponseStatus McProcessor::verbosity(uint32_t verbose) {
	LOG_INFO << "Mc FLUSH verbose:" << verbose;
	return adbase::mc::PROTOCOL_BINARY_RESPONSE_SUCCESS;
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::preExecute()

void McProcessor::preExecute() {
	LOG_TRACE << "CMD exec pre.";
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::postExecute()

void McProcessor::postExecute() {
	LOG_TRACE << "CMD exec post.";
}

// }}}
// {{{ adbase::mc::ProtocolBinaryResponseStatus McProcessor::unknownExecute()

void McProcessor::unknownExecute() {
	LOG_TRACE << "CMD exec unknown.";
}

// }}}
