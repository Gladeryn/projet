DIRSDL=/users/prog1a/C/librairie/2011

CFLAGS=-g -I$(DIRSDL)/include -I$(DIRSDL)/include/SDL -I/usr/local/include -I/usr/local/include/SDL -I/usr/include/SDL 
LDFLAGS= -L$(DIRSDL)/lib -L/usr/local/lib -lSDL -lSDL_ttf -lSDL_image -lSDL_draw -lSDL_phelma -lSDL_sound -lX11 -lm
#exemple : exemple.o
encode : encode.o f_image.o f_encode.o 
	gcc -o $@ $^ $(LDFLAGS)
decode : f_image.o f_decode.o decode.o
	gcc -o $@ $^ $(LDFLAGS)
all : encode decode
test : test.o f_encode.o f_decode.o f_image.o 
	gcc -o $@ $^ $(LDFLAGS)

%.o : %.c
	gcc -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o encode 
