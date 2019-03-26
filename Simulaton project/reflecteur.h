/*!
 \file reflecteur.h
 \brief l'interface de reflecteur.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#ifndef REFLECTEUR_H
#define REFLECTEUR_H

#include "utilitaire.h"

typedef struct Reflecteur Reflecteur_t;

Reflecteur_t * reflecteur_new (Point_t p1, Point_t p2);

void lecture_reflecteur (char *p,  int *etat);

int reflecteur_comparer(void);

int get_nbre_reflecteur(void);

Reflecteur_t * get_tableau_reflecteur(void);

Point_t reflecteur_get_p1(int i);

Point_t reflecteur_get_p2(int i);

int comparer_reflecteur_projecteur(void);

void draw_reflecteur(void);

void reset_reflecteur ();

void reflecteurs_vers_fichier(FILE * fp);

void increment_nb_reflecteur(void);

int refl_pls_prche(Point_t Psc);

void draw_refl_sc(int refl_sel);

void destruc_refl(int i);

void destruc_last_refl();

void reflecteur_reflecter(Reflecteur_t *reflecteur);

void reflecteurs_reflecter();

#endif
