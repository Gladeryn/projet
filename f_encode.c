#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "f_encode.h"

/*
* Cache l’octet « b » dans l’image img, en démarrant au pixel « k ».
* Cette fonction cache l’octet « b » dans les pixels k,k+1,k+2,k+3), etc.
selon le nombre de bits
* de poids faible bitParPixel cachés dans un pixel.
* PARAMETRES :
*
img : l’image dans laquelle cacher l’octet « b ».
*
b : l’octet à cacher, à raison de bitParPixel bits par pixel.
*
k : indice du pixel ou on commence à cacher l’octet.
* RETOUR : l’indice du prochain pixel pour cacher le prochain octet secret.
*
Avec 2 bits par pixel, c’est k+4.
*/

int cacheunoctet( unsigned char** img, unsigned char b,int k, char bitParPixel) 
{
    
    int n;
    switch(bitParPixel)
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
    
    int i;    
    int bit = 0b1;
    bitParPixel = 0b0;
    for(i=0;i<n;i++)   //generation du masque de bitParPixel (Avec bitParPixel=4, le masque est 0b1111)
    {
        bitParPixel = bitParPixel | bit;
        bit <<=1;
    }
    i=0;
    while(8./n-i>0)
    {
         if(8-n*(i+1)<0)
            *(*img+k)=(*(*img+k) & (~bitParPixel)) ^ ((b <<(-8+n*(i+1)))&bitParPixel);
        else
            *(*img+k)=(*(*img+k)& (~bitParPixel)) ^ ((b >>(8-n*(i+1)))&bitParPixel);
        k+=1;
        i++;
    }
    return k;
}


/*
 *  Fonction de test de cacheunoctet
 * 
 * Test le cas potentiellement pathologique du codage sur nombre de bit non multiple de 8
 * verifie que l'indice retourné est le bon
 * verifie que la valeur des pixels apres les avoir caché est celleprévue
 * 
 */


void test_cacheunoctet()
{
    unsigned char** image_test;
    unsigned char * image_test1;
    unsigned char * image_test2;
    image_test1 = malloc(sizeof(unsigned char)*4);
    image_test1[0] = 0xAB;
    image_test1[1]=0xCD;
    image_test1[2]=0xEF;
    image_test1[3]=0x87;
    image_test2=malloc(sizeof(unsigned char)*4);
    image_test2[0] = 0xAB;
    image_test2[1]=0xCD;
    image_test2[2]=0xEF;
    image_test2[3]=0x87;
    image_test = malloc(sizeof(image_test1)*2);
    image_test[0]=image_test1;
    image_test[1]=image_test2;
    
    unsigned char b = 0b10101101;
    int k=0;
    int testok = 1;
    printf("==========Test fonction cacheunoctet de f_encode.c========== \n");
    char bitParPixel = '3';
    k = cacheunoctet(image_test,b,k,bitParPixel);
    if(k!=3)
    {
        testok=0;
        printf("cacheunoctet : Erreur indice pixel\n");
    }
    else
        printf("[OK] : indice pixel\n");
    if(image_test[0][0] != 0b10101101)
    {
        printf("cacheunoctet : Erreur  information caché\n");
        testok = 0;
    }
    else
        printf("[OK] : premier pixel\n");
     if(image_test[0][1] != 0b11001011)
     {
        printf("cacheunoctet : Erreur  information caché\n");
        testok = 0;
     }
     else
        printf("[OK] : second pixel\n");
    if(image_test[0][2] != 0b11101010)
    {
        testok=0;
        printf("cacheunoctet : Erreur  information caché\n");
    }
    else
        printf("[OK] : troisieme pixel\n");
    if(image_test[0][3] != 0b10000111)
    {
        testok=0;
        printf("cacheunoctet : Erreur  information caché\n");
    }
    else
        printf("[OK] : dernier pixel\n");    
    free(image_test2);
    free(image_test1);
    free(image_test);
    if(testok==1)
        printf("==============f_encode.c : cacheunoctet [OK]================\n");
    else
        printf("==============[ERREUR] f_encode.c : cacheunoctet================\n");
}

/*
Cache le contenu du fichier de nom fileName dans l’image img,
à raison de bitParPixel bits de poids faible par pixels.
Cette fonction commence par cacher le nom et la taille du fichier à cacher,
puis lit le contenu du fichier octet par octet et cache ce contenu
PARAMETRES :
fic: nom du fichier contenant le message à cacher
img : l’image dans laquelle cacher le fichier.
bitParPixel : nombre de bits de poids faible utilisé
*
nl : nombre de lignes de l’image
*
nc : nombre de colonnes de l’image
* RETOUR : 0 en l’absence d’erreur, un nombre non nul en cas d’erreur.


Information a cacher :
12 caracteres contenant le nom du fichier caché
4 octets contenant le nombre d'octets du fichier caché
Le message (contenu du fichier)
*/

