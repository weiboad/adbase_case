#include "Wander.hpp"
#include "../Wander/BinarySearch.hpp"

namespace adserver {
namespace http {
// {{{ Wander::Wander()

Wander::Wander(AdServerContext* context) :
	HttpInterface(context) {
}

// }}}
// {{{ void Wander::registerLocation()

void Wander::registerLocation(adbase::http::Server* http) {
	ADSERVER_HTTP_ADD_API(http, Wander, index);
	ADSERVER_HTTP_ADD_API(http, Wander, location)
}

// }}}
// {{{ void Wander::index()

void Wander::index(adbase::http::Request* request, adbase::http::Response* response, void*) {
	(void)request;
	responseJson(response, "{\"msg\": \"hello adinf\"}", 0, "");
}

// }}}

// {{{ void Wander::index()

void Wander::location(adbase::http::Request* request, adbase::http::Response* response, void*) {
	(void)request;

	//响应头
	response->addHeader("Content-Type", "text/html; charset=UTF-8");
	response->setHeader("Connection", "keep-alive");

	//rapidjson 参数
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
	rapidjson::Value result(rapidjson::kObjectType);
	rapidjson::Value dataObj(rapidjson::kArrayType);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	rapidjson::Value code;
	rapidjson::Value msg;
	rapidjson::Value tmpStr;

	std::string ipsStr = request->getQuery("ips");

	if(ipsStr.length() == 0) {
		result.AddMember("code", code.SetInt(-1), allocator);
		result.AddMember("msg", msg.SetString("params is missing, have a check"), allocator);
	} else {
		result.AddMember("code", code.SetInt(0), allocator);
		result.AddMember("msg", msg.SetString("success"), allocator);


		int count = 0;
		ipsStr = adbase::replace("，", ",", ipsStr, count);
	    std::vector<std::string> ips = adbase::explode(ipsStr, ',', true);
		count = 0;
		for(auto &ip:ips) {
			rapidjson::Value location(rapidjson::kObjectType);
			try {
				PtrIPZoneZipCode position = _context->bs->FindZipCode(ip);
				tmpStr.SetString(ip.c_str(), ip.size(), allocator);
				location.AddMember("ip", tmpStr, allocator);
				tmpStr.SetString(position->zone.c_str(), position->zone.size(), allocator);
				location.AddMember("location", tmpStr, allocator);
				tmpStr.SetString(position->zipCode.c_str(), position->zipCode.size(), allocator);
				location.AddMember("zipcode", tmpStr, allocator);
				location.AddMember("result", "search success", allocator);
			} catch(std::string e) {
				tmpStr.SetString(ip.c_str(), ip.size(), allocator);
				location.AddMember("ip", tmpStr, allocator);
				location.AddMember("location", "", allocator);
				location.AddMember("zipcode", "", allocator);
				tmpStr.SetString(e.c_str(), e.size(), allocator);
				location.AddMember("result", tmpStr, allocator);
			}

			// if(position == nullptr) {
			// 	tmpStr.SetString(ip.c_str(), ip.size(), allocator);
			// 	location.AddMember("ip", tmpStr, allocator);
			// 	location.AddMember("location", "", allocator);
			// 	location.AddMember("zipcode", "", allocator);
			// } else {
			// 	tmpStr.SetString(ip.c_str(), ip.size(), allocator);
			// 	location.AddMember("ip", tmpStr, allocator);
			// 	tmpStr.SetString(position->zone.c_str(), position->zone.size(), allocator);
			// 	location.AddMember("location", tmpStr, allocator);
			// 	tmpStr.SetString(position->zipCode.c_str(), position->zipCode.size(), allocator);
			// 	location.AddMember("zipcode", tmpStr, allocator);
			// }
			dataObj.PushBack(location, allocator);
			count++;
			if(count > 20) {
				break;
			}
		}
	}

	result.AddMember("data", dataObj, allocator);
	result.Accept(writer);

	response->setContent(buffer.GetString());
	response->sendReply();

}

// }}}



}
}
