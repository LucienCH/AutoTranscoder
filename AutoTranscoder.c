#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#include "Fonctions.h"


int main(int argc, char const *argv[])
{
    // -- definition des variables --
    tabTitre_t tab;
    tabTitre_t tabFin;
    
    char dirName[1024];

    struct dirent * de = NULL;

    DIR * dr;

    // -- Capture des paramètres -
    if(argc < 2)
    {
        perror("Erreur : Il manque des arguments ... \n");
        printf("usage : <nom_programme> <dossier_cible> \n");
        exit(-1);
    }

    strcpy(dirName, argv[1]);
    // printf("test : %s", dirName);

    
    if(newDir(dirName) != 1)
    {
        perror("Erreur dans la création du nouveau dossier ... \n");
        exit(-1);
    }

    dr = opendir(dirName);

    if(dr == NULL)
    {
        perror("Erreur dans opendir ... \n");
        exit(-1);
    }

    // -- initialisation des variables --
    if(initTab(&tab) != 1)
    {
        perror("Impossible d'initialiser le tableau ...\n");
        exit(-1);
    }
    
    
    // -- liste les films et les affecte dans la structure --
    if(listFilm(dr, de, &tab, dirName) != 1)
    {
        perror("Erreur dans listFilm... \n");
        exit(-1);
    }

    printf("Nombre de films : %d\n", tab.nbTitre);
    
    multiTrans(dirName, &tab, &tabFin);

   
    closedir(dr);

    
   











    // printf("Tentative de nom ... \n");
    // printf("Saisir un nom : ");

    // scanf("%[^\n]", nom);

    // printf("Vous avez ecrit : %s \n", nom);

    // strcpy(tab.tabTitre[0], nom);
    

    // printf("Le nom du film est : %s \n", tab.tabTitre[0]);

    // printf("phase 2 ------------------------- \n\n");

    // if(affectation(&tab, nom) == 1)
    // {
    //    for(int i = 0; i < tab.nbTitre; i++)
    //    {
    //        printf("Entrée numéro : %d | nom : %s \n", i, tab.tabTitre[i]);
    //    }
    // }


    





    return 0;
}
