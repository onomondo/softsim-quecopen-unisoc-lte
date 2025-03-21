## Getting Started
### Clone the repository
`git clone --recurse-submodules git@github.com:onomondo/softsim-eg91x.git`
<!-- `git clone --recurse-submodules git@github.com:onomondo/softsim-quecopen-unisoc-lte.git` -->

### Build SoftSIM firmware within a docker container
`docker-compose run dev ./build.sh`

### Flash firmware

For firmware flashing and general provisioning of your desired radio module, please refer to the content of firmware_flashing_tools.zip

Quectel offers three different firmware downloading tools to choose from. For more information on the tools supplied by Quectel, please refer to the attached guides within firmware_flashing_tools.zip:
- *Quectel_QMulti_DL_User_Guide_V2.8.pdf*
- *Quectel_QMulti_DL_CMD_User_Guide_V3.3.pdf*
- *Quectel_Customer_FW_Download_Tool_User_Guide_V3.2.pdf*

For production usage, each tool requires you to obtain a license. You can use the "QLicense tool" to get the device ID of PC, and share it with your Quectel sales representative, and a license will be assigned.

### SoftSIM Profile Provisioning
Onomondo SoftSIM can be transferred to the radio module using a custom AT command.

#### SoftSIM AT Command
As part of the Quectel SDK integration effort, Onomondo has designed a new custom AT command specifically for SoftSIM profile provisioning.

Test that the SoftSIM AT Command is supported by the radio module and its firmware by:
```
AT+SOFTSIM=?
+SOFTSIM="<profile>"
```
Parsing an actual Onomondo SoftSIM profile (as exported by the Onomondo SoftSIM CLI tool) to the radio module would then look like this:
```
AT+SOFTSIM="01120809101010325406360214980010325476981032140320000000000000000000
000000000000000420000102030405060708090A0B0C0D0E0F0520000102030405060708090A0B0C
0D0E0F0620000102030405060708090A0B0C0D0E0F"
```
#### SoftSIM Activation
Having successfully transferred the SoftSIM profile to the radio module, it is the responsibility of the radio module firmware to decode and utilize the newly transferred profile. This development has been undertaken by Onomondo and added to this Quectel SDK project. The Onomondo SoftSIM will be the default SIM selected by the Quectel radio module in its current config.

The profile transferred using the custom `AT+SOFTSIM` command will take effect following a reset or reboot of the radio module. The Quectel radio module can, for example, be reset using the AT command: `AT+CFUN=1,1`.

Test that the profile has made its way into the SoftSIM filesystem within the radio module by reading out the ICCID or IMSI. Use either `AT+QCCID` or `AT+CIMI` and see that either of the commands reads out information provided by the SoftSIM. Or extract all information at once: `AT+QGMR;+CIMI;+QCCID;+CPIN?`

## Prerequsites

### Onomondo SoftSIM profile
Onomondo has developed a command line interface for fetching SoftSIM profiles. This tool is expected to be used for profile downloads and profile encoding. The profile encoding is required to comply with the SoftSIM profile format expected by this project.

For more information on how to set up and use the SoftSIM CLI, visit our Help Center: [SoftSIM CLI tool for provisioning SoftSIM](https://help.onomondo.com/en/softsim-cli-tool-for-provisioning)

Onomondo strongly encourage that the SoftSIM profile information is managed in a responsible and secure manner without exposure to third parties not trusted with this information.

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

### Filesystem
The current state of the Onomondo SoftSIM integration beta into the Quectel SDK will see the SoftSIM filesystem located at `UFS:` which is a general usage user filesystem.

Alternatively, the Quectel SDK allows for the SoftSIM filesystem to be relocated to `SFS:` which is an encrypted filesystem. This does however introduce the need for secure cryptographic key generation and secure key storage.
