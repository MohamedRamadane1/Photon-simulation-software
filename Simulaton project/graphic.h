/*!
 \file graphic.h
 \brief gérer graphic.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "utilitaire.h"

void ligneBlue(float x, float y, float d, float a);
void ligneRed(float x, float y, float d, float a);
void ligneGreen(float x, float y, float d, float a);
void circle(float x, float y, float a);
void draw_rectangle(Point_t P1,Point_t P2,Point_t P3,Point_t P4);
void ligneYellow(float x ,float y,float d, float a);

#endif


