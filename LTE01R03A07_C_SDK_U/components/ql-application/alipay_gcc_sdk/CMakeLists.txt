# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

set(target ql_app_alipay_arm_sdk)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/aid_code/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/ant_forrest/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/antchain/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/bind_net/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/pay_code/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/pay_result/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/provision/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/provision_ota/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/scan_pay/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/setting/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/trans_code/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/cert/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/device_info/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/file/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/network/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/os/inc
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/share/common/inc
)

target_sources(${target} PRIVATE
	ql_alipay_cert.c
	ql_alipay_vendor_os.c
	ql_alipay_vendor_device_info.c
	ql_alipay_file.c
	ql_alipay_net_kal.c
	alipay_api_demo.c			#demo only.
	
)

if(QL_APP_FEATURE_ALIPAY_GCC_SE_V1)
target_include_directories(${target} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/se_v1/inc
    ${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v1
)

target_sources(${target} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v1/vendor_ssp.c
	${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v1/hs_private.c
)
endif()

if(QL_APP_FEATURE_ALIPAY_GCC_SE_V2)
target_include_directories(${target} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/a5_gcc/hal/se_v2/inc
    ${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v2
)

target_sources(${target} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v2/vendor_ssp.c
	${CMAKE_CURRENT_LIST_DIR}/alipay_encryption_chip/se_v2/hs_private.c
)
endif()

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})

