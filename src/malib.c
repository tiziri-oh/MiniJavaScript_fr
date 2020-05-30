#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include "malib.h"


struct liste* creer_liste(){
	struct liste* lst = (struct liste*) malloc (sizeof(struct liste));
	lst->debut = NULL;
	lst->fin	 = NULL; 
	return lst;
}


struct liste_noeud* creer_liste_noeud(void* valeur){
	struct liste_noeud* lst = (struct liste_noeud*) malloc (sizeof(struct liste_noeud));
	if(lst != NULL){
		lst->valeur  = valeur;
		lst->suivant = NULL; 
	}
	return lst;
}

void ajout_liste_debut(struct liste* l, struct liste_noeud* node){
	if(l == NULL){
			return;
		}

	if((l->debut == NULL) && (l->fin == NULL)){
		l->debut = node;
		l->fin = node;
		node->suivant = NULL;	
	}else{		
		node->suivant = l->debut;
		l->debut = node;
	}	
}

void ajout_liste_fin(struct liste* l, struct liste_noeud* node){
	if(l == NULL){
		return;
	}
	if(l->debut == NULL){
		l->debut = node;
		l->fin = node;
		node->suivant = NULL;
	}else{
		l->fin->suivant = node;
		l->fin = node;
		node->suivant = NULL;
	}
}

void supprimer_liste(struct liste* l){
	if(l == NULL)
		return;
	struct liste_noeud* ptr = l->debut;
	struct liste_noeud* tmp = NULL;
	while(ptr != NULL){
		tmp = ptr->suivant;
		free(ptr->valeur);
		free(ptr);
		ptr = tmp;	
	}
	free(l);
}

char* lire_ligne_fichier (FILE* fichier){
	int   taille = 64;
	char* ligne = NULL;
	int   i = 0, c = 0;

	while (EOF != (c = fgetc(fichier))) {
		if(i == 0){
			ligne = (char*) malloc(sizeof(char) * taille);
		}
		if (i > taille) {//stratégie d'allocation
			taille *= 2;
			ligne = (char*) realloc(ligne, sizeof(char)*taille);
		}
		//printf("getc : %c\n", c);
        ligne[i] = (char) c;
        if(ligne[i] == '\n'){
        	i++;
        	break;
        }
        i++;
    }
    if(ligne != NULL){
    	ligne[i] = '\0';
    }
    return ligne;
}