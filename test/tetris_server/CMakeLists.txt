cmake_minimum_required(VERSION 2.8)
#########################################################################

# Basic environment setting
SET(BUILD_PROJECT_NAME "tetris_server")

#########################################################################
#INCLUDE_DIRECTORIES(${LIBVZNET_INCLUDE_DIR})
MESSAGE(STATUS "Print the include directores")
get_property(inc_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(inc_dir ${inc_dirs})
  message(STATUS "    include directory='${inc_dir}'")
endforeach()

#########################################################################
#Step 2 : Add library directories
MESSAGE(STATUS "Step 2 : Add library directories")
#LINK_DIRECTORIES(${LIBVZNET_LIBRARY_DIR})
MESSAGE(STATUS "Print the link directores")
get_property(link_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(link_dir ${link_dirs})
  message(STATUS "    link directory='${link_dir}'")
endforeach()

#########################################################################
#Step 3 : Add code source
SET(SRC_LIST 
	${CMAKE_CURRENT_SOURCE_DIR}/tetris_server.cpp
	)

SOURCE_GROUP(${BUILD_PROJECT_NAME} FILES
	${CMAKE_CURRENT_SOURCE_DIR}/tetris_server.cpp
	)
#########################################################################
#Step 4 : Add PROJECT define 
MESSAGE(STATUS "Step 4 : Add code source")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#########################################################################
#Step 5 : Add executable or library target
MESSAGE(STATUS "Step 5 : Add executable or library target")
ADD_executable(${BUILD_PROJECT_NAME} ${SRC_LIST})
set_property(TARGET ${BUILD_PROJECT_NAME} PROPERTY FOLDER ${PROJECT_SET_NAME_TEST})
IF(UNIX)
    add_custom_command(TARGET ${BUILD_PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_STRIP} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_PROJECT_NAME}
        COMMAND cp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_PROJECT_NAME} ${FTP_DIR}/${BUILD_PROJECT_NAME})
ENDIF()
#########################################################################
#Step 6 : link with other library
MESSAGE(STATUS "Step 6 : link with other library")
if(WIN32)
	TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
		${TETRIS_SERVICE_LINK_LIB}
		Winmm.lib
        libtetris.lib
	)
else()
	TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
        libtetris.a
		${TETRIS_SERVICE_LINK_LIB}
	)
endif()