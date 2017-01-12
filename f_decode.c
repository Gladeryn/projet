#include <stdio.h>
#include <SDL_phelma.h>
#include "f_decode.h"

int extraitnbits(int n, int pixel)
{
	int uselessbits = pixel/(pow2(2,n));
	uselessbits=uselessbits*(pow2(2,n));
	pixel = pixel - uselessbits ;
	
	return(pixel) ;
}



int placenbits(int position, int bits)
{
	if (position == 0)
	{
		printf("ERREUR DANS PLACENBITS : POSITION = 0") ;
	}
	else
	{
		printf("%d %d\n",bits, position);
		return((bits<<(position))&0b11111111);
	}
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
	printf("Titre du fichier cahché :%s\n",title);
	
	unsigned int taille;
	taille = extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel)+extraitunoctet(img,&p_k,bitParPixel);
	printf("Taille du fichier : %u\n",taille);	
	return 0;
}
