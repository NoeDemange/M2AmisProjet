<h2 align="center"> 💻 Comparaison de molécules : Similarité des graphes de cycles ⚛️</h2>
<p>
</p>

<!-- badges: start -->
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
<!-- badges: end -->

Ce projet est réalisé dans le cadre de notre M2 Algorithmique et modélisation à l'interface des sciences (AMIS). L'objectif est de déterminer la similarité des molécules issues d'une base de données en comparant leur graphe de cycles. Pour cela un calcul de similarité par sous-graphe induit commun maximum (MCIS) et une similarité de Levenshtein sont utilisés.

# Table des matières
1. [Téléchargement de la bibliothèque NAUTY](#nauty)
2. [Utilisation](#Utilisation)
    1. [Compilation](#Compilation)
    2. [Chargement des données](#data)
    3. [Exécution](#exec)
    4. [Résultats](#res)
    5. [Nettoyage des fichiers](#clean)
3. [Structure du projet](#structure)
4. [Auteurs](#Auteurs)
5. [Encadrants](#Encadrants)

## Téléchargement de la bibliothèque NAUTY pour la numérotation canonique des sommets d'un graphe <a name="nauty"></a>

Ce programme requiert d'utiliser la bibliothèque nauty, de Brendan McKay and Adolfo Piperno. Elle peut être téléchargée depuis la page web des auteurs (la version 2.8.8 a été utilisée).
- [page web "nauty and Traces"](https://pallini.di.uniroma1.it/#howtogetit)
- [lien de téléchargement de nauty version 2.8.8](https://pallini.di.uniroma1.it/#howtogetit)

Une fois la bibliothèque téléchargée, il faut lancer les commandes suivantes, à la racine du dossier contenant la bibliothèque (ie. le dossier *nautyx_x_x*) :

```
./confige
make
```

Puis il faut déplacer les fichiers `nauty.h` et `nauty.a` dans le dossier de ce dépôt nommé *nauty*.

## Utilisation <a name="Utilisation"></a>

### Compilation <a name="Compilation"></a>

Pour compiler le projet utilisez :
```sh
make
```
### Chargement des données <a name="data"></a>

Pour charger les molécules depuis la base de données ChEBI :
```sh
make data
```

### Exécution <a name="exec"></a>
Pour lancer la comparaison entre toutes les molécules de la base de données, entrez :
```sh
./similarite
```
Les options suivantes sont également disponibles :
```sh
./similarite [-a chebi_id1 (défaut : vide)] [-b chebi_id2 (défaut : vide)] [-n nb_fichiers (défaut : tous)] [-g (défaut : non)] [-h (usage)]
```

Les options `-a` et `-b` servent à renseigner l'id des deux molécules à comparer. Si `-a` est utilisé seul, la molécule sera comparée à toutes les autres. 

L'option `-n` permet de limiter le nombre de comparaisons, et peut-être utilisé avec `-a`. 

L'option `-g` permet d'obtenir des fichiers DOT pour visualiser les graphes de cycles des molécules lorsque combinée à `-a` et `-b`. Pour convertir les fichiers en image .png, entrez la commande `make png`. Vous pouvez également les convertir en image vectorielle avec `make svg`.


### Résultats <a name="res"></a>

Les résultats sont présentés dans le fichier *matRes.csv* lorsque la comparaison est lancée sur plusieurs molécules. Quand seulement deux molécules sont comparées, le coefficient de similarité s'affiche dans la console. 

Avec les options `-a`, `-b` et `-g`, les fichiers DOT des graphes moléculaires réduits, des graphes de cycles et du graphe produit sont générés et stockés dans le dossier **graphs/**.

### Nettoyage des fichiers <a name="clean"></a>

Pour supprimer l'éxécutable, les fichiers objets et les fichiers DOT :
```sh
make clean
```
Pour supprimer en plus les fichiers de molécules et le fichier de résultats : 
```sh
make mrproper
```

## Structure du projet <a name="structure"></a>

Ce projet suit une structure de dossier organisée pour une meilleure gestion et maintenabilité. Voici une explication des différents dossiers :

- **bin/** : contient l'exécutable généré après la compilation du projet.

- **include/** : contient les fichiers d'en-tête (.h) du programme C. Ces fichiers définissent les interfaces des fonctions et structures utilisées dans le projet.

- **nauty/** : l'emplacement pour les fichiers de la bibliothèque nauty, qui doit être pré-installée manuellement (cf. la section ci-dessous).

- **obj/** : contient les fichiers objets (.o) générés lors de la compilation. 

- **data/** : Ce dossier contient les fichiers de molécules chargés depuis la base de données.

- **src/** : contient les fichiers source (.c) du programme C.

- **Makefile** : Le fichier de configuration pour la construction du projet.

- **chebi_molecule_loader.py** : Script Python pour récupérer les informations des molécules stockée dans le fichier *ChEBI_lite_3star.sdf* sur le site de la base de données ChEBI. Ces informations sont ensuite stockées dans des fichiers individuels dans le dossier **data/**.

## Auteurs <a name="Auteurs"></a>

<div align="center">

👤 **Noé DEMANGE** : Github: [@NoeDemange](https://github.com/NoeDemange)

👤 **Ronan DOCHERTY** :  Github: [@Zatsky](https://github.com/Zatsky)

👤 **Anne FERNET** : Github: [@uvsq21915170](https://github.com/uvsq21915170)

👤 **Victor NESSI** : Github: [@nectorv](https://github.com/nectorv)

</div>

## Encadrants <a name="Encadrants"></a>
 <div align="center">
  <b>Sandrine VIAL &emsp; Franck QUESSETTE</b>
</div>