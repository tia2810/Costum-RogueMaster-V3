#pragma once

#include "flipboard.h"

#include <gui/view_dispatcher.h>
#include <gui/modules/widget.h>
#include "app_menu.h"
#include "flipboard_model.h"
#include "key_config.h"
#include "leds.h"
#include "keyboard.h"

/**
 * @brief    The flipboard struct.
 * @details  This struct contains all the data needed for the flipboard;
 *          the View Dispatcher, the UI components and the FlipboardModel.
 */
struct Flipboard {
    ViewDispatcher* view_dispatcher;

    AppMenu* app_menu;
    KeyConfig* key_config;
    View* view_primary;
    Widget* widget_about;

    FlipboardModel* model;
};

/**
 * @brief    The flipboard view id enum.
 * @details  This enum contains all the view ids for the flipboard.
 */
typedef enum {
    FlipboardViewAppMenuId, // The first entry must be the app menu
    FlipboardViewConfigureId,
    FlipboardViewConfigureSubviewId,
    FlipboardViewConfigureTextInputId,
    FlipboardViewConfigureKeyboardInputId,

    FlipboardViewAboutId,
    FlipboardViewPrimaryId,
} FlipboardViewId;
