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

function(print arg)
    message(STATUS "${arg}: ${${arg}}")
endfunction()

macro(install_header src dst)
    # get_filename_component(dst_dir ${dst} DIRECTORY)
    # file(RELATIVE_PATH src_rel ${dst_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    # file(GENERATE OUTPUT ${dst} CONTENT "#include \"${src_rel}\"\n")
    configure_file(${src} ${dst} COPYONLY)
endmacro()

macro(install_headers) # <sources> OUTPUT <dstdir>
    cmake_parse_arguments(MY "" "" "OUTPUT" ${ARGN})
    foreach(src ${MY_UNPARSED_ARGUMENTS})
        get_filename_component(src_name ${src} NAME)
        configure_file(${src} ${MY_OUTPUT}/${src_name} COPYONLY)
    endforeach()
endmacro()

# Helper to set source with conditions
macro(target_sources_if) # <conditon> THEN <command>
    cmake_parse_arguments(MY "" "" "THEN" ${ARGN})
    if(${MY_UNPARSED_ARGUMENTS})
        target_sources(${MY_THEN})
    endif()
endmacro()

# Helper to link libraries with conditions
macro(target_link_libraries_if)  # <conditon> THEN <command>
    cmake_parse_arguments(MY "" "" "THEN" ${ARGN})
    if(${MY_UNPARSED_ARGUMENTS})
        target_link_libraries(${MY_THEN})
    endif()
endmacro()

# Helper to link libraries with conditions
macro(target_include_targets_if)  # <conditon> THEN <command>
    cmake_parse_arguments(MY "" "" "THEN" ${ARGN})
    if(${MY_UNPARSED_ARGUMENTS})
        target_include_targets(${MY_THEN})
    endif()
endmacro()

# Helper to set variable with conditions
macro(set_if var) # <condition> THEN <val_true> OTHERWISE <val_false>
    cmake_parse_arguments(MY "" "" "THEN;OTHERWISE" ${ARGN})
    if(${MY_UNPARSED_ARGUMENTS})
        if(MY_THEN)
            set(${var} ${MY_THEN})
        endif()
    else()
        if (MY_OTHERWISE)
            set(${var} ${MY_OTHERWISE})
        endif()
    endif()
endmacro()

function(add_subdirectory_if)
    cmake_parse_arguments(MY "" "" "THEN" ${ARGN})
    if(${MY_UNPARSED_ARGUMENTS})
        add_subdirectory(${MY_THEN})
    endif()
endfunction()

# Set a target to include all interface directories of depended targets.
# The arguments must be target.
function(target_include_targets target type)
    foreach(arg ${ARGN})
        target_include_directories(${target} ${type}
            $<TARGET_PROPERTY:${arg},INTERFACE_INCLUDE_DIRECTORIES>)
    endforeach()
endfunction()

# Include all interface directories of depended targets
# The arguments must be target.
function(include_targets)
    foreach(arg ${ARGN})
        include_directories($<TARGET_PROPERTY:${arg},INTERFACE_INCLUDE_DIRECTORIES>)
    endforeach()
endfunction()

# Set library files as application libraries
function(add_app_libraries)
    get_property(app_libraries GLOBAL PROPERTY app_libraries)
    set_property(GLOBAL PROPERTY app_libraries ${app_libraries} ${ARGN})
endfunction()
function(add_apps_libraries)
    get_property(app_libraries GLOBAL PROPERTY app_libraries)
    set_property(GLOBAL PROPERTY app_libraries ${app_libraries} ${ARGN})
endfunction()
function(add_components_libraries)
    get_property(app_libraries GLOBAL PROPERTY app_libraries)
    set_property(GLOBAL PROPERTY app_libraries ${app_libraries} ${ARGN})
endfunction()

