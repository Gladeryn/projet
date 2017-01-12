#include <stdio.h>
#include <SDL_phelma.h>
#include "f_decode.h"


int charintoint (char BitParPixel)

{
	int n;
    switch(BitParPixel)
    {
        case '0':
        {n=0;break;}
        case '1':
        {n=1;break;}
        case '2':
        {n=2;break;}
        case '3':
        {n=3;break;}
        case '4':
        {n=4;break;}
        case '5':
        {n=5;break;}
        case '6':
        {n=6;break;}
        case '7':
        {n=7;break;}
        case '8':
        {n=8;break;}
        default:
            n=8;
	}
	return(n);
}

int extraitnbits(int n, int pixel)
{
	return 0xFF&(pixel>>8-n);
}



int placenbits(int position, int bits)
{
	if (position == 0)
	{
		printf("ERREUR DANS PLACENBITS : POSITION = 0") ;
	}
	else
	{
		return((bits<<(position))&0b11111111);
	}
}

 
int extraitunoctet (unsigned char **img, int *p_k, char bitParPixel)

{	
	int n = charintoint(bitParPixel);
	int octet_cache = 0b00000000 ;
	int i = *p_k ;
     
	while (*p_k-i+1 <= 8/n)
	{
		octet_cache+=placenbits(n*(*p_k)-i+1,extraitnbits(n,*(*img+*p_k))) ;
		*p_k+=1;
	}
	return(octet_cache);
}



/**
* Extrait le fichier caché dans l’image « img », à raison de « bitParPixel »
* bits cachés dans un pixel, et recrée le fichier qui contenait le message.
* Cette fonction commence par extraire de l’image le nom du fichier caché,
* puis recrée pas à pas ce fichier à l’identique en extrayant son contenu
* caché dans l’image.
* PARAMETRES :
*
img : l’image dans laquelle est caché le fichier.
*
bitParPixel : nombre de bits de poids faible utilisé
pour cacher le message dans chaque pixel
*
nl : nombre de lignes de l’image
*
nc : nombre de colonnes de l’image
* RETOUR : 0 en l’absence d’erreur, -1 en cas d’erreur.
*/
int imdecode (unsigned char** img, char bitParPixel, int nl, int nc) 
{
	if(nl*nc<(12+4)*bitParPixel)
	{
		printf("Image trop petite\n");
		return(-1);
	}
	char title[12];
	int i;
	int p_k=0;
	for(i=0;i<12;i++)
	{
		title[i] = extraitunoctet(img,&p_k,bitParPixel);
	}
	printf("Titre du fichier caché :%s\n",title);
	
	unsigned int taille;
	taille = extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel);
	printf("Taille du fichier : %u\n",taille);	
	return 0;
}
