/*!
  \file absorbeur.c
  \brief gérer l'absorbeur 
  \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
  \date 22 mai 2016
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "error.h"
#include "absorbeur.h"
#include "constantes.h"
#include "utilitaire.h"
#include "reflecteur.h"
#include "projecteur.h"
#include "photon.h"
#include "graphic.h"

static int absorbeur_buffer_size = DEFAULT_BUFFER_SIZE;
static int nb_element_absorbeur = VAL_IMPOSSIBLE;
static int c_absorbeur = ZERO; 
static int nb = ZERO;

struct Absorbeur
{
    // tableau de points
    Point_t points[MAX_PT];
    unsigned int nb_point;
};

static Absorbeur_t * tab_absorbeur = NULL;

// Analyse d'une ligne d'absorbeur, incrémentation d'état
void lecture_absorbeur(char *tab, int *etat)
{
    if (tab[ZERO] == 'F')
    {
        if (((nb_element_absorbeur) < c_absorbeur) && 
            (nb_element_absorbeur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_ABSORBEUR, ERR_TROP);
            exit(EXIT_FAILURE);
        }
        if ((nb_element_absorbeur) > c_absorbeur && 
            (nb_element_absorbeur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_ABSORBEUR, ERR_PAS_ASSEZ);
            exit(EXIT_FAILURE);
        }
        if (nb_element_absorbeur == VAL_IMPOSSIBLE)
        {
            error_lect_nb_elements(ERR_ABSORBEUR);
            exit(EXIT_FAILURE);
        }
        else	
        {
            *etat = *(etat) + 1;
        }
    }
    float a1, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
    if ((sscanf(tab, "%f%f", &a1, &x1) == NB_ELEMENT) && 
       (nb_element_absorbeur == VAL_IMPOSSIBLE))
    {
        nb_element_absorbeur = a1;
        if (nb_element_absorbeur >= absorbeur_buffer_size)
        {
           absorbeur_buffer_size = nb_element_absorbeur;
        }
        if (tab_absorbeur == NULL)
            tab_absorbeur = malloc(absorbeur_buffer_size * sizeof(Absorbeur_t));
    }
    if ((sscanf(tab, "%f%f%f%f%f%f%f%f%f%f%f%f%f", 
                &a1, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4, &x5, &y5, &x6, &y6) 
                == 2 * a1 + 1) && (a1 != ZERO)) 
    {
        Point_t p[6];//p1, p2, p3, p4, p5 et p6
        p[0].x = x1;
        p[1].x = x2;
        p[2].x = x3;
        p[3].x = x4;
        p[4].x = x5;
        p[5].x = x6;
        p[0].y = y1;
        p[1].y = y2;
        p[2].y = y3;
        p[3].y = y4;
        p[4].y = y5;
        p[5].y = y6;  

        comparer_absorbeur(p, a1); 
        absorbeur_new(p, a1); 

        if ((nb > nb_element_absorbeur) && (a1 > MAX_PT))
        {
            error_msg ("Absorbeur: MAX_RENDU1 < nb, ou MAX_PT < nb_point.");
        }
        c_absorbeur++;
    }	
}

// Création d'absorbeur et le stock dans le tableau
Absorbeur_t *absorbeur_new (Point_t *points, unsigned int nb_point)
{
    if (nb >= absorbeur_buffer_size)
    {
        absorbeur_buffer_size *= 2;
        tab_absorbeur = realloc (tab_absorbeur, 
                                 absorbeur_buffer_size * sizeof (Absorbeur_t));
    }
    int i;
    tab_absorbeur[nb].nb_point = nb_point;
    for(i = 0; i < nb_point; i++)
    {
        tab_absorbeur[nb].points[i] = points[i];
    }     
    return &tab_absorbeur[nb++];
}

int get_nbre_absorbeur()
{
    return nb;
}

void reset_absorbeur()
{
    nb_element_absorbeur = VAL_IMPOSSIBLE;
    c_absorbeur = ZERO; 
    nb = ZERO;
}

// écrire un absorbeur dans le fichier
static void absorbeur_vers_fichier (Absorbeur_t * abs, FILE * fp)
{
    fprintf (fp, "%i ", abs->nb_point);
    int i;
    for(i = 0; i < abs->nb_point; i ++)
    { 
        Point_t p = abs->points[i];
        fprintf(fp, "%f %f ", p.x, p.y);
    }
    fprintf(fp, "\n");
}

// écrire tous les absorbeurs dans le fichier
void absorbeurs_vers_fichier(FILE * fp)
{
    int i;

    fprintf(fp, "#absorbeur\n");
    fprintf(fp, "%i\n", nb);

    for (i = 0; i < nb; i ++)
    { 
        absorbeur_vers_fichier(&tab_absorbeur[i], fp);
    }
    fprintf(fp, "FIN_LISTE\n");
}

// Tester la distance entre les points d'un absorbeur
void comparer_absorbeur(Point_t * p, int a)
{
    int i, j;
    for (i = 0; i < a; i++)
    {
        for (j = 0; j < a; j++)
        {
            if ((i != j) && (distance_points(p[i], p[j]) < EPSIL_CREATION))
            {
                error_lecture_point_trop_proche(ERR_ABSORBEUR, nb);
            }
        }
    }
}

int comparer_absorbeur_reflecteur(void)
{
    int i, j, k;
    int nb_reflecteur, val_retour_i, val_retour_j;
    nb_reflecteur = get_nbre_reflecteur();

    for (i = 0; i < nb_reflecteur; i ++)
    {
        Bipoint_t b1 = bipoint_new(reflecteur_get_p1(i), reflecteur_get_p2(i));
        for (j = 0; j < nb_element_absorbeur; j ++)
        {
            for (k = 0; k < tab_absorbeur[j].nb_point - 1; k ++)
            {
                Bipoint_t b2 = bipoint_new(tab_absorbeur[j].points[k],
                                           tab_absorbeur[j].points[k + 1]);   
                val_retour_i = intersection(b1, b2);
                val_retour_j = para_sup(b1, b2);
               
                if ((val_retour_i == 1) && (val_retour_j == 3))
                {
                    error_lecture_intersection(ERR_REFLECTEUR, i,
                                               ERR_ABSORBEUR, j);
                    return 1;
                }
            }	
        }
    }
    return 0;
}          

int comparer_absorbeur_projecteur(void)
{
    int i, j, k;
    int nb_projecteur, val_retour_i, val_retour_j;
    nb_projecteur = get_nbre_projecteur();

    for(i = 0; i < nb_projecteur; i++)
    {
        Bipoint_t b1  = bipoint_new(get_p1_projecteur(i), get_p2_projecteur(i));
        for(j = 0; j < nb; j++)
        {
            for(k = 0; k < tab_absorbeur[j].nb_point - 1; k++)
            {
                Bipoint_t b2 = bipoint_new(tab_absorbeur[j].points[k],
                                         tab_absorbeur[j].points[k + 1]);   
                val_retour_i = intersection(b1, b2);
                val_retour_j = para_sup(b1, b2);

                if((val_retour_i == 1) && (val_retour_j == 3))
                {
                    error_lecture_intersection(ERR_REFLECTEUR, i,
                                               ERR_ABSORBEUR, i);
                    return 1;
                }
            }	
        }
    }
    return 0;
}

void draw_absorbeur(void)
{
    int k ,i;
    float a, b, c, d;

    for(i = 0; i < nb; i ++)
    {
        for(k = 0; k < tab_absorbeur[i].nb_point - 1; k ++)
        {
            Point_t p1 = tab_absorbeur[i].points[k];
            Point_t p2 = tab_absorbeur[i].points[k + 1];
            a = p1.x;
            b = p1.y;
            c = p2.x;
            d = p2.y;
            ligneBlue(a, b, c, d);
        }
    }
}

void crush_absorbeur(Point_t * points, int nb_point)
{
    int i;
    tab_absorbeur[nb-1].nb_point = nb_point;
    for(i = 0; i < nb_point; i++)
    {
        tab_absorbeur[nb-1].points[i] = points[i];
    }     
    // free(tab_absorbeur);
    // tab_absorbeur = tab_temporaire;
    // error_msg ("Absorbeur: MAX_RENDU1 < nb, ou MAX_PT < nb_point.");
}

int abs_pls_prche(Point_t psc)
{
    int i, j, k;
    if(nb != 0)
    {
        Bipoint_t b = bipoint_new(tab_absorbeur[0].points[0], psc);
        double n = bipoint_norme(b);
        for(i = 0; i < nb; i++)
        {
            for(j = 0; j < MAX_PT; j ++)
            {
                b = bipoint_new(tab_absorbeur[i].points[j], psc);
                if(bipoint_norme(b) < n)
                {
                    n = bipoint_norme(b);
                    k = i;
                }
            }
        }
        return k;
    }
}	

void draw_abs_sc(int abs_sel)
{
    int k ;
    float a, b, c, d;
    for(k = 0; k < tab_absorbeur[abs_sel].nb_point - 1; k ++)
    {
        Point_t p1 = tab_absorbeur[abs_sel].points[k];
        Point_t p2 = tab_absorbeur[abs_sel].points[k + 1];
        a = p1.x;
        b = p1.y;
        c = p2.x;
        d = p2.y;
        ligneYellow(a, b, c, d);
    }
}

void absorbeur_detruire(int id)
{
    tab_absorbeur[id] = tab_absorbeur[nb-1];
    nb --;
}

void absorbeur_detruire_dernier()
{
    absorbeur_detruire(nb-1);
}

void absorbeur_absorber(Absorbeur_t *absorbeur)
{
    int j;
    int i;
    for (j = 0; j < absorbeur->nb_point - 1; j++)
    {
        for (i = 0; i < get_nb_photon(); i ++)
        {
            Point_t p1 = photon_position(i);
            Angle_t a = photon_angle(i);
            Point_t p2 = point_new(cos(a) * DELTA_T * VPHOT + p1.x, sin(a) * DELTA_T * VPHOT + p1.y);
            Bipoint_t b1 = bipoint_new (p1, p2);
            Bipoint_t b2 = bipoint_new (absorbeur->points[j], absorbeur->points[j+1]);
            if (intersection(b1, b2) == TRUE)
            {
                photon_detruire (i);
                i--;
            }
        }
    }    
}

void absorbeurs_absorber()
{
    int i;
    for (i = 0; i < nb; i ++)
    {
        absorbeur_absorber(&tab_absorbeur[i]);
    }
}

