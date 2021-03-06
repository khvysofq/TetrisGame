#F:\code\osc\aliyun_opensearch\aliyun_opensearch\builds\vs2013>cmake ../.. -DCMAKE_BUILD_TYPE=Release -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
#  
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(tetris_game)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

SET(PROJECT_SET_NAME_TEST "test")
SET(PROJECT_SET_NAME_LIB "lib")
SET(PROJECT_SET_NAME_THIRD_PART "third_part")
#SET(PROJECT_MAIN_SRC_LIST "")
##########################################################
## Step 1 
SET(LIB_CURL ${PROJECT_SOURCE_DIR}/third_part/curl)
SET(JSON_CPP ${PROJECT_SOURCE_DIR}/third_part/jsoncpp)
SET(GLOG ${PROJECT_SOURCE_DIR}/third_part/glog)
# SET(OSS_C_SDK ${PROJECT_SOURCE_DIR}/third_part/oss_c_sdk_windows/oss_c_sdk_lib)

option(USE_GLOG "Set to ON to USE Glog executable." ON)

##1. INCLUDE_DIRECTORES
MESSAGE(STATUS "Step 1 :  Add include directores")
IF(USE_GLOG)
	SET(TETRIS_SERVICE_INCLUDE_DIR
		${LIB_CURL}/include
		${PROJECT_BINARY_DIR}/third_part/curl/include/curl
		${PROJECT_BINARY_DIR}/third_part/glog
		${GLOG}/src
		${JSON_CPP}/include
		${PROJECT_SOURCE_DIR}/src
	)
ELSE()
	SET(TETRIS_SERVICE_INCLUDE_DIR
		${LIB_CURL}/include
		${PROJECT_BINARY_DIR}/third_part/curl/include/curl
		${JSON_CPP}/include
		${PROJECT_SOURCE_DIR}/src
	)
ENDIF()

IF(USE_GLOG)
	IF(WIN32)
		SET(GLOG_LIB_NAME glog.lib)
	ELSE()
		SET(GLOG_LIB_NAME glog.a)
	ENDIF()
ELSE()
		SET(GLOG_LIB_NAME "")
ENDIF()

IF(WIN32)
	SET(LIBCURL_LIB_NAME libcurl.lib)
ELSEIF(APPLE)
	SET(LIBCURL_LIB_NAME libcurl.dylib)
ELSE()
	SET(LIBCURL_LIB_NAME libcurl.so)
ENDIF()

##2 LIBRARY PATH
MESSAGE(STATUS "Step 2 :  Add library path and public link library")
IF(WIN32)
	MESSAGE(STATUS "       WINDOWS")
	SET(TETRIS_SERVICE_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/win
		)
	SET(TETRIS_SERVICE_LINK_LIB
		libcurl_imp.lib
		jsoncpp.lib
		winmm.lib
		ws2_32.lib
		crypt32.lib
		${GLOG_LIB_NAME}
		)
ELSE()
	MESSAGE(STATUS "       LINUX")
	SET(TETRIS_SERVICE_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/unix
		)
	SET(TETRIS_SERVICE_LINK_LIB
		libjsoncpp.a
		${LIBCURL_LIB_NAME}
		${GLOG_LIB_NAME}
		pthread
		-ldl
		)
ENDIF()

##3 add defines
MESSAGE(STATUS "Step 3 :  Add More defines")
 if(WIN32)
	 ADD_DEFINITIONS(
		 -D_CRT_SECURE_NO_WARNINGS
		-DGLOG_NO_ABBREVIATED_SEVERITIES
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
	 )
 else()
	 ADD_DEFINITIONS(
		-std=c++11
	 )
 endif()

 
IF(USE_GLOG)
	 ADD_DEFINITIONS(
		-DGOOGLE_GLOG_LIBRARY
		-DGOOGLE_GLOG_DLL_DECL=
	 )
ENDIF()
 
INCLUDE_DIRECTORIES(${TETRIS_SERVICE_INCLUDE_DIR})
LINK_DIRECTORIES(${TETRIS_SERVICE_LIBRARY_DIR})
#####################################################################
# Step 3 :Set visual studio runtime type
if(WIN32)
	#set(CompilerFlags
	#		CMAKE_CXX_FLAGS
	#		CMAKE_CXX_FLAGS_DEBUG
	#		CMAKE_CXX_FLAGS_RELEASE
	#		CMAKE_C_FLAGS
	#		CMAKE_C_FLAGS_DEBUG
	#		CMAKE_C_FLAGS_RELEASE
	#		)
	#foreach(CompilerFlag ${CompilerFlags})
	#  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	#  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
	#  #string(REPLACE "/EDITANDCONTINUE" "/SAFESEH" ${CompilerFlag} "${${CompilerFlag}}")
	#endforeach()
	
################################################################
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
		 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
else()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")

	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
ENDIF()
##########################################################
## Setp 3
ADD_SUBDIRECTORY(third_part/curl)
ADD_SUBDIRECTORY(third_part/jsoncpp)

IF(USE_GLOG)
	ADD_SUBDIRECTORY(third_part/glog)
ENDIF()

ADD_SUBDIRECTORY(src)

# TEST
# ADD_SUBDIRECTORY(test/web_server)
ADD_SUBDIRECTORY(test/tetris_server)
