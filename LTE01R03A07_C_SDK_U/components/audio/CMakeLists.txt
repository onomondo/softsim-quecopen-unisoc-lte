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

configure_file(include/audio_config.h.in ${out_inc_dir}/audio_config.h)

set(target audio)
include(core.cmake)

set(target audio_media)
add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_AUDIO)
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE ${out_rpc_dir})
target_include_targets(${target} PRIVATE kernel driver hal fs cfw nvm)

target_sources(${target} PRIVATE
    src/audio_decoder.c
    src/audio_encoder.c
)