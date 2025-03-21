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

#configure_file(include/lv_gui_config.h.in ${out_inc_dir}/lv_gui_config.h)

set(target ql_qcloud_iot)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_ALIYUN)
target_include_directories(${target} PUBLIC inc include include/exports platform platform/os/qlrtos platform/tls/mbedtls  samples samples/data_template samples/dynreg_dev  samples/gateway samples/mqtt  samples/raw_data samples/scenarized sdk_src sdk_src/internal_inc sdk_src/network sdk_src/network/socket sdk_src/network/tls sdk_src/protocol sdk_src/protocol/mqtt sdk_src/protocol/http sdk_src/services sdk_src/services/data_template sdk_src/services/dynreg sdk_src/services/gateway sdk_src/services/log sdk_src/services/ota sdk_src/services/system sdk_src/utils)
#target_link_libraries(${target} PRIVATE kernel driver hal ql_api_common)
target_sources(${target} PRIVATE
   	platform/os/qlrtos/HAL_AT_UART_qlrtos.c
	platform/os/qlrtos/HAL_Device_qlrtos.c
	platform/os/qlrtos/HAL_Log_qlrtos.c
	platform/os/qlrtos/HAL_OS_qlrtos.c
	platform/os/qlrtos/HAL_TCP_qlrtos.c
	platform/os/qlrtos/HAL_Timer_qlrtos.c
                platform/os/qlrtos/HAL_UDP_qlrtos.c
	#platform/tls/mbedtls/HAL_DTLS_mbedtls.c
	#platform/tls/mbedtls/HAL_TLS_mbedtls.c
	#samples/data_template/action_config.c
	#samples/data_template/data_config.c
	samples/data_template/data_template_sample.c
	#samples/data_template/events_config.c
	samples/dynreg_dev/dynreg_dev_sample.c
	samples/gateway/gateway_sample.c
	samples/gateway/sub_dev001.c
	samples/mqtt/mqtt_sample.c
                samples/ota/ota_mqtt_sample.c
	samples/raw_data/raw_data_sample.c
	samples/scenarized/light_data_template_sample.c
	sdk_src/network/socket/network_socket.c
	sdk_src/network/tls/network_tls.c
	sdk_src/network/network_interface.c
	sdk_src/protocol/http/utils_httpc.c
                sdk_src/protocol/mqtt/mqtt_client.c
                sdk_src/protocol/mqtt/mqtt_client_common.c
                sdk_src/protocol/mqtt/mqtt_client_connect.c
                sdk_src/protocol/mqtt/mqtt_client_net.c
                sdk_src/protocol/mqtt/mqtt_client_publish.c
                sdk_src/protocol/mqtt/mqtt_client_subscribe.c
                sdk_src/protocol/mqtt/mqtt_client_unsubscribe.c
                sdk_src/protocol/mqtt/mqtt_client_yield.c
	sdk_src/services/data_template/data_template_aciton.c
	sdk_src/services/data_template/data_template_client.c
	sdk_src/services/data_template/data_template_client_common.c
	sdk_src/services/data_template/data_template_client_json.c
	sdk_src/services/data_template/data_template_client_manager.c
	sdk_src/services/data_template/data_template_event.c
	sdk_src/services/dynreg/dynreg.c
	sdk_src/services/gateway/gateway_api.c
	sdk_src/services/gateway/gateway_common.c
	sdk_src/services/log/log_mqtt.c
	sdk_src/services/log/log_upload.c
	sdk_src/services/ota/ota_client.c
	sdk_src/services/ota/ota_fetch.c
	sdk_src/services/ota/ota_lib.c
	sdk_src/services/ota/ota_mqtt.c
	sdk_src/services/system/system_mqtt.c
	sdk_src/utils/json_parser.c
                sdk_src/utils/json_token.c
	sdk_src/utils/qcloud_iot_ca.c
                sdk_src/utils/qcloud_iot_device.c
	sdk_src/utils/qcloud_iot_log.c
                sdk_src/utils/string_utils.c
	sdk_src/utils/utils_aes.c
                sdk_src/utils/utils_base64.c
	sdk_src/utils/utils_getopt.c
                sdk_src/utils/utils_hmac.c
	sdk_src/utils/utils_list.c
                sdk_src/utils/utils_md5.c
	sdk_src/utils/utils_ringbuff.c
                sdk_src/utils/utils_sha1.c
	sdk_src/utils/utils_timer.c
)
relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})