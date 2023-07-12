#include "nfc_protocol_support_gui_common.h"

#include "nfc/nfc_app_i.h"

void nfc_protocol_support_common_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    NfcApp* instance = context;
    view_dispatcher_send_custom_event(instance->view_dispatcher, index);
}

void nfc_protocol_support_common_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    furi_assert(context);
    NfcApp* instance = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(instance->view_dispatcher, result);
    }
}

void nfc_protocol_support_common_on_enter_empty(NfcApp* instance) {
    UNUSED(instance);
}

bool nfc_protocol_support_common_on_event_empty(NfcApp* instance, uint32_t event) {
    UNUSED(instance);
    UNUSED(event);
    return false;
}
