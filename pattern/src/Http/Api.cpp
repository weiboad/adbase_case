#include "Api.hpp"

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
}

// }}}
// {{{ void Api::index()

void Api::index(adbase::http::Request* request, adbase::http::Response* response, void*) {
	(void)request;
	responseJson(response, "{\"msg\": \"hello adinf\"}", 0, "");
}

// }}}
}
}