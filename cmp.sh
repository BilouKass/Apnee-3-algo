#!/bin/bash
echo ""

# Vérification des arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 fichier1 [fichier2 fichier3 ...]"
    exit 1
fi

# Création des dossiers
mkdir -p encoded
mkdir -p decoded

# Traitement de chaque fichier
for fichier_original in "$@"; do
    echo "======================================"
    
    # Vérification de l'existence du fichier
    if [ ! -f "$fichier_original" ]; then
        echo "⚠ Le fichier '$fichier_original' n'existe pas, ignoré."
        echo ""
        continue
    fi

    # Extraction du nom de base du fichier
    nom_base=$(basename "$fichier_original")

    # Noms des fichiers générés
    fichier_compresse="encoded/${nom_base}.huf"
    fichier_decompresse="decoded/${nom_base}"

    # Compression
    echo "Compression de '$fichier_original'..."
    ./huff_encode "$fichier_original" "$fichier_compresse" &> /dev/null
    if [ $? -ne 0 ]; then
        echo "✗ Erreur lors de la compression de '$fichier_original'"
        echo ""
        continue
    fi

    # Décompression
    echo "Décompression de '$fichier_compresse'..."
    ./huff_decode "$fichier_compresse" "$fichier_decompresse" &> /dev/null
    if [ $? -ne 0 ]; then
        echo "✗ Erreur lors de la décompression de '$fichier_compresse'"
        echo ""
        continue
    fi

    # Tailles des fichiers
    taille_original=$(stat -c%s "$fichier_original")
    taille_compresse=$(stat -c%s "$fichier_compresse")
    taille_decompresse=$(stat -c%s "$fichier_decompresse")

    # Calcul du ratio et taux de compression
    if [ "$taille_original" -ne 0 ]; then
        ratio=$(echo "scale=4; $taille_compresse / $taille_original * 100" | bc)
        gain=$(echo "scale=2; 100 - $ratio" | bc)
    else
        echo "⚠ Fichier original vide, rien à décompresser"
        taille_original=0
        taille_compresse=0
        taille_decompresse=0
        ratio=0
        gain=0
        echo ""
    fi

    # Vérification de l'intégrité
    echo ""
    echo "=== RAPPORT DE COMPRESSION ==="
    echo "Fichier original      : $fichier_original ($taille_original octets)"
    echo "Fichier compressé     : $fichier_compresse ($taille_compresse octets)"
    echo "Fichier décompressé   : $fichier_decompresse ($taille_decompresse octets)"
    echo "------------------------------"
    echo "Taux de compression   : $ratio%"
    echo "Gain d'espace         : $gain%"
    echo ""

    # Vérification que la décompression est identique
    if cmp -s "$fichier_original" "$fichier_decompresse"; then
        echo "✓ Décompression réussie : fichiers identiques"
    else
        echo "✗ ERREUR : Le fichier décompressé diffère de l'original !"
        $fichier_decompresse < "ERREUR DE DECOMPRESSION"
    fi
    
    echo ""
done

echo "======================================"
echo "Traitement terminé pour tous les fichiers."