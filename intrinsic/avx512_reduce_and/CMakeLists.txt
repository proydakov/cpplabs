#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_avx512_reduce_and ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_avx512_reduce_and PRIVATE -mavx512f)
endif()
