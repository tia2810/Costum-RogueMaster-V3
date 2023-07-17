#pragma once

typedef enum {
    // Reserve first 100 events for button types and indexes, starting from 0
    NfcCustomEventReserved = 100,

    // Mf classic dict attack events
    NfcCustomEventDictAttackComplete,
    NfcCustomEventDictAttackSkip,
    NfcCustomEventDictAttackCardDetected,
    NfcCustomEventDictAttackCardNotDetected,
    NfcCustomEventDictAttackFoundKeyA,
    NfcCustomEventDictAttackFoundKeyB,
    NfcCustomEventDictAttackNewSector,
    NfcCustomEventDictAttackNewKeyBatch,
    NfcCustomEventDictAttackKeyAttackStart,
    NfcCustomEventDictAttackKeyAttackStop,
    NfcCustomEventDictAttackKeyAttackNextSector,

    NfcCustomEventViewExit,
    NfcCustomEventWorkerExit,
    NfcCustomEventWorkerUpdate,
    NfcCustomEventTimerExpired,
    NfcCustomEventByteInputDone,
    NfcCustomEventTextInputDone,
    NfcCustomEventDictAttackDone,
    NfcCustomEventRpcLoad,
    NfcCustomEventRpcSessionClose,

    NfcCustomEventPollerSuccess,
    NfcCustomEventPollerFailure,

    NfcCustomEventListenerUpdate,
} NfcCustomEvent;
