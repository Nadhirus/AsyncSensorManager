# Acquisition Asynchrone Multitâche

## Description
Ce projet implémente un accumulateur multitâche nommé **MultitaskingAccumulator**. L'objectif principal est de gérer l'acquisition asynchrone de données provenant de capteurs et de produire des diagnostics en temps réel.

## Fonctionnalités
- **Acquisition asynchrone** : Récupération simultanée de données provenant de plusieurs capteurs.
- **Traitement cumulatif** : Somme des entrées acquises pour générer une sortie agrégée.
- **Diagnostics en temps réel** : Rapport détaillé des messages produits, consommés et en attente.

## Structure du projet
- `src/` : Contient le code source en C.
- `include/` : Contient les fichiers d'en-tête nécessaires.
- `docs/` : Documentation et diagrammes UML.
- `bin/` : Contient les fichiers binaires générés après compilation.
- `Makefile` : Automatisation de la compilation et de l'exécution.

## Compilation et Exécution
Pour compiler et exécuter les différents composants, vous pouvez utiliser les commandes suivantes :

```bash
# Créer tous les exécutables dans le répertoire 'bin/'
make all

# Exécuter l'implémentation avec le modèle préambule
make runpreambule

# Exécuter l'implémentation avec le modèle POSIX
make runposix

# Exécuter l'implémentation avec le modèle atomic
make runatomic

# Exécuter l'implémentation avec le modèle TestAndSet
make runtestandset

# Nettoyer les fichiers générés
make clean
```

## Prérequis
- Système d'exploitation Linux avec support POSIX.
- Compilateur `gcc`.
- Bibliothèques POSIX pour la gestion des threads et des sémaphores.

## Documentation
Pour plus d'informations sur l'architecture et la conception, consultez la documentation dans le dossier `docs/`.

## Auteur
Ce projet a été conçu dans le cadre de travaux pratiques sur les systèmes multitâches événementiels et en temps réel.

* * *

> _Pour plus de détails sur le modèle d'exécution ou les diagrammes UML, consultez les fichiers dans `docs/diagrams/`._
