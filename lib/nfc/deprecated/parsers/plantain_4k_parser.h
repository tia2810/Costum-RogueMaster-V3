#pragma once

#include "nfc_supported_card.h"

bool plantain_4k_parser_verify(NfcWorker* nfc_worker, FuriHalNfcTxRxContext* tx_rx);

bool plantain_4k_parser_read(NfcWorker* nfc_worker, FuriHalNfcTxRxContext* tx_rx);

bool plantain_4k_parser_parse(NfcDeviceOldDataOld* dev_data);
