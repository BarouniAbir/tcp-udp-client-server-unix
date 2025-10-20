#!/bin/bash

# * Nom des fichiers source

client_source="clientUDP.c"
serveur_source="serveurUDP.c"

# * Nom des exécutables

client_executable="clientUDP"
serveur_executable="serveurUDP"

# * Commande de compilation

compile_command="gcc -o"

# * Compilation du client

echo "Compilation du client..."
$compile_command $client_executable $client_source

# * Vérification du résultat de la compilation du client

if [ $? -eq 0 ]; then
    echo "Compilation du client terminée avec succès."
else
    echo "Erreur lors de la compilation du client."
    exit 1
fi

# * Compilation du serveur

echo "Compilation du serveur..."
$compile_command $serveur_executable $serveur_source

# * Vérification du résultat de la compilation du serveur

if [ $? -eq 0 ]; then
    echo "Compilation du serveur terminée avec succès."
else
    echo "Erreur lors de la compilation du serveur."
    exit 1
fi

echo "Compilation et édition des liens terminés avec succès."