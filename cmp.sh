#!/bin/bash
echo ""
# Vérification des arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 fichier1 fichier2"
    exit 1
fi

fichier1="$1"
fichier2="$2"

# Vérification de l'existence des fichiers
if [ ! -f "$fichier1" ] || [ ! -f "$fichier2" ]; then
    echo "Erreur : Les deux fichiers doivent exister."
    exit 1
fi

# Taille des fichiers
taille1=$(stat -c%s "$fichier1")
taille2=$(stat -c%s "$fichier2")

# Calcul du ratio de compression
if [ "$taille2" -ne 0 ]; then
    ratio=$(echo "scale=2; $taille1 / $taille2" | bc)
else
    echo "Erreur : La taille du fichier 2 est zéro, impossible de calculer le ratio."
    exit 1
fi

# Affichage des résultats
echo "Taille de $fichier1 : $taille1 octets"
echo "Taille de $fichier2 : $taille2 octets"
echo "Ratio de compression (fichier1 / fichier2) : 0$ratio"