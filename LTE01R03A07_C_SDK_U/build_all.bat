:: Copyright (C) 2018 RDA Technologies Limited and\or its affiliates("RDA").
:: All rights reserved.
::
:: This software is supplied "AS IS" without any warranties.
:: RDA assumes no responsibility or liability for the use of the software,
:: conveys no license or title under any patent, copyright, or mask work
:: right to the product. RDA reserves the right to make changes in the
:: software without notification.  RDA also make no representation or
:: warranty that such application will be suitable for the specified use
:: without further testing or modification.

@echo off

::to config task num for ninja
::set build_tasknum=

set Start_at=%time%
set curtitle=cmd: %~n0 %*
title %curtitle% @ %time%

set ret=0
set def_proj=EC200UCN_AA
set buildtype_src=%1
set buildoption=
if "%buildtype_src%"=="" (
	set buildoption=r
) else if /i "%buildtype_src%"=="r" (
	set buildoption=r
) else if /i "%buildtype_src%"=="new" (
	set buildoption=new
) else if /i "%buildtype_src%"=="n" (
	set buildoption=new
) else if /i "%buildtype_src%"=="clean" (
	set buildoption=clean
) else if /i "%buildtype_src%"=="c" (
	set buildoption=clean
) else if /i "%buildtype_src%"=="h" (
	set buildoption=h
) else if /i "%buildtype_src%"=="-h" (
	set buildoption=h
) else if /i "%buildtype_src%"=="help" (
	set buildoption=h
) else if /i "%buildtype_src%"=="/h" (
	set buildoption=h
) else if /i "%buildtype_src%"=="/?" (
	set buildoption=h
) else if /i "%buildtype_src%"=="?" (
	set buildoption=h
) else (
	echo !!!unknown build type: %buildtype_src%, should be r/new/clean/h/-h!!!
	echo=
	goto helpinfo
)

echo=

if /i "%buildoption%"=="h" (
	goto buildinfo
)

if /i "%buildoption%"=="clean" (
	goto buildstart
)

echo %PATH%
echo=

set ql_buildproj=%2
if "%ql_buildproj%"=="" (
	set ql_buildproj=%def_proj%
)

if /i "%3" == "" (
	echo=
	echo we need your version label...
	echo=
	goto helpinfo
)

set buildver=%3

for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call set buildver=%%buildver:%%i=%%i%%

set outputtype=%6
if /i not "%outputtype%"=="debug" (
	set outputtype=release
)

set _ccsdk_build=ON
set _OPEN_=OPEN_CPU
set PROJECT_ROOT=%CD%
set BUILD_TARGET=8915DM_cat1_open
set BUILD_RELEASE_TYPE=%outputtype%
set ql_app_ver=%buildver%_APP

if not exist "%PROJECT_ROOT%\components\ql-config\build\%ql_buildproj%\%BUILD_TARGET%" (
	echo.
	echo ********************        ERROR        ***********************
	echo your target.config is not exist:
	echo     %PROJECT_ROOT%\components\ql-config\build\%ql_buildproj%\%BUILD_TARGET%
	echo ****************************************************************
	echo.
	set ret=1
	goto buildinfo
)


set KCONFIG_CONFIG=%PROJECT_ROOT%\components\ql-config\build\%ql_buildproj%\%BUILD_TARGET%\target.config
set target_out_dir=out\%BUILD_TARGET%_%BUILD_RELEASE_TYPE%
set PROJECT_OUT=%PROJECT_ROOT%\%target_out_dir%

set region=%ql_buildproj:~6,3%
if /i "%region%" == "CN_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\cn
) else if /i "%region%" == "EU_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\eu
) else if /i "%region%" == "EC_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\eu
) else if /i "%region%" == "LA_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\la
) else if /i "%region%" == "AU_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\au
) else if /i "%region%" == "GL_" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\gl
) else if /i "%region%" == "EAL" (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\eal
) else  (
	set IMS_DELTA_NV_ROOT=components\ql-kernel\modem\ims\xx
)
echo %IMS_DELTA_NV_ROOT%

call :add_path %PROJECT_ROOT%\prebuilts\win32\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\cmake\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\python3
call :add_path %PROJECT_ROOT%\prebuilts\win32\gcc-arm-none-eabi\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\gcc-mips-rda-elf\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\gcc-rv32-elf\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\nanopb
call :add_path %PROJECT_ROOT%\tools
call :add_path %PROJECT_ROOT%\tools\win32
::call :add_path %PROJECT_ROOT%\tools\win32\vlrsign

