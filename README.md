# R505 - Cocktail problem

Projet de R5-05 sur les sémaphores en C++20 (std::counting_semaphore et std::thread), fait par LAMBERT Alexis et Mariam POGHOSSIAN.

Au départ on est partis du problème des fumeurs de cigarettes du Little Book of Semaphores (section 4.5). Ensuite on a changé le lore pour en faire un bar à cocktails. L'agent devient un serveur, les fumeurs deviennent des barmen, et le tabac, le papier et les allumettes deviennent du rhum, de la menthe et du citron.

Ce dépôt contient la version qui se bloque (interblocage). La version corrigée est ici : https://github.com/james3756/R505Cocktail-problem-worked

Pour lancer il suffit d'ouvrir Cocktail_SansCorrection.pro dans Qt Creator et de faire Ctrl+R (il faut un compilateur qui gère le C++20, par exemple GCC 11 ou plus).

## Pourquoi ça bloque

Le serveur pose deux ingrédients sur le comptoir et le barman qui a le troisième doit les prendre pour faire son cocktail. Ensuite il prévient le serveur, qui pose deux autres ingrédients.

Chaque barman prend ses ingrédients un par un :

```cpp
void barmanAvecCitron()   // a du citron, attend rhum + menthe
{
    rhum.acquire();
    menthe.acquire();
    // ...
}
```

Le problème c'est qu'un barman peut prendre un ingrédient qui était pour un autre barman et attendre le deuxième pour rien.

Par exemple :

```
Serveur A pose : rhum + menthe
Barman avec menthe : a pris le rhum, attend le citron...
Barman avec rhum : a pris la menthe, attend le citron...
```

Ici le rhum et la menthe étaient pour le barman qui a le citron. Les deux autres en ont pris un chacun et attendent du citron qui n'arrivera jamais, parce que le serveur attend qu'un cocktail soit fait avant de reposer quelque chose. Donc tout le monde attend et le programme reste bloqué.

Source : The Little Book of Semaphores, Allen B. Downey (https://greenteapress.com/wp/semaphores/)
