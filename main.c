#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
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
    int Nstates;
    int states[50];
    char alph[50];
    int Nalph;
} Automate;
typedef struct {
    int etat;
    struct ListeEtatsATraiter *suivant;
} ListeEtatsATraiter;

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
int afficherEtats(Automate automate){//pas besoin de lire fichier car prend trop d'espace dans la memoire
    printf("Etats Initiaux : ");
    for (int i = 0; i < automate.nbreEtatsInitiaux; i++) {
        printf("%d ", automate.etatsInitiaux[i]);
    }
    printf("\n");

    printf("Etats finaux : ");
    for (int i = 0; i < automate.nbreEtatsFinaux; i++) {
        printf("%d ", automate.etatsFinaux[i]);
    }
    printf("\n");
    return 0;
}
//############################

Automate lireAutomate(const char *nomFichier) {
    Automate automate;
    FILE *fichier = fopen(nomFichier, "r");

    // Assurez-vous que le fichier est ouvert avec succès
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(0);
    }

    // Initialisation des compteurs et tableaux
    automate.nombreTransitions = 0;
    automate.Nstates = 0;
    automate.Nalph = 0;

    // Allocation dynamique pour le tableau transitions
    int nombreMaxTransitions = 100;
    automate.transitions = malloc(nombreMaxTransitions * sizeof(Transition));

    char buffer[26];
    rewind(fichier);

    // Lire les états finaux à partir de la dernière ligne
    char **lastLines = NULL;
    int lastLineCount = 0;
    int *derniereValeurs = NULL;
    int derniereCount = 0;
    lireLigne(fichier, &lastLines, &lastLineCount, &derniereValeurs, &derniereCount);
    automate.nbreEtatsFinaux = derniereCount;
    automate.etatsFinaux = derniereValeurs;

    rewind(fichier);

    // Lire les transitions
    int i = 1;
    while ((i <= lastLineCount - 2) && (fscanf(fichier, "%d %d %s", &automate.transitions[automate.nombreTransitions].depart, &automate.transitions[automate.nombreTransitions].arrivee, buffer) == 3)) {
        automate.transitions[automate.nombreTransitions].etiquette = strdup(buffer);
        automate.nombreTransitions++;
        i++;
    }

    // Lire les états initiaux à partir de l'avant-dernière ligne
    char **lines = NULL;
    int lineCount = 0;
    int *avantDerniereValeurs = NULL;
    int avantDerniereCount = 0;
    lireAvantDerniereLigne(fichier, &lines, &lineCount, &avantDerniereValeurs, &avantDerniereCount);
    automate.etatsInitiaux = avantDerniereValeurs;
    automate.nbreEtatsInitiaux = avantDerniereCount;

    // Construction des tableaux states et alph
    rewind(fichier);
    for (int i = 0; i < automate.nombreTransitions; i++) {
        // Vérifier si l'état 'from' existe déjà dans le tableau states
        int j;
        for(j = 0; j < automate.Nstates; j++){
            if (automate.states[j] == automate.transitions[i].depart) {
                // L'état existe déjà, passer à la transition suivante
                break;
            }
        }
        if(j == automate.Nstates){
            // L'état n'existe pas encore, l'ajouter au tableau states
            automate.states[automate.Nstates] = automate.transitions[i].depart;
            automate.Nstates++;
        }

        // Vérifier si l'état 'to' existe déjà dans le tableau states
        for(j = 0; j < automate.Nstates; j++){
            if(automate.states[j] == automate.transitions[i].arrivee){
                // L'état existe déjà, passer à la transition suivante
                break;
            }
        }
        if(j == automate.Nstates){
            // L'état n'existe pas encore, l'ajouter au tableau states
            automate.states[automate.Nstates] = automate.transitions[i].arrivee;
            automate.Nstates++;
        }

        // Vérifier si la lettre existe déjà dans le tableau alph
        int k;
        for(k = 0; k < automate.Nalph; k++){
            if(automate.alph[k] == automate.transitions[i].etiquette[0]){
                // La lettre existe déjà, passer à la transition suivante
                break;
            }
        }
        if(k == automate.Nalph){
            // La lettre n'existe pas encore, l'ajouter au tableau alph
            automate.alph[automate.Nalph] = automate.transitions[i].etiquette[0];
            automate.Nalph++;
        }
    }

    // printf("\nstates:%d\n", automate.Nstates);
    // printf("\n alph:%d\n", automate.Nalph);
    // Assurez-vous de fermer le fichier avant de quitter la fonction
    fclose(fichier);

    return automate;
}
//##############################

