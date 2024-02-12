<h1 align="center"> 💻 Comparaison de molécules : Similarité des graphes des cycles ⚛️</h1>
<p>
</p>

<!-- badges: start -->
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
<!-- badges: end -->

Ce projet est réalisé dans le cadre de notre M2 Algorithmique et modélisation à l'interface des sciences (AMIS). L'objectif est de comparer des molécules issues d'une base de données par comparaison de leur graphes des cycles.

## Rapport et présentation

[latex présentation](https://www.overleaf.com/4279531282ghsdfmdxbmqv#2be995)

[latex ancienne présentation](https://www.overleaf.com/9936189727ddqhdmmvxmqq#957509)

[latex rapport](https://www.overleaf.com/9143591999bkqtcxgnwvfz#696441)

## Structure du projet

Ce projet suit une structure de dossier organisée pour une meilleure gestion et maintenabilité. Voici une explication des différents dossiers :

- **bin/** : Contient l'exécutable généré après la compilation du projet.

- **include/** : Les fichiers d'en-tête (.h) nécessaires pour le projet en C sont placés ici. Ces fichiers définissent les interfaces des fonctions et structures utilisées dans le projet.

- **obj/** : Les fichiers objets (.o) générés lors de la compilation sont stockés ici. Ces fichiers sont les résultats intermédiaires de la compilation des fichiers source.

- **data/** : Ce dossier contient les fichiers de molécules chargé depuis la base de données.

- **src/** : Les fichiers source (.c) pour le projet en C sont placés ici. C'est là que vous écrivez le code principal de votre projet.

- **.gitignore** : Un fichier utilisé pour spécifier les fichiers ou dossiers que Git doit ignorer lors du suivi des modifications. Cela inclut généralement les fichiers objets, l'exécutable et d'autres fichiers générés automatiquement.

- **Makefile** : Le fichier de configuration pour la construction du projet.

- **script.py** : Script Python pour récupérer des données de la base de données.

Cette structure de dossier organisée aide à maintenir le projet propre et bien organisé.

## Compilation

Pour compiler le projet utilisez :
```sh
make
```

## Utilisation

Pour charger les molécules depuis la base de données Chebi.
```sh
make data
```

<!--### Démo
Pour lancer une démo sur le substrat adénosine :
```sh
make demo
```-->

### Exécution
Pour exécuter le programme entrez :
```sh
make run
```
<!--```sh
./bin/cageMol.exe -i [fichier_substrat.xyz]
```
Puis les paramètres alpha et sizemax peuvent être aussi modifiés.
Alpha est utilisé pour la génération d'une enveloppe concave et sizemax correspond au nombre d'atomes maximum que l'on veut dans un chemin qu'on génère.
```sh
alpha (défaut 3) : -a [double]

sizemax (défaut 5) : -s [entier]
```
Pour avoir de l'aide : 
 ```sh
-h
```

### Nettoyage des fichiers

Pour supprimer l'éxécutable et les fichiers objets :
```sh
make clean
```
Pour supprimer en plus les résultats : 
```sh
make mrproper
```

### Visualisation

Pour visualiser les résultats vous pouvez utiliser Pymol ou tout autres logiciels de visualisation moléculaire. -->

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