macro(target_add_revision target)
    set(revision_file ${CMAKE_CURRENT_BINARY_DIR}/${target}_revision.c)
    set(revision_variable ${target}_build_revision)
    set(revision_value ${BUILD_TARGET}-${BUILD_RELEASE_TYPE}-${BUILD_AUTO_REVISION})
    configure_file(${SOURCE_TOP_DIR}/cmake/auto_revision.c.in ${revision_file})
    target_sources(${target} PRIVATE ${revision_file})
endmacro()

# Link targets with --whole-archive. PUBLIC/PRIVATE is required as parameter,
# but PRIVATE will be used forcedly.
function(target_link_whole_archive target signature)
    target_link_libraries(${target} PRIVATE -Wl,--whole-archive)
    foreach(arg ${ARGN})
        target_link_libraries(${target} PRIVATE ${arg})
    endforeach()
    target_link_libraries(${target} PRIVATE -Wl,--no-whole-archive)
endfunction()

# Link targets with --start-group. PUBLIC/PRIVATE is required as parameter,
# but PRIVATE will be used forcedly.
function(target_link_group target signature)
    target_link_libraries(${target} PRIVATE -Wl,--start-group)
    foreach(arg ${ARGN})
        target_link_libraries(${target} PRIVATE ${arg})
    endforeach()
    target_link_libraries(${target} PRIVATE -Wl,--end-group)
endfunction()

function(add_subdirectory_if_exist dir)
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
            add_subdirectory(${dir})
        endif()
    endif()
endfunction()

# don't compile soure codes with lto, even lto is enabled by default
function(source_files_no_lto)
    set_source_files_properties(${ARGN} PROPERTIES COMPILE_OPTIONS "-fno-lto")
endfunction()

# don't compile target with lto, even lto is enabled by default
function(target_no_lto target)
    target_compile_options(${target} PRIVATE "-fno-lto")
endfunction()

function(cpp_only target file)
    add_library(${target} OBJECT ${file})
    set_source_files_properties(${file} PROPERTIES LANGUAGE C)
    target_compile_options(${target} PRIVATE -E -P -x c)
    foreach(dep ${ARGN})
        target_include_directories(${target}
            PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_INCLUDE_DIRECTORIES>)
    endforeach()
    foreach(dep ${ARGN})
        target_compile_definitions(${target}
            PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_COMPILE_DEFINITIONS>)
    endforeach()
endfunction()

# Final link depends on generated linker script
function(target_depend_rom_ld ${target})
    if(CONFIG_SOC_8811)
        add_dependencies(${target}_ldscript nbrom_for_link)
        target_include_directories(${target}_ldscript PRIVATE ${out_hex_dir})
    endif()
endfunction()

# Generate asm file with incbin, with optional alignment.
function(target_incbin target binfile sym) # BALIGN <balign>
    cmake_parse_arguments(MY "" "" "BALIGN" ${ARGN})
    set(symbol_balign 4)
    if(${MY_BALIGN})
        set(symbol_balign ${MY_BALIGN})
    endif()

    get_filename_component(bin_path ${binfile} ABSOLUTE)
    get_filename_component(bin_file_name ${binfile} NAME)
    set(symbol_name ${sym})
    set(asmfile ${CMAKE_CURRENT_BINARY_DIR}/${bin_file_name}.S)
    configure_file(${SOURCE_TOP_DIR}/cmake/incbin.S.in ${asmfile})
    target_sources(${target} PRIVATE ${asmfile})
    set_source_files_properties(${asmfile} PROPERTIES OBJECT_DEPENDS ${binfile})
endfunction()

# Generate asm file with incbin, and provide size with symbol (rather than variable)
function(target_incbin_size target binfile sym symsize) # BALIGN <balign>
    cmake_parse_arguments(MY "" "" "BALIGN" ${ARGN})
    set(symbol_balign 4)
    if(${MY_BALIGN})
        set(symbol_balign ${MY_BALIGN})
    endif()

    get_filename_component(bin_path ${binfile} ABSOLUTE)
    get_filename_component(bin_file_name ${binfile} NAME)
    set(symbol_name ${sym})
    set(symbol_size_name ${symsize})
    set(asmfile ${CMAKE_CURRENT_BINARY_DIR}/${bin_file_name}.S)
    configure_file(${SOURCE_TOP_DIR}/cmake/incbin_size.S.in ${asmfile})
    target_sources(${target} PRIVATE ${asmfile})
    set_source_files_properties(${asmfile} PROPERTIES OBJECT_DEPENDS ${binfile})
