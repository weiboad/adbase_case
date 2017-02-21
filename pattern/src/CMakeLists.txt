INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(./thirdparty)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -W -static-libstdc++")  

configure_file (
	"${PROJECT_SOURCE_DIR}/src/Version.hpp.in"	
	"${PROJECT_SOURCE_DIR}/src/Version.hpp"	
)

configure_file (
	"${PROJECT_SOURCE_DIR}/rpm/pattern.spec.in"	
	"${PROJECT_SOURCE_DIR}/rpm/pattern.spec"	
)

configure_file (
	"${PROJECT_SOURCE_DIR}/rpm/build_rpm.in"	
	"${PROJECT_SOURCE_DIR}/rpm/build_rpm"	
	@ONLY
)

SET(PATTERN_SRC pattern.cpp
	BootStrap.cpp	
	App.cpp	
	AdServer.cpp
	Http.cpp
	Http/HttpInterface.cpp
	Http/Server.cpp
	Http/Api.cpp
	McProcessor.cpp
	HeadProcessor.cpp
	Timer.cpp
    app/WuManber.cpp
    app/PatternManager.cpp
)
ADD_EXECUTABLE(pattern ${PATTERN_SRC})
TARGET_LINK_LIBRARIES(pattern libadbase.a 
	libevent.a
	pthread
	rt
	)
INSTALL(TARGETS pattern RUNTIME DESTINATION adinf/pattern/bin)
