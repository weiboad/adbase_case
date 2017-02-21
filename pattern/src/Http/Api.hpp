#ifndef PATTERN_HTTP_API_HPP_
#define PATTERN_HTTP_API_HPP_

#include "HttpInterface.hpp"

namespace adserver {
namespace http {
class Api : HttpInterface {
public:
	Api(AdServerContext* context);
	void registerLocation(adbase::http::Server* http);
	void index(adbase::http::Request* request, adbase::http::Response* response, void*);
	void pattern(adbase::http::Request* request, adbase::http::Response* response, void*);
};
}
}

#endif
