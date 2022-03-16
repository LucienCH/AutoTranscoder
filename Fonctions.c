#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "Fonctions.h"

// -- variables communes --

struct stat stat_fd_base;

// -- initialise la structure --
int initTab(tabTitre_t * tab)
{
    tab->nbTitre = 0;

    for(int i = 0; i < NOMBRE_TITRE; i++)
    {
       for(int j = 0; j < NOMBRE_CHAR; j++)
       {
           strcpy(tab->tabTitre[i], (char *)".");
       }
    }

    return 1;
}


// -- applique une expression régulière pour séléctionner les fichiers --
int regTest(char * filename)
{

    char* pattern = ".*.mkv|avi|mp4";

    regex_t re;
    
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    
    int status = regexec(&re, filename, 0, NULL, 0);
    
    regfree(&re);
    
    if (status != 0) return 0;
    
    return 1;
}

// -- affiche le nombre et/ou la liste de film
int dispList(tabTitre_t * tab, int etat)
{
    if(etat)
    {
        printf("Nombre de films : %d \n", tab->nbTitre);
    }
    else
    {
        printf("Nombre de films : %d \n", tab->nbTitre);
        
        for(int i = 0; i < tab->nbTitre; i++)
        {
            printf("Numéro : %d | titre : %s \n", tab->nbTitre, tab->tabTitre[i]);
        }
    }
    return 1;
}

int affichage(int fd)
{
    printf("Tentative de lecture du fichier ... \n");

    system("cat log.txt | grep ^frame");

    // FILE * fic;

    // fic = fopen("log.txt", O_RDONLY);

    // struct stat stat_fd_new;

    // if(stat(fd, &stat_fd_new) == -1)
    // {
    //     perror("Erreur dans le fichier log.txt stat_fd_new \n");
    //     exit(EXIT_FAILURE);
    // }

    // if(difftime(stat_fd_base.st_mtim, stat_fd_new.st_mtim))
    // {
    //     fscanf(fic, "%s", )
    // }
    return 1;
}



// -- creer un nouveau dossier de sortie --
int newDir(char * dirName)
{
    char newDirName[1024];

    strcpy(newDirName, dirName);
    strcat(newDirName, "Sortie/");

    printf("new dir name : %s\n", newDirName);
    
    struct stat st = {0};

    if (stat(newDirName, &st) == -1) 
    {   
        printf("Il n'existe pas je vais le créer \n");
        mkdir(newDirName, 0776);
        return 1;
    }
    return 0;
}


// -- incrémente le nombre de films dans la liste et affecte le titre des films --
int affectation(tabTitre_t * tab, char * __restrict__ source, int position)
{   

    tab->nbTitre += 1;
    strcpy(tab->tabTitre[position], source);

    // printf("Entrée numéro : %d | nom : %s \n", position, tab->tabTitre[position]);

    return 1;
}


// -- liste les fichiers d'un dossier et les affectes dans une structure --
int listFilm(DIR * dr, struct dirent * de, tabTitre_t * tab, char * __restrict__ source)
{
    int position = 0;
    
    while((de = readdir(dr)) != NULL)
    {
        if(de->d_type == DT_REG && (regTest(de->d_name) == 1))
        {
            // printf("pos : %d\n", position);
            // printf("%s\n", de->d_name);
            
            affectation(tab, de->d_name, position);
            printf("Entrée numéro : %d | nom : %s \n", position, tab->tabTitre[position]);
            
            position += 1;
        }
    }
    return 1;
}


// -- execute la fonction de transcodage sur chaque fichiers --
int transcode(char * path, char * filename)
{
    char output[256];
    char input[256];

    // ffmpeg -hwaccel cuda -hwaccel_output_format cuda -i input -c:v h264_nvenc -preset slow output


    strcpy(output, path);
    strcat(output, "Sortie/");
    strcat(output, filename);
    
    strcpy(input, path);
    strcat(input, filename);

    printf("Input : %s \n", input);
    printf("Output : %s \n\n\n", output);

    
    char *args[] = {"ffmpeg", "-hwaccel", "cuda","-hwaccel_output_format", "cuda", "-i", input, "-c:v", "h264_nvenc", "-preset", "slow",  output, NULL};

    //char *args[] = {"ffmpeg", "-i", input, "-vcodec", "libx264", "-acodec", "aac", output, NULL};


   
    int fd;

    if ((fd = open("log.txt", O_CREAT|O_RDWR, 0666)) == -1)
    {
        perror("Problème ouverture du fichier log.txt \n");
        exit(-1);
    }
    
    // -- on ferme les entrées/ sorties standard
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    
    if( execvp("ffmpeg", args) == -1)
    {
        perror("Erreur dans le execvp ... \n");
        exit(-1);
    }
    
    return 1;
}

// -- execute la fonction de transcodage sur une liste de films --
int multiTrans(char * path, tabTitre_t * tab, tabTitre_t * tabFin)
{
    // printf("NB Films multiTrans : %d\n", tab->nbTitre);
    pid_t pid;
    int child_stat;
    


    for(int i = 0; i < tab->nbTitre; i++)
    {
        switch (pid = fork())
        {
            case -1:
                perror("Erreur dans le fork \n");
                exit(-1);
            break;
            
            // -- Processus du fils -- 
            case 0:
                // printf("Je suis ton fils \n");
                
                

                if( transcode(path, tab->tabTitre[i]) != 1)
                {
                    perror("Erreur dans le transcodage ... \n");
                    exit(-1);
                }

            break;
        
            // -- Processus du père --
            default: 
                waitpid(pid, &child_stat, 0);
                
                // printf("CT child : %d\n", WEXITSTATUS(child_stat));

                if(WEXITSTATUS(child_stat) == 0)
                {
                    tabFin->nbTitre += 1;
                    strcpy(tabFin->tabTitre[i], tab->tabTitre[i]);
                }
                
                // printf("Je sui ton père \n");
                // dispList(tabFin, 0);
                printf("Nombre de fichiers transcodés : %d \n", tabFin->nbTitre);
                
            break;
        }
    }
    return 1;
}