#include "graph.h"
#include <sstream>
#include <fstream>

namespace patch
{
template < typename T > std::string to_string(const T& n)
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}
}
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( patch::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( patch::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( patch::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

void Edge::setfrom(int from)
{
    m_from=from;
}

void Edge::setto(int to)
{
    m_to=to;
}

/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    //affichage de boutton supp
    m_tool_box.add_child(m_boutonsupp);//affichage boutton sur la zone des bouttons
    //m_boutonsupp.set_dim(40,30);//coordoner du boutton sur lecran
    m_boutonsupp.set_frame(2,2,40,30);//
    // m_boutonsupp.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_boutonsupp.set_bg_color(NOIR);
    m_boutonsupp.add_child(m_bouton_supp_image);//ajout de l'image
    m_bouton_supp_image.set_pic_name("supp.png");

    //affichage de boutton save
    m_tool_box.add_child(m_boutonsave);//affichage boutton sur la zone des bouttons
    //m_boutonsave.set_dim(40,30);
    m_boutonsave.set_frame(2,50,40,30);
    //m_boutonsave.set_gravity_x(grman::GravityX::Right);
    m_boutonsave.set_bg_color(NOIR);
    m_boutonsave.add_child(m_bouton_save_image);//ajout de l'image
    m_bouton_save_image.set_pic_name("save.png");

    m_tool_box.add_child(m_boutonajouter);
    m_boutonajouter.set_frame(2,90,40,30);
    m_boutonajouter.set_bg_color(NOIR);
    m_boutonajouter.add_child(m_boutonajouter_image);
    m_boutonajouter_image.set_pic_name("ajouter.png");



}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

void Graph::chargergraphe()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    ///std::string nomgraphe;
    ///std::cout << "Entrez le nom du fichier contenant le graphe : " << std::endl;
    ///std::cin >> nomgraphe;

    std::ifstream fichier("Marin_Antartique.txt");
    if(fichier)
    {
        //Tout est prêt pour la lecture.
    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }

    /// on récupère le nombre de sommets et d'arcs du graphe en début de fichier :
    int nbarcs, nbsommets;
    fichier >> nbsommets;
    fichier >> nbarcs;

    /// on récupère les informations de chaques sommets
    int i;
    int valeur1, valeur3, valeur4;
    double valeur2;
    std::string nomdelimage;

    for(i=0; i<nbsommets; i++)
    {
        fichier >> valeur1;
        fichier >> valeur2;
        fichier >> valeur3;
        fichier >> valeur4;
        fichier >> nomdelimage;


        add_interfaced_vertex(valeur1, valeur2, valeur3, valeur4, nomdelimage);
    }

    /// on récupère les informations de chaques arcs
    for(i=0; i<nbarcs; i++)
    {
        fichier >> valeur1;
        fichier >> valeur3;
        fichier >> valeur4;
        fichier >> valeur2;

        add_interfaced_edge(valeur1, valeur3, valeur4, valeur2);

        //m_edges[i].setfrom(valeur3);
        //m_edges[i].setto(valeur4);

    }
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    //si on appuie sur suprrimer
    if(m_interface->m_boutonsupp.clicked())
    {
        /*std::cout<< "ok"<<std::endl ;
        int a;
        std::cout<< "quel sommet voulez vous supp" <<std::endl;
        std::cin>>a;
        test_remove_edge(a);*/
        //action a faire
        test_remove_sommet();
    }

    //si on appuie sur sauvegarder
    if(m_interface->m_boutonsave.clicked())
    {
        std::cout<< "bon"<<std::endl ;

        sauvegarderfichier();


    }

     if(m_interface->m_boutonajouter.clicked())
    {
        std::cout<< "mal"<<std::endl ;
        std::cout<< "Quel sommet voulez vous rajouter?"<<std::endl ;
        int a;
        std::cin>>a;
        ajoutsommetutilisateur(a);




    }



}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);


    m_edges[idx].m_from=id_vert1;
    m_edges[idx].m_to=id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

    /// OOOPS ! Prendre en compte l'arc ajouté dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);








}

//void Graph::algodeconnexion()
//{
//    int nbsommets, i,j,k;
//    int ctp=0;
//    nbsommets=m_vertices.size();
//
//    std::vector<std::vector<Vertex>> mapi;
//    std::vector<Vertex> sauvegarde;
//
//    for(i=0;i<nbsommets;i++)
//    {
//        sauvegarde.push_back(m_vertices[i]);
//        test_remove_vertex(i);
//        nbsommets=m_vertices.size();
//
//        for(j=0;j<nbsommets;j++)
//        {
//            if(m_vertices[j].m_in.size()==0 && m_vertices[j].m_out.size()==0)
//            {
//                mapi[cpt].push_back(m_vertices[j]);
//                cpt++;
//            }
//        }
//
//        for(k=0;k<sauvegarde.size();k++)
//        {
//            test_ajouter_vertex(sauvegarde[k]);
//        }
//
//        for(k=0;k<sauvegarde.size();k++)
//        {
//            sauvegarde.pop_back();
//        }
//
//
//    }
//
//    for(i=0;i<mapi.size();i++)
//    {
//        std::cout << i << " : ";
//
//        for(j=0;j<mapi[i].size();j++)
//        {
//           /// afficher le numero du sommet
//        }
//
//        std::cout << std::endl;
//    }
//}


