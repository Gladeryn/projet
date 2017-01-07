#include <stdio.h>
#include <stdlib.h>
/**
* Libère la mémoire préalablement allouée pour une image de nbl*nbc pixels.
* PARAMETRES :
*
p_image : l’image à libérer
*/
void libere_image(unsigned char ** p_image) 
{
        free(p_image);
}

/**
* Alloue une image de nbl*nbc pixels.
* La zone de donnée de l’image, dans le champ img, est contiguë en mémoire.
* La zone de donnée est initialisée à 0.
* PARAMETRES :
*
nbl : nombre de lignes de l’image à allouer
*
nbc : nombre de colonnes de l’image à allouer
* RETOUR : pointeur vers l’image allouée, ou NULL en cas d’erreur
*/
unsigned char** * alloue_image(int nbl, int nbc) 
{
    unsigned char * image=calloc(nbl*nbc,sizeof(unsigned char)); //allocation d'un espace contiguë de memoire de la taille de l'image
    int i;
    unsigned char** * ligne = calloc(nbl,nbc*sizeof(unsigned char*)); //allocation de nbl lignes
    for(i=0;i<nbl;i++)
    {
       * ligne[i] = &image[i*nbc]; 
    }
    return(ligne);
}
