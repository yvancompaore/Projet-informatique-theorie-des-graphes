#include "grman/grman.h"
#include <iostream>
#include "graph.h"



int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    ///g.make_example();
    g.chargergraphe();



    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

         if(key[KEY_SPACE])
    {
    std::cout << "nSCCs in first graph " << std::endl ;
    Graph g1(5);
    g1.addEdge(1, 0);
    g1.addEdge(0, 2);
    g1.addEdge(2, 1);
    g1.addEdge(0, 3);
    g1.addEdge(3, 4);
    g1.SCC();
    }
    }

    grman::fermer_allegro();



  ///  g.algokconex();
    g.sauvegarderfichier();

    return 0;
}
END_OF_MAIN();


