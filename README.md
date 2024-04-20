# Automaton
# Automate fini déterministe
Ce programme implémente un automate fini déterministe (AFD) à partir d'un fichier texte spécifié. L'automate ainsi créé peut être utilisé pour diverses opérations telles que l'affichage des états initiaux et finaux, la génération d'un fichier graphique représentant l'automate, la vérification si un mot est accepté par l'automate, etc.

## Installation des ressources nécessaires

Avant d'exécuter le programme, assurez-vous d'avoir les ressources suivantes installées :

- **GCC** : Le programme est écrit en langage C, vous aurez donc besoin d'un compilateur C pour le construire et l'exécuter. Vous pouvez l'installer en suivant les instructions spécifiques à votre système d'exploitation.

- **Graphviz** : Pour générer le fichier graphique représentant l'automate, le programme utilise Graphviz. Assurez-vous que Graphviz est installé sur votre système. Si ce n'est pas le cas, vous pouvez le télécharger et l'installer depuis [le site officiel de Graphviz](https://www.graphviz.org/download/).

## Utilisation

1. **Compilation du programme** : Utilisez GCC pour compiler le programme. Par exemple, pour compiler sur un système Unix, utilisez la commande suivante dans votre terminal :

   ```
   gcc -o automate main.c
   ```

2. **Exécution du programme** : Après compilation, vous pouvez exécuter le programme en utilisant la commande suivante :

   ```
   ./main
   ```

   Assurez-vous de spécifier le chemin correct vers le fichier texte contenant la description de l'automate lors de l'exécution.

3. **Options disponibles** : Une fois le programme lancé, vous pouvez choisir parmi les options suivantes :

   - **Afficher les Alphabets** : Affiche l'alphabet de l'automate.
   - **Afficher les états initiaux et finaux** : Affiche les états initiaux et finaux de l'automate.
   - **Afficher l'automate** : Génère un fichier graphique représentant l'automate à l'aide de Graphviz.
   - **Afficher les mots acceptés** : Affiche les mots acceptés par l'automate.
   - **Tester un mot** : Vous permet de tester si un mot est accepté par l'automate.
   - **Supression d'epsilon Transitions** : ..
   - **Inversion** : ..
   - **Minimisation** : ...
   - **Determinisation** : ...
   - **Quitter** : Termine le programme.
