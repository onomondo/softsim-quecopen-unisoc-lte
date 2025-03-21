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

configure_file(include/boot_config.h.in ${out_inc_dir}/boot_config.h)

if(CONFIG_SOC_8910)
    set(target bootloader)
    add_library(${target} STATIC)
	
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_include_directories(${target} PUBLIC include)
    target_link_libraries(${target} PRIVATE boot_encrypt calclib boot_calclib_crc driver fupdate boot_ex_ql)
    target_include_targets(${target} PRIVATE hal fs sffs bdev driver fsmount nvm cpio_parser fupdate boot_ex_ql lcdpanel)
	
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC THEN ${target} PRIVATE boot_ex_sdmmc)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC THEN ${target} PRIVATE boot_ex_emmc)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY THEN ${target} PRIVATE boot_ex_fat_rdonly)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW THEN ${target} PRIVATE boot_ex_fat_rw)
    target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_BUS THEN ${target} PRIVATE boot_ex_spi)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR THEN ${target} PRIVATE boot_ex_spi6nor)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI_FLASH THEN ${target} PRIVATE boot_ex_spiflash)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH THEN ${target} PRIVATE boot_ex_nandflash boot_ex_nandflash_cfg)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND THEN ${target} PRIVATE boot_ex_spi6nand boot_ex_spi6nand_cfg)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_LCD THEN ${target} PRIVATE boot_ex_lcd)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_LED THEN ${target} PRIVATE boot_ex_led)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC THEN ${target} PRIVATE boot_ex_adc)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SCATTER THEN ${target} PRIVATE boot_ex_scatter)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS THEN ${target} PRIVATE boot_ex_spi4exnsffs boot_ex_spi4exnsffs_cfg)
	target_link_libraries_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_NORFLASH THEN ${target} PRIVATE boot_ex_spi4norflash)
	
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC THEN ${target} PRIVATE boot_ex_sdmmc)	
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC THEN ${target} PRIVATE boot_ex_emmc)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY THEN ${target} PRIVATE boot_ex_fat_rdonly)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW THEN ${target} PRIVATE boot_ex_fat_rw)
    target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_BUS THEN ${target} PRIVATE boot_ex_spi)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR THEN ${target} PRIVATE boot_ex_spi6nor)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI_FLASH THEN ${target} PRIVATE boot_ex_spiflash)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH THEN ${target} PRIVATE boot_ex_nandflash boot_ex_nandflash_cfg)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND THEN ${target} PRIVATE boot_ex_spi6nand boot_ex_spi6nand_cfg)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_LCD THEN ${target} PRIVATE boot_ex_lcd)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_LED THEN ${target} PRIVATE boot_ex_led)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC THEN ${target} PRIVATE boot_ex_adc)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SCATTER THEN ${target} PRIVATE boot_ex_scatter)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS THEN ${target} PRIVATE boot_ex_spi4exnsffs boot_ex_spi4exnsffs_cfg)
	target_include_targets_if(CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_NORFLASH THEN ${target} PRIVATE boot_ex_spi4norflash)
	
	target_sources(${target} PRIVATE
        src/8910/boot_platform_8910.c
        src/8910/boot_secure.c
        src/boot_entry_v7a.S
        src/boot_sffs_port.c
        src/boot_delay.c
        src/boot_mem.c
        src/boot_spi_flash.c
        src/boot_debuguart.c
        src/boot_trace.c
        src/boot_fdl.c
        src/boot_fdl_dnld.c
        src/boot_aes.c
        src/boot_vfs.c
        src/boot_irq.c
        src/boot_timer.c
        src/boot_uart.c
        src/boot_uart_cmn.c
        src/boot_mmu.c
        src/boot_uimage.c
        src/boot_udc.c
        src/boot_usb_fdl_channel.c
        src/boot_pdl.c
        src/boot_pdl_uimage.c
        src/boot_lcd.c
        src/boot_unity.c
        src/boot_override.c
		src/boot_adi_bus.c
		src/boot_log.c
    )
	
