#ifndef WANDERINGIPS_HTTP_WANDER_HPP_
#define WANDERINGIPS_HTTP_WANDER_HPP_

#include "HttpInterface.hpp"
#include "Json.hpp"


namespace adserver {
namespace http {
class Wander : HttpInterface {
public:
	Wander(AdServerContext* context);
	void registerLocation(adbase::http::Server* http);
	void index(adbase::http::Request* request, adbase::http::Response* response, void*);
	void location(adbase::http::Request* request, adbase::http::Response* response, void*);
};
}
}

#endif
