#include <AppCore/Dialogs.h>
#include <AppCore/App.h>
#include <gtk/gtk.h>
#include "AppGLFW.h"

namespace ultralight {

ButtonResult ShowMessageBox(const String& title, const String& message, DialogIcon icon, ButtonType buttons) {
    gtk_init(nullptr, nullptr);

    GtkMessageType gtkIcon;
    switch (icon) {
        case DialogIcon::Info:
            gtkIcon = GTK_MESSAGE_INFO;
            break;
        case DialogIcon::Warning:
            gtkIcon = GTK_MESSAGE_WARNING;
            break;
        case DialogIcon::Error:
            gtkIcon = GTK_MESSAGE_ERROR;
            break;
        case DialogIcon::Question:
            gtkIcon = GTK_MESSAGE_QUESTION;
            break;
        default:
            gtkIcon = GTK_MESSAGE_INFO;
    }

    GtkButtonsType gtkButtons;
    switch (buttons) {
        case ButtonType::OK:
            gtkButtons = GTK_BUTTONS_OK;
            break;
        case ButtonType::OKCancel:
            gtkButtons = GTK_BUTTONS_OK_CANCEL;
            break;
        case ButtonType::YesNo:
            gtkButtons = GTK_BUTTONS_YES_NO;
            break;
        default:
            gtkButtons = GTK_BUTTONS_OK;
    }

    GtkWidget* dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_MODAL, gtkIcon, gtkButtons, "%s", message.utf8().data());
    gtk_window_set_title(GTK_WINDOW(dialog), title.utf8().data());

    // Set the dialog position to center
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);

    // Connect the dialog response signal to handle dialog closure
    gint result = GTK_RESPONSE_NONE;
    g_signal_connect(dialog, "response", G_CALLBACK(+[](GtkDialog* dialog, gint response, gpointer data) {
        *static_cast<gint*>(data) = response;
        gtk_widget_destroy(GTK_WIDGET(dialog));
        gtk_main_quit();
    }), &result);

    // Show the dialog
    gtk_widget_show_all(dialog);

    // Run the GTK main loop
    gtk_main();

    switch (result) {
        case GTK_RESPONSE_OK:
            return ButtonResult::OK;
        case GTK_RESPONSE_CANCEL:
            return ButtonResult::Cancel;
        case GTK_RESPONSE_YES:
            return ButtonResult::Yes;
        case GTK_RESPONSE_NO:
            return ButtonResult::No;
        default:
            return ButtonResult::Cancel;
    }
}

} // namespace ultralight