#include "mf_ultralight.h"

#include <furi.h>

static const uint16_t mf_ultralight_total_pages[MfUltralightTypeNum] = {
    [MfUltralightTypeUnknown] = 16,
    [MfUltralightTypeNTAG203] = 42,
    [MfUltralightTypeUL11] = 20,
    [MfUltralightTypeUL21] = 41,
    [MfUltralightTypeNTAG213] = 45,
    [MfUltralightTypeNTAG215] = 135,
    [MfUltralightTypeNTAG216] = 231,
    [MfUltralightTypeNTAGI2C1K] = 231,
    [MfUltralightTypeNTAGI2C2K] = 485,
    [MfUltralightTypeNTAGI2CPlus1K] = 236,
    [MfUltralightTypeNTAGI2CPlus2K] = 492,
};

static const uint32_t mf_ultarlight_feature_sets[MfUltralightTypeNum] = {
    [MfUltralightTypeUnknown] = MfUltralightFeatureSupportCompatibleWrite,
    [MfUltralightTypeNTAG203] = MfUltralightFeatureSupportCompatibleWrite |
                                MfUltralightFeatureSupportCounterInMemory,
    [MfUltralightTypeUL11] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportReadCounter | MfUltralightFeatureSupportCheckTearingFlag |
        MfUltralightFeatureSupportFastRead | MfUltralightFeatureSupportIncCounter |
        MfUltralightFeatureSupportCompatibleWrite | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportVcsl,
    [MfUltralightTypeUL21] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportReadCounter | MfUltralightFeatureSupportCheckTearingFlag |
        MfUltralightFeatureSupportFastRead | MfUltralightFeatureSupportIncCounter |
        MfUltralightFeatureSupportCompatibleWrite | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportVcsl,
    [MfUltralightTypeNTAG213] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportReadCounter | MfUltralightFeatureSupportFastRead |
        MfUltralightFeatureSupportCompatibleWrite | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportSingleCounter | MfUltralightFeatureSupportAsciiMirror,
    [MfUltralightTypeNTAG215] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportReadCounter | MfUltralightFeatureSupportFastRead |
        MfUltralightFeatureSupportCompatibleWrite | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportSingleCounter | MfUltralightFeatureSupportAsciiMirror,
    [MfUltralightTypeNTAG216] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportReadCounter | MfUltralightFeatureSupportFastRead |
        MfUltralightFeatureSupportCompatibleWrite | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportSingleCounter | MfUltralightFeatureSupportAsciiMirror,
    [MfUltralightTypeNTAGI2C1K] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportFastRead |
        MfUltralightFeatureSupportFastWrite | MfUltralightFeatureSupportSectorSelect,
    [MfUltralightTypeNTAGI2C2K] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportFastRead |
        MfUltralightFeatureSupportFastWrite | MfUltralightFeatureSupportSectorSelect,
    [MfUltralightTypeNTAGI2CPlus1K] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportFastRead | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportSectorSelect,
    [MfUltralightTypeNTAGI2CPlus2K] =
        MfUltralightFeatureSupportReadVersion | MfUltralightFeatureSupportReadSignature |
        MfUltralightFeatureSupportFastRead | MfUltralightFeatureSupportAuthentication |
        MfUltralightFeatureSupportSectorSelect,
};

MfUltralightType mf_ultralight_get_type_by_version(MfUltralightVersion* version) {
    furi_assert(version);

    MfUltralightType type = MfUltralightTypeUnknown;

    if(version->storage_size == 0x0B || version->storage_size == 0x00) {
        type = MfUltralightTypeUL11;
    } else if(version->storage_size == 0x0E) {
        type = MfUltralightTypeUL21;
    } else if(version->storage_size == 0x0F) {
        type = MfUltralightTypeNTAG213;
    } else if(version->storage_size == 0x11) {
        type = MfUltralightTypeNTAG215;
    } else if(version->prod_subtype == 5 && version->prod_ver_major == 2) {
        if(version->prod_ver_minor == 1) {
            if(version->storage_size == 0x13) {
                type = MfUltralightTypeNTAGI2C1K;
            } else if(version->storage_size == 0x15) {
                type = MfUltralightTypeNTAGI2C2K;
            }
        } else if(version->prod_ver_minor == 2) {
            if(version->storage_size == 0x13) {
                type = MfUltralightTypeNTAGI2CPlus1K;
            } else if(version->storage_size == 0x15) {
                type = MfUltralightTypeNTAGI2CPlus2K;
            }
        }
    } else if(version->storage_size == 0x13) {
        type = MfUltralightTypeNTAG216;
    }

    return type;
}

