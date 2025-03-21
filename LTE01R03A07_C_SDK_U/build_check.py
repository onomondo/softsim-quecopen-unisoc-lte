# !/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import re
import datetime
import sys
import json
import argparse

class BuildCheck(object):
    def __init__(self):
        self._des = []
        pass
    
    def partition_check(self,path):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            temp = int("1000000",16)
            Align4K = int("1000",16)
            IN_FLASH_ADDR = int("60000000",16)  #内置flash地址
            EXT_FLASH_ADDR = int("70000000",16) #外置flash地址
            BOOT_SIZE_UNISOC = int("10000",16)  #boot分区固定大小,不可改
            FACTORY_ADDR_UNISOC = int("607e0000",16) #FACTORY分区固定地址,不可改
            FACTORY_SIZE_UNISOC = int("20000",16)    #FACTORY分区固定大小,不可改
            FS_EXT_NAME = 'SFL2'  #内置文件系统
            FS_INT_NAME = 'SFL1'  #外置文件系统
            
            #####   macros   ######
            BOOT_ADDR = int(partiton_data['macros']['CONFIG_BOOT_FLASH_ADDRESS'].replace('0x',''),16)       #BOOT 地址
            BOOT_SIZE = int(partiton_data['macros']['CONFIG_BOOT_FLASH_SIZE'].replace('0x',''),16)          #BOOT 大小
            KERNEL_ADDR = int(partiton_data['macros']['CONFIG_APP_FLASH_ADDRESS'].replace('0x',''),16)      #KERNEL 地址
            KERNEL_SIZE = int(partiton_data['macros']['CONFIG_APP_FLASH_SIZE'].replace('0x',''),16)         #KERNEL 大小
            APP_IMG_ADDR = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_ADDRESS'].replace('0x',''),16)  #APP IMG 地址
            APP_IMG_SIZE = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_SIZE'].replace('0x',''),16)     #APP IMG 大小          
            FS_SYS_ADDR = int(partiton_data['macros']['CONFIG_FS_SYS_FLASH_ADDRESS'].replace('0x',''),16)   #FS 地址
            FS_SYS_SIZE = int(partiton_data['macros']['CONFIG_FS_SYS_FLASH_SIZE'].replace('0x',''),16)      #FS 大小    
            FS_SYS_TYPE = str(partiton_data['descriptions'][0]['flash'])                                     #文件系统位置
            MODEM_ADDR = int(partiton_data['macros']['CONFIG_FS_MODEM_FLASH_ADDRESS'].replace('0x',''),16)  #MODEM分区 地址
            MODEM_SIZE = int(partiton_data['macros']['CONFIG_FS_MODEM_FLASH_SIZE'].replace('0x',''),16)     #MODEM分区 大小
            FACTORY_ADDR = int(partiton_data['macros']['CONFIG_FS_FACTORY_FLASH_ADDRESS'].replace('0x',''),16)  #FACTORY分区 地址
            FACTORY_SIZE = int(partiton_data['macros']['CONFIG_FS_FACTORY_FLASH_SIZE'].replace('0x',''),16)     #FACTORY分区 大小

#1. 判断地址是否存在冲突              
            if((BOOT_ADDR != IN_FLASH_ADDR) or (BOOT_SIZE != BOOT_SIZE_UNISOC) or (BOOT_SIZE + KERNEL_SIZE > APP_IMG_ADDR)): #BOOT分区地址/大小不允许被修改
                raise Exception("boot addr/size error")
     
            if((FACTORY_ADDR != FACTORY_ADDR_UNISOC) or (FACTORY_SIZE != FACTORY_SIZE_UNISOC)): #FACTORY分区地址/大小不允许被修改
                raise Exception("factory addr/size error")     
            
            if(MODEM_ADDR + MODEM_SIZE > FACTORY_ADDR):
                raise Exception("modem addr/size error")  
            
            if((APP_IMG_ADDR < EXT_FLASH_ADDR) and (FS_SYS_ADDR < EXT_FLASH_ADDR)): #非XIP模式
                if((KERNEL_ADDR + KERNEL_SIZE > APP_IMG_ADDR) or (APP_IMG_ADDR + APP_IMG_SIZE > FS_SYS_ADDR)): #内核踩了app, 或者app踩了文件系统
                    raise Exception("kernel or app addr/size error")   
                if(FS_SYS_ADDR + FS_SYS_SIZE > MODEM_ADDR): #文件系统踩了modem分区
                    raise Exception("fs or modem addr/size error") 
            elif((APP_IMG_ADDR >= EXT_FLASH_ADDR) and (FS_SYS_ADDR < EXT_FLASH_ADDR)): #xip模式,app放在外置flash,文件系统放内置flash
                if(KERNEL_ADDR + KERNEL_SIZE > FS_SYS_ADDR):  #内核踩了文件系统
                    raise Exception("kernel addr/size error")
            elif((APP_IMG_ADDR < EXT_FLASH_ADDR) and (FS_SYS_ADDR >= EXT_FLASH_ADDR)): #app放内置flash, 文件系统分区放在外置
                if(KERNEL_ADDR + KERNEL_SIZE > APP_IMG_ADDR):  #内核踩了app
                    raise Exception("kernel addr/size error")
            else: #文件系统和app都放在外置flash上
                if((KERNEL_ADDR + KERNEL_SIZE > MODEM_ADDR) or (APP_IMG_ADDR + APP_IMG_SIZE > FS_SYS_ADDR)): #内核踩了modem分区,或者app踩了文件系统
                   raise Exception("kernel or app addr/size error") 
                        