int imencode(char *fic, unsigned char**img, char bitParPixel, int nl, int nc)
{
    FILE* fichier = NULL;
    unsigned long taille;
    if((fichier=fopen(fic,"r"))==NULL)
    {
        printf("Erreur ouverture fichier\n");
        return -1;
    }
    
    fseek(fichier,0,SEEK_END);
    taille = ftell(fichier); // on lit la position du curseur placé à la fin du fichier, c'est sa taille en octets
    taille = taille + 12 + 4; // la taille totale à cacher en octets
    if(taille/((float)bitParPixel) > taille/bitParPixel) // taille devient la taille nécessaire en pixel
        taille = taille/bitParPixel +1;
    else
        taille /=bitParPixel;
    
    if(taille>nl*nc)
    {
        printf("Image trop petite\n");
        return -1;
    }
    
    // encodage du nom du fichier
    int i;
    int k = 0; // indice du pixel à inscrire
    if(strlen(fic)+1>12)
    {
        printf("titre fichier trop long\n");
        return -1;
    }
    
    for(i=0;i<12;i++) 
    {
        if(i<strlen(fic))
            k=cacheunoctet(img,(unsigned char)(fic[i]),k,bitParPixel);
        else if(i==strlen(fic))
            k = cacheunoctet(img,'\0',k,bitParPixel);
        else
            k = cacheunoctet(img,0x00,k,bitParPixel);
    }
        
    fseek(fichier,0,SEEK_END);
    unsigned int len_message = ftell(fichier);
    fclose(fichier);
    fichier = NULL;
    if((fichier=fopen(fic,"rb"))==NULL)
    {
        printf("Erreur ouverture fichier");
        return -1;
    }
    if(len_message>=4294967295) // valeur maximale codée sur 32 bits (il faudrait déja un fichier de plus de 4GiO donc une image d'au moins cette taille). Le = sert a considerer le message comme trop long si est d'exactement la valeur maximale possible, car de toute manière un unsigned int ne peut pas prendre de valeur plus grande. On s'assure ainsi qu'aucune information incomplète ne puisse etre encodée dans l'image. Sinon il faudrait prendre des uint64. 
    {
        printf("Taille message trop grande ou image trop petite\n");
        return-1;
    }
    
    k=cacheunoctet(img,(unsigned char)(len_message & 0xFF000000),k,bitParPixel);//cache le premier octet de la taille
    k=cacheunoctet(img,(unsigned char)(len_message & 0x00FF0000),k,bitParPixel);//Le deuxieme
    k=cacheunoctet(img,(unsigned char)(len_message & 0x0000FF00),k,bitParPixel);//troisieme
    k=cacheunoctet(img,(unsigned char)(len_message & 0x000000FF),k,bitParPixel);//quatrieme
      
    char octet;
    while(fread(&octet,1,1,fichier)!=0)
    {
        if(octet != EOF)
            k=cacheunoctet(img,octet,k,bitParPixel);
    }
    
    fclose(fichier);
}

/*
 * affiche l'image sur le terminal, ne sert que pourle debugage 
 */

void affiche( unsigned char** im, int nl, int nc) { 
    
    int i,j ;
    
    for (i=0 ; i<nl ; i++)
    {
        for (j=0 ; j<nc ; j++)
        {
            printf("%d " ,im[i][j]) ;
        }
        printf("\n ") ;
    }
}
/*
 * fonction de test de imencode
 * 
 * crée un fichier et une image, et verifie qu'apres encodage la valeur des pixels est la bonne
 */
void test_imencode()
{
    FILE* fichier=NULL;
    char* fic ="a.txt"; 
    if((fichier = fopen(fic,"wb"))==NULL)
    {
        printf("fichier inouvrable\n");
        exit(-1);
    }
    unsigned char message[7] = "banana";
    fprintf(fichier,"%s\n",message);
    fclose(fichier);
    fichier = NULL;
    if((fichier = fopen(fic,"rb"))==NULL)
       exit(-1);
    unsigned char* img1 = calloc(25,sizeof(unsigned char));
    unsigned char* img2 = calloc(25,sizeof(unsigned char));
    unsigned char ** img = calloc(2,sizeof(img2));
    img[0]=img1;
    img[1]=img2;
    imencode(fic,img,8,25,2);
    int test = 0;
    printf("\n");
    printf("==============Test fonction imencode de f_encode.c==============\n");
    if(img[0][0] != 'a' || img[0][1] != '.' || img[0][2]!='t' || img[0][3]!='x' || img[0][4]!='t' || img[0][5] != '\0' || img[0][5] != 0x00 || img[0][6]!=0x00|| img[0][7]!=0x00|| img[0][8]!=0x00|| img[0][9]!=0x00|| img[0][10]!=0x00|| img[0][11]!=0x00)
    {
        printf("imencode : erreur encodage titre\n");
        test=1;
    }
    
    else
        printf("[OK] : nom fichier caché\n");
    
    if(img[0][12] != 0x00 || img[0][13] != 0x00 || img[0][14] != 0x00 || img[0][15] != 0x07) //caractere '\o' dans le message encodé
    {
        test = 1;
        printf("imencode : erreur encodage taille message\n");
    }
    else
        printf("[OK] : taille message caché\n");
    
    if(img[0][16]!='b' ||img[0][17]!='a' ||img[0][18]!='n' ||img[0][19]!='a' ||img[0][20]!='n' ||img[0][21]!='a' )
    {
        printf("imencode : erreur encodage message\n");
        test = 1;
    }
    else
        printf("[OK] : message caché\n");
    
    if(test==1)
        printf("==============[ERREUR] f_encode.c : imencode==============\n\n");
    else
        printf("==============f_encode.c : imencode [OK]==============\n\n");
    fclose(fichier);
    free(img1);
    free(img2);
    free(img);
    remove(fic);
}
