/*!
 \file utilitaire.c
 \brief gérer les outils 
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilitaire.h"
#include "constantes.h"

Vect2_t vecteur_new(float x, float y)
{
	Vect2_t v;
	v.x = x;
	v.y = y;
	return v;
}
Vect2_t bipoint_vecteur(Bipoint_t b) 
{
    return vecteur_new(b.fin.x - b.deb.x, b.fin.y - b.deb.y);
}

Bipoint_t bipoint_new(Point_t deb, Point_t fin)
{
	Bipoint_t b;
    b.deb = deb;
    b.fin = fin;
	return b;
}

Point_t point_new(double a,double b)
{
	Point_t p;
	p.x = a;
	p.y = b;
	return p;
}

double produit_vectoriel(Vect2_t v1, Vect2_t v2)
{
    return (v1.x * v2.y) - (v1.y * v2.x);
}

double norme(Vect2_t v)
{
	return sqrt (v.x * v.x + v.y * v.y); 
}

double bipoint_norme(Bipoint_t b)
{
    return norme(bipoint_vecteur(b));
}

Vect2_t bipoint_normal(Bipoint_t b)
{
    return vecteur_unitaire(vecteur_normal(bipoint_vecteur(b)));
}

double produit_scalaire(Vect2_t v1, Vect2_t v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

Vect2_t vecteur_mult(Vect2_t v, double m)
{
    return vecteur_new(v.x * m, v.y * m);
}

Vect2_t vecteur_normal(Vect2_t v)
{
    return vecteur_new(-v.y, v.x); 
}

Vect2_t vecteur_unitaire(Vect2_t v)
{
    float l = norme(v);
	if(l > -EPSILON_ZERO && l < EPSILON_ZERO)
    {
        return vecteur_new(0,0);
    }
	return vecteur_new(v.x / l, v.y / l);
}

double distance_points(Point_t p1, Point_t p2)
{
    return bipoint_norme(bipoint_new(p1, p2));
}

//retourne 0 si parallele, 1 si colineaire, 2 si superposé, 3 si non paralleles
int para_sup (Bipoint_t b1, Bipoint_t b2)
{
    Vect2_t v1 = bipoint_vecteur (b1);
    Vect2_t u1 = vecteur_unitaire (v1);
    Vect2_t v2 = bipoint_vecteur (b2);
    Vect2_t u2 = vecteur_unitaire (v2);
    double pv = produit_vectoriel (u1, u2);
    Vect2_t vd1d2 = vecteur_new (b2.deb.x - b1.deb.x, b2.deb.y - b1.deb.y);
    Vect2_t ud1d2 = vecteur_unitaire (vd1d2);
    double nvd1d2 = norme (vd1d2);

    if (fabs(pv) <= EPSIL_PARAL)
    {
        double pvdeb = produit_vectoriel(u1, ud1d2);
        if (fabs(pvdeb) <= EPSIL_PARAL)
        {
            double psu1v2 = produit_scalaire(u1, v2);
            double psdeb = produit_scalaire(u1, vd1d2);
            
            if ((psdeb > 0 && ((psdeb + psu1v2 > norme (v1) + EPSIL_CONTACT) || psdeb < -EPSIL_CONTACT)) ||
                (psdeb > 0 && ((psdeb > norme(v1) + EPSIL_CONTACT) || psdeb + psu1v2 < -EPSIL_CONTACT)))
            {
                return 1;
            }
            return 2;
        }
        return 0;
    }
    return 3;
}

//retourne 0 si parallele, 1 si colineaire, 2 si superposé, 3 si non paralleles
/*
int para_sup(Vect2_t v1,Vect2_t v2)
{ 
    Vect2_t vd1d2 = Bipoint_vecteur(b1, b2);
    double pv = produit_vectoriel(vecteur_unitaire(v1), vecteur_unitaire(v2));
    double pvdeb = produit_vectoriel(vecteur_unitaire(v1),vecteur_unitaire(vd1d2));
    double psdeb = produit_scalaire(vd1d2, vecteur_unitaire(v1));
    double psuv = produit_scalaire(vecteur_unitaire(v1), v2);
    if(fabs(pv) <= EPSIL_PARAL)
    {
        if(fabs(pvdeb) <= EPSIL_PARAL)
        {
            if(psuv < 0)
            {
                if(psdeb > 0 && 
                   psdeb + psuv > norme(Bipoint_vecteur(b1, b2)) + EPSIL_CONTACT) || 
                  (psdeb < -EPSIL_CONTACT)
                {
                    return 1;
                }
            }
            if(psuv > 0)
            {
                if(psdeb > 0 && 
                   psdeb > norme(Bipoint_vecteur(b1, b2)) + EPSIL_CONTACT) ||
                  (psdeb + psuv < -EPSIL_CONTACT)
                {
                    return 1;
                }
            }
            return 2;
        }
        return 0;
    }
    return 3;
}
*/

