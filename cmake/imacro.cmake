###############################################################################
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

MACRO(I_ADD_EXECUTABLE args)

SET(EXECUTABLE_NAME ${ARGV0})
ADD_EXECUTABLE(${EXECUTABLE_NAME} ${ARGN})
SOURCE_GROUP(source FILES ${ARGN})
SET_PROPERTY(TARGET ${EXECUTABLE_NAME} PROPERTY FOLDER ${PROJECT_FOLDER})

ENDMACRO(I_ADD_EXECUTABLE)


MACRO(I_ADD_DIRECTX_EXECUTABLE args)

SET(EXECUTABLE_NAME ${ARGV0})
ADD_EXECUTABLE(${EXECUTABLE_NAME} ${ARGN})
SOURCE_GROUP(source FILES ${ARGN})
SET_PROPERTY(TARGET ${EXECUTABLE_NAME} PROPERTY FOLDER ${PROJECT_FOLDER})
IF(MSVC)
    SET_PROPERTY(TARGET ${EXECUTABLE_NAME} PROPERTY LINK_FLAGS "/SUBSYSTEM:WINDOWS")
ENDIF(MSVC)

ENDMACRO(I_ADD_DIRECTX_EXECUTABLE)


MACRO(I_ADD_LIBRARY args)

SET(LIBRARY_NAME ${ARGV0})
ADD_LIBRARY(${LIBRARY_NAME} ${ARGN})
SOURCE_GROUP(source FILES ${ARGN})
SET_PROPERTY(TARGET ${LIBRARY_NAME} PROPERTY FOLDER ${PROJECT_FOLDER})

ENDMACRO(I_ADD_LIBRARY)
