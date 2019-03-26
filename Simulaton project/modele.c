/*!
 \file modele.c
 \brief gérer la lecture du ficheir 
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#include <stdio.h>
#include <stdlib.h>

#include "absorbeur.h"
#include "reflecteur.h"
#include "photon.h"
#include "projecteur.h"
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include "modele.h"

enum EtatLecture_t
{
	PROJECTEUR,
	REFLECTEUR,
	ABSORBEUR,
	PHOTON,	
	FINAL
};

int etat = PROJECTEUR;
int delta_photon = ZERO, delta_projecteur = ZERO;
int delta_absorbeur = ZERO, delta_reflecteur = ZERO;

// Lecture de fichier
void modele_lecture(char *p)
{
	char ligne[MAX_LINE];
	FILE * fichier = NULL; 
	if((fichier = fopen (p, "r")) != NULL) 
	{
		while(fgets(ligne, MAX_LINE, fichier)) 
		{
			if((ligne[ZERO] == '#')  ||
			   (ligne[ZERO] == '\n') ||
			   (ligne[ZERO] == '\r'))
				continue;
			else 
				analyse_ligne (ligne);
		}
		if (delta_photon + delta_reflecteur + delta_absorbeur + 
		    delta_projecteur != TOTAL_SUCCESS)
		{
			error_fichier_incomplet();
			exit(EXIT_FAILURE);	
		}
		if (delta_photon + delta_reflecteur + delta_absorbeur + 
		    delta_projecteur == TOTAL_SUCCESS)
		{
		    modele_verification_rendu2();
			error_success();	
		}
	}	
	else 
	{
		error_fichier_inexistant() ;
		exit(EXIT_FAILURE);
	}
}

//délégation de l'analyse d'une ligne
void analyse_ligne (char *ligne)
{
	switch (etat)
	{
		case PROJECTEUR:
			lecture_projecteur(ligne, &etat);
			break;

		case REFLECTEUR:
			delta_projecteur = MODULE_SUCCESS;
			lecture_reflecteur(ligne, &etat);
			break;

		case ABSORBEUR:
			delta_reflecteur = MODULE_SUCCESS;
			lecture_absorbeur(ligne, &etat);
			break;

		case PHOTON:
			lecture_photon(ligne, &etat);
            delta_absorbeur = MODULE_SUCCESS ;
            if(etat == FINAL)
            {
                delta_photon = MODULE_SUCCESS;
            }
		break;
	}
}	

int modele_verification_rendu2(void)
{
    int r = 0;
	r += reflecteur_comparer();
	r += comparer_absorbeur_projecteur();
	r += comparer_absorbeur_reflecteur();
	r += comparer_reflecteur_projecteur();
    return r;
}

void reset_modele_etat ()
{
    etat = PROJECTEUR;
    delta_photon = ZERO;
    delta_projecteur = ZERO;
    delta_absorbeur = ZERO;
    delta_reflecteur = ZERO;
}
   