endfunction()

function(add_uimage target ldscript) # <sources> OUTPUT_DIR <dir>
    cmake_parse_arguments(MY "WITH_FLASH2" "" "OUTPUT_DIR" ${ARGN})
    set(MY_SOURCES ${MY_UNPARSED_ARGUMENTS})
    if(NOT MY_OUTPUT_DIR)
        set(MY_OUTPUT_DIR ${out_hex_dir})
    endif()

    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${MY_OUTPUT_DIR}/${target}.map)
    set(target_img_file ${MY_OUTPUT_DIR}/${target}.img)
    set(target_flash2bin_file ${MY_OUTPUT_DIR}/${target}.flash2.bin)
    cpp_only(${gen_ldscript} ${ldscript} hal)

    add_executable(${target} ${MY_SOURCES})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections ${link_cref_option})
    target_depend_rom_ld(${target})

	#quectel update
    if(CONFIG_QUEC_PROJECT_FEATURE_IMAGE_TIMESTAMP)
		set(cfg_img_timestamp )
	else()
		set(cfg_img_timestamp --timestamp 0)
	endif()

    if(MY_WITH_FLASH2)
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${cmd_mkuimage} --name "${BUILD_REVISION}-${BUILD_AUTO_REVISION}" ${cfg_img_timestamp}#quectel update
				$<TARGET_FILE:${target}> ${target_img_file}
			COMMAND ${cmd_elf2bin} --start __flash2_start --end __flash2_end --allow-empty
				$<TARGET_FILE:${target}> ${target_flash2bin_file}
			BYPRODUCTS ${target_img_file} ${target_map_file} ${target_flash2bin_file}
		)
    else()
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${cmd_mkuimage} --name "${BUILD_REVISION}-${BUILD_AUTO_REVISION}" ${cfg_img_timestamp}#quectel update
				$<TARGET_FILE:${target}> ${target_img_file}
			BYPRODUCTS ${target_img_file} ${target_map_file}
		)
    endif()
endfunction()

function(add_simage target ldscript) # <sources> OUTPUT_DIR <dir>
    cmake_parse_arguments(MY "" "" "OUTPUT_DIR" ${ARGN})
    set(MY_SOURCES ${MY_UNPARSED_ARGUMENTS})
    if(NOT MY_OUTPUT_DIR)
        set(MY_OUTPUT_DIR ${out_hex_dir})
    endif()

    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${MY_OUTPUT_DIR}/${target}.map)
    set(target_img_file ${MY_OUTPUT_DIR}/${target}.img)
    cpp_only(${gen_ldscript} ${ldscript} hal)
    add_executable(${target} ${MY_SOURCES})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections ${link_cref_option})
    target_depend_rom_ld(${target})

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${cmd_mksimage} --name "${BUILD_REVISION}-${BUILD_AUTO_REVISION}"
            --imgver "1.1" --platform "8811" --check crc
            $<TARGET_FILE:${target}> ${target_img_file}
        BYPRODUCTS ${target_img_file} ${target_map_file}
    )
endfunction()

