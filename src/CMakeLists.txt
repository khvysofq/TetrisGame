cmake_minimum_required(VERSION 2.8)
#########################################################################

# Basic environment setting

if(WIN32)
	SET(BUILD_PROJECT_NAME "libtetris")
elseif(UNIX)
	SET(BUILD_PROJECT_NAME "tetris")
endif()

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
	${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.c
	${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetris.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetris.h
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetromino.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetromino.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/server/tetris_web_server.h
	${CMAKE_CURRENT_SOURCE_DIR}/server/tetris_web_server.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/database/sqlite3.h
	${CMAKE_CURRENT_SOURCE_DIR}/database/sqlite3.c
	${CMAKE_CURRENT_SOURCE_DIR}/database/recordmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/database/recordmanager.cpp
	)
	
# ------------------------------------------------------------------------------
SOURCE_GROUP(mongoose FILES
	${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.c
	${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.h
)

SOURCE_GROUP(database FILES
	${CMAKE_CURRENT_SOURCE_DIR}/database/sqlite3.h
	${CMAKE_CURRENT_SOURCE_DIR}/database/sqlite3.c
	${CMAKE_CURRENT_SOURCE_DIR}/database/recordmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/database/recordmanager.cpp
)

SOURCE_GROUP(tetris FILES
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetris.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetris.h
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetromino.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tetris/tetromino.h
)

SOURCE_GROUP(server FILES
	${CMAKE_CURRENT_SOURCE_DIR}/server/tetris_web_server.h
	${CMAKE_CURRENT_SOURCE_DIR}/server/tetris_web_server.cpp
)

#########################################################################
#Step 4 : Add PROJECT define 
MESSAGE(STATUS "Step 4 : Add code source")

#########################################################################
#Step 5 : Add executable or library target
MESSAGE(STATUS "Step 5 : Add executable or library target")
#ADD_executable(${BUILD_PROJECT_NAME} ${SRC_LIST})
ADD_LIBRARY(${BUILD_PROJECT_NAME} STATIC ${SRC_LIST})
set_property(TARGET ${BUILD_PROJECT_NAME} PROPERTY FOLDER ${PROJECT_SET_NAME_LIB})
#########################################################################
#Step 6 : link with other library
MESSAGE(STATUS "Step 6 : link with other library")

TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
	${VZPROJECT_LINK_LIB}
)