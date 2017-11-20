PROJECT ( lua )

IF( NOT WIN32 )
    message( FATAL_ERROR "Written for windows only" )
ENDIF()

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# remove warnings
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS )

SET (HDR_LUBLUA
src/lapi.h src/lauxlib.h src/lcode.h src/lctype.h
src/ldebug.h src/ldo.h src/lfunc.h
src/lgc.h src/llex.h src/llimits.h src/lmem.h
src/lobject.h src/lopcodes.h src/lparser.h
src/lprefix.h
src/lstate.h src/lstring.h src/ltable.h src/ltm.h
src/lua.h src/luaconf.h src/lualib.h
src/lundump.h src/lvm.h src/lzio.h
)

#SET_SOURCE_FILES_PROPERTIES(${HDR_LUBLUA} PROPERTIES HEADER_FILE_ONLY TRUE)
#include_directories(src)

# Build Libraries
SET (SRC_LIBLUA
src/lapi.c src/lauxlib.c src/lbaselib.c src/lbitlib.c src/lcode.c src/lcorolib.c
src/lctype.c src/ldblib.c src/ldebug.c src/ldo.c src/ldump.c src/lfunc.c src/lgc.c
src/linit.c src/liolib.c src/llex.c src/lmathlib.c src/lmem.c src/loadlib.c src/lobject.c
src/lopcodes.c src/loslib.c src/lparser.c src/lstate.c src/lstring.c src/lstrlib.c
src/ltable.c src/ltablib.c src/ltm.c src/lundump.c
src/lutf8lib.c
src/lvm.c src/lzio.c
)

SET ( SRC_LUA src/lua.c )
SET ( SRC_LUAC src/luac.c )

# compile with C++ compiler
set_source_files_properties(${SRC_LIBLUA} ${SRC_LUA} ${SRC_LUAC} PROPERTIES LANGUAGE CXX)

# append headers to sources to make them show up in MSVC GUI
LIST(APPEND SRC_LIBLUA ${HDR_LIBLUA})

# Library
ADD_LIBRARY ( lualib ${SRC_LIBLUA} )

# DLL
ADD_LIBRARY ( lua5.3 SHARED ${SRC_LIBLUA} )
SET_TARGET_PROPERTIES (lua5.3 PROPERTIES DEFINE_SYMBOL  "LUA_BUILD_AS_DLL" )

# exe
ADD_EXECUTABLE ( lua ${SRC_LUA} )
ADD_EXECUTABLE ( luac ${SRC_LUAC} )

# lua uses a DLL; luac uses a library
TARGET_LINK_LIBRARIES ( lua lua5.3 )
TARGET_LINK_LIBRARIES ( luac lualib )