:buildstart
set optionhelp=0
if /i "%buildoption%"=="clean" (
	echo=
	rd /s/q out
	echo=
	echo cleaning done
	echo=
	goto doneseg
) else if /i "%buildoption%"=="r" (
	echo=
	goto buildseg
) else if /i "%buildoption%"=="new" (
	echo cleaning...
	echo=
	rd /s/q out
	echo=
	echo cleaning done
	echo=
	goto buildseg
) else (
	set optionhelp=1
	goto doneseg
)

:buildseg
set volte_enable=%4
set quec_dsim=%5
set ql_dsim_cfg=n

::set vsim or vsim2 for VSIM STD solution
set ql_vsim_std=vsim2

echo %volte_enable% %quec_dsim% %build_tasknum%

set modemdir=cat1_UIS8915DM_BB_RF_SS_cus
set partitionfile=components/hal/config/8910/partinfo_8910_8m_opencpu.json
if /i not "%quec_dsim%"=="DOUBLESIM" (
	set quec_dsim=SINGLESIM
)
if /i "%quec_dsim%"=="DOUBLESIM" (
	set ql_dsim_cfg=y
	set modemdir=cat1_UIS8915DM_BB_RF_DS_cus
	set partitionfile=components/hal/config/8910/partinfo_8910_8m_opencpu_ds.json
)
set prepack_json_path=components/ql-config/download/prepack/ql_prepack.json
set ap_ram_offset=0xC00000
set ap_ram_size=0x400000
set ims_delta_nv=y
set quec_ims_feature=y
if /i not "%volte_enable%"=="VOLTE" (
	set volte_enable=NOVOLTE
)
if /i "%volte_enable%"=="NOVOLTE" (
	set quec_ims_feature=n
	set ap_ram_offset=0x980000
	set ap_ram_size=0x680000
	set ims_delta_nv=n
	set modemdir=cat1_UIS8915DM_BB_RF_SS_NoVolte_cus
	set partitionfile=components/hal/config/8910/partinfo_8910_8m_opencpu_novolte.json
	if /i "%quec_dsim%"=="DOUBLESIM" (
		set modemdir=cat1_UIS8915DM_BB_RF_DS_NoVolte_cus
	)
)

call python3.exe build_check.py --partinfo %partitionfile%

if not %errorlevel% == 0 (
	echo.
	echo ****************************************************************
	echo xxxxxxxxxxxxxxxxx  flash partition error  xxxxxxxxxxxxxxxxxxxxxx
	echo ****************************************************************
	set ret=1
	goto doneseg
)

if /i "%ql_vsim_std%"=="vsim" (
    set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim.json
    if /i "%quec_dsim%"=="DOUBLESIM" (
        set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim_ds.json
    )
    if /i "%volte_enable%"=="NOVOLTE" (
        set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim_novolte.json
    )
) else if /i "%ql_vsim_std%"=="vsim2" (
    set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim2.json
    if /i "%quec_dsim%"=="DOUBLESIM" (
        set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim2_ds.json
    )
    if /i "%volte_enable%"=="NOVOLTE" (
        set partitionfile=components/hal/config/8910/partinfo_8910_8m_quec_vsim2_novolte.json
    )
)

::if exist prebuilts\modem\8910\%modemdir%\nvitem\*.prj del /f/a prebuilts\modem\8910\%modemdir%\nvitem\*.prj
copy /y "components\ql-config\build\%ql_buildproj%\nvitem\delta_base.nv" "components\nvitem\8910\default\deltanv\"
copy /y "components\ql-config\build\%ql_buildproj%\nvitem\delta_ims.nv" "components\nvitem\8910\default\deltanv\"
copy /y "components\ql-config\build\%ql_buildproj%\nvitem\delta_simcard2.nv" "components\nvitem\8910\default\deltanv\"
copy /y "components\ql-config\build\public\delta_base_common.nv" "components\nvitem\8910\default\deltanv\"
copy /y "components\ql-config\build\public\delta_ims_common.nv" "components\nvitem\8910\default\deltanv\"
copy /y "components\ql-config\build\public\delta_simcard2_common.nv" "components\nvitem\8910\default\deltanv\"
::if /i "%volte_enable%"=="NOVOLTE" (
::	if /i "%quec_dsim%"=="DOUBLESIM" (
::		copy /y components\ql-config\build\%ql_buildproj%\nvitem\nvitem_modem_novolte_ds.prj prebuilts\modem\8910\%modemdir%\nvitem\nvitem_modem.prj
::	) else (
::		copy /y components\ql-config\build\%ql_buildproj%\nvitem\nvitem_modem_novolte.prj prebuilts\modem\8910\%modemdir%\nvitem\nvitem_modem.prj
::	)
::) else (
::	if /i "%quec_dsim%"=="DOUBLESIM" (
::		copy /y components\ql-config\build\%ql_buildproj%\nvitem\nvitem_modem_ds.prj prebuilts\modem\8910\%modemdir%\nvitem\nvitem_modem.prj
::	) else (
::		copy /y components\ql-config\build\%ql_buildproj%\nvitem\nvitem_modem.prj prebuilts\modem\8910\%modemdir%\nvitem\nvitem_modem.prj
::	)
::)

