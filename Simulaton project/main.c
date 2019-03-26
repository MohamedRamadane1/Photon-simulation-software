/*!
 \file main.c
 \brief gérer tout
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#include <stdlib.h>
#include <stdio.h>

#include "modele.h"
#include "constantes.h"
#include "reflecteur.h"
#include "utilitaire.h"
#include "absorbeur.h"

//void ligneBlue(float x, float y, float d, float a);
int main (int argc, char *argv[])
{
	if(argc == 2)
	{
		ligneBlue(1, 1, 2, 3);
		modele_lecture(argv[1]);
	    return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

