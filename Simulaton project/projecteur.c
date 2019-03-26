/*!
  \file projecteur.c
  \brief gérer les projecteurs 
  \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
  \date 22 mai 2016
  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "projecteur.h"
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#include "graphic.h"
#include "photon.h"

#define M_PI           3.14159265358979323846
#define LONGUEUR_PROJECTEUR (NBPH-1)*EPSIL_PROJ 

static int projecteur_buffer_size = DEFAULT_BUFFER_SIZE;
static int nbre_element_projecteur = VAL_IMPOSSIBLE;
static int c_projecteur = ZERO; 
static int nb = ZERO;

struct Projecteur
{
    Angle_t angle;
    Point_t position;
};

static Projecteur_t * tab_projecteur = NULL;

// Analyse d'une ligne de projecteur, incrémentation d'état
void lecture_projecteur (char* p, int* etat)
{
    float a, b, c1, d, e, f;
    if ( *p == 'F')
    {
        if (((nbre_element_projecteur) < c_projecteur) && 
                (nbre_element_projecteur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_TROP, ERR_PROJECTEUR);
            exit(EXIT_FAILURE);
        }

        if (((nbre_element_projecteur) > c_projecteur) && 
                (nbre_element_projecteur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_TROP, ERR_REFLECTEUR);
            exit(EXIT_FAILURE);
        }

        if (nbre_element_projecteur == VAL_IMPOSSIBLE)
        {	
            error_lect_nb_elements(ERR_PROJECTEUR);
            exit(EXIT_FAILURE);
        }
        else
        {
            *etat = *(etat) + 1;
        }

    }
    if (sscanf(p, "%f%f", &a, &b) == NB_ELEMENT)
    {
        nbre_element_projecteur = a;
        if (nbre_element_projecteur >= projecteur_buffer_size)
        {
            projecteur_buffer_size = nbre_element_projecteur;
        }
        if (tab_projecteur == NULL)
            tab_projecteur = malloc(projecteur_buffer_size * sizeof(Projecteur_t));
    }
    if ((sscanf(p, "%f%f%f", &a, &b, &c1) == NB_COOR_PROJ))
    {
        Point_t point_projecteur;
        point_projecteur.x = a;
        point_projecteur.y = b;
        projecteur_new(c1, point_projecteur);
        c_projecteur = c_projecteur + 1;			
    }
}

// Création d'absorbeur et le stock dans le tableau
Projecteur_t * projecteur_new (Angle_t angle, Point_t position)
{
    if (nb >= projecteur_buffer_size)
    {
        projecteur_buffer_size *= 2;
        tab_projecteur = realloc (tab_projecteur,
                                    projecteur_buffer_size * sizeof (Projecteur_t));
    }
    if ((angle > M_PI) && (angle < -M_PI))
    {
        error_msg ("L'angle doit être inclu entre -pi et pi.");	
        return NULL;
    }

    tab_projecteur[nb].angle = angle;
    tab_projecteur[nb].position = position;
    return &tab_projecteur[nb++];
}

void reset_projecteur()
{
    nbre_element_projecteur = VAL_IMPOSSIBLE;
    c_projecteur = ZERO; 
    nb = ZERO;
}

void projecteurs_vers_fichier(FILE * fp)
{
    int i;
    fprintf (fp, "#projecteur\n");
    fprintf (fp, "%i\n", nb);

    for (i = 0; i < nb; i++)
    {  
        Point_t p = tab_projecteur[i].position;
        float a  = tab_projecteur[i].angle;

        fprintf (fp, "%f %f %f\n", p.x, p.y, a);
    }
    fprintf (fp, "FIN_LISTE\n");
}

int get_nbre_projecteur(void)
{   
    return nb;
}

Point_t get_p1_projecteur(int i)
{   
    Point_t p1;
    p1 = tab_projecteur[i].position;
    return p1;
}	

Point_t get_p2_projecteur(int i)
{  
    Point_t p1 = tab_projecteur[i].position;
    Point_t p2;
    Angle_t alpha = tab_projecteur[i].angle;
    p2.x = p1.x - LONGUEUR_PROJECTEUR * sin(alpha);
    p2.y = p1.y + LONGUEUR_PROJECTEUR * cos(alpha);
    return p2;
}

void draw_projecteur(void)
{
    int i;
    Point_t p1, p2;
    for (i = 0; i < nb; i ++)
    {    
        p1 = get_p1_projecteur(i);
        p2 = get_p2_projecteur(i);
        ligneGreen(p1.x, p1.y, p2.x, p2.y);
    }
}

int proj_pls_prche(Point_t Psc)
{
    if(nbre_element_projecteur != 0)
    {
        int i;
        int j = 0;
        Bipoint_t b = bipoint_new(tab_projecteur[0].position, Psc);
        double n = bipoint_norme(b);

        for(i = 0; i < nb; i++)
        {
            b = bipoint_new(tab_projecteur[i].position, Psc);
            if(bipoint_norme(b) < n)
            {
                n = bipoint_norme(b);
                j = i;
            }
        }
        return j;
    }
}	

void draw_proj_sc(int proj_sel)
{
    Point_t p1 = tab_projecteur[proj_sel].position;
    Point_t p2 = get_p2_projecteur(proj_sel);
    ligneYellow(p2.x, p2.y, p1.x, p1.y);
}

void destruc_proj(int i)
{
    tab_projecteur[i] = tab_projecteur[nb-1];
    nb--;
    /*
    if(nb != 0)
    {
        Projecteur_t * tab_temporaire = malloc((nbre_element_projecteur-1) * sizeof(Projecteur_t));
        int j;
        for(j = 0;j < nbre_element_projecteur - 1; j ++)
        {
            if(j < i)
            {
                tab_temporaire[j] = tab_projecteur[j];
            }
            if(j >= i)
            {
                tab_temporaire[j] = tab_projecteur[j + 1];  
            }
        }
        nbre_element_projecteur --;
        nb --;   
        free(tab_projecteur);
        tab_projecteur = NULL;
        tab_projecteur = tab_temporaire;
    }
    */
}

void destruc_last_proj()
{
    destruc_proj(nb - 1);
}

void projecteurs_projettent()
{
    int i;
    for (i = 0; i < nb; i ++)
    {
        float x, y;
        Point_t p1 = get_p1_projecteur(i);
        Angle_t alpha = tab_projecteur[i].angle;

        p1.x = p1.x;
        p1.y = p1.y;
        x = -sin(alpha) * EPSIL_PROJ;
        y = cos(alpha) * EPSIL_PROJ;

        int j;
        for (j = 0; j < 5; j ++)
        {
            Point_t pos = p1;
            pos.x = pos.x + j * x;
            pos.y = pos.y + j * y;
            photon_new(pos, alpha);
        }
    }
}

