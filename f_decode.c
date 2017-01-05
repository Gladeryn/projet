#include <stdio.h>
#include <SDL_phelma>
#include "f_decode.h"

/*
 *Cache l’octet « b » dans l’image img, en démarrant au pixel « k ».
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

int cacheunoctet( unsigend char** img, unsigned char b,int k, char bitParPixel)
{
    
};
