/*!
  \file reflecteur .c
  \brief gérer les reflecteurs 
  \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
  \date 22 mai 2016
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "reflecteur.h"
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#include "projecteur.h"
#include "graphic.h"
#include "photon.h"

static int reflecteur_buffer_size = DEFAULT_BUFFER_SIZE;
static int nbre_element_reflecteur = VAL_IMPOSSIBLE;
static int c_reflecteur = ZERO;
static int nb;

struct Reflecteur
{
    Point_t p1;
    Point_t p2;
};

static Reflecteur_t * tab_reflecteur = NULL;

// Analyse d'une ligne de reflecteur, incrémentation d'état
void lecture_reflecteur (char *p, int *etat)
{
    float a, b, c1, d, e, f;
    if (*p == 'F')
    {
        if (((nbre_element_reflecteur) < c_reflecteur) && 
                (nbre_element_reflecteur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_PAS_ASSEZ, ERR_PROJECTEUR);
            exit(EXIT_FAILURE);
        }

        if (((nbre_element_reflecteur) > c_reflecteur) && 
                (nbre_element_reflecteur != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_PAS_ASSEZ, ERR_REFLECTEUR);
            exit(EXIT_FAILURE);
        }

        if (nbre_element_reflecteur == VAL_IMPOSSIBLE)
        {
            error_lect_nb_elements(ERR_PAS_ASSEZ);
            exit(EXIT_FAILURE);
        }
        else
        {	
            *etat = *(etat) + 1;
        }
    }
    if (sscanf(p, "%f%f%f%f%f%f", &a, &b, &c1, &d, &e, &f) == NB_ELEMENT)
    {
        nbre_element_reflecteur = a;
        if (nbre_element_reflecteur >= reflecteur_buffer_size)
        {
            reflecteur_buffer_size = nbre_element_reflecteur;
        }

        if (tab_reflecteur == NULL)
        {
            tab_reflecteur = malloc(reflecteur_buffer_size * sizeof(Reflecteur_t));
        }

    }
    if ((sscanf(p, "%f%f%f%f", &a, &b, &c1, &d) == NB_COO_REF))
    {
        Point_t point_reflecteur1;
        point_reflecteur1.x = a;
        point_reflecteur1.y = b;

        Point_t point_reflecteur2;
        point_reflecteur2.x = c1;
        point_reflecteur2.y = d;
        reflecteur_new(point_reflecteur1, point_reflecteur2); 
        c_reflecteur = c_reflecteur + 1;		
    }
}

void reset_reflecteur()
{
    reflecteur_buffer_size = 0;
    nbre_element_reflecteur = VAL_IMPOSSIBLE;
    c_reflecteur = ZERO; 
    nb = ZERO;
}

void reflecteurs_vers_fichier(FILE * fp)
{
    int i;

    fprintf(fp, "#reflecteur\n");
    fprintf(fp, "%i\n", nbre_element_reflecteur);

    for(i = 0; i < nbre_element_reflecteur; i++)
    {  
        Point_t p1 = tab_reflecteur[i].p1;
        Point_t p2 = tab_reflecteur[i].p2;

        fprintf (fp, "%f %f %f %f\n", p1.x, p1.y, p2.x, p2.y);
    }
    fprintf(fp, "FIN_LISTE\n");
}

Reflecteur_t * reflecteur_new(Point_t p1, Point_t p2)
{
    if (nb >= reflecteur_buffer_size)
    {    
        reflecteur_buffer_size *= 2;
        tab_reflecteur = realloc (tab_reflecteur, 
                                    reflecteur_buffer_size * sizeof (Reflecteur_t));

    }
    if(distance_points(p1, p2) < EPSIL_CREATION)
    {
        error_lecture_point_trop_proche(ERR_REFLECTEUR, nb);
        exit(EXIT_FAILURE);
    }
    tab_reflecteur[nb].p1 = p1;
    tab_reflecteur[nb].p2 = p2;
    return &tab_reflecteur[nb++];
}

int reflecteur_comparer(void)
{
    int i, j;
    int val_retour_i, val_retour_j;

    for(i = 0; i < nb; i++)
    {
        for(j = 0; j < nb; j++)
        {
            if(i != j)
            {
                Bipoint_t b1 = bipoint_new(tab_reflecteur[i].p1,
                                           tab_reflecteur[i].p2);
                Bipoint_t b2 = bipoint_new(tab_reflecteur[j].p1,
                                           tab_reflecteur[j].p2);

                val_retour_i = intersection(b1,b2);
                val_retour_j = para_sup(b1,b2);
                if((val_retour_i == 1) && (val_retour_j == 3))
                {
                    error_lecture_intersection(ERR_REFLECTEUR, j, 
                                               ERR_REFLECTEUR, i);
                    return 1;
                    
                }
                if (val_retour_j == 2)
                {
                    error_lecture_intersection(ERR_REFLECTEUR, j, 
                                               ERR_REFLECTEUR, i);
                    return 1;
                }	 
            }
        }
    }
    return 0;
}

int get_nbre_reflecteur(void)
{
    return nb;	
}	

Point_t reflecteur_get_p1(int i)
{
    Point_t a = tab_reflecteur[i].p1;
    return a;
}

Point_t reflecteur_get_p2(int i)
{
    Point_t a = tab_reflecteur[i].p2;
    return a;
}

int comparer_reflecteur_projecteur(void)
{
    int i, j;
    int val_retour_i, val_retour_j;
    int nb_projecteur = get_nbre_projecteur();

    for (i = 0; i < nb; i ++)
    {
        for (j = 0; j < nb_projecteur; j ++)
        {
            Bipoint_t b1 = bipoint_new(tab_reflecteur[i].p1, tab_reflecteur[i].p2);
            Bipoint_t b2 = bipoint_new(get_p1_projecteur(j), get_p2_projecteur(j));
            val_retour_i = intersection(b1, b2);
            val_retour_j = para_sup(b1, b2);
            if((val_retour_i == 1) && (val_retour_j == 3))
            {
                error_lecture_intersection(ERR_REFLECTEUR, i, ERR_PROJECTEUR,j);
                return 1;
            }
            if (val_retour_j == 2)
            {
                error_lecture_intersection(ERR_REFLECTEUR, i, ERR_PROJECTEUR,j);
                return 1;
            } 
        }
    }
    return 0;
}

void draw_reflecteur(void)
{
    int i;
    for(i = 0; i < nb; i ++)
    {   
        Point_t p1 = tab_reflecteur[i].p1;
        Point_t p2 = tab_reflecteur[i].p2;
        ligneRed(p2.x, p2.y, p1.x, p1.y);
    }
}

int refl_pls_prche(Point_t Psc)
{
    int i;
    int j = 0;
    Bipoint_t b1, b2;
    b1 = bipoint_new(tab_reflecteur[0].p1, Psc);
    double n = bipoint_norme(b1);

    for (i = 0; i < nb; i ++)
    {
        b1 = bipoint_new(tab_reflecteur[i].p1, Psc);
        b2 = bipoint_new(tab_reflecteur[i].p2, Psc);
        if (bipoint_norme(b1) < n)
        {
            n = bipoint_norme(b1);
            j = i;
        }
        if (bipoint_norme(b2) < n)
        {
            n = bipoint_norme(b2);
            j = i;	
        }
    }
    return j;
}

void draw_refl_sc(int refl_sel)
{
    Point_t p1 = tab_reflecteur[refl_sel].p1;
    Point_t p2 = tab_reflecteur[refl_sel].p2;
    ligneYellow(p2.x, p2.y, p1.x, p1.y);
}

void destruc_refl(int i)
{
    tab_reflecteur[i] = tab_reflecteur[nb-1];
    nb --;
    /*
    if(nbre_element_reflecteur != 0)
    {
        Reflecteur_t * tab_temporaire = malloc((nbre_element_reflecteur - 1)
                                      * sizeof(Reflecteur_t));
        int j;
        for(j = 0; j < nbre_element_reflecteur - 1; j ++)
        {
            if(j < i)
            {
                tab_temporaire[j] = tab_reflecteur[j];
            }
            if(j >= i)
            {
                tab_temporaire[j] = tab_reflecteur[j + 1];  
            }
        }
        nb --;   
        free(tab_reflecteur);
        tab_reflecteur = NULL;
        tab_reflecteur = tab_temporaire;
    }
    */
}

void destruc_last_refl()
{
    destruc_refl(nb-1);
}

