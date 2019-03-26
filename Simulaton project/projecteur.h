/*!
 \file projecteur.h
 \brief l'interface de projecteur.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */
 
#ifndef PROJECTEUR_H
#define PROJECTEUR_H

#include "utilitaire.h"

typedef struct Projecteur Projecteur_t;

void lecture_projecteur(char *p, int *etat);

Projecteur_t * projecteur_new(Angle_t angle, Point_t position);

int get_nbre_projecteur(void);

Point_t get_p1_projecteur(int i);

Point_t get_p2_projecteur(int i);

void draw_projecteur(void);

void projecteurs_vers_fichier(FILE * fp);

void destruc_proj(int i);

void destruc_last_proj();

void increment_nb_projecteur(void);

void reset_projecteur();

int proj_pls_prche(Point_t Psc);

void draw_proj_sc(int proj_sel);

void projecteurs_projettent();

#endif

