# Les Aventuriers du Rail - Mon Premier Voyage

Projet C++ réalisé dans le cadre du TP noté SMP - ECN.

## Compilation

```bash
make
```

ou depuis zéro :

```bash
make rebuild
```

## Exécution

```bash
./jeu
```

Le programme demande le nombre de joueurs (2 à 4) puis le nom de chacun.

## Règles implémentées

- Pioche de cartes Train (2 par tour)
- Pose de wagons sur une voie ferrée
- Échange de tickets (passer son tour)
- Vérification automatique des tickets via BFS
- Grande Traversée (côte ouest → côte est)
- Fin de partie : 6 tickets réussis ou plus de wagons

## Structure

| Fichier | Contenu |
|---|---|
| `Models.hpp` | Structures de base (Couleur, Ticket, Route) |
| `CarteTrain` | Carte Train avec sa couleur |
| `Joueur` | Gestion d'un joueur (wagons, cartes, tickets) |
| `Board` | Plateau de jeu, chargement CSV |
| `Jeu` | Moteur de jeu complet |
| `main.cpp` | Point d'entrée |

## Données

Les routes et tickets sont chargés depuis `csv/map.csv` et `csv/ticket.csv`.