if(NOT QL_PROJECT_MIXER)
    set(rmarun_ld ldscripts/ramrun_uimage_arm.ld)

    set(target boot2)
    add_uimage(${target} ${rmarun_ld} boot/boot2_start_8910.c)
    target_compile_definitions(${target}_ldscript PRIVATE
        RAMRUN_IMAGE_START=${CONFIG_BOOT2_IMAGE_START}
        RAMRUN_IMAGE_SIZE=${CONFIG_BOOT2_IMAGE_SIZE}
        RAMRUN_MMU_ENABLE=1
    )
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_link_whole_archive(${target} PRIVATE bootloader)
    target_link_group(${target} PRIVATE hal osi_lib fupdate fupdate_security_boot fsmount sffs bdev bdev_core boot_ex_ql)
    target_include_targets(${target} PRIVATE hal fs sffs bdev fupdate driver boot_ex_ql)
    target_link_libraries(${target} PRIVATE ${libc_file_name} ${libgcc_file_name})

    set(boot2_img ${out_hex_dir}/boot2.img)
    set(boot2_lzma ${boot2_img}.lzmar)
    add_custom_command(OUTPUT ${boot2_lzma}
        COMMAND ${cmd_lzmare2} ${boot2_img} ${boot2_lzma}
        DEPENDS ${boot2_img}
    )

    set(target boot)
    add_uimage(${target} ${rmarun_ld} boot/boot_start_8910.c)
    target_compile_definitions(${target}_ldscript PRIVATE
        RAMRUN_IMAGE_START=${CONFIG_BOOT_IMAGE_START}
        RAMRUN_IMAGE_SIZE=${CONFIG_BOOT_IMAGE_SIZE}
        RAMRUN_FLASH_SIZE=${CONFIG_BOOT_UNSIGN_IMAGE_SIZE}
        RAMRUN_MMU_ENABLE=0
    )
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_link_whole_archive(${target} PRIVATE bootloader)
    target_link_group(${target} PRIVATE hal calclib boot_calclib_crc)
    target_include_targets(${target} PRIVATE kernel)
    target_link_libraries(${target} PRIVATE ${libc_file_name} ${libgcc_file_name})
    target_incbin(${target} ${boot2_lzma} gBoot2CompData BALIGN 8)

    set(target fdl1)
    add_uimage(${target} ${rmarun_ld} fdl1/fdl1_main.c)
    target_compile_definitions(${target}_ldscript PRIVATE
        RAMRUN_IMAGE_START=${CONFIG_FDL1_IMAGE_START}
        RAMRUN_IMAGE_SIZE=${CONFIG_FDL1_IMAGE_SIZE}
        RAMRUN_FLASH_SIZE=${CONFIG_BOOT_UNSIGN_IMAGE_SIZE}
        RAMRUN_MMU_ENABLE=0
    )
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_link_whole_archive(${target} PRIVATE bootloader)
    target_link_group(${target} PRIVATE hal osi_lib)
    target_include_targets(${target} PRIVATE driver)
    target_link_libraries(${target} PRIVATE ${libc_file_name} ${libgcc_file_name})

    set(target fdl2)
    add_uimage(${target} ${rmarun_ld} fdl2/fdl2_main.c)
    target_compile_definitions(${target}_ldscript PRIVATE
        RAMRUN_IMAGE_START=${CONFIG_FDL2_IMAGE_START}
        RAMRUN_IMAGE_SIZE=${CONFIG_FDL2_IMAGE_SIZE}
        RAMRUN_MMU_ENABLE=1
    )
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_include_targets(${target} PRIVATE fs fsmount nvm calclib cpio_parser driver)
    target_link_whole_archive(${target} PRIVATE bootloader cpio_parser)
    target_link_group(${target} PRIVATE hal calclib boot_calclib_crc sffs fsmount bdev bdev_core osi_lib nvm bt_nv)

    target_link_libraries(${target} PRIVATE ${libc_file_name} ${libgcc_file_name})
    target_incbin_size(${target} ${CMAKE_CURRENT_SOURCE_DIR}/fdl2/calibparam.bin gDefCalibParam gDefCalibParamSize)

    set(target flash_test_fdl)
    add_uimage(${target} ${rmarun_ld} flash_test_fdl/flash_test_main.c)
    target_compile_definitions(${target}_ldscript PRIVATE
        RAMRUN_IMAGE_START=${CONFIG_FDL1_IMAGE_START}
        RAMRUN_IMAGE_SIZE=${CONFIG_FDL1_IMAGE_SIZE}
        RAMRUN_MMU_ENABLE=0
    )
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_BOOT)
    target_link_whole_archive(${target} PRIVATE bootloader)
    target_link_group(${target} PRIVATE hal osi_lib)
    target_include_targets(${target} PRIVATE driver)
    target_link_libraries(${target} PRIVATE ${libc_file_name} ${libgcc_file_name})
endif()	
endif()

relative_glob(srcs include/*.h src/*.[ch]
    fdl1/*.[ch] fdl2/*.[ch] flash_test_fdl/*.[ch]
    boot/*.[ch] uart_ramrun/*.[ch]
)
beautify_c_code(bootloader ${srcs})
