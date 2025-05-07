# Onomondo SoftSIM for Quectel QuecOpen SDK

The Onomondo SoftSIM is an [Open Source](https://github.com/onomondo/onomondo-uicc) C based UICC implementation, allowing new and innovative cellular device designs to see the light of day in the ever-growing landscape of IoT!

This project aims to demonstrate how the SoftSIM UICC form factor can be integrated into one of Quectel's QuecOpen SDK's that covers more than 25 radio modules. This list includes Quectel EG912U-GL, EG912U-EAL, EG915U-EU, and EG915U-LA just to name a few.

## Quick Setup Guide
#### Clone the repository
`git clone --recurse-submodules git@github.com:onomondo/softsim-quecopen-unisoc-lte.git`

#### Build SoftSIM firmware within a docker container
`docker compose run dev ./build.sh`

> [!Note]
> The QuecOpen SDK (LTE01R03A07_C_SDK_U) cannot be published with this repository and must be obtained directly from Quectel.

## Prerequisites

### Obtaining a SoftSIM Profile
For SoftSIM to work, you need to obtain the keys and data that would otherwise be found in you physical SIM. This is done using what we call a SoftSIM profile. Get your free SoftSIM profile by signing up for a [free trial](https://onomondo.com/go/free-trial/).

Onomondo has developed a command line interface for fetching SoftSIM profiles. This tool is expected to be used for profile downloads and profile encoding. The profile encoding is required to comply with the SoftSIM profile format expected by this project.

For more information on how to set up and use the SoftSIM CLI, visit the [Onomondo Help Center](https://help.onomondo.com/en/collections/544860-softsim)

Onomondo strongly encourages that the SoftSIM profile information be managed responsibly and securely without exposure to untrusted third parties.

## SoftSIM Profile Provisioning
Onomondo SoftSIM can be transferred to the radio module using a custom AT command.

### SoftSIM AT Command
As part of the Quectel SDK integration effort, a new custom AT command specifically for SoftSIM profile provisioning.

Test that the SoftSIM AT Command is supported by the radio module firmware:
```
AT+SOFTSIM=?
+SOFTSIM="<profile>"
```
Parse an Onomondo SoftSIM profile (as exported by the Onomondo SoftSIM CLI tool) to the radio module:
```
AT+SOFTSIM="01120809101010325406360214980010325476981032140320000000000000000000
000000000000000420000102030405060708090A0B0C0D0E0F0520000102030405060708090A0B0C
0D0E0F0620000102030405060708090A0B0C0D0E0F"
```

### SoftSIM Activation
After successfully transferring the SoftSIM profile to the radio module, it becomes the responsibility of the module's firmware to decode and utilize the newly transferred profile. This functionality has been developed by Onomondo and integrated into this Quectel SDK project. In this projects current configuration, the Onomondo SoftSIM is the default SIM selected by the Quectel radio module.

The profile transferred using the custom `AT+SOFTSIM` command will take effect following a reset or reboot. The Quectel radio module can, for example, be reset using the AT command: `AT+CFUN=1,1`

Verify that the profile has been correctly written to the SoftSIM filesystem by reading out the ICCID or IMSI. Use either `AT+QCCID` or `AT+CIMI` to confirm that the returned information is provided by the SoftSIM.
Alternatively, retrieve all relevant information at once with: `AT+QGMR;+CIMI;+QCCID;+CPIN?`

### SIM Management and Monitoring
Having successfully flashed and provisioned the SoftSIM on your device, it's time to test it.

1. Log into the [Onomondo platform](https://app.onomondo.com/) with your account information.
2. Check the SIM associated with your device. You'll observe that the connection is established and behaves just like a physical SIM card.

With the Onomondo platform, you have the advantage of diving deep into the SIM's connection details using features like [Network Logs](https://fast.wistia.com/embed/channel/d6nn9stg53?wchannelid=d6nn9stg53&wmediaid=xm90cpagb7), [Traffic Monitor](https://fast.wistia.com/embed/channel/d6nn9stg53?wchannelid=d6nn9stg53&wmediaid=xh5csjjcq8), and [Signaling Logs](https://fast.wistia.com/embed/channel/d6nn9stg53?wchannelid=d6nn9stg53&wmediaid=wq95alp088).

## General Project Information

This repository is the outcome of an Onomondo development project where the onomondo-uicc repository has been introduced to the Quectel QuecOpen SDK.
During the development, the QuecOpen SDK have seen improvements and bug fixes as a result of this projects development efforts.
The following SDK versions have been used during development:
- **LTE01R03A04_C_SDK_U** (January 2024)
- **LTE01R03A06_C_SDK_U** (April 2024)
- **LTE01R03A07_C_SDK_U** (January 2025)

### Firmware Flashing Tools

For firmware flashing and general provisioning of your radio module, please refer the tools supplied by Quectel alongside the QuecOpen SDK. Quectel offers three different firmware downloading tools to choose from: ***QExplorer***, ***QFirehose***, and ***QLog***.

For production usage, each tool requires you to obtain a license. You can use the "QLicense tool" to get the device ID of your machine, share it with your Quectel sales representative, and a license will be issued.

### Design

#### vSIM Image
The main integration of the SoftSIM functionality within the SDK is found in [softsim_vsim_img_demo.c](onomondo/src/softsim_vsim_img_demo.c). The integration includes an implementation of callback functions used during module booting, and as stated in the SDK documentation, the callback function is to be registered as soon as possible after booting; otherwise, the `QUEC_VSIM_ADAPT_POWERON_IND` event may not be obtained.

```mermaid
    flowchart LR
    B(["ql_vsim_adapt_register_callback() <br>"]) --> C(["QUEC_VSIM_ADAPT_POWERON_IND"])
    C --> n2(["ql_vsim_adapt_set_sim_type()"])
```

An APDU and ATR interface that ties to the modules kernel have been written as well to process this communication and tie the kernel to the onomondo-uicc. The kernel will utilize this custom interface through `ql_vsim_adapt_handler_s` when the SIM type is set to `QL_VSIM_ADAPT_SIM_TYPE_SSIM`.

The structure of the APDU and ATR interface is defined as:
```
typedef struct {
    ql_vsim_adapt_process_apdu  process_apdu;
    ql_vsim_adapt_reset         reset;
} ql_vsim_adapt_handler_s
```
With the following parameters:
| Function    | Parameter | Description |
| -------- | ------- | ------- |
| ql_vsim_adapt_process_apdu | process_apdu | APDU interface implemented in this repository
| ql_vsim_adapt_reset | reset | ATR interface implemented in this repository

Diving into the `ql_vsim_adapt_process_apdu()` function prototype:
```
int prv_process_apdu(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp, uint16_t *apdu_rsp_len, uint8_t slot)
```
| Function    | Parameter | Direction | Description |
| -------- | ------- | ------- | ------- |
| ql_vsim_adapt_process_apdu | apdu_req | in | Command APDU
|  | apdu_req_len | in | The length of command APDU
|  | apdu_rsp | out | Response APDU
|  | apdu_rsp_len | out | The length of Response APDU
|  | slot | in | The (U)SIM card used. Set this parameter to 0 if the module supports only one (U)SIM interface

The return value of this function is a SW1-SW2 status word.

Diving into the `ql_vsim_adapt_reset()` function prototype:
```
uint16_t prv_process_reset(uint8_t *atr_data, uint8_t *atr_size, uint8_t nSimID)
```
| Function    | Parameter | Direction | Description |
| -------- | ------- | ------- | ------- |
| ql_vsim_adapt_reset | atr_data | out | The returned ATR data
|  | atr_size | out | The length of the returned ATR data
|  | nSimID | in | The (U)SIM card used. Set this parameter to 0 if the module supports only one (U)SIM interface

The return value of this function is `0` upon successful execution.

#### vSIM Adapt
For more information about the vSIM adapt approach, please refer to the Quectels document named *Quectel_EC200U&EG91xU_Series_QuecOpen(SDK)_vSIM_Development_Guide.pdf*. We have had some blockers on this approach, especially with the poweron indicator and initialization of the SIM when the module is powered on. The integration [softsim_adapt_demo.c](onomondo/src/softsim_adapt_demo.c) is for now kept in the repository in case fixes are released to the QuecOpen SDK. Until then, we recommend using the [vSIM Image](#vsim-image) approach.

### Wrappers
The core of this integration is the implementation of wrappers that tie onomondo-uicc to the SDK. Wrappers for the filesystem are provided in [ss_fs.c](onomondo/ss_fs.c), memory functions are wrapped in [mem.c](onomondo/mem.c), and onomondo-uicc debug logging is enabled via the wrapper in [log.c](onomondo/log.c).

### License
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

The SoftSIM for QuecOpen SDK repository, softsim-quecopen-unisoc-lte, is provided under:

- SPDX-License-Identifier: GPL-3.0-only

Being under the terms of the GNU General Public License version 3 only,
according with:

- LICENSE

### Contributing
Contributions are very welcome. Please feel free to submit issues or open pull requests for review.
