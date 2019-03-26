/*!
 \file absorbeur.h
 \brief l'interface de absorbeur.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#ifndef ABSORBEUR_H
#define ABSORBEUR_H

#include "utilitaire.h"

typedef struct Absorbeur Absorbeur_t;

Absorbeur_t *absorbeur_new (Point_t *points, unsigned int nb_point);

int get_nbre_absorbeur();

int abs_pls_prche(Point_t Psc);

void comparer_absorbeur(Point_t * p, int a);

void lecture_absorbeur(char *p, int *etat);

void draw_absorbeur(void);

int comparer_absorbeur_projecteur(void);

int comparer_absorbeur_reflecteur(void);

void reset_absorbeur ();

void absorbeurs_vers_fichier(FILE * fp);

void crush_absorbeur(Point_t * tab_point, int compteur);

void draw_abs_sc(int abs_sel);

void absorbeur_detruire(int id);

void absorbeur_detruire_dernier();

void absorbeur_absorber(Absorbeur_t *absorbeur);

void absorbeurs_absorber();

#endif

