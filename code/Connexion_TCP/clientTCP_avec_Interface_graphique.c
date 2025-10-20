#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>

#define MAX_BUFFER_SIZE 1024

GtkWidget *window;
GtkWidget *auth_username_entry;
GtkWidget *auth_password_entry;
GtkWidget *service_label;
GtkWidget *service_combo;
GtkWidget *result_textview;
GtkWidget *connect_button;
GtkWidget *quit_button;

int client_socket;

void show_message_dialog(GtkMessageType type, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void connect_to_server(GtkWidget *widget, gpointer data) {
    const gchar *server_address = gtk_entry_get_text(GTK_ENTRY(data));

    // * Créer un socket

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        show_message_dialog(GTK_MESSAGE_ERROR, "Échec de la création d'un socket");
        return;
    }

    // * Configurer l'adresse du serveur

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    if (inet_pton(AF_INET, server_address, &(server_addr.sin_addr)) <= 0) {
        show_message_dialog(GTK_MESSAGE_ERROR, "Adresse du serveur invalide");
        return;
    }

    // * Connexion au serveur
    
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        show_message_dialog(GTK_MESSAGE_ERROR, "Échec de la connexion au serveur");
        return;
    }

}


void quit_application(GtkWidget *widget, gpointer data) {
    if (client_socket != -1) {
        close(client_socket);
    }
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    
}