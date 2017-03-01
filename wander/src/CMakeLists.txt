INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(./thridpart)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -W -static-libstdc++")

configure_file (
	"${PROJECT_SOURCE_DIR}/src/Version.hpp.in"
	"${PROJECT_SOURCE_DIR}/src/Version.hpp"
)

configure_file (
	"${PROJECT_SOURCE_DIR}/rpm/wander.spec.in"
	"${PROJECT_SOURCE_DIR}/rpm/wander.spec"
)

configure_file (
	"${PROJECT_SOURCE_DIR}/rpm/build_rpm.in"
	"${PROJECT_SOURCE_DIR}/rpm/build_rpm"
	@ONLY
)

SET(WANDER_SRC wander.cpp
	BootStrap.cpp
	App.cpp
	AdServer.cpp
	Http.cpp
	Http/HttpInterface.cpp
	Http/Server.cpp
	Http/Wander.cpp
	McProcessor.cpp
	HeadProcessor.cpp
	Timer.cpp
	Wander/IPOperator.cpp
	Wander/BinarySearch.cpp
	Common/Common.cpp
)
ADD_EXECUTABLE(wander ${WANDER_SRC})
TARGET_LINK_LIBRARIES(wander libadbase.a
	libevent.a
	pthread
	rt
	)
INSTALL(TARGETS wander RUNTIME DESTINATION adinf/wander/bin)
