/*!
 \file photon.c
 \brief gérer les photon 
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "photon.h"
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#include "graphic.h"
#include "reflecteur.h"
static int photon_buffer_size = DEFAULT_BUFFER_SIZE;
static int nbre_element_photon = VAL_IMPOSSIBLE;
static int c_photon = ZERO; 
static int nb = ZERO;

struct Photon
{
    Point_t position;
    double angle;
    int reflecteur_id;
    //struct Photon * next;
    //struct Photon * prev;
};

static Photon_t * tab_photon = NULL;

// Analyse d'une ligne de photon, incrémentation d'état
void lecture_photon(char *p, int *etat)
{
    float a, b, c1, d, e, f;
    if (*p == 'F')
    {
        if (((nbre_element_photon) < c_photon) && 
                (nbre_element_photon != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_PHOTON, ERR_TROP);
            exit(EXIT_FAILURE);
        }
        if ((nbre_element_photon) > c_photon && 
                (nbre_element_photon != VAL_IMPOSSIBLE))
        {
            error_lecture_elements(ERR_PHOTON, ERR_PAS_ASSEZ);
            exit(EXIT_FAILURE);
        }
        if (nbre_element_photon == VAL_IMPOSSIBLE)
        {
            error_lect_nb_elements(ERR_PHOTON);
            exit(EXIT_FAILURE);
        }
        else
        {		
            *etat = *(etat) + 1;
        }
    }
    if (sscanf(p, "%f%f", &a, &b) == NB_ELEMENT)
    {
        nbre_element_photon = a;
        if (photon_buffer_size <= nbre_element_photon)
            photon_buffer_size = nbre_element_photon; 

        if(tab_photon == NULL) 
            tab_photon = malloc(photon_buffer_size * sizeof(Photon_t));
    }
    if ((sscanf(p, "%f%f%f", &a, &b, &c1) == NB_COOR_PHTON))
    {
        Point_t point_photon;
        point_photon.x = a;
        point_photon.y = b;
        if (nb >= nbre_element_photon)
        {
            error_msg("Le nombre de photons est dépassé NBPH.");
            exit(EXIT_FAILURE);
        }
        photon_new (point_photon, c1);
        c_photon = c_photon + 1;		
    }
}

void reset_photon ()
{
    nbre_element_photon = VAL_IMPOSSIBLE;
    c_photon = ZERO; 
    nb = ZERO;
}

void photons_vers_fichier(FILE * fp)
{
    int i;

    fprintf (fp, "#photon\n");
    fprintf (fp, "%i\n", nb);

    for(i = 0; i < nb; i++)
    {  
        Point_t p = tab_photon[i].position;
        float a = tab_photon[i].angle;

        fprintf (fp, "%f %f %f\n", p.x, p.y, a);
    }
    fprintf (fp, "FIN_LISTE\n");
}

// Création d'absorbeur et le stock dans le tableau
Photon_t *photon_new(Point_t position, Angle_t angle)
{
    if (nb >= photon_buffer_size)
    {
        photon_buffer_size *= 2;
        tab_photon = realloc(tab_photon, photon_buffer_size * sizeof (Photon_t));
    }
    tab_photon[nb].position = position;
    tab_photon[nb].angle = angle;
    tab_photon[nb].reflecteur_id = -1;
    return &tab_photon[nb++];
}

void draw_photon(void)
{
    int i;
    for(i = 0; i < nb; i++)
    {
        circle(tab_photon[i].position.x,
               tab_photon[i].position.y, 
               tab_photon[i].angle);
    }
}

// l'ordre n'est pas important
// on remplace le photon par le dernier et
// décrémente le nombre de photon
void photon_detruire (int id)
{
    tab_photon[id] = tab_photon[nb - 1];
    nb --;
}

Point_t photon_position(int id)
{
    return tab_photon[id].position;
}

Angle_t photon_angle(int id)
{
    return tab_photon[id].angle;
}

Photon_t * photon_get (int id)
{
    return &tab_photon[id];
}

unsigned int get_nb_photon ()
{
    return nb;
}

static int photon_reflect(Photon_t *photon, int reflecteur_id, float *trajet)
{ 
    if (*trajet <= EPSIL_CONTACT) { return 0; }
    Point_t p;
    Angle_t a = photon->angle;
    p.x = cos(a) * (*trajet) + photon->position.x;
    p.y = sin(a) * (*trajet) + photon->position.y;
    Bipoint_t b_photon = bipoint_new (photon->position, p);
    Bipoint_t reflecteur = bipoint_new (reflecteur_get_p1(reflecteur_id),
                                        reflecteur_get_p2(reflecteur_id));

    if (intersection(b_photon, reflecteur) && photon->reflecteur_id != reflecteur_id)
    {
        Point_t pi = bipoint_intersection(b_photon, reflecteur);
        (*trajet) -= distance_points(pi, photon->position);

        Vect2_t n = bipoint_normal(reflecteur);
        Vect2_t dir = vecteur_new(cos(a), sin(a));
        Vect2_t rv;

        if (produit_scalaire(n, dir) > 0.0)
            rv = reflection(dir, n);
        else
            rv = reflection(dir, vecteur_new(-n.x, -n.y));

        photon->angle = atan2(rv.y, rv.x);
        photon->position = pi;
        photon->reflecteur_id = reflecteur_id;
        return 1;
    }
    return 0;
}

//Gere multiple reflections
static void photon_update_helper(Photon_t *photon, float trajet)
{
    int i;
    for (i = 0; i < get_nbre_reflecteur(); i++)
    {
        if (photon_reflect(photon, i, &trajet))
        {
            photon_update_helper(photon, trajet);
            return;
        }
    }    
    Point_t p;
    p.x = cos(photon->angle) * trajet;
    p.y = sin(photon->angle) * trajet;
    photon->position = vecteur_add(photon->position, p);
    return;
}

//Gere une seule reflection (Deprecated)
int reflect(Photon_t * p)
{
    int i;
    for (i = 0; i < get_nbre_reflecteur(); i++)
    {
        Bipoint_t refl = bipoint_new(reflecteur_get_p1(i), reflecteur_get_p2(i));
        Angle_t a = p->angle;
        Vect2_t dir = vecteur_new(cos(a), sin(a));
        Point_t dst = vecteur_add(p->position, vecteur_mult(dir, VPHOT * DELTA_T));
        Bipoint_t bipoint_photon = bipoint_new(p->position, dst);
        
        if (intersection(refl, bipoint_photon) && p->reflecteur_id != i)
        {
            Point_t pi = bipoint_intersection(refl, bipoint_photon);
            double trajet = distance_points(pi, p->position);
            
            Vect2_t n = bipoint_normal(refl);
            Vect2_t rv;

            if (produit_scalaire(n, dir) > 0.0)
                rv = reflection(dir, n);
            else
                rv = reflection(dir, vecteur_new(-n.x, -n.y));

            p->position = vecteur_add(pi, vecteur_mult(rv, VPHOT * DELTA_T - trajet));
            p->angle = atan2(rv.y, rv.x);
            p->reflecteur_id = i;
            return TRUE;
        }
    }
    return FALSE;
}

void photons_update ()
{
    int i;
    for (i = 0; i < nb; i++)
    {
        photon_update_helper(&tab_photon[i], VPHOT * DELTA_T);
    }   
}
/*  
void photons_update ()
{
    int i;
    for (i = 0; i < nb; i++)
    {
        Point_t p = tab_photon[i].position;
        Angle_t a = tab_photon[i].angle;
        p.x += cos(a) * VPHOT * DELTA_T;
        p.y += sin(a) * VPHOT * DELTA_T;
        tab_photon[i].position = p;
    }   
}
*/


