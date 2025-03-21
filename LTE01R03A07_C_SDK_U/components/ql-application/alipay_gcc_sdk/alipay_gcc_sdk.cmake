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

#3rdparty dir
add_library(lib_cjson STATIC IMPORTED)
set_target_properties(lib_cjson PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/cJSON/lib/lib3rdparty_cJSON_a5_gcc10.a)

add_library(lib_mbedtls STATIC IMPORTED)
set_target_properties(lib_mbedtls PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/mbedtls/lib/lib3rdparty_mbedtls_a5_gcc10.a)

add_library(lib_protobuf STATIC IMPORTED)
set_target_properties(lib_protobuf PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/quickjs/lib/lib3rdparty_quickjs_a5_gcc10.a)

add_library(lib_quickjs STATIC IMPORTED)
set_target_properties(lib_quickjs PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/quickjs/lib/lib3rdparty_quickjs_a5_gcc10.a)

add_library(lib_regex STATIC IMPORTED)
set_target_properties(lib_regex PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/regex/lib/lib3rdparty_regex_a5_gcc10.a)

add_library(lib_sm3 STATIC IMPORTED)
set_target_properties(lib_sm3 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/sm3/lib/lib3rdparty_sm3_a5_gcc10.a)

add_library(lib_uEcc STATIC IMPORTED)
set_target_properties(lib_uEcc PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/3rdparty/uEcc/lib/lib3rdparty_uEcc_a5_gcc10.a)

#app dir
add_library(lib_aid_code STATIC IMPORTED)
set_target_properties(lib_aid_code PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/aid_code/lib/libapp_aid_code_a5_gcc10.a)

add_library(lib_ant_chaim STATIC IMPORTED)
set_target_properties(lib_ant_chaim PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/antchain/lib/libapp_antchain_a5_gcc10.a)

add_library(lib_bind_net STATIC IMPORTED)
set_target_properties(lib_bind_net PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/bind_net/lib/libapp_bind_net_a5_gcc10.a)

add_library(lib_pay_code STATIC IMPORTED)
set_target_properties(lib_pay_code PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/pay_code/lib/libapp_pay_code_a5_gcc10.a)

add_library(lib_pay_result STATIC IMPORTED)
set_target_properties(lib_pay_result PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/pay_result/lib/libapp_pay_result_a5_gcc10.a)

add_library(lib_provision STATIC IMPORTED)
set_target_properties(lib_provision PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/provision/lib/libapp_provision_a5_gcc10.a)

add_library(lib_scan_pay STATIC IMPORTED)
set_target_properties(lib_scan_pay PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/scan_pay/lib/libapp_scan_pay_a5_gcc10.a)

add_library(lib_setting STATIC IMPORTED)
set_target_properties(lib_setting PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/setting/lib/libapp_setting_a5_gcc10.a)

add_library(lib_trans_code STATIC IMPORTED)
set_target_properties(lib_trans_code PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/app/trans_code/lib/libapp_trans_code_a5_gcc10.a)

#fwk dir
add_library(lib_bind_status STATIC IMPORTED)
set_target_properties(lib_bind_status PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/bind_status/lib/libfwk_bind_status_a5_gcc10.a)

add_library(lib_common STATIC IMPORTED)
set_target_properties(lib_common PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/common/lib/libfwk_common_a5_gcc10.a)

add_library(lib_ble_communicate STATIC IMPORTED)
set_target_properties(lib_ble_communicate PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/ble_communicate/lib/libfwk_ble_communicate_a5_gcc10.a)

add_library(lib_crypt STATIC IMPORTED)
set_target_properties(lib_crypt PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/crypt/lib/libfwk_crypt_a5_gcc10.a)

add_library(lib_crypt_ext STATIC IMPORTED)
set_target_properties(lib_crypt_ext PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/crypt_ext/lib/libfwk_crypt_ext_a5_gcc10.a)

add_library(lib_device_certificate STATIC IMPORTED)
set_target_properties(lib_device_certificate PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/device_certificate/lib/libfwk_device_certificate_a5_gcc10.a)

add_library(lib_js_api STATIC IMPORTED)
set_target_properties(lib_js_api PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/js_api/lib/libfwk_js_api_a5_gcc10.a)

add_library(lib_network STATIC IMPORTED)
set_target_properties(lib_network PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/network/lib/libfwk_network_a5_gcc10.a)

add_library(lib_quickjs_adapt STATIC IMPORTED)
set_target_properties(lib_quickjs_adapt PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/quickjs_adapt/lib/libfwk_quickjs_adapt_a5_gcc10.a)

add_library(lib_secure_mode_se STATIC IMPORTED)
set_target_properties(lib_secure_mode_se PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/secure_mode_se/lib/libfwk_secure_mode_se_a5_gcc10.a)

add_library(lib_secure_mode_se_v2 STATIC IMPORTED)
set_target_properties(lib_secure_mode_se_v2 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/secure_mode_se_v2/lib/libfwk_secure_mode_se_v2_a5_gcc10.a)

add_library(lib_sm2 STATIC IMPORTED)
set_target_properties(lib_sm2 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/sm2/lib/libfwk_sm2_a5_gcc10.a)

add_library(lib_storage STATIC IMPORTED)
set_target_properties(lib_storage PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/fwk/storage/lib/libfwk_storage_a5_gcc10.a)

#share dir
add_library(lib_share_common STATIC IMPORTED)
set_target_properties(lib_share_common PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/a5_gcc/share/common/lib/libshare_common_a5_gcc10.a)

target_link_group(${target} PRIVATE ql_app_alipay_arm_sdk
	lib_cjson lib_mbedtls lib_protobuf lib_quickjs lib_regex lib_sm3 lib_uEcc
	lib_aid_code lib_ant_chaim lib_bind_net lib_pay_code lib_pay_result lib_provision lib_scan_pay lib_setting lib_trans_code
	lib_bind_status lib_ble_communicate lib_common lib_crypt lib_crypt_ext lib_device_certificate lib_js_api lib_network lib_secure_mode_se lib_secure_mode_se_v2 lib_sm2 lib_storage
	lib_share_common)