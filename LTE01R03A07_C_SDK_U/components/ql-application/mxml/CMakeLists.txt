# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#
set(target ql_app_mxml) 
add_library(${target} STATIC) 
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir}) 
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC) 
target_include_directories(${target} PUBLIC inc)
#target_link_libraries(${target} PRIVATE kernel driver ql_api_common fs fsmount ql_urc ql_file ql_utils)

target_sources(${target} PRIVATE 
    mxml_demo.c
	src/mxml-attr.c 
	src/mxml-entity.c 
	src/mxml-file.c
	src/mxml-get.c
	src/mxml-index.c
	src/mxml-node.c
	src/mxml-private.c
	src/mxml-search.c
	src/mxml-set.c
	src/mxml-string.c
	 )
	 
relative_glob(srcs include/*.h src/*.h src/*.c) 
beautify_c_code(${target} ${srcs}) 