// void photon_retirer (Photon_t **photon_head)
// Analyse d'une ligne de photon, incrémentation d'état
// double linked list
/*
static Photon_t * photon_head = NULL;

Photon_t * photon_add (Photon_t * new)
{
    // new->next ~ (*new).next;
    // NULL->new->nead, head->new
    new->next = photon_head;
    new->prev = NULL;
    if (photon_head != NULL)
    {
        photon_head->prev = new;
    }
    photon_head = new;
    return new;
}

Photon_t * photon_del (Photon_t * photon)
{
    if (photon->next != NULL && photon->prev != NULL)
    {
        photon->prev->next = photon->next;
        photon->next->prev = photon->prev;
    }
    else if (photon->next != NULL)
    {
        photon->next->prev = NULL;
        photon_head = photon->next;
    }
    else if (photon->prev != NULL)
    {
        photon->prev->next = NULL;
    }
    photon->next = NULL;
    photon->prev = NULL;
    return photon;
}
*/

void detruct_photon_hors_cadre(double gauche,double droite,double haut,double bas)
{
    int i;
    for (i=0;i<nb;i++)
    {
        if((tab_photon[i].position.x>droite)||(tab_photon[i].position.x<gauche))
        {
            photon_detruire(i);
            i--;
        }
        else if((tab_photon[i].position.y>haut)||(tab_photon[i].position.y<bas))
        {
            photon_detruire(i);	 
            i--;
        }
    }	 
}	

