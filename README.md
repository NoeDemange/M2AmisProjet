<h1 align="center"> 💻 Comparaison de molécules : Similarité des graphes des cycles ⚛️</h1>
<p>
</p>

<!-- badges: start -->
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
<!-- badges: end -->

Ce projet est réalisé dans le cadre de notre M2 Algorithmique et modélisation à l'interface des sciences (AMIS). L'objectif est de déterminer la similarité des molécules issues d'une base de données en comparant leur graphe de cycles.

## Rapport et présentation

[latex présentation finale](https://www.overleaf.com/4279531282ghsdfmdxbmqv#2be995)

[latex première présentation](https://www.overleaf.com/9936189727ddqhdmmvxmqq#957509)

[latex rapport](https://www.overleaf.com/9143591999bkqtcxgnwvfz#696441)

## Structure du projet

Ce projet suit une structure de dossier organisée pour une meilleure gestion et maintenabilité. Voici une explication des différents dossiers :

- **bin/** : contient l'exécutable généré après la compilation du projet.

- **include/** : contient les fichiers d'en-tête (.h) du programme C. Ces fichiers définissent les interfaces des fonctions et structures utilisées dans le projet.

- **nauty/** : l'emplacement pour les fichiers de la bibliothèque nauty, qui doit être pré-installée manuellement (cf. la section ci-dessous).

- **obj/** : contient les fichiers objets (.o) générés lors de la compilation. 

- **data/** : Ce dossier contient les fichiers de molécules chargés depuis la base de données.

- **src/** : contient les fichiers source (.c) du programme C.

- **Makefile** : Le fichier de configuration pour la construction du projet.

- **script.py** : Script Python pour récupérer les informations des molécules stockée dans le fichier *ChEBI_lite_3star.sdf* sur le site de la base de données ChEBI. Ces informations sont ensuite stockées dans des fichiers individuels dans le dossier **data/**.


## Téléchargement de la bibliothèque NAUTY pour la numérotation canonique des sommets d'un graphe

Ce programme requiert d'utiliser la bibliothèque nauty, de Brendan McKay and Adolfo Piperno. Elle peut être téléchargée depuis la page web des auteurs (la version 2.8.8 a été utilisée).
- [page web "nauty and Traces"](https://pallini.di.uniroma1.it/#howtogetit)
- [lien de téléchargement de nauty version 2.8.8](https://pallini.di.uniroma1.it/#howtogetit)

Une fois la bibliothèque téléchargée, il faut lancer les commandes suivantes, à la racine du dossier contenant la bibliothèque (ie. le dossier *nautyx_x_x*) :

```
./confige
make
```

Puis il faut déplacer les fichiers `nauty.h` et `nauty.a` dans le dossier de ce dépôt nommé *nauty*.

## Utilisation

### Compilation

Pour compiler le projet utilisez :
```sh
make
```
### Chargement des données

Pour charger les molécules depuis la base de données ChEBI :
```sh
make data
```

### Exécution
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


### Résultats

Les résultats sont présentés dans le fichier *matRes.csv* lorsque la comparaison est lancée sur plusieurs molécules. Quand seulement deux molécules sont comparées, le coefficient de similarité s'affiche dans la console. 

Avec les options `-a`, `-b` et `-g`, les fichiers DOT des graphes moléculaires réduits, des graphes de cycles et du graphe produit sont générés et stockés dans le dossier **graphs/**.

### Nettoyage des fichiers

Pour supprimer l'éxécutable, les fichiers objets et les fichiers DOT :
```sh
make clean
```
Pour supprimer en plus les fichiers de molécules et le fichier de résultats : 
```sh
make mrproper
```

## Auteurs

<div align="center">

👤 **Noé DEMANGE** : Github: [@NoeDemange](https://github.com/NoeDemange)

👤 **Ronan DOCHERTY** :  Github: [@Zatsky](https://github.com/Zatsky)

👤 **Anne FERNET** : Github: [@uvsq21915170](https://github.com/uvsq21915170)

👤 **Victor NESSI** : Github: [@nectorv](https://github.com/nectorv)

</div>

## Encadrants
 <div align="center">
  <b>Sandrine VIAL &emsp; Franck QUESSETTE</b>
</div>