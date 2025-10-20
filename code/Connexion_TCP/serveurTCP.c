#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>

#define MAX_BUFFER_SIZE 1024

int authenticate(int client_socket) {
    char request[MAX_BUFFER_SIZE];
    ssize_t received_bytes = recv(client_socket, request, sizeof(request) - 1, 0);
    if (received_bytes <= 0) {
        perror("Erreur lors de la réception des informations d'authentification du client");
        return -1;
    }

    request[received_bytes] = '\0';

    char username[100];
    char password[100];
    sscanf(request, "%[^,],%s", username, password);

    // * Vérifier les informations d'authentification

    if (strcmp(username, "abir") == 0 && strcmp(password, "abir1234") == 0) {
        char *success_message = "SUCCESS";
        ssize_t sent_bytes = send(client_socket, success_message, strlen(success_message), 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi de la réponse d'authentification au client");
            return -1;
        }

        return 0;
    } else if (strcmp(username, "Rawaa") == 0 && strcmp(password, "rawaa1234") == 0) {
         char *success_message = "SUCCESS";
        ssize_t sent_bytes = send(client_socket, success_message, strlen(success_message), 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi de la réponse d'authentification au client");
            return -1;
        }

        return 0;
    } else {
        char *failure_message = "FAILURE";
        ssize_t sent_bytes = send(client_socket, failure_message, strlen(failure_message), 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi de la réponse d'authentification au client");
            return -1;
        }

        return -1;
    }
}

void handle_client(int client_socket) {
    if (authenticate(client_socket) != 0) {
        close(client_socket);
        return;
    }

    char request[MAX_BUFFER_SIZE];
    ssize_t received_bytes, sent_bytes;

    while (1) {

        // * Recevoir la demande du client

        received_bytes = recv(client_socket, request, sizeof(request) - 1, 0);
        if (received_bytes <= 0) {
            perror("Erreur lors de la réception de la demande du client");
            break;
        }

        request[received_bytes] = '\0';

        int choice = atoi(request);

        // * Traiter la demande du client

        switch (choice) {
            case 1: {
                time_t current_time;
                time(&current_time);
                char *date_time = ctime(&current_time);
                snprintf(request, sizeof(request), "Date et heure du serveur : %s", date_time);
                break;
            }
            case 2:{
                DIR *directory;
                struct dirent *entry;
                char file_list[MAX_BUFFER_SIZE];
                file_list[0] = '\0';

                directory = opendir(".");
                if (directory == NULL) {
                    snprintf(request, sizeof(request), "Erreur lors de l'ouverture du répertoire");
                    break;
                }

                while ((entry = readdir(directory)) != NULL) {
                    strcat(file_list, entry->d_name);
                    strcat(file_list, "\n");
                }
                closedir(directory);
                snprintf(request, sizeof(request), "Liste des fichiers du répertoire courant :\n%s", file_list);
                break;}
            case 3: {
                char filename[MAX_BUFFER_SIZE];
                snprintf(request, sizeof(request), "Entrez le nom du fichier : ");
                sent_bytes = send(client_socket, request, strlen(request), 0);
                if (sent_bytes == -1) {
                    perror("Erreur lors de l'envoi de la demande de nom de fichier au client");
                    break;
                }

                // ? Recevoir le nom de fichier du client

                received_bytes = recv(client_socket, filename, sizeof(filename) - 1, 0);
                if (received_bytes <= 0) {
                    perror("Erreur lors de la réception du nom de fichier du client");
                    break;
                }
                filename[received_bytes] = '\0';

                FILE *file = fopen(filename, "r");
                if (file == NULL) {
                    snprintf(request, sizeof(request), "Erreur lors de l'ouverture du fichier");
                    break;
                }

                // ? Lire le contenu du fichier

                char file_content[MAX_BUFFER_SIZE];
                size_t content_length = fread(file_content, 1, sizeof(file_content) - 1, file);
                file_content[content_length] = '\0';

                fclose(file);
                snprintf(request, sizeof(request), "Contenu du fichier '%s' :\n%s", filename, file_content);
                break;
        }
            case 4: {
                time_t current_time;
                time(&current_time);
                snprintf(request, sizeof(request), "Durée de connexion : %ld secondes", current_time);
                break;
            }
            default:
                snprintf(request, sizeof(request), "Choix invalide");
                break;
        }

        // * Envoyer la réponse au client

        sent_bytes = send(client_socket, request, strlen(request), 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi de la réponse au client");
            break;
        }
    }

    // * Fermer la connexion avec le client

    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilisation : %s <port>\n", argv[0]);
        return 1;
    }

    int server_port = atoi(argv[1]);

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;

    // * Créer le socket serveur

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur lors de la création du socket serveur");
        return 1;
    }

    // * Préparer l'adresse du serveur

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    // * Lier le socket serveur à l'adresse et au port spécifiés

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1) {
        perror("Erreur lors de la liaison du socket serveur");
        return 1;
    }

    // * Mettre le serveur en mode écoute

    if (listen(server_socket, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket serveur");
        return 1;
    }

    printf("Le serveur est en attente de connexions sur le port %d...\n", server_port);

    while (1) {

        // * Accepter une connexion entrante

        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1) {
            perror("Erreur lors de l'acceptation de la connexion entrante");
            continue;
        }

        printf("Nouvelle connexion acceptée\n");

        // * Gérer le client dans un processus enfant

        pid_t pid = fork();
        if (pid == -1) {
            perror("Erreur lors de la création d'un processus enfant pour gérer le client");
            close(client_socket);
            continue;
        } else if (pid == 0) {

            // ! Processus enfant

            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else {

            // ! Processus parent

            close(client_socket);
        }
    }

    // * Fermer le socket serveur

    close(server_socket);

    return 0;
}