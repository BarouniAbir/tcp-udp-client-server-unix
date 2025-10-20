#!/bin/bash

# * Compilation du fichier clientTCP.c

echo "Compilation de clientTCP.c..."
gcc -c clientTCP.c -o clientTCP.o
if [ $? -ne 0 ]; then
  echo "Erreur de compilation pour clientTCP.c."
  exit 1
fi

# * Compilation du fichier serveurTCP.c

echo "Compilation de serveurTCP.c..."
gcc -c serveurTCP.c -o serveurTCP.o
if [ $? -ne 0 ]; then
  echo "Erreur de compilation pour serveurTCP.c."
  exit 1
fi

# * Édition des liens pour créer les exécutables finaux

echo "Édition des liens pour créer clientTCP..."
gcc clientTCP.o -o clientTCP
if [ $? -ne 0 ]; then
  echo "Erreur lors de l'édition des liens pour clientTCP."
  exit 1
fi

echo "Édition des liens pour créer serveurTCP..."
gcc serveurTCP.o -o serveurTCP
if [ $? -ne 0 ]; then
  echo "Erreur lors de l'édition des liens pour serveurTCP."
  exit 1
fi

# * Supprimer les fichiers objets intermédiaires

rm -f clientTCP.o serveurTCP.o

echo "Compilation et édition des liens terminées avec succès."