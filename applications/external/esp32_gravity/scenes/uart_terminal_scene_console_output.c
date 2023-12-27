#include "../uart_terminal_app_i.h"

#define MIN_VERSION_STRLEN 43

uint32_t launchTime;

void uart_terminal_console_output_handle_rx_data_cb(uint8_t* buf, size_t len, void* context) {
    furi_assert(context);
    UART_TerminalApp* app = context;

    // If text box store gets too big, then truncate it
    app->text_box_store_strlen += len;
    if(app->text_box_store_strlen >= UART_TERMINAL_TEXT_BOX_STORE_SIZE - 1) {
        furi_string_right(app->text_box_store, app->text_box_store_strlen / 2);
        app->text_box_store_strlen = furi_string_size(app->text_box_store) + len;
    }

    // Null-terminate buf and append to text box store
    uint8_t* buf2 = malloc(sizeof(uint8_t) * (len + 1));
    /* If it fails leave in the stack overflow that was previously there (lazy) */
    if(buf2 == NULL) {
        buf2 = buf;
    }
    memcpy(buf2, buf, len);
    buf2[len] = '\0';
    furi_string_cat_printf(app->text_box_store, "%s", buf2);
    /* Free buf2 if we malloc'd it */
    if(buf != buf2) {
        free(buf2);
    }

    /* Appending to the text box resets the view to its default. If it's been
       more than 10 seconds since running the command, focus the text box at end */
    uint32_t delay = 10000;
    if(furi_get_tick() - launchTime >= delay) {
        text_box_set_focus(app->text_box, TextBoxFocusEnd);
    }

    view_dispatcher_send_custom_event(
        app->view_dispatcher, UART_TerminalEventRefreshConsoleOutput);
}

