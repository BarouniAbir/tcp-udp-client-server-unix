#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define NMAX 100

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Utilisation : %s <adresse serveur> <numéro de port>\n", argv[0]);
        exit(1);
    }

    char *serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // * Création du socket

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erreur lors de la création du socket");
        exit(1);
    }

    // * Configuration de l'adresse du serveur

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);

    // * Envoi d'un nombre aléatoire au serveur

    srand(time(NULL));
    int n = rand() % NMAX + 1;
    sprintf(buffer, "%d", n);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // * Réception de la réponse du serveur

    memset(buffer, 0, sizeof(buffer));
    socklen_t serverLength = sizeof(serverAddress);
    recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddress, &serverLength);
    printf("Réponse du serveur : %s\n", buffer);

    // * Fermeture du socket
    
    close(sockfd);

    return 0;
}