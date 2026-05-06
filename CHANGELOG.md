# Changelog

## [1.0.0](https://github.com/onomondo/softsim-quecopen-unisoc-lte/releases/tag/v1.0.0) (2025-05-08)

### Initial Version

* Initial SoftSIM integration for Quectel QuecOpen LTE01R03A07_C_SDK_U ([ecf9242](https://github.com/onomondo/softsim-quecopen-unisoc-lte/commit/ecf92422a5d596b47397ada7abc50b6e927b0ecb))

### Features

* SoftSIM (onomondo-uicc) integration as a static library built against the QuecOpen SDK
* QuecOpen SDK patches for vSIM support (`reformat_filesystem_during_download`, `ql-config`, `vsim-cmake`, `build_all`)
* vSIM core images for EG912UGL-AA, EG912UEAL-AC, EG915UEU-AB, and EG915ULA-AB
* Docker build environment for reproducible SDK builds
