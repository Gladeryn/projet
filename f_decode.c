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

unsigned char extraitnbits(int n,unsigned char  pixel)
{
    //printf("Extract %d\n",(0xFF&(pixel<<8-n))>>8-n);
	return ((0xFF&(pixel<<8-n))>>8-n);
}

void test_extraitnbits()
{
    unsigned char pixel = 0b01101010;
    if(extraitnbits(2,pixel)!=0b10 || extraitnbits(3,pixel)!=0b010)
        printf("Erreur extraitnbits\n");
    else
        printf("Extraitnbits [OK]\n");
}



unsigned char placenbits(int position, unsigned char bits)
{
	if (position == 0)
	{
        //printf("--------%d\n",bits);
		return bits;
	}
	else
	{
        
        //printf("--------%d %d %d\n",bits,position,(bits<<(position))&0b11111111);
		return((bits<<(position))&0b11111111);
	}
}


 
unsigned char extraitunoctet (unsigned char **img, int *p_k, char bitParPixel)

{	
	int n = charintoint(bitParPixel);
	unsigned char octet_cache = 0b00000000 ;
	int i = 1 ;
     
	while (i*n <= 8)
	{
		octet_cache=octet_cache | (placenbits(8-n*(i),extraitnbits(n,*(*img+*p_k)))) ;
		*p_k+=1;
        i++;
	}
	//printf("Octet : %d\n",octet_cache);
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
	unsigned char title[12];
	int i;
	int p_k=0;
	for(i=0;i<12;i++)
	{
		title[i] = extraitunoctet(img,&p_k,bitParPixel);
	}
	printf("Titre du fichier caché :%s\n",title);
	
	unsigned int taille=0;
	
    unsigned char t1,t2,t3,t4;
    t4 = extraitunoctet(img,&p_k,bitParPixel);
    t3 = extraitunoctet(img,&p_k,bitParPixel);
    t2 = extraitunoctet(img,&p_k,bitParPixel);
    t1 = extraitunoctet(img,&p_k,bitParPixel);
    
    unsigned int mask;
    
    if(t1==0x00)
        taille = 0;
    else if(t2==0x00)
        mask = 0xFF;
    else if(t3==0x00)
        mask = 0xFFFF;
    else if(t4==0x00)
        mask = 0xFFFFFF;
    else
        mask = 0xFFFFFFFF;
    
    taille =( t1&mask)|(t2&mask )|(t3&mask)|(t4&mask);
    printf("Taille du fichier : %u\n",taille);	   
    FILE *fichier=NULL;
    if((fichier=fopen(title,"wb"))==NULL)
    {
        printf("Erreur ouverture fichier\n");
        return -1;
    }
    
    i = 0;
    while(i<taille)
    {
        mask = extraitunoctet(img,&p_k,bitParPixel);//octet a ecrire (on reutilisemask pour  nepas consommer plus de memoire)
        if(fwrite(&mask,1,1,fichier)!=1)
        {    
            printf("Erreur ecriture fichier\n");
            return EXIT_FAILURE;
        }
        i++;
    }
    
    fclose(fichier);
    printf("Message caché écrit dans %s\n",title);

	return 0;
}