Automate Produit_Automate(Automate automate1, Automate automate2) {
    Automate automateFinal;
    automateFinal.nombreTransitions = 0;
    automateFinal.nbreEtatsFinaux = 0;
    automateFinal.nbreEtatsInitiaux = 0;
    automateFinal.transitions = (Transition*)malloc(100 * sizeof(Transition));
    automateFinal.etatsFinaux = (int*)malloc(50 * sizeof(int));
    automateFinal.etatsInitiaux = (int*)malloc(50 * sizeof(int));
    automateFinal.Nstates = 0;
    automateFinal.Nalph = 0;
    int i, j, k;
    // printf("\nStarted\n");
    // printf("\n %d\n",automate1.Nstates);
    // printf("\n %d\n",automate2.Nstates);
    // Étape 1: Calcul de l'ensemble des états de l'automate résultant
    for (i = 0; i < automate1.Nstates; i++) {
        for (j = 0; j < automate2.Nstates; j++) {
            if (automate1.states[i] == automate2.states[j]) {
                automateFinal.states[automateFinal.Nstates++] = automate1.states[i];
            }
        }
    }
    //printf("\nDone0\n");
    // Étape 2: Calcul de l'ensemble des états initiaux de l'automate résultant
    for (i = 0; i < automate1.nbreEtatsInitiaux; i++) {
        for (j = 0; j < automate2.nbreEtatsInitiaux; j++) {
            if (automate1.etatsInitiaux[i] == automate2.etatsInitiaux[j]) {
                automateFinal.etatsInitiaux[automateFinal.nbreEtatsInitiaux++] = automate1.etatsInitiaux[i];
            }
        }
    }
    //printf("\nDone1\n");
    // Étape 3: Calcul de l'ensemble des états finaux de l'automate résultant
    for (i = 0; i < automate1.nbreEtatsFinaux; i++) {
        for (j = 0; j < automate2.nbreEtatsFinaux; j++) {
            if (automate1.etatsFinaux[i] == automate2.etatsFinaux[j]) {
                automateFinal.etatsFinaux[automateFinal.nbreEtatsFinaux++] = automate1.etatsFinaux[i];
            }
        }
    }
    //printf("\nDone2\n");
    // Étape 4: Calcul de l'ensemble des transitions de l'automate résultant
    for (i = 0; i < automate1.nombreTransitions; i++) {
        for (j = 0; j < automate2.nombreTransitions; j++) {
            if (strcmp(automate1.transitions[i].etiquette, automate2.transitions[j].etiquette) == 0) {
                for (k = 0; k < automateFinal.Nstates; k++) {
                    if (automateFinal.states[k] == automate1.transitions[i].depart &&
                        automateFinal.states[k] == automate2.transitions[j].depart) {
                        Transition transitions;
                        transitions.depart = automate1.transitions[i].depart;
                        transitions.arrivee = automate2.transitions[j].arrivee;
                        transitions.etiquette = automate1.transitions[i].etiquette;
                        automateFinal.transitions[automateFinal.nombreTransitions++] = transitions;
                    }
                }
            }
        }
    }

    //printf("\nDone\n");
    // Étape 5: Calcul du nombre d'alphabets de l'automate résultant
    automateFinal.Nalph = automate1.Nalph;
    
    return automateFinal;
}

