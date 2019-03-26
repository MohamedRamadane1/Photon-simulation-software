/*!
 \file utilitaire.h
 \brief l'interface de utilitaire.c
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */
 
#ifndef UTILITAIRE_H
#define UTILITAIRE_H


typedef struct Point_t
{
	double x;
	double y;
}Point_t;

typedef Point_t Vect2_t;

typedef struct Rectangle_t
{
	Point_t P1;
	Point_t P2;
	Point_t P3;
	Point_t P4;
}Rectangle_t;

typedef struct Bipoint_t
{
    Point_t deb;
    Point_t fin; 
}Bipoint_t;

typedef double Angle_t;

Vect2_t vecteur_new(float x, float y);

Vect2_t bipoint_vecteur(Bipoint_t b);

Bipoint_t bipoint_new(Point_t deb, Point_t fin);

Point_t point_new(double a,double b);

double produit_vectoriel(Vect2_t v1, Vect2_t v2);

double norme(Vect2_t v);

double bipoint_norme(Bipoint_t b);

Vect2_t bipoint_normal(Bipoint_t b);

double produit_scalaire(Vect2_t v1, Vect2_t v2);

Vect2_t vecteur_mult(Vect2_t v, double m);

Vect2_t vecteur_normal(Vect2_t v);

Vect2_t vecteur_unitaire(Vect2_t v);

double distance_points(Point_t p1, Point_t p2);

int para_sup (Bipoint_t b1, Bipoint_t b2);

int intersection(Bipoint_t b1, Bipoint_t b2);

Point_t bipoint_intersection(Bipoint_t b1, Bipoint_t b2);

Point_t pixel_to_coordonne(int x, int y, int wi,  int he, 
                           float g,  float d, float h, float b);

void rectangle_new(Point_t P1, Point_t P2, Point_t P3, Point_t P4, Rectangle_t r);

double get_angle(Vect2_t v1, Vect2_t v2);

double get_angle_bipoint(Bipoint_t b1, Bipoint_t b2);

void initialiser_points(Point_t *tab_point,Point_t Pi);

void ini_point(Point_t P1, Point_t P2, Point_t P3, 
               Point_t P4, Point_t P5, Point_t P6);

Vect2_t vecteur_add (Vect2_t v0, Vect2_t v1);

Vect2_t vecteur_substract (Vect2_t v0, Vect2_t v1);

Vect2_t reflection(Vect2_t v, Vect2_t n);

#endif