::xcopy /y components\ql-config\build\%ql_buildproj%\quec_proj_config.h components\ql-kernel\inc
::xcopy /y components\ql-config\build\%ql_buildproj%\quec_proj_config_at.h components\ql-kernel\inc
xcopy /y components\ql-config\build\%ql_buildproj%\ql_libs\libql_at.a components\ql-kernel\libs

Setlocal Enabledelayedexpansion

if not exist %PROJECT_OUT% mkdir %PROJECT_OUT%
if not exist %PROJECT_OUT%\include mkdir %PROJECT_OUT%\include
cd /d out\%BUILD_TARGET%_%BUILD_RELEASE_TYPE%

echo=

echo %PATH%

echo=

cmake ..\.. -G Ninja

echo=
if not "%build_tasknum%"=="" (
	set /a tasknumCheck=%build_tasknum%
	if "!tasknumCheck!" == "%build_tasknum%" (
		ninja -j !tasknumCheck!
	) else (
		cd /d %PROJECT_ROOT%
		echo.
		echo ****************************************************************
		echo xxxxxxxxxxxxxxxxxx  ninja tasknum error   xxxxxxxxxxxxxxxxxxxxxx
		echo ****************************************************************
		set ret=1
		goto doneseg
	)
) else (
	ninja
)

if not %errorlevel% == 0 (
	cd /d %PROJECT_ROOT%
	echo.
	echo.
	echo ********************        ERROR        ***********************
	echo ********************        ERROR        ***********************
	echo ****************************************************************
	echo xxxxxxxxxxxxxxxxxx   build ended error   xxxxxxxxxxxxxxxxxxxxxxx
	echo ****************************************************************
	echo ****************************************************************
	@echo 
	@echo 
	@echo 
	set ret=1
	goto doneseg
)

echo=
echo.
echo ********************        PASS         ***********************
echo **************   build ended successfully   ********************
echo ********************        PASS         ***********************
@echo 
echo=

call python3.exe ..\..\build_check.py --partinfo ..\..\%partitionfile% --coreimg .\hex\%BUILD_TARGET%.img

if %errorlevel% NEQ 0 (
	cd /d %PROJECT_ROOT%
	echo=
	echo ********************          ERROR          ***********************
	echo ********************    FILE SIZE ERROR      ***********************
	set ret=1
	goto doneseg
)

call python3.exe ..\..\build_check.py --partinfo ..\..\%partitionfile% --appimg .\hex\examples\%ql_app_ver%.img

if %errorlevel% NEQ 0 (
	cd /d %PROJECT_ROOT%
	echo=
	echo ********************          ERROR          ***********************
	echo ********************    FILE SIZE ERROR      ***********************
	set ret=1
	goto doneseg
)

echo=

echo %ql_buildproj% %buildver% %BUILD_RELEASE_TYPE% %volte_enable% %quec_dsim% %build_tasknum%
echo=

set version_path=..\..\target\%ql_buildproj%_%buildver%\

if exist %version_path%\ rd /s/q %version_path%\
mkdir %version_path%\

::每次编译完删除out目录下生成的modemgen，避免-r编译导致NV没有更新
rd /s/q %PROJECT_OUT%\modemgen

