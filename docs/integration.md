# Integration

## What the build does

[`build.sh`](../build.sh) runs in the Docker image from [`Dockerfile`](../Dockerfile) and:

1. Applies [`quectel-compat.patch`](../patch/onomondo-uicc/quectel-compat.patch) to the
   onomondo-uicc submodule. The modem needs a PIN status template (`C6`) in every EF's FCP;
   TS 102 221 only puts it on DFs and ADFs.
2. Builds onomondo-uicc as static libraries with the SDK's ARM toolchain.
3. Copies the libraries, their headers and [`onomondo/`](../onomondo) into the SDK as
   `components/onomondo`, and installs
   [`softsim_vsim_img_demo.c`](../onomondo/src/softsim_vsim_img_demo.c) as
   `components/ql-application/vsim_img/vsim_img_init.c`.
4. Applies the [SDK patches](#sdk-patches) and copies the vSIM core images from
   [`patch/cores/`](../patch/cores) over the SDK's stock ones.
5. Runs the SDK's `build_all.sh` for one module.

The library options are the `CMAKEFLAGS` in `build.sh`. CI
([`build-softsim-lib.yml`](../.github/workflows/build-softsim-lib.yml)) builds the library with
its own copy of them, so change both. CI never builds the firmware: the SDK is not public.

## Choosing a module

The last line of `build.sh` picks the module:

```
./build_all.sh new EG912UGL_AA softsim_debug_v1.0.0 VOLTE
```

Replace `EG912UGL_AA` with `EG912UEAL_AC`, `EG915UEU_AB` or `EG915ULA_AB`. The firmware lands in
`LTE01R03A07_C_SDK_U/target/<MODULE>_SOFTSIM_DEBUG_V1.0.0/`; flash the `_merge.pac`, which holds
both the core and the application image.

Any other QuecOpen module needs a vSIM-enabled core image from Quectel and the vSIM options in
its `target.config` (see `ql-config.patch` below).

## SDK patches

In [`patch/quecopen/`](../patch/quecopen), applied by `build.sh` and skipped when already applied:

| Patch | Why |
|---|---|
| `ql-config.patch` | Enables the vSIM adaptation layer (`CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT`, `CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD`) for each supported module. |
| `vsim-cmake.patch` | Adds the `onomondo` component and links the SoftSIM libraries into the `vsim_img` application. |
| `build_all.patch` | Selects the `vsim2` vSIM standard and replaces two bash-only expansions in `build_all.sh`. |
| `reformat_filesystem_during_download.patch` | Formats the user filesystem on every firmware download, so a flashed module always starts from the template. |

## Flashing

Flash with the download tools Quectel supplies with the QuecOpen SDK. Production use needs a
license: the QLicense tool prints your machine's device ID, which your Quectel sales
representative turns into a license.
