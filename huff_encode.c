
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {

    int c;
    for (int i = 0; i < 256; i++) {
        TableOcc->tab[i] = 0;
    }
    c = fgetc(fichier);
    while (c != EOF) {
        TableOcc->tab[c] += 1; 
        c = fgetc(fichier);
    };
    int i;
    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    /* A COMPLETER */
    fap F = creer_fap_vide();
    for (int i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0) {
            Arbre A = NouveauNoeud(NULL, i, NULL);
            F = inserer(F, A, TableOcc->tab[i]);
        }
    }
    return F;
}

Arbre ConstruireArbre(fap file) {
    /* A COMPLETER */
    Arbre A;
    Arbre B;
    Arbre Z;
    int prioA; int prioB;
    while (1) {
        file = extraire(file,&A,&prioA);
        file = extraire(file,&B,&prioB);

        Z = NouveauNoeud(A,-1,B);

        if (est_fap_vide(file))  // il ne reste plus rien → Z est la racine
            return Z;

        file = inserer(file, Z, prioA+prioB);
    }
    

}

void constru(Arbre huff, int prof, char tab[])
{ // T BON AU BOWLING ?
    if (huff->fg == NULL && huff->fd == NULL)
    {
        tab[prof] = '\0';
        for (int i = 0; i < prof; i++)
        {
            printf("%c",tab[i]);
            HuffmanCode[huff->etiq].code[i] = tab[i];
        }
        printf(" %c\n", huff->etiq);
        HuffmanCode[huff->etiq].lg = prof;
        return;
    }
    tab[prof] = 0;
    constru(huff->fg, prof+1, tab);
    tab[prof] = 1;
    constru(huff->fd, prof+1, tab);
}

void ConstruireCode(Arbre huff) {
    char tab[100];
    int prof = 0;
    if (huff != NULL)
    {
        constru(huff,prof,tab);
        
    }
    for (int i = 0; i < 256; i++)
    {
        
        //printf("%c,%d,%s\n", i, HuffmanCode[i].lg, HuffmanCode[i].code);
    }

}



void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    /* A COMPLETER */
    EcrireArbre(fic_out, ArbreHuffman);
    BFILE *bf = bstart(fic_out,"w");
    int c;
    c = fgetc(fic_in);
    while (c != EOF) {
        for (int i = 0; i < HuffmanCode[c].lg ;  i++) {
            printf("%d", HuffmanCode[c].code[i]);
            bitwrite(bf, HuffmanCode[c].code[i]);
        }

        c = fgetc(fic_in);
    };
    printf("\n");
    bstop(bf);
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

    AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