macro(pac_init_fdl cmd)
    if(CONFIG_SOC_8910)
        set(${cmd}
            cfg-init --pname "UIX8910_MODEM" --palias ${BUILD_TARGET}
                --pversion "8910 MODULE" --version "BP_R1.0.0"
                --flashtype 1
            cfg-host-fdl -a ${CONFIG_FDL1_IMAGE_START} -s ${CONFIG_FDL1_IMAGE_SIZE}
                -p ${out_hex_dir}/fdl1.sign.img
            cfg-fdl2 -a ${CONFIG_FDL2_IMAGE_START} -s ${CONFIG_FDL2_IMAGE_SIZE}
                -p ${out_hex_dir}/fdl2.sign.img
        )
    endif()
    if(CONFIG_SOC_8811)
        set(${cmd}
            cfg-init --pname "8811_MODEM" --palias ${BUILD_TARGET}
                --pversion "8811 MODULE" --version "BP_R1.0.0"
                --flashtype 0
            cfg-fdl -a ${CONFIG_NORFDL_IMAGE_START} -s ${CONFIG_NORFDL_IMAGE_SIZE}
                -p ${out_hex_dir}/norfdl.img
        )
    endif()
endmacro()

# Build unittest target. Parameters are source files.
# When PAC is supported, PAC is create with unittest only.
function(add_unittest target)
    if(CONFIG_SOC_8910)
        add_uimage(${target} ${unittest_ldscript} EXCLUDE_FROM_ALL ${ARGN}
            OUTPUT_DIR ${out_hex_unittest_dir})
        target_link_libraries(${target} PRIVATE all_libs)
        add_dependencies(unittests ${target})

        set(pac_file ${out_hex_unittest_dir}/${target}.pac)
        set(target_img_file ${out_hex_unittest_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_unittest_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i UNITTEST -a ${CONFIG_APP_FLASH_ADDRESS}
                    -s ${CONFIG_APP_FLASH_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(unittests ${target}_pacgen)
    endif()
    if(CONFIG_SOC_8811)
        add_simage(${target} ${unittest_ldscript} EXCLUDE_FROM_ALL ${ARGN}
            OUTPUT_DIR ${out_hex_unittest_dir})
        target_link_libraries(${target} PRIVATE all_libs)
        add_dependencies(unittests ${target})

        set(pac_file ${out_hex_unittest_dir}/${target}.pac)
        set(target_img_file ${out_hex_unittest_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_unittest_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i UNITTEST -a ${CONFIG_APP_FLASH_ADDRESS}
                    -s ${CONFIG_APP_FLASH_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(unittests ${target}_pacgen)
    endif()
endfunction()

# Build unittest target. Parameters are source files.
# When PAC is supported, PAC is create with unittest only.
function(add_boot_unittest target)
    if(CONFIG_SOC_8910)
        add_uimage(${target} ${ramrun_ldscript} EXCLUDE_FROM_ALL ${ARGN}
            OUTPUT_DIR ${out_hex_unittest_dir})
        target_link_whole_archive(${target} PRIVATE bootloader)
        target_link_libraries(${target} PRIVATE all_libs)
        add_dependencies(unittests ${target})

        target_compile_definitions(${target}_ldscript PRIVATE
            RAMRUN_IMAGE_START=${CONFIG_FDL1_IMAGE_START}
            RAMRUN_IMAGE_SIZE=${CONFIG_FDL1_IMAGE_SIZE}
            RAMRUN_MMU_ENABLE=1
        )

        set(pac_file ${out_hex_unittest_dir}/${target}.pac)
        set(target_img_file ${out_hex_unittest_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_unittest_dir}/${target}.sign.img)

        sign_image(${target_img_file} ${target_sign_img_file})
        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py}
                cfg-init --pname "UIX8910_MODEM" --palias ${BUILD_TARGET}
                    --pversion "8910 MODULE" --version "BP_R1.0.0"
                    --flashtype 1
                cfg-host-fdl -a ${CONFIG_FDL1_IMAGE_START} -s ${CONFIG_FDL1_IMAGE_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(unittests ${target}_pacgen)
    endif()
    if(CONFIG_SOC_8811)
        add_simage(${target} ${ramrun_ldscript} EXCLUDE_FROM_ALL ${ARGN}
            OUTPUT_DIR ${out_hex_unittest_dir})
        target_link_whole_archive(${target} PRIVATE bootloader)
        target_link_libraries(${target} PRIVATE all_libs)
        add_dependencies(unittests ${target})

        target_compile_definitions(${target}_ldscript PRIVATE
            RAMRUN_IMAGE_START=${CONFIG_NORFDL_IMAGE_START}
            RAMRUN_IMAGE_SIZE=${CONFIG_NORFDL_IMAGE_SIZE}
        )

        set(pac_file ${out_hex_unittest_dir}/${target}.pac)
        set(target_img_file ${out_hex_unittest_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_unittest_dir}/${target}.sign.img)

        sign_image(${target_img_file} ${target_sign_img_file})
        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py}
                cfg-init --pname "8811_MODEM" --palias ${BUILD_TARGET}
                    --pversion "8811 MODULE" --version "BP_R1.0.0"
                    --flashtype 0
                cfg-fdl -a ${CONFIG_NORFDL_IMAGE_START} -s ${CONFIG_NORFDL_IMAGE_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(unittests ${target}_pacgen)
    endif()
endfunction()

# Build example target. Parameters are source files.
# When PAC is supported, PAC is create with unittest only.
function(add_example target)
    if(CONFIG_SOC_8910)
        add_uimage(${target} ${example_ldscript} ${ARGN}
            OUTPUT_DIR ${out_hex_example_dir})
        target_link_libraries(${target} PRIVATE all_libs)

        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i EXAMPLE -a ${CONFIG_APP_FLASH_ADDRESS}
                    -s ${CONFIG_APP_FLASH_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen ALL DEPENDS ${pac_file})
    endif()
    if(CONFIG_SOC_8811)
        add_simage(${target} ${example_ldscript} ${ARGN}
            OUTPUT_DIR ${out_hex_example_dir})
        target_link_libraries(${target} PRIVATE all_libs)

        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i EXAMPLE -a ${CONFIG_APP_FLASH_ADDRESS}
                    -s ${CONFIG_APP_FLASH_SIZE}
                    -p ${target_sign_img_file}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen ALL DEPENDS ${pac_file})
    endif()
endfunction()

function(add_rom_lod target ldscript)
    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${out_hex_dir}/${target}.map)
    set(target_lod_file ${out_hex_dir}/${target}.lod)
    cpp_only(${gen_ldscript} ${ldscript} hal)
    add_executable(${target} ${ARGN})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${out_hex_dir})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections ${link_cref_option})

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${cmd_elf2lod} $<TARGET_FILE:${target}> ${target_lod_file}
        BYPRODUCTS ${target_lod_file} ${target_map_file}
    )
endfunction()

function(add_elf target ldscript)
    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${out_hex_dir}/${target}.map)
    cpp_only(${gen_ldscript} ${ldscript} hal)
    add_executable(${target} ${ARGN})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${out_hex_dir})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections ${link_cref_option})
    target_depend_rom_ld(${target})
