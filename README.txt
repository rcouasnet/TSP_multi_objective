# Projet Optimisation : TSP Multi-objectif
	Par COUASNET Robin et DAVID Florian


## Compilation : Utiliser le CMakeLists.txt
Les fichiers KDevelop pour faciliter une éventuelle importation

On peut modifier la ligne "#define NB_SEEDS 500" dans instance_tsp.h pour modifie le nombre de solutions testées aléatoirement et recompiler.

Pour obtenir des traces, on peut aller dans le fichier debug.h et mettre les define voulus à 1. 


## Pour lancer le programme :
	dossier build
./tsp_multi-objectif  <fichier1> <fichier2>

"./tsp_multi-objectif  ../data/kroA100.tsp ../data/kroB100.tsp " par exemple

On choisi ensuite quelle algorithme on veut tester


## Pour GNUPlot : 

Créer un fichier ou modifier le fichier "filtre.plot" dans le dossier "data/results" sous la forme :

	set term jpeg

	set output 'filtreNBSEEDS_nom_instance.jpeg'
	plot 'offline_NBSEEDS_nom_instance.txt'

	set output 'filtreParetoNBSEEDS_nom_instance.jpeg'
	plot 'offlineParetoNBSEEDS_nom_instance.txt'


Puis, dans le terminal :

    gnuplot filtre.plot


## Remarques :
le Offline et Online marchent, par contre mTSP avec PLS doit être mal implémenter car il est beaucoup trop long pour effectuer une seule itération

Veuillez nous excuser pour le retard, nous avons justement eu des problèmes avec le mTSP.
