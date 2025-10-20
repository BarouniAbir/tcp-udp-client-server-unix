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
    if (argc < 2) {
        printf("Utilisation : %s <numéro de port>\n", argv[0]);
        exit(1);
    }

    int serverPort = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
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
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // * Association du socket à l'adresse du serveur

    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Erreur lors de l'association du socket");
        exit(1);
    }

    printf("Le serveur est à l'écoute sur le port %d...\n", serverPort);

    // * Réception du nombre du client

    memset(buffer, 0, sizeof(buffer));
    socklen_t clientAddrLength = sizeof(clientAddress);
    recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddress, &clientAddrLength);
    int n = atoi(buffer);
    printf("Nombre reçu du client : %d\n", n);

    // * Génération et envoi des nombres aléatoires au client

    srand(time(NULL));
    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < n; i++) {
        int randomNum = rand() % NMAX + 1;
        char numStr[10];
        sprintf(numStr, "%d ", randomNum);
        strcat(buffer, numStr);
    }
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));

    // * Fermeture du socket
    close(sockfd);

    return 0;
}