endfunction()

# Build appimg with specified link script.
function(add_appimg target ldscript) # <sources> LINK_LIBRARIES <libs> OUTPUT_DIR <dir>
    cmake_parse_arguments(MY "" "" "LINK_LIBRARIES;OUTPUT_DIR" ${ARGN})
    set(MY_SOURCES ${MY_UNPARSED_ARGUMENTS} ${core_stub_o})
    set(MY_LINK_LIBRARIES ${MY_LINK_LIBRARIES} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
    if(NOT MY_OUTPUT_DIR)
        set(MY_OUTPUT_DIR ${out_hex_dir})
    endif()

    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${MY_OUTPUT_DIR}/${target}.map)
    set(target_img_file ${MY_OUTPUT_DIR}/${target}.img)
	#quectel update
	set(target_flashlibbin_file ${MY_OUTPUT_DIR}/${target}.flashlib.bin)
    cpp_only(${gen_ldscript} ${ldscript})
    add_executable(${target} ${MY_SOURCES})
    set_source_files_properties(${core_stub_o} PROPERTIES GENERATED on)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections ${link_cref_option})
    target_link_libraries(${target} PRIVATE ${MY_LINK_LIBRARIES})
    target_depend_rom_ld(${target})
	#quectel update
	if(CONFIG_APPLIB_FLASH_EXT_ENABLED)
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${cmd_mkappimg} $<TARGET_FILE:${target}> ${target_img_file}
			COMMAND ${cmd_elf2bin} --start __flashlib_start --end __flashlib_end --allow-empty
				$<TARGET_FILE:${target}> ${target_flashlibbin_file}
			BYPRODUCTS ${target_img_file} ${target_map_file} ${target_flashlibbin_file}
		)
	else()
		add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${cmd_mkappimg} $<TARGET_FILE:${target}> ${target_img_file}
		BYPRODUCTS ${target_img_file} ${target_map_file}
	)
	endif()
