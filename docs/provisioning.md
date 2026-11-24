# Provisioning

A freshly flashed module carries the filesystem [template](architecture.md#the-filesystem): a
complete SIM with no identity. Provisioning writes one profile's identity and keys into it.

## The profile

A SoftSIM profile is a hex string of TLV records: IMSI, ICCID, keys and, optionally, SMS
parameters. The format and its decoder live in onomondo-uicc, in
`onomondo-uicc/include/onomondo/utils/ss_profile.h`.

Profiles are delivered encrypted through Onomondo's API, and the
[onomondo-softsim-cli](https://github.com/onomondo/onomondo-softsim-cli) fetches and decrypts
them (see the [README](../README.md#quick-start)). A [free trial](https://onomondo.com/go/free-trial/)
gets you one; the [Onomondo Help Center](https://help.onomondo.com/en/collections/544860-softsim)
covers the CLI.

Treat the decrypted string as a secret: whoever holds it holds the SIM's keys.

## AT commands

The application image adds these commands to the module's AT interface:

| Command | Does |
|---|---|
| `AT+SOFTSIM="<profile>"` | Decodes the profile and writes ICCID, IMSI, keys and SMS parameters to the filesystem. `OK` on success; an error if the profile does not decode or a write fails. |
| `AT+SOFTSIM=?` | Prints the syntax. |
| `AT+SOFTSIMKILLFS=1` | Rewrites the whole filesystem from the template, deleting the profile. |
| `AT+QLVER` | Prints the core version and the application image's build date. |

## Activation

The modem reads the SIM at power on, so a new profile takes effect after a reset, for example
`AT+CFUN=1,1`. Then check that the module reports the profile's identity:

```
AT+QGMR;+CIMI;+QCCID;+CPIN?
```

## Re-provisioning

- `AT+SOFTSIM` replaces the identity and keys but keeps the rest of the SIM's state, such as
  authentication sequence numbers and OTA counters. To give a module a new identity on a
  clean SIM, run `AT+SOFTSIMKILLFS=1`, then `AT+SOFTSIM`, then reset.
- Flashing firmware formats the filesystem
  ([`reformat_filesystem_during_download.patch`](integration.md#sdk-patches)), so provision
  again after every flash.
