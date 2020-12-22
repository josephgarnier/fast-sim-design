# TODO list

## Gestion des entités et Entity-System-Component (ESC)

Les sources d'inspiration sont : [EntityManager de SFML-Book](https://github.com/Krozark/SFML-book/tree/master/extlibs/SFML-utils/include/SFML-utils/es), [Artemis-Cpp](https://github.com/vinova/Artemis-Cpp), [Anax](https://github.com/miguelmartin75/anax), [EntityX](https://github.com/alecthomas/entityx) et [EnTT](https://github.com/skypjack/entt)

- Revoir le cycle de vie des entités (init et term ne sont pas satisfaisants, voir [Unreal](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Actors/ActorLifecycle/index.html), [Unity](https://docs.unity3d.com/es/2018.3/Manual/ExecutionOrder.html), [Medium](https://medium.com/@spicuzza157/component-lifecycle-in-unity-8458f6df77e8), [GameDev](https://gamedev.stackexchange.com/questions/148833/how-to-handle-entity-initialisation-and-destruction), [Android 1](https://developer.android.com/reference/androidx/lifecycle/Lifecycle), et [Android 2](https://developer.android.com/reference/androidx/lifecycle/LifecycleOwner)). L'implémentation d'un ESC devrait résoudre une partie du problème. Dans World, remplacer, peut-être, createEntity et destroyEntity par spawnEntity et unspawnEntity. Puis, créer l'interface ILifeCycle.
- Il existe 3 manières de gérer les entités. La 1ère, en utilisant un vector sans se soucier de la gestion des indexes (comme [ici](https://github.com/miguelmartin75/anax)). La 2ème, en gérant son propre tableau d'indexes par une réutilisation de ceux dont la valeur aura été effacée (comme [ici](https://github.com/Krozark/SFML-book) ou [ici](https://github.com/vinova/Artemis-Cpp)). La 2ème solution étant plus optimisée que la 1ère. Enfin la 3ème, très optimisée, dans laquelle la classe entité n'existe qu'au travers de son ID, avec une déportation complète de sa structure et de sa logique dans des composants et des systèmes (comme [ici](https://github.com/alecthomas/entityx) ou [ici](https://github.com/skypjack/entt)).

## Le Monde (*world*) et ses Couches (*layers*)

- Créer possiblement une layer spéciale pour les collisions appelée "CollisionLayer"
- Dans World, revoir methode getTotalTime() et GetWorldTime()
- Dans AxisAlignesBB, enlever la dépendance à Entity et revoir la modélisation des sprites
- Tester MathUtil.rescaledData() et MathUtil.normalizedData()
- Tester constructeur de Location
- Tester Location.ManhattanDistanceTo()
- Tester Object.GetAllFreeSlots() et Object.GetAllHeldSlots()
- Tester Entity.SetTargetLookAt() et Entity.IsDirectedTo()
- Revoir Entity.IsCollideWith() car il faut faire le test sur la future position, pas sur la position courante.
- Tester MathUtil.cantorPairingFunction() et MathUtil.cantorDepairingFunction()
- Test AlgorithmUtil.concatenate()

## Les Ressources

Les sources d'inspiration sont : [ResourceManager de SFML-Book](https://github.com/Krozark/SFML-book/tree/master/extlibs/SFML-utils/include/SFML-utils/core), [Configuration de SFML-Book](https://github.com/Krozark/SFML-book/tree/master/07_2D_iso_game/include/SFML-Book), [WorldManager de Tiled](https://github.com/mapeditor/tiled/tree/master/src/libtiled) et [ResourceManager de ManaMobile](https://github.com/tales/tales-client/tree/master/src/mana).

- Dans map_resource, remplacer tous les QScopedPointer par unique_ptr

## Futures fonctionnalités

- Ajouter un mode fullscreen