CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

FIND_PACKAGE(OpenGL)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(SDL)
IF (SDL_FOUND)
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
ENDIF (SDL_FOUND)

INCLUDE_DIRECTORIES(. collision math physics system)

ADD_SUBDIRECTORY(collision)
ADD_SUBDIRECTORY(math)
ADD_SUBDIRECTORY(physics)
ADD_SUBDIRECTORY(system)

ADD_EXECUTABLE(TheBox tbMain.c tbCommon.h tbConfig.h tbMemory.c tbMemory.h)
TARGET_LINK_LIBRARIES(TheBox collision math system physics m ${SDL_LIBRARY} ${OPENGL_LIBRARIES})

INCLUDE(CheckIPOSupported)
CHECK_IPO_SUPPORTED(RESULT IPO_SUPPORTED)
IF (IPO_SUPPORTED)
  SET_PROPERTY(TARGET TheBox PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
ENDIF()
