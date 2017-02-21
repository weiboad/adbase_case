#include "Api.hpp"
#include "app/Json.hpp"
#include <set>
#include "app/PatternManager.hpp"

namespace adserver {
namespace http {
// {{{ Api::Api()

Api::Api(AdServerContext* context) :
	HttpInterface(context) {
}

// }}}
// {{{ void Api::registerLocation()

void Api::registerLocation(adbase::http::Server* http) {
	ADSERVER_HTTP_ADD_API(http, Api, index)
	ADSERVER_HTTP_ADD_API(http, Api, pattern)
}

// }}}
// {{{ void Api::index()

void Api::index(adbase::http::Request* request, adbase::http::Response* response, void*) {
	(void)request;
	responseJson(response, "{\"msg\": \"hello adinf\"}", 0, "");
}

// }}}
// {{{ void Api::pattern()

void Api::pattern(adbase::http::Request* request, adbase::http::Response* response, void*) {
    std::string data = request->getPostData();
    rapidjson::Document documentData;
    documentData.Parse(data.c_str());
    if (documentData.HasParseError()) {
        LOG_ERROR << rapidjson::GetParseError_En(documentData.GetParseError());
	    responseJson(response, "", 10000, rapidjson::GetParseError_En(documentData.GetParseError()), true);
        return;
    }

    if (!documentData.IsObject()) {
        std::string error = "pattern format is invalid.";
        LOG_ERROR << error;
	    responseJson(response, "", 10000, error, true);
        return;
    }

    if (!documentData.HasMember("dict") || !documentData.HasMember("contents")) {
        std::string error = "pattern format is invalid.";
        LOG_ERROR << error;
	    responseJson(response, "", 10000, error, true);
        return;
    }

    rapidjson::Value& contents = documentData["contents"];
    if (!contents.IsArray() || !static_cast<uint32_t>(contents.Size())) {
        std::string error = " contents not is array or empty.";
        LOG_ERROR << error;
	    responseJson(response, "", 10000, error, true);
        return;
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

	responseJson(response, buffer.GetString(), 0, "");
}

// }}}
}
}