#2. 判断文件系统内型是否正确
            if(FS_SYS_ADDR > EXT_FLASH_ADDR): #文件系统放在了外置flash上
                if(FS_SYS_TYPE != FS_EXT_NAME):
                    raise Exception("fs flash should be SFL2")
            else:
                if(FS_SYS_TYPE != FS_INT_NAME):
                    raise Exception("fs flash should be SFL1")               
            
            
#3: 检查文件系统内部偏移地址是否合法           
            DES_FS_OFFEST = int(partiton_data['descriptions'][0]['offset'].replace('0x',''),16)             #FS 偏移
            DES_FS_SIZE = int(partiton_data['descriptions'][0]['size'].replace('0x',''),16)                 #FS 大小
            Fs_Align = int(partiton_data['descriptions'][0]['erase_block'].replace('0x',''),16)             #FS 对齐大小
            
            if (FS_SYS_SIZE != DES_FS_SIZE) or (FS_SYS_ADDR % temp != DES_FS_OFFEST):  #偏移地址或者文件系统大小,和descriptions中不一致
                raise Exception("partition file system size error")
            
            if (APP_IMG_SIZE % Align4K != 0) or (FS_SYS_SIZE % Fs_Align !=0) or (APP_IMG_ADDR % Align4K != 0) or (FS_SYS_ADDR % Fs_Align !=0):     #检测地址和大小对齐
                raise Exception("partition size align error")

            f.close()
            return 0
            
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1
      
    def kernel_size_check(self,path,kernel_img):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            APP_IMG_SIZE = int(partiton_data['macros']['CONFIG_APP_FLASH_SIZE'].replace('0x',''),16)
            stats = os.stat(kernel_img)
            coreimg_size = stats.st_size
            if(coreimg_size > APP_IMG_SIZE):
                raise Exception("kernel img oversize")
            remain_size = APP_IMG_SIZE-coreimg_size
            print("kernel total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,remain:0x{:04x}|{:.2f}KB".format(APP_IMG_SIZE,APP_IMG_SIZE/1024,coreimg_size,coreimg_size/1024,remain_size,remain_size/1024))
            f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1      
        
    def app_size_check(self,path,app_img):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            APP_SIZE = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_SIZE'].replace('0x',''),16)
            stats = os.stat(app_img)
            app_size = stats.st_size
            if(app_size > APP_SIZE):
                raise Exception("app img oversize")
            remain_size = APP_SIZE-app_size
            print("app total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,remain:0x{:04x}|{:.2f}KB".format(APP_SIZE,APP_SIZE/1024,app_size,app_size/1024,remain_size,remain_size/1024))
            f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1  
    def file_size_check(self,path,size):
        try:
            stats = os.stat(path)
            file_size = stats.st_size
            if(file_size < size):
                raise Exception("file size error")
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            return 1
    def file_line_ending_check(self,path):
        try:
            with open(path, 'rb') as f:
                data = f.read()
                if b'\r\n' in data:
                    raise Exception("file ending with CRLF!")
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            return 1
            
def main(argv):
    try:
        parser = argparse.ArgumentParser(description='manual to this script' , formatter_class=argparse.RawTextHelpFormatter)
        
        parser.add_argument("--coreimg", type=str, default="none")
        parser.add_argument("--appimg", type=str, default="none")
        parser.add_argument("--partinfo", type=str, default="none")
        
        parser.add_argument("--file", type=str, default="none")
        parser.add_argument("--size", type=int, default=0)
        parser.add_argument("--line_ending", 
                            type=str, 
                            default="quectel_build/tool/build_all.sh",
                            help="check if the linux build script ends with LF")
     
        opt = parser.parse_args()
        
        file_path = opt.file
        file_size = opt.size
        
        partiton_path = opt.partinfo
        core_img_path = opt.coreimg
        app_img_path = opt.appimg

        linux_build_file_path = opt.line_ending 
        
        build_check = BuildCheck()
        
        if(partiton_path != "none"):
        
            if(core_img_path == "none" and app_img_path == "none"):
                return build_check.partition_check(partiton_path)  
            
            elif (core_img_path != "none" and app_img_path == "none"):
                return build_check.kernel_size_check(partiton_path,core_img_path)
        
            elif (core_img_path == "none" and app_img_path != "none"):
                return build_check.app_size_check(partiton_path,app_img_path)
            
        elif (file_path != "none" and file_size!=0):
            return build_check.file_size_check(file_path,file_size)
        
        elif (linux_build_file_path != "none"):
            return build_check.file_line_ending_check(linux_build_file_path)
            
        else:
            raise Exception("invalid parameters") 
    
    except Exception as e:
        import traceback
        print(traceback.format_exc())
        return 1
        
if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
