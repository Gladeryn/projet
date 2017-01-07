#include <stdio.h>
#include "f_encode.h"
#include "f_image.h"
#include <SDL_phelma.h>


void ecritureimagepgm2(char* fic, unsigned char** img, int pnl, int pnc)
{
    FILE* fichier=NULL;
    fichier=fopen(fic,"wb");
    
    fprintf(fichier,"P5\n");
    fprintf(fichier,"%d %d\n",pnc,pnl);
    fprintf(fichier,"255\n");
    int i,j;
    for(i=0;i<pnl;i++)
    {
        fwrite(img[i],sizeof(unsigned char),pnc,fichier);
    }
    fclose(fichier);   
}

int main(int argc, char** argv)
{
    if(argc != 5)
    {
        printf("\nsyntaxe : encode <imageIn.pgm> <fileToHide> <imageOut.pgm> <nbbitsparpixel>\n");
        return -1;
    }
    int pnl=0;
    int pnc=0;
    unsigned char ** image = lectureimage8( argv[1],&pnl,&pnc);
    if(image == NULL)
    {
        printf("l'image %s ne s'ouvre pas\n",argv[1]);
        return -1;
    }
    
   unsigned char ** image_copie = NULL;
   image_copie = alloue_image(pnl,pnc);
    
   int i,j;
    for(i=0;i<pnl;i++)
    {
        for(j=0;j<pnc;j++)
        {
            image_copie[i][j] = image[i][j];
        }
    }
    libere_image(image);
   
    
    if( imencode(argv[2],image_copie,*argv[4],pnl,pnc)!=0)
    {
        printf("Une erreur est survenue lors de l'encodage\n");
        return -1;
    }
    ecritureimagepgm2(argv[3],image_copie,pnl,pnc);
}


