#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<stdbool.h>
typedef struct {
    int depart;
    int arrivee;
    char *etiquette;
} Transition;

typedef struct {
    Transition *transitions;
    int *etatsInitiaux;
    int nbreEtatsInitiaux;
    int *etatsFinaux;
    int nbreEtatsFinaux;
    int nombreTransitions;
} Automate;

void lireLigne(FILE *fichier, char ***lines, int *lineCount, int **valeurs, int *valueCount) {
    Automate automate;
    fseek(fichier, 0, SEEK_SET);
    char buffer[256];
    *lineCount = 0;
    while (fgets(buffer, sizeof(buffer), fichier) != NULL) {
        (*lineCount)++;
        *lines = realloc(*lines, (*lineCount) * sizeof(char *));
        (*lines)[(*lineCount) - 1] = strdup(buffer);
    }

    if (*lineCount >= 1) {
        char *token = strtok((*lines)[*lineCount - 1], ",");
        *valueCount = 0;
        while (token != NULL) {
            (*valueCount)++;
            *valeurs = realloc(*valeurs, (*valueCount) * sizeof(int));
            sscanf(token, "%d", &((*valeurs)[(*valueCount) - 1]));
            token = strtok(NULL, ",");
        }
    } else {
        *valueCount = 0;
    }
    automate.nbreEtatsFinaux = *valueCount;
    for (int i = 0; i < *lineCount; i++) {
        free((*lines)[i]);
    }
    free(*lines);
}
void lireAvantDerniereLigne(FILE *fichier, char ***lines, int *lineCount, int **avantDerniereValeurs, int *avantDerniereCount) {
    Automate automate;
    fseek(fichier, 0, SEEK_SET);
    char buffer[256];
    *lineCount = 0;
    while (fgets(buffer, sizeof(buffer), fichier) != NULL) {
        (*lineCount)++;
        *lines = realloc(*lines, (*lineCount) * sizeof(char *));
        (*lines)[(*lineCount) - 1] = strdup(buffer);
    }
    // Lire avant derniere
    if (*lineCount >= 2) {
        char *token = strtok((*lines)[*lineCount - 2], ",");
        *avantDerniereCount = 0;
        while (token != NULL) {
            (*avantDerniereCount)++;
            *avantDerniereValeurs = realloc(*avantDerniereValeurs, (*avantDerniereCount) * sizeof(int));
            sscanf(token, "%d", &((*avantDerniereValeurs)[(*avantDerniereCount) - 1]));
            token = strtok(NULL, ",");
        }
    } else {
        *avantDerniereCount = 0;
    }
    automate.nbreEtatsInitiaux = *avantDerniereCount;
    for (int i = 0; i < *lineCount; i++) {
        free((*lines)[i]);
    }
    free(*lines);
}
int afficherEtats(){//pas besoin de lire fichier car prend trop d'espace dans la memoire
    char* nomFichier = "C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt";
    Automate automate;
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(0);
    }
    //avant derniere
    char **lines = NULL;
    int lineCount = 0;
    int *avantDerniereValeurs = NULL;
    int avantDerniereCount = 0;
    lireAvantDerniereLigne(fichier, &lines, &lineCount, &avantDerniereValeurs, &avantDerniereCount);

    printf("Etats Initiaux : ");
    for (int i = 0; i < avantDerniereCount; i++) {
        printf("%d ", avantDerniereValeurs[i]);
    }
    printf("\n");

    rewind(fichier);

    char **lastLines = NULL;
    int lastLineCount = 0;
    int *derniereValeurs = NULL;
    int derniereCount = 0;
    lireLigne(fichier, &lastLines, &lastLineCount, &derniereValeurs, &derniereCount);

    printf("Etats finaux : ");
    for (int i = 0; i < derniereCount; i++) {
        printf("%d ", derniereValeurs[i]);
    }
    printf("\n");
    fclose(fichier);
    free(avantDerniereValeurs);
    free(derniereValeurs);
    return 0;
}
Automate lireAutomate(const char *nomFichier) {
    Automate automate;
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(0);
    }

    int nombreMaxTransitions = 100;
    automate.transitions = malloc(nombreMaxTransitions * sizeof(Transition));
    automate.nombreTransitions = 0;
    char **lines = NULL;
    int lineCount = 0;
    int *avantDerniereValeurs = NULL;
    int avantDerniereCount = 0;
    lireAvantDerniereLigne(fichier, &lines, &lineCount, &avantDerniereValeurs, &avantDerniereCount);
    char buffer[26];
    rewind(fichier);
    int i=1;
    while ((i<=lineCount-2) && (fscanf(fichier, "%d %d %s", &automate.transitions[automate.nombreTransitions].depart,&automate.transitions[automate.nombreTransitions].arrivee, buffer) == 3)) {
        automate.transitions[automate.nombreTransitions].etiquette = strdup(buffer);
        automate.nombreTransitions++;
        i++;
    }
    automate.etatsInitiaux = avantDerniereValeurs;
    automate.nbreEtatsInitiaux = avantDerniereCount;
    fclose(fichier);
    return automate;
}
void libererAutomate(Automate *automate) {
    for (int i = 0; i < automate->nombreTransitions; i++) {
        free(automate->transitions[i].etiquette);
    }
    free(automate->transitions);
    free(automate->etatsInitiaux);
    free(automate->etatsFinaux);
}
void afficherAlphabet(const Automate *automate) {
    char alphabet[256] = "";
    for (int i = 0; i < automate->nombreTransitions; i++) {
        char *etiquette = automate->transitions[i].etiquette;

        for (int j = 0; j < strlen(etiquette); j++) {
            char symbole = etiquette[j];

            if ((isalpha(symbole) && strchr(alphabet, symbole) == NULL)||(symbole=='_')) {
                strncat(alphabet, &symbole, 1);
            }
        }
    }
    printf("Alphabet de l'automate :");
    for (int k = 0; k <= strlen(alphabet) - 2; k++) {
        printf(" %c,", alphabet[k]);
    }
    printf(" %c.\n", alphabet[strlen(alphabet) - 1]);
}
void DFS(const Automate *automate, int etat, int *marquage) {
    if (etat < 0 || etat > automate->nombreTransitions) {
        return; // n'est pas dans l'ens
    }
    if (marquage[etat]) {
        return; // visite
    }
    marquage[etat] = 1; // atteint
    for (int i = 0; i < automate->nombreTransitions; i++) {
        if (automate->transitions[i].depart == etat) {
            DFS(automate, automate->transitions[i].arrivee, marquage);
        }
    }
}

