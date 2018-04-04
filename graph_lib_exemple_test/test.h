#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "grman/grman.h"
#include <iostream>
#include <stack>

/***************************************************
                    THING (test)

  Cette classe correspond au cadre en bas � gauche
  avec diff�rents bidules interactifs assembl�s dessus
  VOIR LE CONSTRUCTEUR ET LA METHODE UPDATE EN DETAIL
  ( dans test.cpp ) pour d�crypter l'utilisation
  des widgets propos�s ( vous pouvez en ajouter d'autres
  en compl�tant widget.h et widget.cpp, r�pertoire
  de projet grman )
****************************************************/
class Thing {

    private :
        /// Utiliser une top_box de type WidgetBox pour encapsuler
        /// tous les �l�ments de l'interface associ�s � votre objet
        grman::WidgetBox m_top_box;

        /// Les Widgets qui constitueront l'interface de l'objet
        grman::WidgetImage m_img;            // Sera l'arbre � droite
        grman::WidgetImage m_img_anime;      // Sera le clown marcheur au milieu
        grman::WidgetCheckBox m_marche;      // Sera la boite � cocher en bas � gauche (faire marcher le clown)
        grman::WidgetText m_legende;         // Sera le texte "CLICK ME" en bas

        grman::WidgetBox m_boite_regles;     // Sera la boite � sliders en haut � gauche
        grman::WidgetVSlider m_regle_reel;   // Sera le slider de gauche (ordonn�e du clow)
        grman::WidgetVSlider m_regle_entier; // Sera le slider de droite ( vitesse marche clown)

        grman::WidgetBox m_boite_boutons;    // Sera la boite � boutons en haut � droite
        grman::WidgetButton m_bouton1;       // Sera le bouton avec le texte NEW
        grman::WidgetText m_bouton1_label;   // Le texte NEW
        grman::WidgetButton m_bouton2;       // Sera le bouton avec l'image de clown m�chant
        grman::WidgetImage m_bouton2_image;  // L'image de clown m�chant
        grman::WidgetText m_dragme;          // Le message "DRAG ME" � droite des 2 boutons

        grman::WidgetEdge m_lien1;           // Sera un lien non orient� entre l'arbre et le clown
        grman::WidgetEdge m_lien2;           // Sera un lien orient� entre la boite � boutons et le clown
        grman::WidgetText m_lien2_label;     // Sera un label sur le lien pr�c�dent
        grman::WidgetEdge m_lien3;           // Sera un lien orient� entre la check box et le clown

        std::stack<grman::WidgetImage *> m_dynaclowns;    // Exp�rimental : les clowns ajout�s/enlev�s avec les boutons

    public :

        /// Le constructeur de la classe (pas forc�ment par d�faut !)
        /// initialise les donn�es des widgets, place la hi�rarchie des sous-cadres etc...
        Thing();

        /// Une m�thode "update" de la classe doit �tre appel�e dans la boucle de jeu
        /// et cette m�thode doit propager l'appel � update sur les widgets contenus...
        /// Cette m�thode fait le lien entre l'interface, les �v�nements, et les cons�quences
        //  ( Cette m�thode pourrait s'appeler autrement, avoir des param�tres... )
        void update();

        /// On a des allocations dynamiques dans m_dynaclowns => � nettoyer dans le destructeur
        ~Thing();

};


#endif // TEST_H_INCLUDED