xcopy /y .\hex %version_path%\prepack\
move /y %version_path%\prepack\%BUILD_TARGET%.elf %version_path%\
move /y %version_path%\prepack\%BUILD_TARGET%.map %version_path%\
move /y %version_path%\prepack\%BUILD_TARGET%*%BUILD_RELEASE_TYPE%.pac %version_path%\
rename %version_path%\%BUILD_TARGET%*%BUILD_RELEASE_TYPE%.pac %BUILD_TARGET%_%buildver%.pac
xcopy .\target.cmake %version_path%\prepack\
xcopy /y .\hex\examples\*.* %version_path%\app\

set ql_prepack_opt=N
if exist ql_prepack.opt (
	set ql_prepack_opt=Y
)

set ql_ext_app=N
if exist ql_ext_app.opt (
	set ql_ext_app=Y
)

if /i "%ql_prepack_opt%"=="Y" (
	if /i "%ql_ext_app%"=="Y" (
		..\..\tools\win32\dtools pacmerge --id APPIMG,PS --id APPIMG2,PS --id PREPACK,NV %version_path%\%BUILD_TARGET%_%buildver%.pac %version_path%\app\%ql_app_ver%.pac %version_path%\%BUILD_TARGET%_%buildver%_merge.pac
	) else (
		..\..\tools\win32\dtools pacmerge --id APPIMG,PS --id PREPACK,NV %version_path%\%BUILD_TARGET%_%buildver%.pac %version_path%\app\%ql_app_ver%.pac %version_path%\%BUILD_TARGET%_%buildver%_merge.pac
	)
) else (
	if /i "%ql_ext_app%"=="Y" (
		..\..\tools\win32\dtools pacmerge --id APPIMG,PS --id APPIMG2 %version_path%\%BUILD_TARGET%_%buildver%.pac %version_path%\app\%ql_app_ver%.pac %version_path%\%BUILD_TARGET%_%buildver%_merge.pac
	) else (
		..\..\tools\win32\dtools pacmerge --id APPIMG,PS %version_path%\%BUILD_TARGET%_%buildver%.pac %version_path%\app\%ql_app_ver%.pac %version_path%\%BUILD_TARGET%_%buildver%_merge.pac
	)
)
echo=

call pythonw.exe ..\..\tools\codesize.py --map .\hex\%BUILD_TARGET%.map
move /y .\outlib.csv %version_path%\prepack\
move /y .\outobj.csv %version_path%\prepack\
move /y .\outsect.csv %version_path%\prepack\

cd /d %PROJECT_ROOT%

:doneseg
if "%optionhelp%"=="0" (
	echo=
	echo %date%
	echo START TIME:  %Start_at%
	echo END TIME:    %time%
	echo=
	goto exit
)

:helpinfo
echo=
echo ********************************************************************
echo !!!!!!!!!!!      Please input right build option      !!!!!!!!!!!!!!
echo ********************************************************************
echo=
:buildinfo
echo For your information:
echo.Usage: %~n0 r/new Project Version [VOLTE] [DSIM] [debug/release]
echo.       %~n0 clean
echo.       %~n0 h/-h
echo.Example:
echo.       %~n0 new %def_proj% your_version_label
echo.       %~n0 new %def_proj% your_version_label VOLTE SINGLESIM debug
echo.       %~n0 new %def_proj% your_version_label NOVOLTE DOUBLESIM release
echo=
echo Note: Optional parameter: [VOLTE] [DSIM]
echo       The previous optional parameter is required for the latter optional parameter.
echo       For example, If you need DOUBLESIM with NOVOLTE, please set [VOLTE] as NOVOLTE and [DSIM] as DOUBLESIM.
echo=
echo       [VOLTE] option is for VoLTE feature, and NOVOLTE is default.
echo       If you want volte, you can ignore [VOLTE] with SINGLESIM or set it as VOLTE. If you want no volte, please set [VOLTE] as NOVOLTE!
echo       [DSIM] option is for single or double sim card. and SINGLESIM is default.
echo       DOUBLESIM is used for Dual SIM Dual Standby(DSDS), and SINGLESIM is used for single SIM or Dual SIM Single Standby(DSSS).
echo       furthermore, you should enable the CONFIG_QUEC_PROJECT_FEATURE_DSSS feature for DSSS.
echo=
echo you can get supported [Project] at the directory components\ql-config\build,
echo the name of folders are supported [Project], as below:
echo=
dir components\ql-config\build\
echo=
echo=

:exit
exit /B %ret%

:add_path
(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
goto :eof
