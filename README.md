# Onomondo SoftSIM for Quectel QuecOpen SDK

The Onomondo SoftSIM is an [Open Source](https://github.com/onomondo/onomondo-uicc) C based UICC implementation, allowing new and innovative cellular device designs to see the light of day in the ever-growing landscape of IoT!

This repository integrates it into Quectel's QuecOpen SDK for Unisoc LTE Cat 1 modules such as the EG912U-GL, EG912U-EAL, EG915U-EU and EG915U-LA. The modem forwards its SIM requests to an application image on the module, where onomondo-uicc answers them.

This README is the quick path to a provisioned module. How it works, how to build for another module, and how provisioning behaves are in [`docs/`](docs/README.md).

## Quick start

1. **Clone and add the SDK.**

   ```
   git clone --recurse-submodules git@github.com:onomondo/softsim-quecopen-unisoc-lte.git
   ```

   The QuecOpen SDK (`LTE01R03A07_C_SDK_U`) cannot be published with this repository; obtain it from Quectel and extract it over the [`LTE01R03A07_C_SDK_U/`](LTE01R03A07_C_SDK_U/README.md) placeholder.

2. **Build the firmware** in the Docker container:

   ```
   docker compose run --rm dev ./build.sh
   ```

   This builds for the EG912U-GL ([other modules](docs/integration.md#choosing-a-module)). The image is
   `LTE01R03A07_C_SDK_U/target/EG912UGL_AA_SOFTSIM_DEBUG_V1.0.0/8915DM_cat1_open_SOFTSIM_DEBUG_V1.0.0_merge.pac`.

3. **Flash it** with the download tools Quectel supplies with the SDK. Flashing erases the module's user filesystem, provisioned profile included.

4. **Fetch a profile.** Profiles are delivered through Onomondo's API, wrapped by the
   [onomondo-softsim-cli](https://github.com/onomondo/onomondo-softsim-cli). Generate an API key
   at [app.onomondo.com/api-keys](https://app.onomondo.com/api-keys), then:

   ```
   ./softsim fetch --api-key <your_api_key> -n 1    # download 1 encrypted profile into ./profiles
   ./softsim next --key=<path to your private key>  # decrypt and print the next unused profile
   ```

5. **Provision the module** on its AT port, then reset it so the modem reads the new SIM:

   ```
   AT+SOFTSIM="<profile>"
   OK
   AT+CFUN=1,1
   ```

   After the reboot, `AT+CIMI` and `AT+QCCID` return the profile's IMSI and ICCID, and the
   SIM shows up as connected on the [Onomondo platform](https://app.onomondo.com/).

At a glance:

```
            ┌───────────────────────────────────────────────┐
LTE ~~~)))  │  Modem · Quectel vSIM core image              │
            └───────────┬─────────────────────▲─────────────┘
                        │                     │
                        │  reset · APDU       │  ATR · response APDU + SW
                        │                     │
            ┌───────────▼─────────────────────┴─────────────┐
            │  SoftSIM application image (this repo)        │◄─── AT+SOFTSIM="<profile>"
            │  vSIM handlers · AT commands                  │
            ├───────────────────────────────────────────────┤
            │  onomondo-uicc — the SIM core                 │
            ├───────────────────────────────────────────────┤
            │  QuecOpen ports: UFS:/softsim · heap · log    │
            └───────────────────────────────────────────────┘
```

## License

The SoftSIM for QuecOpen SDK repository, softsim-quecopen-unisoc-lte, is provided under:

- SPDX-License-Identifier: GPL-3.0-only

This repository is licensed under the terms of the GNU General Public License version 3 only, as provided in:

- [LICENSE](LICENSE)
