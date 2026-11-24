# Onomondo SoftSIM documentation

These pages document **softsim-quecopen-unisoc-lte**, the Quectel QuecOpen integration. Just
want it running? The [top-level README](../README.md) is the quick path.

Two codebases make up the SoftSIM:

| Repository | Role |
|---|---|
| [onomondo-uicc](https://github.com/onomondo/onomondo-uicc) | The SIM. A portable C implementation of a SIM/UICC/USIM, vendored as a git submodule at `onomondo-uicc/` and built as static libraries. |
| softsim-quecopen-unisoc-lte (this repository) | The QuecOpen integration: the vSIM handlers and AT commands, the ports onto the SDK, the SDK patches and the build. |

## Pages

| Page | What it covers |
|---|---|
| [Architecture](architecture.md) | Boot flow, the filesystem, the ports. |
| [Integration](integration.md) | What the build does, choosing a module, the SDK patches, flashing. |
| [Provisioning](provisioning.md) | The profile, the AT commands, activation, re-provisioning. |

## Supported hardware

The integration is built against QuecOpen SDK `LTE01R03A07_C_SDK_U` (January 2025); earlier
development used `LTE01R03A04_C_SDK_U` and `LTE01R03A06_C_SDK_U`. vSIM core images ship for
the EG912U-GL (`EG912UGL_AA`), EG912U-EAL (`EG912UEAL_AC`), EG915U-EU (`EG915UEU_AB`) and
EG915U-LA (`EG915ULA_AB`).
