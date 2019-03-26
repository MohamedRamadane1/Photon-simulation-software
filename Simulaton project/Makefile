# Makefile for generating rendu2.x

all: rendu3.x
LIBS   = -lstdc++ -lglut -lGL -lGLU -lm -Llib -lglui -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi -g
graphic.o:graphic.c graphic.h
	gcc	 -g -c graphic.c -o graphic.o
error.o: error.c 
	gcc  -g -c error.c -o error.o 
projecteur.o: projecteur.c projecteur.h error.h constantes.h utilitaire.h graphic.h
	gcc  -g -c projecteur.c -o projecteur.o
reflecteur.o: reflecteur.c reflecteur.h error.h constantes.h utilitaire.h 
	gcc  -g -c reflecteur.c -o reflecteur.o
photon.o: photon.c photon.h error.h constantes.h utilitaire.h 
	gcc  -g -c photon.c -lm -o photon.o
absorbeur.o: absorbeur.c absorbeur.h error.h constantes.h utilitaire.h 
	gcc  -g -c absorbeur.c -o absorbeur.o
modele.o: modele.c modele.h absorbeur.h reflecteur.h projecteur.h photon.h 
	gcc  -g -c modele.c -o modele.o
utilitaire.o: utilitaire.c
	gcc  -g -c utilitaire.c -o utilitaire.o 
main.o:	main.cpp	modele.h
	gcc	 -g -c main.cpp	-o main.o
rendu3.x: graphic.o absorbeur.o projecteur.o reflecteur.o photon.o modele.o error.o utilitaire.o main.o
	gcc graphic.o absorbeur.o error.o projecteur.o reflecteur.o photon.o modele.o utilitaire.o main.o -lm -o rendu3.x ${LIBS}
	
clean:
	rm *.o
	