//retourne 1 si s'intersectent , 0 si s'instersectent pas
int intersection(Bipoint_t b1, Bipoint_t b2)
{
    Vect2_t vd1f2 = vecteur_new(b2.fin.x - b1.deb.x, b2.fin.y - b1.deb.y);
    Vect2_t vd2f1 = vecteur_new(b1.fin.x - b2.deb.x, b1.fin.y - b2.deb.y);
    Vect2_t vd1d2 = vecteur_new(b2.deb.x - b1.deb.x, b2.deb.y - b1.deb.y);
    Vect2_t n1 = bipoint_normal(b1);
    Vect2_t n2 = bipoint_normal(b2);
    double ps1d1d2 = produit_scalaire(n1, vd1d2);
    double ps1d1f2 = produit_scalaire(n1, vd1f2);
    double ps2d2d1 = -produit_scalaire(n2, vd1d2);
    double ps2d2f1 = produit_scalaire(n2, vd2f1);

    if((fabs(ps1d1d2) <= EPSIL_CONTACT || fabs(ps1d1f2) <= EPSIL_CONTACT ||
       ps1d1d2 * ps1d1f2 < 0) && (fabs(ps2d2d1) <= EPSIL_CONTACT || 
       fabs(ps2d2f1) <= EPSIL_CONTACT || ps2d2d1 * ps2d2f1 < 0))
    {
        return 1;
    }
    return 0;
}



//retourne 1 si s'intersectent , 0 si s'instersectent pas
/*
int intersection(Vect2_t v1, Vect2_t v2)
{
    Vect2_t vd1f2 = vecteur_new(v2.Pf, v1.Pi);
    Vect2_t vd2f1 = vecteur_new(v1.Pf, v2.Pi);
    Vect2_t vd1d2 = vecteur_new(v2.Pi, v1.Pi);
    double ps1d1d2 = produit_scalaire(vecteur_normal(v1), vd1d2);
    double ps1d1f2 = produit_scalaire(vecteur_normal(v1), vd1f2);
    double ps2d2d1 = -produit_scalaire(vecteur_normal(v2), vd1d2);
    double ps2d2f1 = produit_scalaire(vecteur_normal(v2), vd2f1);
    if((fabs(ps1d1d2) <= EPSIL_CONTACT || fabs(ps1d1f2) <= EPSIL_CONTACT ||
       ps1d1d2 * ps1d1f2 < 0) && (fabs(ps2d2d1) <= EPSIL_CONTACT || 
       fabs(ps2d2f1) <= EPSIL_CONTACT || ps2d2d1 * ps2d2f1 < 0))
    {
            return 1;
    }
    return 0;
}
*/

Point_t bipoint_intersection(Bipoint_t b1, Bipoint_t b2)
{
    Vect2_t vd1d2 = vecteur_new(b2.deb.x - b1.deb.x, b2.deb.y - b1.deb.y);
    Vect2_t u1 = vecteur_unitaire(bipoint_vecteur(b1)); 
    Vect2_t u2 = vecteur_unitaire(bipoint_vecteur(b2)); 
    float u2_X_vd1d2 = produit_vectoriel(u2, vd1d2);
    float u2_X_u1 = produit_vectoriel(u2, u1);

    return vecteur_add(b1.deb, vecteur_mult(u1, u2_X_vd1d2 / u2_X_u1));
}

Point_t pixel_to_coordonne(int x, int y, int wi,  int he, 
                           float g,  float d, float h, float b)
{
	Point_t P1;
	P1.x = ((d - g) * x / wi) + g;
	P1.y = h - (h - b) * y / he;
	return P1;
}

void rectangle_new(Point_t P1, Point_t P2, Point_t P3, Point_t P4, Rectangle_t r)
{
	r.P1 = P1;
	r.P2 = P2;
	r.P3 = P3;
	r.P4 = P4;
}

double get_angle(Vect2_t v1, Vect2_t v2)
{
    double pscalaire = produit_scalaire(v1, v2);
    double Vone = norme(v1);
    double Vtwo = norme(v2);
    double cosalpha = pscalaire / (Vone * Vtwo);
    double alpha = acos(cosalpha);
    return alpha;
}


double get_angle_bipoint(Bipoint_t b1, Bipoint_t b2)
{
    Vect2_t v1 = bipoint_vecteur(b1);
    Vect2_t v2 = bipoint_vecteur(b2);
    return get_angle(v1, v2);
}

void initialiser_points(Point_t *tab_point,Point_t Pi)
{
    int i; 
    for(i = 0; i < 6; i++)
    {
        tab_point[i] = Pi;
    }	
}

void ini_point(Point_t P1, Point_t P2, Point_t P3, 
               Point_t P4, Point_t P5, Point_t P6)
{
	P1.x = 0;
	P1.y = 0;
	P2.y = 0;
	P2.x = 0;
	P3.x = 0;
	P3.y = 0;
	P4.y = 0;
	P4.x = 0;
	P5.x = 0;
	P5.y = 0;
	P6.x = 0;
	P6.y = 0;
}

Vect2_t vecteur_add (Vect2_t v0, Vect2_t v1)
{
    v0.x = v0.x + v1.x;
    v0.y = v0.y + v1.y;
    return v0;
}

Vect2_t vecteur_substract (Vect2_t v0, Vect2_t v1)
{
    v0.x = v0.x - v1.x;
    v0.y = v0.y - v1.y;
    return v0;
}

Vect2_t reflection(Vect2_t v, Vect2_t n)
{
   n = vecteur_mult (n, -2.0 * produit_scalaire(v, n));
   return vecteur_add (v, n);
}

