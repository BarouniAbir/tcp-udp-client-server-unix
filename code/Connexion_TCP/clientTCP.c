#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024

// * Affiche le menu des options disponibles

void print_menu() {
    printf("\nMenu :\n");
    printf("1. Obtenir la date et l'heure du serveur\n");
    printf("2. Obtenir la liste des fichiers du répertoire\n");
    printf("3. Obtenir le contenu d'un fichier\n");
    printf("4. Obtenir la durée de connexion\n");
    printf("5. Quitter\n");
    printf("Choix : ");
}

int authenticate(int client_socket) {
    char username[100];
    char password[100];

 // * Demande à l'utilisateur de saisir son nom d'utilisateur et son mot de passe

    printf("Nom d'utilisateur : ");
    scanf("%s", username);

    printf("Mot de passe : ");
    scanf("%s", password);

    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "%s,%s", username, password);

    ssize_t sent_bytes = send(client_socket, request, strlen(request), 0);
    if (sent_bytes == -1) {
        perror("Erreur lors de l'envoi des informations d'authentification au serveur");
        return -1;
    }

    char response[MAX_BUFFER_SIZE];
    ssize_t received_bytes = recv(client_socket, response, sizeof(response) - 1, 0);
    if (received_bytes <= 0) {
        perror("Erreur lors de la réception de la réponse du serveur");
        return -1;
    }

    response[received_bytes] = '\0';

    if (strcmp(response, "SUCCESS") == 0) {
        printf("Authentification réussie !\n");
        return 0;
    } else {
        printf("Échec de l'authentification. Veuillez réessayer.\n");
        return -1;
    }
}

void request_service(int client_socket, int choice) {
    char request[MAX_BUFFER_SIZE];
    ssize_t sent_bytes, received_bytes;

    snprintf(request, sizeof(request), "%d", choice);
    sent_bytes = send(client_socket, request, strlen(request), 0);
    if (sent_bytes == -1) {
        perror("Erreur lors de l'envoi de la demande de service au serveur");
        return;
    }

    char response[MAX_BUFFER_SIZE];
    received_bytes = recv(client_socket, response, sizeof(response) - 1, 0);
    if (received_bytes <= 0) {
        perror("Erreur lors de la réception de la réponse du serveur");
        return;
    }
    response[received_bytes] = '\0';

    printf("Réponse du serveur :\n%s\n", response);

    // * Si le choix est 3, demander et envoyer le nom du fichier au serveur

    if (choice == 3) {
        char filename[MAX_BUFFER_SIZE];
        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);

        sent_bytes = send(client_socket, filename, strlen(filename), 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi du nom de fichier au serveur");
            return;
        }

        received_bytes = recv(client_socket, response, sizeof(response) - 1, 0);
        if (received_bytes <= 0) {
            perror("Erreur lors de la réception du contenu du fichier");
            return;
        }
        response[received_bytes] = '\0';

        printf("Contenu du fichier '%s' :\n%s\n", filename, response);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Utilisation : %s <adresse IP> <port>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int client_socket;
    struct sockaddr_in server_address;

    // * Créer le socket client

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur lors de la création du socket client");
        return 1;
    }

    // * Préparer l'adresse du serveur

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // * Connecter le socket client au serveur

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Erreur lors de la réception de la demande du client : %s\n", strerror(errno));
        return 1;
    }

    printf("Client connecté\n");

    if (authenticate(client_socket) != 0) {
        close(client_socket);
        return 1;
    }

    int choice;

    do {
        print_menu();
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 4) {
            request_service(client_socket, choice);
        } else if (choice != 5) {
            printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choice != 5);

    // * Fermer la connexion avec le serveur
    
    close(client_socket);

    return 0;
}