void generate_dotFile(const char* nomFichier) {
    Automate automate = lireAutomate(nomFichier); 
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    int* marquage = calloc(automate.nombreTransitions, sizeof(int));

    for (int i = 0; i < automate.nbreEtatsInitiaux; i++) {
        
        DFS(&automate, automate.etatsInitiaux[i], marquage);
    }
    FILE *dot = fopen("dotFile.dot", "w");
    if (dot == NULL) {
        printf("Erreur lors de l'ouverture du fichier dot.\n");
        free(marquage);
        fclose(fichier);
        exit(1);
    }
    fprintf(dot,"digraph{\n");
    fprintf(dot,"   node [fontcolor=red style=filled fillcolor=black]\n");
    for (int i = 0; i < automate.nombreTransitions; i++) {
        fprintf(dot,"   %d -> %d [label=\"%s\"]\n", automate.transitions[i].depart, automate.transitions[i].arrivee, automate.transitions[i].etiquette);
    }
    for (int i = 0; i < automate.nombreTransitions; i++) {
        if (!marquage[automate.transitions[i].depart]) {
            fprintf(dot, "   %d [style=filled fillcolor=gray]\n", automate.transitions[i].depart);
        }
    }
    char **lines = NULL;
    int lineCount = 0;
    int *avantDerniereValeurs = NULL;
    int avantDerniereCount = 0;
    lireAvantDerniereLigne(fichier, &lines, &lineCount, &avantDerniereValeurs, &avantDerniereCount);
    for (int i = 0; i < avantDerniereCount; i++) {
        fprintf(dot,"   d->%d\n %d[style=filled fillcolor=green]\n   d[style=filled fillcolor=white fontcolor=white color=white]\n", avantDerniereValeurs[i],avantDerniereValeurs[i]);
    }
    rewind(fichier);
    char **lastLines = NULL;
    int lastLineCount = 0;
    int *derniereValeurs = NULL;
    int derniereCount = 0;
    lireLigne(fichier, &lastLines, &lastLineCount, &derniereValeurs, &derniereCount);
    for (int i = 0; i < derniereCount; i++) {
        fprintf(dot,"   %d->f\n %d[style=filled fillcolor=blue]\n   f[style=filled fillcolor=white fontcolor=white color=white]\n",derniereValeurs[i],derniereValeurs[i]);
    }
    fprintf(dot, "}\n");
    fclose(dot);
    free(marquage);
    fclose(fichier);
    system("dot -Tpng dotFile.dot -o dotFile.png");
    system("start dotFile.png");
}
int est_etat_final(Automate automate, int etat) {
    char *nomFichier ="C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt";
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(0);
    }
    char **lastLines = NULL;
    int lastLineCount = 0;
    int *derniereValeurs = NULL;
    int derniereCount = 0;
    lireLigne(fichier, &lastLines, &lastLineCount, &derniereValeurs, &derniereCount);
    int i;
    for (i = 0; i < derniereCount; i++) {
        if (derniereValeurs[i] == etat) {
            return 1;
        }
    }
    return 0;
}
bool estAccepter(Automate automate, char *mot) {
    int i, j, k;
    int *etatsActu = malloc(automate.nbreEtatsInitiaux * sizeof(int));
    memcpy(etatsActu, automate.etatsInitiaux, automate.nbreEtatsInitiaux * sizeof(int));
    int nbreEtatsActu = automate.nbreEtatsInitiaux;

    int longueur_mot = strlen(mot);

    for (j = 0; j < longueur_mot; j++) {
        int *etatsNew = malloc(automate.nombreTransitions * sizeof(int));
        int nbreEtatsNew = 0;
        for (i = 0; i < nbreEtatsActu; i++) {
            for (k = 0; k < automate.nombreTransitions; k++) {
                if (automate.transitions[k].depart == etatsActu[i] &&
                    strchr(automate.transitions[k].etiquette, mot[j]) != NULL) {
                    etatsNew[nbreEtatsNew++] = automate.transitions[k].arrivee;
                }
            }
        }
        free(etatsActu);
        etatsActu = malloc(nbreEtatsNew * sizeof(int));
        memcpy(etatsActu, etatsNew, nbreEtatsNew * sizeof(int));
        nbreEtatsActu = nbreEtatsNew;

        free(etatsNew);
    }
    for (i = 0; i < nbreEtatsActu; i++) {
        if (est_etat_final(automate, etatsActu[i])) {
            free(etatsActu);
            return true;
        }
    }
    free(etatsActu);
    return false;
}
void afficherMotsEngendres() {
    Automate automate = lireAutomate("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
    FILE* fichier = fopen("mots.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }
    char mot[26];
    while (fgets(mot, 26, fichier)) {
        
        mot[strcspn(mot, "\n")] = '\0';
        if (estAccepter(automate, mot)) {
            printf("%s\n", mot);
        }
    }
    fclose(fichier);
}
int main() {
    Automate automate= lireAutomate("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
    int e_menu;
    do {
        printf("\n<------------------------------Veuillez choisir une option ---------------------------------->\n");
        printf("Afficher les Alphabets(click 1)\n");
        printf("Afficher les etats initiaux et finaux(click 2)\n");
        printf("Afficher l'automate(click 3) (.png) \n");
        printf("Afficher les mots accepter(click 4) \n");
        printf("Tester un mot (click 5)\n");
        printf("Quitter (Click 0)\n");
        printf("Entrer votre choix: ");
        scanf(" %d",&e_menu);
        system("cls");
        switch (e_menu) {
            case 1:
                printf("\n<------Alphabets de l'automate------->\n");
                automate = lireAutomate("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
                afficherAlphabet(&automate);
            break;
            case 2:
                printf("\n<------Etats initiaux et finaux------->\n");
                afficherEtats();
            break;
            case 3:
                automate = lireAutomate("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
                generate_dotFile("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
            break;
            case 4:
                printf("\n<--------Mots Acccepter par l'automate-------->\n");
                automate = lireAutomate("C:\\Users\\hp\\Desktop\\THL_PROJECT\\text_file.txt");
                afficherMotsEngendres();
            break;
            case 5:
                char ch[52];
                printf("Entrez le mot a tester:");
                scanf("%s",ch);
                if(estAccepter(automate,ch)){
                    printf("Votre mot est accepte.\n");
                }else{
                    printf("Votre mot n'est pas accepte.\n");
                }
            break;
            case 0:
                libererAutomate(&automate);
            break;
            default:
                printf("\nMauvaise option.\n");
            break;
    }
    } while (e_menu != 0);
    
    return 0;
}