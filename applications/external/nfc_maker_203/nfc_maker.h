#include <gui/gui.h>
#include <gui/view.h>
#include <gui/modules/validators.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include "nfc_maker_icons.h"
#include <gui/modules/submenu.h>
#include "nfc_maker_text_input.h"
#include <gui/modules/byte_input.h>
#include <gui/modules/popup.h>
#include "scenes/nfc_maker_scene.h"
#include <lib/flipper_format/flipper_format.h>
#include <toolbox/name_generator.h>
#include <furi_hal_bt.h>
#include "newstrnlen.h"

#define NFC_MK_APP_FOLDER EXT_PATH("nfc")
#define NFC_MK_APP_EXTENSION ".nfc"
#define TEXT_INPUT_LEN 248

typedef struct {
    Gui* gui;
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    NFCMaker_TextInput* text_input;
    ByteInput* byte_input;
    Popup* popup;

    uint8_t mac_buf[GAP_MAC_ADDR_SIZE];
    char text_buf[TEXT_INPUT_LEN];
    char name_buf[TEXT_INPUT_LEN];
} NfcMaker;

typedef enum {
    NfcMakerViewSubmenu,
    NfcMakerViewTextInput,
    NfcMakerViewByteInput,
    NfcMakerViewPopup,
} NfcMakerView;
