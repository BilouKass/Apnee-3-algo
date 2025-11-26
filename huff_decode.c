#include <stdio.h>
#include <stdlib.h>
#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, FILE *fichier_decode, Arbre ArbreHuffman) {
    BFILE *Benc = bstart(fichier_encode, "r");
    Arbre current = ArbreHuffman;
    while (!beof(Benc)) {
        int c = bitread(Benc);
        if (c == EOF) break;
        
        if (c == 0) {
            if (EstVide(FilsGauche(current))) { //cas singleton
                fputc(current->etiq, fichier_decode);
                current = ArbreHuffman;
                continue;
            } else {
                current = FilsGauche(current);
            }
        } else {
            current = FilsDroit(current);
        }
        
        // Si on arrive sur une feuille, écrire le caractère et repartir de la racine
        if (EstVide(FilsGauche(current)) && EstVide(FilsDroit(current))) {
            fputc(current->etiq, fichier_decode);
            current = ArbreHuffman;
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "rb");
    FILE *fichier_decode = fopen(argv[2], "w");
    int c = fgetc(fichier_encode);
    if (c == EOF) {
        printf("Fichier vide\n");
        fclose(fichier_encode);
        return 0;
    }
    ungetc(c, fichier_encode);
    Arbre ArbreHuffman = LireArbre(fichier_encode);
    Decoder(fichier_encode, fichier_decode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier_decode);
    
    return 0;
}
