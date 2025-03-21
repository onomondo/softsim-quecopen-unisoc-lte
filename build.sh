#!/bin/sh

ROOT_DIR=$PWD
SDK_ROOT=$ROOT_DIR/LTE01R03A07_C_SDK_U
SOFTSIM_ROOT=$ROOT_DIR/onomondo-uicc

################
# BUILD SOFTSIM
################
PATH=$SDK_ROOT/prebuilts/linux/cmake/bin:$PATH
PATH=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin:$PATH

# If the build fails due to permissions or compiler errors
# chmod 755 prebuilts/
# try and run chmod -R 755 prebuilts/linux/

echo ""
echo ""
echo "------------------------------------------------------------"
echo "Configuring Onomondo SoftSIM library"
echo "------------------------------------------------------------"
CMAKEFLAGS="-DCMAKE_C_COMPILER=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-gcc"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_AR=$SDK_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-ar"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY" # Compiler can't produce an executable file
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_BUILD_TYPE=Release"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_EXPERIMENTAL_SUSPEND_COMMAND=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_SYSTEM_HEAP=n"
CMAKEFLAGS="$CMAKEFLAGS -DCMAKE_TOOLCHAIN_FILE=$ROOT_DIR/arm-none-eabi.cmake"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_BUILD_LIB_ONLY=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_NO_DEFAULT_IMPL=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_LOGS=y"
CMAKEFLAGS="$CMAKEFLAGS -DCONFIG_USE_UTILS=y"

echo "Build Configuration Options:"
echo $CMAKEFLAGS
echo "------------------------------------------------------------"
echo "Building Onomondo SoftSIM for Quectel EG91x Series Devices"
echo "------------------------------------------------------------"
echo Patching the SoftSIM submodule to generate static library
cp -v $ROOT_DIR/storage-path.patch $SOFTSIM_ROOT/
cp -v $ROOT_DIR/compiler-flags.patch $SOFTSIM_ROOT/
cp -v $ROOT_DIR/path-prefix.patch $SOFTSIM_ROOT/
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

################
# COPY SOFTSIM LIB
################
echo "------------------------------------------------------------"
# Copy the content of the static library release folder to the SDK.
cp -a -p -v	$SOFTSIM_ROOT/lib_/Release/. $SDK_ROOT/components/onomondo
# Copy the SoftSIM headers files to the corresponding include folder of the SDk.
cp -R -p -v $SOFTSIM_ROOT/include $SDK_ROOT/components/onomondo/
# Copy the SoftSIM static files and headers
# cp -R -p -v $SOFTSIM_ROOT/utils/files-c-array/*_hex.c $SDK_ROOT/components/onomondo/
# cp -R -p -v $SOFTSIM_ROOT/utils/files-c-array/*_hex.h $SDK_ROOT/components/onomondo/

################
# BUILD SDK
################
echo "------------------------------------------------------------"
echo "Building Firmware for EG91x Series Devices featuring SoftSIM"
echo "------------------------------------------------------------"

cd $SDK_ROOT
./build_all.sh clean
./build_all.sh r EG912UGL_AA softsim_beta_debug_v0.1.1 VOLTE
./build_all.sh r EG912UEAL_AC softsim_beta_debug_v0.1.1 VOLTE
./build_all.sh r EG915UEU_AB softsim_beta_debug_v0.1.1 VOLTE
./build_all.sh r EG915ULA_AB softsim_beta_debug_v0.1.1 VOLTE

# Small cleanup of untracked build folders
echo "Cleaning up your environment..."
git checkout --no-overlay -- $SDK_ROOT/prebuilts/
git checkout --no-overlay -- $SDK_ROOT/tools/
git checkout --no-overlay -- $SDK_ROOT/components/onomondo/

echo "------------------------------------------------------------"
