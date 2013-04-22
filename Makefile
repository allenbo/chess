HDR=resource.h game.h board.h
OBJ=game.o board.o main.o
LIB=`pkg-config --libs gtk+-2.0`
FLG=`pkg-config --cflags gtk+-2.0`

main:main.o game.o board.o
	gcc -g -o main $(LIB) $(FLG) $(OBJ)
main.o:main.c game.h resource.h
	gcc -g -c main.c $(LIB) $(FLG)
game.o:game.c $(HDR)  
	gcc -g -c game.c $(LIB) $(FLG)
board.o:board.c board.h resource.h  
	gcc -g -c board.c $(FLG) $(LIB) 

clean:
	rm *.o main

