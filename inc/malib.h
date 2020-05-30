#ifndef MALIB_H
#define MALIB_H

struct liste_noeud{
	void* valeur;
	struct liste_noeud* suivant;
};

struct liste{
	struct liste_noeud* debut;
	struct liste_noeud* fin;
};

struct liste_noeud* creer_liste_noeud(void* valeur);

struct liste* creer_liste();

void ajout_liste_debut(struct liste* l, struct liste_noeud* node);

void ajout_liste_fin(struct liste* l, struct liste_noeud* node);

void supprimer_liste(struct liste* l);

//Autres fonctions

char* lire_ligne_fichier (FILE* fichier);

#endif