//#################################

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

            if ((isalpha(symbole) && strchr(alphabet, symbole) == NULL)) {// ||(symbole=='Ɛ')au cas ou on fera le traitement sur l'union...
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
    if (etat < 0 || etat > 300) {
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

void generate_dotFile(Automate automate) {
    int* marquage = calloc(300, sizeof(int));// nbre transitions

    for (int i = 0; i < automate.nbreEtatsInitiaux; i++) {
        DFS(&automate, automate.etatsInitiaux[i], marquage);
    }
    FILE *dot = fopen("dotFile.dot", "w");
    if (dot == NULL) {
        printf("Erreur lors de l'ouverture du fichier dot.\n");
        free(marquage);
        exit(1);
    }
    char randomd[26];
    char randomf[26];
    for (int i = 0; i < 26; ++i) {
        randomd[i] = 'a' + i;
    }
    for (int i = 0; i < 26; ++i) {
        randomf[i] = 'A' + i;
    }
    fprintf(dot,"digraph{\n");
    fprintf(dot,"   node [fontcolor=red style=filled fillcolor=black]\n");
    fprintf(dot,"   rankdir=LR\n");
    for (int i = 0; i < automate.nombreTransitions; i++) {
        fprintf(dot,"   %d -> %d [label=\"%s\"]\n", automate.transitions[i].depart, automate.transitions[i].arrivee, automate.transitions[i].etiquette);
    }
    for (int i = 0; i < automate.nombreTransitions; i++) {
        if (!marquage[automate.transitions[i].depart]) {
            fprintf(dot, "   %d [style=filled fillcolor=gray]\n", automate.transitions[i].depart);
        }
    }

    for (int i = 0; i < automate.nbreEtatsInitiaux; i++) {
        fprintf(dot,"   %c->%d\n %d[style=filled fillcolor=green shape=doublecircle]\n   %c[style=filled fillcolor=white fontcolor=white color=white]\n",randomd[i],automate.etatsInitiaux[i],automate.etatsInitiaux[i],randomd[i]);
    }

    for (int i = 0; i < automate.nbreEtatsFinaux; i++) {
        fprintf(dot,"   %d->%c\n %d[style=filled fillcolor=blue]\n   %c[style=filled fillcolor=white fontcolor=white color=white]\n",automate.etatsFinaux[i],randomf[i],automate.etatsFinaux[i],randomf[i]);
    }
    fprintf(dot, "}\n");
    fclose(dot);
    free(marquage);
    system("dot -Tpng dotFile.dot -o dotFile.png");
    system("start dotFile.png");
}
// Vérifie si l'état q est atteignable depuis l'état p par des transitions epsilon
int estAtteignableParEpsilon(Automate automate, int p, int q) {
    // Tableau pour marquer les etats visites
    int* visited = (int*)calloc(automate.Nstates, sizeof(int));
    // File pour le parcours en largeur
    int* file = (int*)malloc(automate.Nstates * sizeof(int));
    int debut = 0;
    int fin = 0;
    
    // Initialisation : mettre l'etat initial p dans la file et le marquer comme visite
    file[fin] = p;
    fin++;
    visited[p] = 1;
    
    // Parcours en largeur
    while (debut < fin) {
        int current = file[debut];
        debut++;
        // Parcourir les transitions sortantes de l'etat courant
        for (int i = 0; i < automate.nombreTransitions; i++) {
            if (automate.transitions[i].depart == current && strcmp(automate.transitions[i].etiquette, "Ɛ") == 0) {
                int nextState = automate.transitions[i].arrivee;
                // Si l'etat suivant est q, il est atteignable par epsilon
                if (nextState == q) {
                    free(visited);
                    free(file);
                    return 1;
                }
                // Si l'etat suivant n'a pas ete visite, le mettre dans la file et le marquer comme visite
                if (!visited[nextState]) {
                    file[fin++] = nextState;
                    visited[nextState] = 1;
                }
            }
        }
    }
    
    free(visited);
    free(file);
    return 0;
}


int est_etat_final(Automate automate, int etat) {
    int i;
    for (i = 0; i <automate.nbreEtatsFinaux; i++) {
        if ((automate.etatsFinaux[i] == etat)) {
            return 1;
        }
    }
    return 0;
}
int est_etat_initial(Automate automate, int etat) {
    int i;
    for (i = 0; i <automate.nbreEtatsInitiaux; i++) {
        if (automate.etatsInitiaux[i] == etat) {
            return 1;
        }
    }
    
    for (int i = 0; i < automate.nbreEtatsInitiaux; i++) {
        if (estAtteignableParEpsilon(automate, automate.etatsInitiaux[i], etat)) {
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
                    strchr(automate.transitions[k].etiquette, mot[j]) != NULL ) {
                    if(strcmp(automate.transitions[k].etiquette,"Ɛ")==0){
                        automate.transitions[k].depart=automate.transitions[k].arrivee;
                    }
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

Automate union_Automate(Automate A, Automate B) {
    Automate automateFinal;
    automateFinal.nombreTransitions = A.nombreTransitions + B.nombreTransitions + (A.nbreEtatsInitiaux + B.nbreEtatsInitiaux)+A.nbreEtatsFinaux+B.nbreEtatsFinaux;
    automateFinal.nbreEtatsFinaux = 1;
    automateFinal.nbreEtatsInitiaux = 1;
    automateFinal.transitions = (Transition*)malloc(100 * sizeof(Transition));
    automateFinal.etatsFinaux = (int*)malloc(50 * sizeof(int));
    automateFinal.etatsInitiaux = (int*)malloc(50 * sizeof(int));
    automateFinal.etatsInitiaux[0] = 100;
    automateFinal.etatsFinaux[0] = 200;

    int stop = 0;

    for (int i = 0; i < A.nbreEtatsInitiaux; i++) {
        automateFinal.transitions[stop].depart = 100;
        automateFinal.transitions[stop].arrivee = A.etatsInitiaux[i];
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }

    for (int i = 0; i < B.nbreEtatsInitiaux; i++) {
        automateFinal.transitions[stop].depart = 100;
        automateFinal.transitions[stop].arrivee = B.etatsInitiaux[i];
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }

    for (int i = 0; i < A.nombreTransitions; i++) {
        automateFinal.transitions[stop] = A.transitions[i];
        stop++;
    }

    for (int i = 0; i < B.nombreTransitions; i++) {
        automateFinal.transitions[stop] = B.transitions[i];
        stop++;
    }
    for (int i = 0; i < A.nbreEtatsFinaux; i++) {
        automateFinal.transitions[stop].depart = A.etatsFinaux[i];
        automateFinal.transitions[stop].arrivee = 200;
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }
    for (int i = 0; i < B.nbreEtatsFinaux; i++) {
        automateFinal.transitions[stop].depart = B.etatsFinaux[i];
        automateFinal.transitions[stop].arrivee = 200;
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }
    return automateFinal;
    free(automateFinal.transitions);
    free(automateFinal.etatsFinaux);
    free(automateFinal.etatsInitiaux);
}


Automate etoil_Automate(Automate A) {
    // Création de l'automate final étendu
    Automate automateFinal;
    automateFinal.nombreTransitions = 0; // Initialisation du nombre de transitions à 0
    automateFinal.nbreEtatsFinaux = 1;
    automateFinal.nbreEtatsInitiaux = 1;
    automateFinal.transitions = (Transition*)malloc((A.nombreTransitions + A.nbreEtatsInitiaux + A.nbreEtatsFinaux + 2) * sizeof(Transition)); // Allocation mémoire pour les transitions
    automateFinal.etatsFinaux = (int*)malloc(sizeof(int)); // Allocation mémoire pour les états finaux
    automateFinal.etatsInitiaux = (int*)malloc(sizeof(int)); // Allocation mémoire pour les états initiaux
    automateFinal.etatsInitiaux[0] = 100; // État initial de l'automate final
    automateFinal.etatsFinaux[0] = 200; // État final de l'automate final

    int stop = 0;

    // Ajout des transitions epsilon de l'état initial de automateFinal vers les états initiaux de A
    for (int i = 0; i < A.nbreEtatsInitiaux; i++) {
        automateFinal.transitions[stop].depart = 100;
        automateFinal.transitions[stop].arrivee = A.etatsInitiaux[i];
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }

    // Copie de toutes les transitions de A dans automateFinal
    for (int i = 0; i < A.nombreTransitions; i++) {
        automateFinal.transitions[stop] = A.transitions[i];
        stop++;
    }
   
    // Ajout des transitions epsilon des états finaux de A vers l'état final de automateFinal
    for (int i = 0; i < A.nbreEtatsFinaux; i++) {
        automateFinal.transitions[stop].depart = A.etatsFinaux[i];
        automateFinal.transitions[stop].arrivee = 200;
        automateFinal.transitions[stop].etiquette = strdup("Ɛ");
        stop++;
    }

    // Ajout de deux nouvelles transitions epsilon directement entre les états 100 et 200, et entre les états 200 et 100
    automateFinal.transitions[stop].depart = 100;
    automateFinal.transitions[stop].arrivee = 200;
    automateFinal.transitions[stop].etiquette = strdup("Ɛ");
    stop++;

    automateFinal.transitions[stop].depart = 200;
    automateFinal.transitions[stop].arrivee = 100;
    automateFinal.transitions[stop].etiquette = strdup("Ɛ");
    stop++;

    automateFinal.nombreTransitions = stop; 

    return automateFinal;
}
//$##########
Automate supprimerEtatsInatteignables(Automate A) {
    Automate B;
    B.Nstates = A.Nstates;
    B.etatsInitiaux = (int*)malloc(B.Nstates * sizeof(int));
    B.etatsFinaux = (int*)malloc(B.Nstates * sizeof(int));
    B.nbreEtatsInitiaux = 0;
    B.nbreEtatsFinaux = 0;
    B.transitions = (Transition*)malloc(A.nombreTransitions * sizeof(Transition));
    B.nombreTransitions = 0;
    
    int* atteignable = (int*)calloc(A.Nstates, sizeof(int));

    for (int i = 0; i < A.nbreEtatsInitiaux; i++) {
        DFS(&A, A.etatsInitiaux[i], atteignable);
    }

    for (int i = 0; i < A.Nstates; i++) {
        if (atteignable[i]) {
            if(est_etat_initial(A,i)){
                B.etatsInitiaux[B.nbreEtatsInitiaux++] = i;
            }
            if (est_etat_final(A, i)) {
                B.etatsFinaux[B.nbreEtatsFinaux++] = i;
            }
        }
    }

    for (int i = 0; i < A.nombreTransitions; i++) {
        Transition transition = A.transitions[i];
        if (atteignable[transition.depart] && atteignable[transition.arrivee]) {
            B.transitions[B.nombreTransitions++] = transition;
        }
    }
    
    free(atteignable);
    return B;
}

Automate supprimerTransitionsEpsilon(Automate A) {
    Automate B;
    B.Nstates = A.Nstates;
    B.etatsInitiaux = (int*)malloc(B.Nstates * sizeof(int));
    B.etatsFinaux = (int*)malloc(B.Nstates * sizeof(int));
    B.nbreEtatsInitiaux = A.nbreEtatsInitiaux;
    B.nbreEtatsFinaux = 0;
    B.transitions = (Transition*)malloc(A.nombreTransitions * sizeof(Transition));
    B.nombreTransitions = 0;

    for (int i = 0; i < A.Nstates; i++) {
        B.etatsInitiaux[i] = A.etatsInitiaux[i];
        B.etatsFinaux[i] = A.etatsFinaux[i];
    }

    for (int i = 0; i < A.nombreTransitions; i++) {
        if (strcmp(A.transitions[i].etiquette, "Ɛ") != 0) {
            B.transitions[B.nombreTransitions] = A.transitions[i];
            B.nombreTransitions++;
        }
    }

    for (int p = 0; p < A.Nstates; p++) {
        for (int q = 0; q < A.Nstates; q++) {
            if (estAtteignableParEpsilon(A, p, q)) {

                if (est_etat_final(A, q)) {
                    B.etatsFinaux[B.nbreEtatsFinaux++] = p;
                }

                for (int i = 0; i < A.nombreTransitions; i++) {
                    if (A.transitions[i].depart == q && strcmp(A.transitions[i].etiquette, "Ɛ") != 0) {
                        B.transitions[B.nombreTransitions].depart = p;
                        B.transitions[B.nombreTransitions].arrivee = A.transitions[i].arrivee;
                        B.transitions[B.nombreTransitions].etiquette = strdup(A.transitions[i].etiquette);
                        B.nombreTransitions++;
                    }
                }
            }
        }
    }
    generate_dotFile(B);
    printf("\nVoulez vous l'afficher sans les etats inatteignables?(1 pour oui | 0 pour non):");
    int choix;
    scanf("%d",&choix);
    if(choix){
        return supprimerEtatsInatteignables(B);
    }
    else{
        exit(0);
    }
}
//###########
Automate determiniserAutomate(Automate automate) {
    Automate automateF;
    automateF.nombreTransitions = 0;
    automateF.nbreEtatsFinaux = 0;
    automateF.nbreEtatsInitiaux = 1;
    automateF.transitions = (Transition*)malloc(100 * sizeof(Transition));
    automateF.etatsFinaux = (int*)malloc(50 * sizeof(int));
    automateF.etatsInitiaux = (int*)malloc(50 * sizeof(int));
    
    // Utilise un seul etat initial pour l'automate deterministe
    automateF.etatsInitiaux[0] = automate.etatsInitiaux[0];
    
    for (int i = 0; i < automate.nombreTransitions; i++) {
        char *current = automate.transitions[i].etiquette;//a,b,a
        int currentDepart = automate.transitions[i].depart;//0,0,0
        int transitionExistante = 0;

        for (int j = 0; j < automateF.nombreTransitions; j++) {
            if (strcmp(automateF.transitions[j].etiquette, current) == 0 &&//a!=b,a==a
                automateF.transitions[j].depart == currentDepart) {
                //doublon++
                transitionExistante = 1;
                break;
            }
        }

        if (!transitionExistante) {
            automateF.transitions[automateF.nombreTransitions] = automate.transitions[i];//0 0 a,0 0 b
            automateF.nombreTransitions++;//1,2
            for (int k = 0; k < automate.nombreTransitions; k++) {//k=1,i=0/ k=2,i=0/i=1,k=0/i=1,k=2
                if (k != i && strcmp(automate.transitions[k].etiquette, current) == 0 &&// b!=a, a==a,a!=b,a==a
                    automate.transitions[k].depart == currentDepart &&//0==0,0==0
                    est_etat_final(automate, automate.transitions[k].arrivee)) {// 1 est final
                    int etatFinal = automate.transitions[i].arrivee;// 0
                    if (!est_etat_final(automateF, etatFinal)) {
                        automateF.etatsFinaux[automateF.nbreEtatsFinaux] = etatFinal;//0
                        automateF.nbreEtatsFinaux++;
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < automate.nbreEtatsFinaux; i++) {// pour prendre en compte les etats finaux de l'Aut.. d'entree
        int etatFinal = automate.etatsFinaux[i];//1
        if (!est_etat_final(automateF, etatFinal)){
            automateF.etatsFinaux[automateF.nbreEtatsFinaux] = etatFinal;//1
            automateF.nbreEtatsFinaux++;
        }
    }
    return (automateF);
}
//##############
Automate inverserAutomate(Automate automate) {
    Automate automateInv;
    automateInv.nombreTransitions = automate.nombreTransitions;
    automateInv.nbreEtatsInitiaux = automate.nbreEtatsFinaux;
    automateInv.nbreEtatsFinaux = automate.nbreEtatsInitiaux;
    automateInv.transitions = (Transition*)malloc(automate.nombreTransitions * sizeof(Transition));
    automateInv.etatsInitiaux = (int*)malloc(automate.nbreEtatsFinaux * sizeof(int));
    automateInv.etatsFinaux = (int*)malloc(automate.nbreEtatsInitiaux * sizeof(int));

    for (int i = 0; i < automate.nbreEtatsFinaux|| i < automate.nbreEtatsInitiaux; i++) {
        automateInv.etatsFinaux[i] = automate.etatsInitiaux[i];
    }

    for (int i = 0; i < automate.nbreEtatsInitiaux|| i < automate.nbreEtatsFinaux; i++) {
        automateInv.etatsInitiaux[i] = automate.etatsFinaux[i];
    }
    
    for (int i = 0; i < automate.nombreTransitions; i++) {//inversion
        automateInv.transitions[i].depart = automate.transitions[i].arrivee;
        automateInv.transitions[i].arrivee = automate.transitions[i].depart;
        automateInv.transitions[i].etiquette = automate.transitions[i].etiquette;
    }

    return automateInv;
}


void minimiserAutomate(Automate automate){
    Automate automateF,automateF1;
    automateF.nombreTransitions = 0,automateF1.nombreTransitions = 0;
    automateF.nbreEtatsFinaux = 0,automateF1.nbreEtatsFinaux = 0;
    automateF.nbreEtatsInitiaux = 1,automateF1.nbreEtatsInitiaux = 1;
    automateF.transitions = (Transition*)malloc(100 * sizeof(Transition)),automateF1.transitions = (Transition*)malloc(100 * sizeof(Transition));
    automateF.etatsFinaux=(int*)malloc(50 * sizeof(int)),automateF1.etatsFinaux=(int*)malloc(50 * sizeof(int));
    automateF.etatsInitiaux=(int*)malloc(50 * sizeof(int)),automateF1.etatsInitiaux=(int*)malloc(50 * sizeof(int));

    automateF= inverserAutomate(automate);
    generate_dotFile(automateF);
    getchar();
    getchar();
    automateF= determiniserAutomate(automateF);
    generate_dotFile(automateF);
    getchar();
    getchar();
    automateF=inverserAutomate(automateF);
    generate_dotFile(automateF);
    getchar();
    getchar();
    automateF=determiniserAutomate(automateF);
    generate_dotFile(automateF);
    getchar();
    getchar();
    free(automateF.transitions);
    free(automateF.etatsFinaux);
    free(automateF.etatsInitiaux);
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
        printf("UNION d'automates (click 6)\n");
        printf("ETOILE automate (click 7)\n");
        printf("PRODUIT de deux automates (click 8)\n");
        printf("SUPPRIMER EPSILONS transitions (click 9)\n");
        printf("Determinisation d'un automate (click 10)\n");
        printf("Minimisation d'un automate (click 11)\n");
        printf("Quitter (Click 0)\n");
        printf("Entrer votre choix: ");
        scanf(" %d",&e_menu);
        system("cls");
        switch (e_menu) {
            case 1:
                printf("\n<------Alphabets de l'automate------->\n");
                afficherAlphabet(&automate);
            break;
            case 2:
                printf("\n<------Etats initiaux et finaux------->\n");
                afficherEtats(automate);
            break;
            case 3:
                char *nomF0;
                nomF0=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier(name.txt):");
                scanf("%s",nomF0);
                generate_dotFile(lireAutomate(nomF0));
            break;
            case 4:
                printf("\n<--------Mots Acccepter par l'automate-------->\n");
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
            case 6:
                char *nomF1;
                char *nomF2;
                nomF1=(char*)malloc(25*sizeof(char));
                nomF2=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier du 1er automate(name.txt):");
                scanf("%s",nomF1);
                printf("\nEntrez le nom du fichier du 2 ieme automate(name.txt):");
                scanf("%s",nomF2);
                Automate automate1=lireAutomate(nomF1);
                Automate automate2=lireAutomate(nomF2);
                Automate A=union_Automate(automate1,automate2);
                printf("\n%d\n",automate.nombreTransitions);
                printf("\n%d\n",automate2.nombreTransitions);
                printf("\n%d\n",A.nombreTransitions);
                printf("\n%d\n",A.nbreEtatsInitiaux);
                printf("\n%d\n",A.nbreEtatsFinaux);
                for(int i=0;i<automate.nombreTransitions;i++){
                    printf("\n%d->%d(%s)\n",automate.transitions[i].depart,automate.transitions[i].arrivee,automate.transitions[i].etiquette);
                }
                printf("Fin 1");
                for(int i=0;i<automate2.nombreTransitions;i++){
                    printf("\n%d->%d(%s)\n",automate2.transitions[i].depart,automate2.transitions[i].arrivee,automate2.transitions[i].etiquette);
                }
                printf("Fin 2");
                for(int i=0;i<A.nombreTransitions;i++){
                    printf("\n%d->%d(%s)\n",A.transitions[i].depart,A.transitions[i].arrivee,A.transitions[i].etiquette);
                }
                printf("Fin");
                generate_dotFile(A);
            break;
            case 7:
                char *nomFi;
                nomFi=(char*)malloc(25*sizeof(char));
                printf("Entrez le nom du fichier de l'automate que vous souhaitez mettre en etoile(name.txt):");
                scanf("%s",nomFi);
                generate_dotFile(etoil_Automate(lireAutomate(nomFi)));
            break;
            case 8:
                char *nomF3;
                char *nomF4;
                nomF3=(char*)malloc(25*sizeof(char));
                nomF4=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier du 1er automate(name.txt):");
                scanf("%s",nomF3);
                printf("\nEntrez le nom du fichier du 2 ieme automate(name.txt):");
                scanf("%s",nomF4);
                Automate automate3=lireAutomate(nomF3);
                Automate automate4=lireAutomate(nomF4);
                Automate B=Produit_Automate(automate3,automate4);
                generate_dotFile(B);
            break;
            case 9:
                char *nomF;
                nomF=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier de l'automate dont vous souhaitez supprimer les epsilons transitions(name.txt):");
                scanf("%s",nomF);
                generate_dotFile(supprimerTransitionsEpsilon(lireAutomate(nomF)));
            break;
            case 10:
                char *nomFichier;
                nomFichier=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier de l'automate dont vous souhaitez determiniser(name.txt):");
                scanf("%s",nomFichier);
                //generate_dotFile(determiniserAutomate(lireAutomate(nomFichier)));
                generate_dotFile((determiniserAutomate(lireAutomate(nomFichier))));
            break;
            case 11:
                char *nomFile;
                nomFile=(char*)malloc(25*sizeof(char));
                printf("\nEntrez le nom du fichier de l'automate que vous souhaitez minimiser(name.txt):");
                scanf("%s",nomFile);
                minimiserAutomate(lireAutomate(nomFile));
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