/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);
  //  std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge);
    }

/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );
/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
   /* std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;*/

  /* int b= m_vertices[1].m_in[0];
   std::cout<<" " << b;*/


}

//fonction pour supprimer sommet
void Graph::test_remove_sommet()
{

    std::cout<< "Quel sommet voulez vous supprimer?? " << std::endl;
    int a;
    std::cin>>a;

    Vertex &remed=m_vertices.at(a);

    std::cout<< m_vertices[a].m_out.size() << std::endl;
    //suppression de ses aretes entrant
    for(int i=0; i<m_vertices[a].m_in.size(); i++)
    {
            int b= m_vertices[a].m_in[i];
             test_remove_edge(b);
    }

    //suppression de ses aretes sortant
    for(int i=0; i<m_vertices[a].m_out.size(); i++)
    {
        //std::cout<< m_vertices[a].m_in.size() << " ";

    std::cout<< m_vertices[a].m_out.size() << std::endl;
    std::cout << " i " << i << std::endl;
            int b= m_vertices[a].m_out[i];
             test_remove_edge(b);
    }

    for(auto& elem: m_edges)
    {
       if((elem.second.m_to==a)||(elem.second.m_from==a))
       {
           test_remove_edge(elem.first);
       }


    }




     m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );
     m_vertices.erase(a);

}
void Graph::sauvegarderfichier()
{
    std::string nom;
    std::cout << "Entrez le nom du fichier de sauvegarde : " << std::endl;
    std::cin >> nom;

    nom=nom+".txt";
    std::ofstream fichier(nom.c_str());

    int nbsommets, nbarcs;
    nbsommets=m_vertices.size();
    nbarcs=m_edges.size();

    fichier << nbsommets;
    fichier << " ";
    fichier << nbarcs << std::endl;

    for (const auto& elem : m_vertices)
    {
        fichier << elem.first << " ";
        fichier << elem.second.m_value << " ";
        fichier << elem.second.m_interface->m_top_box.get_posx() << " ";
        fichier << elem.second.m_interface->m_top_box.get_posy() << " ";
        fichier << elem.second.m_interface->m_img.getpicname() << std::endl;
    }

    for (const auto& elem2 : m_edges)
    {
        fichier << elem2.first << " ";
        fichier << elem2.second.m_from << " ";
        fichier << elem2.second.m_to << " ";
        fichier << elem2.second.m_weight << std::endl;
    }


}

void Graph::ajoutsommetutilisateur(int a)
{
    //m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);


    std::ifstream fichier("Marin_Antartique.txt");
    if(fichier)
    {
        //Tout est prêt pour la lecture.
    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }

    /// on récupère le nombre de sommets et d'arcs du graphe en début de fichier :
    int nbarcs, nbsommets;
    fichier >> nbsommets;
    fichier >> nbarcs;

    /// on récupère les informations de chaques sommets
    int i;
    int valeur1, valeur3, valeur4;
    double valeur2;
    std::string nomdelimage;

    for(i=0; i<nbsommets; i++)
    {
        fichier >> valeur1;
        fichier >> valeur2;
        fichier >> valeur3;
        fichier >> valeur4;
        fichier >> nomdelimage;

        if(a==valeur1)
        {
        add_interfaced_vertex(valeur1, valeur2, valeur3, valeur4, nomdelimage);
        }
    }


    /// on récupère les informations de chaques arcs
   for(i=0; i<nbarcs; i++)
    {
        fichier >> valeur1;
        fichier >> valeur3;
        fichier >> valeur4;
        fichier >> valeur2;

     if(valeur3==a)
      {
          std::cout<< " i= " << i;
           std::cout<< " pk1" << "  " << valeur1 << " " << valeur3<< " " << valeur4  << " " << valeur2;
           std::cout<< std::endl;

        add_interfaced_edge(valeur1, valeur3, valeur4, valeur2);


        m_edges[i].setfrom(valeur3);
        m_edges[i].setto(valeur4);
      }


     if(valeur4==a)
      {

        std::cout<< " i= " << i;
           std::cout<< " pk2" << "  " << valeur1 << " " << valeur3<< " " << valeur4  << " " << valeur2;
           std::cout<< std::endl;
        add_interfaced_edge(valeur1, valeur3, valeur4, valeur2);
    m_edges[i].setfrom(valeur3);
    m_edges[i].setto(valeur4);
    std::cout<< " pk3" << " "<< i << " "<<m_edges[i].m_from << " "<<  m_edges[i].m_to;



      }




    }












}


