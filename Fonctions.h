#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_

#endif 

#define NOMBRE_CHAR 1024
#define NOMBRE_TITRE 256

#include <dirent.h>

typedef struct titre_s
{
    int nbTitre;
    char tabTitre[NOMBRE_TITRE][NOMBRE_CHAR];

}tabTitre_t;



// -- initialise la structure --
int initTab(tabTitre_t * tab);

// -- creer un nouveau dossier de sortie --
int newDir(char * dirName);

// -- incrémente le nombre de films dans la liste et affecte le titre des films --
int affectation(tabTitre_t * tab, char * __restrict__ source, int position);

// -- liste les fichiers d'un dossier et les affectes dans une structure --
int listFilm(DIR * dr, struct dirent * de, tabTitre_t * tab, char * __restrict__ source);

// -- applique une expression régulière pour séléctionner les fichiers --
int regTest(char * filename);

// -- execute la fonction de transcodage sur un film --
int transcode(char * path, char * filename);

// -- execute la fonction de transcodage sur une liste de films --
int multiTrans(char * path, tabTitre_t * tab, tabTitre_t * tabFin);

// -- affiche le nombre et/ou la liste de film
int dispList(tabTitre_t * tab, int etat);