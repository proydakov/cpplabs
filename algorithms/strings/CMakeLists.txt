#==============================================================================
# Copyright (c) 2011-2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    sub.h
    split.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(algorithms_strings ${HEADER_FILES} ${SOURCE_FILES})
