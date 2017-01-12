#include <stdio.h>
#include "f_encode.h"
#include "f_decode.h"
#include <SDL_phelma.h>
#include <stdio.h>

int main()
{
	test_cacheunoctet();
	test_imencode();
    test_extraitnbits();
	return 1;
}