void uart_terminal_scene_console_output_on_enter(void* context) {
    UART_TerminalApp* app = context;

    launchTime = furi_get_tick();

    TextBox* text_box = app->text_box;
    text_box_reset(app->text_box);
    text_box_set_font(text_box, TextBoxFontText);
    if(app->focus_console_start) {
        text_box_set_focus(text_box, TextBoxFocusStart);
    } else {
        text_box_set_focus(text_box, TextBoxFocusEnd);
    }

    if(app->is_command) { /* View console ensures this is false */
        furi_string_reset(app->text_box_store);
        app->text_box_store_strlen = 0;

        /* Handle Flipper commands here - set ap->is_command = false for commands that are consumed */
        if(!strcmp(app->selected_tx_string, "GET_STARTED")) {
            app->is_command = false;
            /* Display detailed instructions on getting started */
            const char string[] =
                "             Flipper Gravity\nGETTING STARTED\nUnless you're doing a basic beacon spam or probe flood attack, or a Mana attack, the first thing to do is turn scanning on and let it run while you explore the menu. View found APs (you can leave scanning on or turn it off), select a few APs or STAs and run a DEAUTH attack against the selected APs or STAs. When an AP is specified for a DEAUTH attack Gravity will use all STAs it identifies as clients of the specified APs. Turn off scanning and deauth, and turn on MANA or LOUD MANA. This is still under development, but you can watch Wireshark to see if any devices send you an association request.\n";
            furi_string_cat_str(app->text_box_store, string);
            app->text_box_store_strlen += strlen(string);
        } else if(!strcmp(app->selected_tx_string, "gravity-version")) {
            /* Display Flipper-Gravity components of the About screen */
            /* Not setting app->is_command = false;
               We want this command to be passed through to esp32-Gravity, so we can also
               display its version */
            /* Display a basic about screen */
            // TODO: See if the following works:
            const char about1[] =
                "              Flipper Gravity\nBy Chris BC\n\nBe The Unseen Force.\n\nhttps://github.com/chris-bc/esp32-gravity\nhttps://github.com/chris-bc/Flipper-Gravity\n\nFlipper-Gravity : v";
            furi_string_cat_str(app->text_box_store, about1);
            app->text_box_store_strlen += strlen(about1);
            furi_string_cat_str(app->text_box_store, GRAVITY_VERSION);
            app->text_box_store_strlen += strlen(GRAVITY_VERSION);
            furi_string_cat_str(app->text_box_store, "\n");
            app->text_box_store_strlen += strlen("\n");
            //furi_string_cat_printf(app->text_box_store, %s\n", GRAVITY_VERSION);
            //app->text_box_store_strlen += 5 + strlen("              Flipper Gravity\nBy Chris BC\n\nBe The Unseen Force.\n\nhttps://github.com/chris-bc/esp32-gravity\nhttps://github.com/chris-bc/Flipper-Gravity\n\nFlipper-Gravity : v\n");
        } else if(!strcmp(app->selected_tx_string, "cls")) {
            furi_string_reset(app->text_box_store);
            app->text_box_store_strlen = 0;
        }

        if(app->show_stopscan_tip) {
            const char* help_msg = "Press BACK to return\n";
            furi_string_cat_str(app->text_box_store, help_msg);
            app->text_box_store_strlen += strlen(help_msg);
        }
    }

    // Set starting text - for "View Log", this will just be what was already in the text box store
    text_box_set_text(app->text_box, furi_string_get_cstr(app->text_box_store));

    scene_manager_set_scene_state(app->scene_manager, UART_TerminalSceneConsoleOutput, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, UART_TerminalAppViewConsoleOutput);

    // Register callback to receive data
    uart_terminal_uart_set_handle_rx_data_cb(
        app->uart, uart_terminal_console_output_handle_rx_data_cb); // setup callback for rx thread

    // Send command with CR+LF or newline '\n'
    /* GRAVITY: Ignore the "cls" command */
    if(app->is_command && app->selected_tx_string && strcmp(app->selected_tx_string, "cls")) {
        if(app->TERMINAL_MODE == 1) {
            // furi_string_cat_str(app->text_box_store, app->selected_tx_string);
            // app->text_box_store_strlen += strlen(app->selected_tx_string);
            // char foo[3];
            // itoa(strlen(app->selected_tx_string), foo, 10);
            // furi_string_cat_str(app->text_box_store, foo);
            // app->text_box_store_strlen += strlen(foo);
            uart_terminal_uart_tx(
                (uint8_t*)(app->selected_tx_string), strlen(app->selected_tx_string));
            uart_terminal_uart_tx((uint8_t*)("\r\n"), 2);
        } else {
            // furi_string_cat_str(app->text_box_store, app->selected_tx_string);
            // app->text_box_store_strlen += strlen(app->selected_tx_string);
            // char foo[3];
            // itoa(strlen(app->selected_tx_string), foo, 10);
            // furi_string_cat_str(app->text_box_store, foo);
            // app->text_box_store_strlen += strlen(foo);
            uart_terminal_uart_tx(
                (uint8_t*)(app->selected_tx_string), strlen(app->selected_tx_string));
            uart_terminal_uart_tx((uint8_t*)("\n"), 1);
        }
    }
    if(app->free_command && app->selected_tx_string != NULL) {
        free(app->selected_tx_string);
        app->selected_tx_string = NULL;
        app->free_command = false;
    }
}

bool uart_terminal_scene_console_output_on_event(void* context, SceneManagerEvent event) {
    UART_TerminalApp* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        text_box_set_text(app->text_box, furi_string_get_cstr(app->text_box_store));
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        consumed = true;
    }

    return consumed;
}

void uart_terminal_scene_console_output_on_exit(void* context) {
    UART_TerminalApp* app = context;

    // Unregister rx callback
    uart_terminal_uart_set_handle_rx_data_cb(app->uart, NULL);

    /* Instruct ESP32 to halt any UIs that may be drawing */
    if(app->selected_tx_string != NULL && !strcmp(app->selected_tx_string, "stalk on")) {
        uart_terminal_uart_tx((uint8_t*)"stalk off\n", strlen("stalk off\n"));
    }
}