endfunction()

# Build example appimg (linked in flash). Parameters are source files.
function(add_appimg_flash_example target)
    set(ldscript ${SOURCE_TOP_DIR}/components/apploader/pack/app_flashimg.ld)
    add_appimg(${target} ${ldscript} EXCLUDE_FROM_ALL ${ARGN}
        OUTPUT_DIR ${out_hex_example_dir})
    add_dependencies(examples ${target})
    set(prepack_cpio ${out_hex_dir}/examples/${target}_prepack.cpio)

    if((CONFIG_SOC_8910) OR (CONFIG_SOC_8811))
        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${prepack_cpio}
            COMMAND python3 ${modemgen_py} prepackgen
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH}
                ${prepack_cpio}
            DEPENDS ${modemgen_py}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        if(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH)
            set(cfg_pack_gpio cfg-pack-cpio -i PREPACK -p ${prepack_cpio})
            set(prepack_file ${prepack_cpio})
        endif()
        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i APPIMG -a ${CONFIG_APPIMG_FLASH_ADDRESS}
                    -s ${CONFIG_APPIMG_FLASH_SIZE}
                    -p ${target_sign_img_file}
                ${cfg_pack_gpio}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file} ${prepack_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(examples ${target}_pacgen)
    endif()
endfunction()

# Build example appimg (linked in file). Parameters are source files.
function(add_appimg_file_example target)
    set(ldscript ${SOURCE_TOP_DIR}/components/apploader/pack/app_fileimg.ld)
    add_appimg(${target} ${ldscript} EXCLUDE_FROM_ALL ${ARGN}
        OUTPUT_DIR ${out_hex_example_dir})
    add_dependencies(examples ${target})
    set(prepack_cpio ${out_hex_dir}/examples/${target}_prepack.cpio)

    if((CONFIG_SOC_8910) OR (CONFIG_SOC_8811))
        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)

        pac_init_fdl(init_fdl)
        sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${prepack_cpio}
            COMMAND python3 ${modemgen_py} prepackgen
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH}
                ${prepack_cpio}
            DEPENDS ${modemgen_py}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        if(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH)
            set(cfg_pack_gpio cfg-pack-cpio -i PREPACK -p ${prepack_cpio})
            set(prepack_file ${prepack_cpio})
        endif()
        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-pack-file -i APPIMG -p ${target_sign_img_file}
                    -n ${CONFIG_APPIMG_LOAD_FILE_NAME}
                ${cfg_pack_gpio}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file} ${prepack_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(examples ${target}_pacgen)
    endif()
endfunction()

# quectel start
# Build ql_example appimg (linked in flash). Parameters are source files.
function(add_appimg_flash_ql_example target)
    set(ldscript ${SOURCE_TOP_DIR}/components/apploader/pack/app_flashimg.ld)
    add_appimg(${target} ${ldscript} EXCLUDE_FROM_ALL ${ARGN}
        OUTPUT_DIR ${out_hex_example_dir})
    add_dependencies(ql_examples ${target})
    set(prepack_cpio ${out_hex_dir}/examples/${target}_prepack.cpio)

    if((CONFIG_SOC_8910) OR (CONFIG_SOC_8811))
        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)
        pac_init_fdl(init_fdl)
		
		if(CONFIG_APPLIB_FLASH_EXT_ENABLED)
            set(target_flashlib_bin ${out_hex_example_dir}/${target}.flashlib.bin)
            set(cfg_ap2 cfg-image -i APPIMG2 -a ${CONFIG_APPLIB_FLASH_ADDRESS}
                -s ${CONFIG_APPLIB_FLASH_SIZE} -p ${target_flashlib_bin})
        endif()
        #sign_image(${target_img_file} ${target_sign_img_file})
	
        add_custom_command(OUTPUT ${prepack_cpio}
            COMMAND python3 ${modemgen_py} prepackgen
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH}
                ${prepack_cpio}
            DEPENDS ${modemgen_py}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        if(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH)
            set(cfg_pack_gpio cfg-pack-cpio -i PREPACK -p ${prepack_cpio})
            set(prepack_file ${prepack_cpio})
        endif()

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i APPIMG -a ${CONFIG_APPIMG_FLASH_ADDRESS}
                    -s ${CONFIG_APPIMG_FLASH_SIZE}
					# quectel update
                    #-p ${target_img_file}
					-p ${target_sign_img_file}
					${cfg_ap2}
                ${cfg_pack_gpio}
                pac-gen ${pac_file}
			# quectel update
            #DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_img_file} ${prepack_file}
			DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file} ${prepack_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(ql_examples ${target}_pacgen)
    endif()
endfunction()

# Build ql_example appimg (linked in file). Parameters are source files.
function(add_appimg_file_ql_example target)
    set(ldscript ${SOURCE_TOP_DIR}/components/apploader/pack/app_fileimg.ld)
    add_appimg(${target} ${ldscript} EXCLUDE_FROM_ALL ${ARGN}
        OUTPUT_DIR ${out_hex_example_dir})
    add_dependencies(ql_examples ${target})
    set(prepack_cpio ${out_hex_dir}/examples/${target}_prepack.cpio)

    if((CONFIG_SOC_8910) OR (CONFIG_SOC_8811))
        set(pac_file ${out_hex_example_dir}/${target}.pac)
        set(target_img_file ${out_hex_example_dir}/${target}.img)
        set(target_sign_img_file ${out_hex_example_dir}/${target}.sign.img)
        pac_init_fdl(init_fdl)
        #sign_image(${target_img_file} ${target_sign_img_file})

        add_custom_command(OUTPUT ${prepack_cpio}
            COMMAND python3 ${modemgen_py} prepackgen
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH}
                ${prepack_cpio}
            DEPENDS ${modemgen_py}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        if(CONFIG_PACKAGE_FILE_APPIMG_JSON_PATH)
            set(cfg_pack_gpio cfg-pack-cpio -i PREPACK -p ${prepack_cpio})
            set(prepack_file ${prepack_cpio})
        endif()

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-pack-file -i APPIMG -p ${target_sign_img_file}
                    -n ${CONFIG_APPIMG_LOAD_FILE_NAME}
				${cfg_pack_gpio}
                pac-gen ${pac_file}
            DEPENDS ${pacgen_py} ${pac_fdl_files} ${target_sign_img_file} ${prepack_file}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        add_custom_target(${target}_pacgen DEPENDS ${pac_file})
        add_dependencies(ql_examples ${target}_pacgen)
    endif()
endfunction()
# quectel end

# Build appimg (flash and file) pac to delete appimg.
function(add_appimg_delete)
    if((CONFIG_SOC_8910) OR (CONFIG_SOC_8811))
        if(CONFIG_APPIMG_LOAD_FLASH)
            set(target appimg_flash_delete)
            set(pac_file ${out_hex_dir}/${target}.pac)
            pac_init_fdl(init_fdl)

            add_custom_command(OUTPUT ${pac_file}
                COMMAND python3 ${pacgen_py} ${init_fdl}
                    cfg-erase-flash -i ERASE_APPIMG
                        -a ${CONFIG_APPIMG_FLASH_ADDRESS}
                        -s ${CONFIG_APPIMG_FLASH_SIZE}
                    pac-gen ${pac_file}
                DEPENDS ${pacgen_py} ${pac_fdl_files}
                WORKING_DIRECTORY ${SOURCE_TOP_DIR}
            )
            add_custom_target(${target}_pacgen ALL DEPENDS ${pac_file})
        endif()

        if(CONFIG_APPIMG_LOAD_FILE)
            set(target appimg_file_delete)
            set(pac_file ${out_hex_dir}/${target}.pac)
            pac_init_fdl(init_fdl)

            add_custom_command(OUTPUT ${pac_file}
                COMMAND python3 ${pacgen_py} ${init_fdl}
                    cfg-del-appimg -i DEL_APPIMG
                    pac-gen ${pac_file}
                DEPENDS ${pacgen_py} ${pac_fdl_files}
                WORKING_DIRECTORY ${SOURCE_TOP_DIR}
            )
            add_custom_target(${target}_pacgen ALL DEPENDS ${pac_file})
        endif()
    endif()
endfunction()

macro(relative_glob var)
    file(GLOB ${var} RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ARGN})
