/*
 * Copyright (c) 2024 Onomondo ApS. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ss_fs_utils.h"
#include "ss_provisioning.h"
#include "onomondo/softsim/mem.h"
#include "onomondo/softsim/utils.h"
#include "onomondo/softsim/fs.h"
#include "onomondo/utils/ss_profile.h"

static uint8_t write_profile_to_fs(const struct ss_profile *profile)
{
    size_t rc = 0;
    ss_FILE f = NULL;

    // write ICCID
    f = ss_fopen(ICCID_FILE, "w");
    rc = ss_fwrite(profile->_3F00_2FE2, 1, ICCID_LEN, f);
    ss_fclose(f);
    if (rc == 0)
        goto exit;

    // write IMSI
    f = ss_fopen(IMSI_FILE, "w");
    rc = ss_fwrite(profile->_3F00_7ff0_6f07, 1, IMSI_LEN, f);
    ss_fclose(f);
    if (rc == 0)
        goto exit;

    // write A001
    f = ss_fopen(A001_FILE, "w");
    rc = ss_fwrite(profile->_3F00_A001, 1, A001_LEN, f);
    ss_fclose(f);
    if (rc == 0)
        goto exit;

    // write A004
    f = ss_fopen(A004_FILE, "w");
    rc = ss_fwrite(profile->_3F00_A004, 1, A004_LEN, f);
    ss_fclose(f);
    if (rc == 0)
        goto exit;

    // write SMSP
    f = ss_fopen(SMSP_FILE, "w");
    rc = ss_fwrite(profile->SMSP, 1, SMSP_RECORD_SIZE, f);
    ss_fclose(f);
    if (rc == 0)
        goto exit;

    return rc;

exit:
    return rc;
}


uint8_t onomondo_profile_provisioning(const char *at_profile)
{
    struct ss_profile *profile = SS_ALLOC(*profile);

    // Validate the size of the profile hiding in the FS
    uint16_t input_string_size = strlen(at_profile);

    uint8_t rc = ss_profile_from_string(input_string_size, at_profile, profile);
    if (rc != 0)
    {
        SS_FREE(profile);
        return -1;
    }

    rc = write_profile_to_fs(profile);
    if (rc == 0)
    {
        SS_FREE(profile);
        return -1;
    }

    SS_FREE(profile);
    return 0;
}
