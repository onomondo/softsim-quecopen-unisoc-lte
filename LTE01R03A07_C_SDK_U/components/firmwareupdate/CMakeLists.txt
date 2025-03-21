# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

configure_file(include/fupdate_config.h.in ${out_inc_dir}/fupdate_config.h)

set(target fupdate_security_app)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC
    src/fupdate_security_pack.c
)

set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
set_target_properties(${target} PROPERTIES OUTPUT_NAME fupdate_security_app)
target_include_directories(${target} PUBLIC include)
target_include_targets(${target} PRIVATE kernel fs sffs calclib driver hal)

relative_glob(srcs src/*.[ch] include/fupdate_security.h)
beautify_c_code(${target} ${srcs})

set(target fupdate_security_boot)

add_library(${target} STATIC
    src/fupdate_security_pack.c 
)

set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
set_target_properties(${target} PROPERTIES OUTPUT_NAME fupdate_security_boot)
target_include_directories(${target} PUBLIC include)
target_include_targets(${target} PRIVATE bootloader kernel fs sffs calclib driver hal)

include(core.cmake)