uint16_t mf_ultralight_get_pages_total(MfUltralightType type) {
    return mf_ultralight_total_pages[type];
}

uint32_t mf_ultralight_get_feature_support_set(MfUltralightType type) {
    return mf_ultarlight_feature_sets[type];
}

const char* mf_ultralight_get_name(MfUltralightType type, bool full_name) {
    if(type == MfUltralightTypeNTAG213) {
        return "NTAG213";
    } else if(type == MfUltralightTypeNTAG215) {
        return "NTAG215";
    } else if(type == MfUltralightTypeNTAG216) {
        return "NTAG216";
    } else if(type == MfUltralightTypeNTAGI2C1K) {
        return "NTAG I2C 1K";
    } else if(type == MfUltralightTypeNTAGI2C2K) {
        return "NTAG I2C 2K";
    } else if(type == MfUltralightTypeNTAGI2CPlus1K) {
        return "NTAG I2C Plus 1K";
    } else if(type == MfUltralightTypeNTAGI2CPlus2K) {
        return "NTAG I2C Plus 2K";
    } else if(type == MfUltralightTypeNTAG203) {
        return "NTAG203";
    } else if(type == MfUltralightTypeUL11 && full_name) {
        return "Mifare Ultralight 11";
    } else if(type == MfUltralightTypeUL21 && full_name) {
        return "Mifare Ultralight 21";
    } else {
        return "Mifare Ultralight";
    }
}

bool mf_ultralight_detect_protocol(NfcaData* nfca_data) {
    furi_assert(nfca_data);

    bool mfu_detected = (nfca_data->atqa[0] == 0x44) && (nfca_data->atqa[1] == 0x00) &&
                        (nfca_data->sak == 0x00);

    return mfu_detected;
}

bool mf_ultralight_get_config_page(MfUltralightData* data, MfUltralightConfigPages** config) {
    furi_assert(data);
    furi_assert(config);

    bool config_pages_found = false;
    switch(data->type) {
    case MfUltralightTypeUL11:
    case MfUltralightTypeUL21:
    case MfUltralightTypeNTAG213:
    case MfUltralightTypeNTAG215:
    case MfUltralightTypeNTAG216:
        *config = (MfUltralightConfigPages*)&data->page[data->pages_total - 4];
        config_pages_found = true;
        break;

    case MfUltralightTypeNTAGI2CPlus1K:
    case MfUltralightTypeNTAGI2CPlus2K:
        *config = (MfUltralightConfigPages*)&data->page[227];
        config_pages_found = true;
        break;

    default:
        break;
    }

    return config_pages_found;
}

bool mf_ultralight_is_all_data_read(MfUltralightData* data) {
    furi_assert(data);

    bool all_read = false;
    if(data->pages_read == data->pages_total) {
        // Having read all the pages doesn't mean that we've got everything.
        // By default PWD is 0xFFFFFFFF, but if read back it is always 0x00000000,
        // so a default read on an auth-supported NTAG is never complete.
        uint32_t feature_set = mf_ultralight_get_feature_support_set(data->type);
        if(feature_set & MfUltralightFeatureSupportAuthentication) {
            all_read = true;
        } else {
            MfUltralightConfigPages* config = NULL;
            if(mf_ultralight_get_config_page(data, &config)) {
                all_read = ((config->password.pass != 0) || (config->pack.pack != 0));
            }
        }
    }

    return all_read;
}

bool mf_ultralight_is_counter_configured(MfUltralightData* data) {
    furi_assert(data);

    MfUltralightConfigPages* config = NULL;
    bool configured = false;

    switch(data->type) {
    case MfUltralightTypeNTAG213:
    case MfUltralightTypeNTAG215:
    case MfUltralightTypeNTAG216:
        if(mf_ultralight_get_config_page(data, &config)) {
            configured = config->access.nfc_cnt_en;
        }
        break;

    default:
        configured = true;
        break;
    }

    return configured;
}
