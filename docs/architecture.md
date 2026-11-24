# Architecture

The modem runs Quectel's vSIM core image. With the SIM type set to SoftSIM, it forwards every
SIM reset and APDU to the application image, where `prv_process_reset()` and
`prv_process_apdu()` answer them with onomondo-uicc. The handlers, the boot flow and the AT
commands live in [`softsim_vsim_img_demo.c`](../onomondo/src/softsim_vsim_img_demo.c); the
build installs it as the SDK's `vsim_img` application.

Quectel's *EC200U & EG91xU Series QuecOpen (SDK) vSIM Development Guide* also describes a vSIM
adapt approach. It hit blockers with the power-on indication and SIM initialization at power
on, so this integration uses the vSIM image approach.

## Boot flow

1. `appimg_enter()` registers the [AT commands](provisioning.md#at-commands) and the vSIM
   event callback (`ql_vsim_adapt_register_callback()`). The SDK documents that a callback
   registered late can miss `QUEC_VSIM_ADAPT_POWERON_IND`, so keep the registration early.
2. On `QUEC_VSIM_ADAPT_POWERON_IND` the callback points onomondo-uicc's storage at
   `UFS:/softsim`, rebuilds the [filesystem](#the-filesystem) if it is not valid, creates the
   SIM context and calls `ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, ...)`. From
   then on the SoftSIM is the module's SIM.

## The filesystem

The SIM's files live on the module's user filesystem under `UFS:/softsim`, each with a `.def`
file holding its FCP. The template, a complete SIM with no identity, is compiled
in from [`ss_static_files_hex.c`](../onomondo/ss_static_files_hex.c). At power on
[`ss_bootstrap.c`](../onomondo/ss_bootstrap.c) checks for the MF, EF.ICCID and EF.IMSI and, if
any is missing, writes the whole template.

[Provisioning](provisioning.md) and OTA updates change the files in place.

## Ports

onomondo-uicc is built with `CONFIG_NO_DEFAULT_IMPL=y`; [`onomondo/`](../onomondo) supplies the
platform side:

| File | Provides |
|---|---|
| [`ss_fs.c`](../onomondo/ss_fs.c) | The stdio-like storage interface (`ss_fopen()`, ...) on the SDK's `ql_fs`, and the storage path. |
| [`mem.c`](../onomondo/mem.c) | `port_malloc()` / `port_free()` on the SDK heap. |
| [`log.c`](../onomondo/log.c) | `ss_logp()` onto `QL_LOG`; per-subsystem levels in `subsys_lvl`. |
| [`ss_bootstrap.c`](../onomondo/ss_bootstrap.c) | Filesystem validation and template recreation. |

Crypto and MILENAGE come from onomondo-uicc's own libraries.
