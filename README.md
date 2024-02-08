<h1 align="center"> 💻 Comparaison de molécules : Similarité des graphes des cycles ⚛️</h1>
<p>
</p>

<!-- badges: start -->
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
<!-- badges: end -->

Ce projet est réalisé dans le cadre de notre M2 Algorithmique et modélisation à l'interface des sciences (AMIS). L'objectif est de comparer des molécules issues d'une base de données par comparaison de leur graphes des cycles.

## Rapport et présentation
[latex présentation](https://www.overleaf.com/9936189727ddqhdmmvxmqq#957509)

[latex rapport](https://www.overleaf.com/9143591999bkqtcxgnwvfz#696441)

## Structure

M2AmisProjet/
│
├── bin/
│   └── graphCycleMol.exe  # L'exécutable généré
│
├── include/
│   └── (fichiers d'en-tête .h)  # Fichiers d'en-tête pour le projet en C
│
├── obj/
│   └── (fichiers objets .o)  # Fichiers objets générés lors de la compilation
│
├── data/
│   └── (Données de la base de données)  # Fichiers de molécules parsés
│
├── src/
│   └── (fichiers source .c)  # Fichiers source pour le projet en C
│
├── .gitignore  # Liste des fichiers/dossiers à ignorer lors du suivi par Git
├── Makefile    # Fichier de configuration pour la construction du projet
└── script.py   # Script Python pour récupérer des données de la base de données



## Compilation

Pour compiler le projet utilisez :
```sh
make
```

## Utilisation

<!--### Démo
Pour lancer une démo sur le substrat adénosine :
```sh
make demo
```-->

### Exécution
Pour exécuter le programme entrez :
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