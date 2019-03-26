/*!
 \file photon.h
 \brief l'interface de photon.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */
 
#ifndef PHOTON_H
#define PHOTON_H

#include "utilitaire.h"

typedef struct Photon Photon_t;

Photon_t * photon_new(Point_t position, Angle_t angle);

Point_t photon_position(int id);

Angle_t photon_angle(int id);

Photon_t * photon_get (int id);

void lecture_photon(char *p, int *etat);

void draw_photon(void);

void reset_photon();

unsigned int get_nb_photon ();

void photons_vers_fichier(FILE * fp);

void photon_detruire(int i);

void photons_update();

Photon_t * photon_get(int id);

void detruct_photon_hors_cadre(double gauche,double droite,double haut,double bas);

#endif