endmacro()

function(beautify_c_code target)
    if(ARGN)
        set(beautify_target beautify_${target})
        if(NOT TARGET ${beautify_target})
            add_custom_target(${beautify_target})
            add_dependencies(beautify ${beautify_target})
        endif()
        add_custom_command(TARGET ${beautify_target} POST_BUILD
            COMMAND clang-format -i ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
endfunction()

function(rpcstubgen xml sender receiver)
    get_filename_component(name ${xml} NAME_WE)
    configure_file(${xml} ${out_rpc_dir}/${name}.xml)
    set(gen ${out_rpc_dir}/${name}_${sender}.c
        ${out_rpc_dir}/${name}_${receiver}.c
        ${out_rpc_dir}/${name}_api.h
        ${out_rpc_dir}/${name}_par.h)
    add_custom_command(
        OUTPUT ${gen}
        COMMAND python3 ${tools_dir}/rpcgen.py stubgen ${xml}
        DEPENDS ${xml} ${tools_dir}/rpcgen.py
        WORKING_DIRECTORY ${out_rpc_dir}
    )
    add_custom_target(${name}_rpcgen DEPENDS ${gen})
    add_dependencies(rpcgen ${name}_rpcgen)
endfunction()

function(rpcdispatchgen cfile side)
    get_filename_component(name ${cfile} NAME_WE)
    set(xmls)
    foreach(xml ${ARGN})
        list(APPEND xmls ${out_rpc_dir}/${xml})
    endforeach()
    add_custom_command(
        OUTPUT ${out_rpc_dir}/${cfile}
        COMMAND python3 ${tools_dir}/rpcgen.py dispatchgen ${cfile} ${side} ${xmls}
        DEPENDS ${tools_dir}/rpcgen.py ${xmls}
        WORKING_DIRECTORY ${out_rpc_dir}
    )
    add_custom_target(${name}_rpcgen DEPENDS ${out_rpc_dir}/${cfile})
    add_dependencies(rpcgen ${name}_rpcgen)
endfunction()

function(nanopbgen)
    foreach(file ${ARGN})
        get_filename_component(name ${file} NAME_WE)
        get_filename_component(fpath ${file} DIRECTORY)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${name}.pb.h ${CMAKE_CURRENT_BINARY_DIR}/${name}.pb.c
            COMMAND protoc --proto_path=${fpath} --nanopb_out=${CMAKE_CURRENT_BINARY_DIR} ${file}
            DEPENDS ${file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach(file ${ARGN})
endfunction()
