#!/bin/sh

ROOT_DIR=$PWD
SDK_ROOT=$ROOT_DIR/LTE01R03A07_C_SDK_U
SOFTSIM_ROOT=$ROOT_DIR/onomondo-uicc

################################
# BUILD SOFTSIM (Onomondo UICC)
################################
chmod --recursive 755 $SDK_ROOT/prebuilts/linux

PATH=$SDK_ROOT/prebuilts/linux/cmake/bin:$PATH
PATH=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin:$PATH

echo "------------------------------------------------------------"
echo "Configuring Onomondo SoftSIM library"
echo "------------------------------------------------------------"
CMAKEFLAGS="-DCMAKE_C_COMPILER=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-gcc"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_AR=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-ar"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_BUILD_TYPE=Release"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_EXPERIMENTAL_SUSPEND_COMMAND=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_SYSTEM_HEAP=n"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_TOOLCHAIN_FILE=$ROOT_DIR/arm-none-eabi.cmake"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_BUILD_LIB_ONLY=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_NO_DEFAULT_IMPL=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_UTILS=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_LOGS=y" # toggle log settings of onomondo-uicc

echo "Build Configuration Options:"
echo $CMAKEFLAGS

echo "------------------------------------------------------------"
echo "Building Onomondo SoftSIM for Quectel EG91x Series Devices"
echo "------------------------------------------------------------"
echo Patching the SoftSIM submodule to generate static library
cp -v $ROOT_DIR/patch/onomondo-uicc/storage-path.patch $SOFTSIM_ROOT/
cp -v $ROOT_DIR/patch/onomondo-uicc/compiler-flags.patch $SOFTSIM_ROOT/
cp -v $ROOT_DIR/patch/onomondo-uicc/path-prefix.patch $SOFTSIM_ROOT/
cd $SOFTSIM_ROOT
git apply storage-path.patch
git apply compiler-flags.patch
git apply path-prefix.patch
set -e
echo SoftSIM Static Library Build
cd $SOFTSIM_ROOT
mkdir -p build
cd build
cmake .. ${CMAKEFLAGS}
make install
set +e

################################
# COPY SOFTSIM LIB and INTEGRATION
################################
echo "------------------------------------------------------------"
cp -R -p $ROOT_DIR/onomondo/ $SDK_ROOT/components/
cp -p -f $ROOT_DIR/onomondo/src/softsim_vsim_img_demo.c $SDK_ROOT/components/ql-application/vsim_img/vsim_img_init.c
cp -a -p -v	$SOFTSIM_ROOT/lib_/Release/. $SDK_ROOT/components/onomondo
cp -R -p -v $SOFTSIM_ROOT/include $SDK_ROOT/components/onomondo/
# Copy the SoftSIM static files and headers (if the default is to be overwritten)
# cp -R -p -v $SOFTSIM_ROOT/utils/files-c-array/*_hex.c $SDK_ROOT/components/onomondo/
# cp -R -p -v $SOFTSIM_ROOT/utils/files-c-array/*_hex.h $SDK_ROOT/components/onomondo/

################################
# APPLY QUECOPEN SDK PATCHES
################################
echo "------------------------------------------------------------"
echo "Applying patches to QuecOpen SDK for Onomondo SoftSIM"
echo "------------------------------------------------------------"
cd $ROOT_DIR
git apply --ignore-space-change --ignore-whitespace $ROOT_DIR/patch/quecopen/reformat_filesystem_during_download.patch
git apply --ignore-space-change --ignore-whitespace $ROOT_DIR/patch/quecopen/ql-config.patch
git apply --ignore-space-change --ignore-whitespace $ROOT_DIR/patch/quecopen/vsim-cmake.patch
git apply --ignore-space-change --ignore-whitespace $ROOT_DIR/patch/quecopen/build_all.patch

################################
# UTILIZE QUECOPEN vSIM CORE IMAGES
################################
cp --force $ROOT_DIR/patch/cores/EG912UGL_AA/8915DM_cat1_EG912UGLAAR03A02M08_BETA250103_VSIM2_core.pac $SDK_ROOT/components/ql-config/download/pack/EG912UGL_AA/8915DM_cat1_open_core.pac
cp --force $ROOT_DIR/patch/cores/EG912UEAL_AC/8915DM_cat1_EG912UEALACR03A01M08_BETA1209_VSIM2_core.pac $SDK_ROOT/components/ql-config/download/pack/EG912UEAL_AC/8915DM_cat1_open_core.pac
cp --force $ROOT_DIR/patch/cores/EG915UEU_AB/8915DM_cat1_EG915UEUABR03A01M08_VSIM2_core.pac $SDK_ROOT/components/ql-config/download/pack/EG915UEU_AB/8915DM_cat1_open_core.pac
cp --force $ROOT_DIR/patch/cores/EG915ULA_AB/8915DM_cat1_EG915ULAABR03A01M08_VSIM2_core.pac $SDK_ROOT/components/ql-config/download/pack/EG915ULA_AB/8915DM_cat1_open_core.pac

################################
# BUILD QUECOPEN SDK w. SOFTSIM
################################
echo "------------------------------------------------------------"
echo "Building Firmware for EG91x Series Devices featuring SoftSIM"
echo "------------------------------------------------------------"
cd $SDK_ROOT
chmod 755 ./build_all.sh
./build_all.sh new EG912UGL_AA softsim_debug_v1.0.0 VOLTE
# Validate build targets: EG912UGL_AA, EG912UEAL_AC, EG915UEU_AB, EG915ULA_AB
