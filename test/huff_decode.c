#include <stdio.h>

#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, FILE *fichier_decode, Arbre ArbreHuffman) {
    BFILE *Benc =bstart(fichier_encode, "r");
    int c = bitread(Benc);
    Arbre current = ArbreHuffman;
    while (!beof(Benc)) {
        if (c == 0) {
            current = FilsGauche(current);
        } else {
            current = FilsDroit(current);
        }
        if (EstVide(FilsGauche(current)) && EstVide(FilsDroit(current))) {
            fputc(current->etiq, fichier_decode);
            current = ArbreHuffman;
        }
        c = bitread(Benc);
    }
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");
    FILE *fichier_decode = fopen(argv[2], "w");

    Arbre ArbreHuffman = LireArbre(fichier_encode);
    //AfficherArbre(ArbreHuffman);
    Decoder(fichier_encode, fichier_decode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier_decode);
    
    return 0;
}
