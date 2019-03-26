/*!
  \file main.cpp
  \brief module de l'interface
  \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
  \date 22 mai 2016
  */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <string.h>

extern "C"
{
#include "graphic.h"
#include "modele.h"
#include "constantes.h"
#include "projecteur.h"
#include "reflecteur.h"
#include "photon.h"
#include "absorbeur.h"
#include "utilitaire.h"
#include <math.h>
}

#define ONE_CLICK   0
#define TWO_CLICK   1
#define ZOOMED      2
#define M_PI           3.14159265358979323846

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)


namespace
{
    char text0[TEXT_MAX_CHAR];
    char text1[TEXT_MAX_CHAR];
    char text_nb_photon[TEXT_MAX_CHAR];
    char text_nb_projecteur[TEXT_MAX_CHAR];
    char text_nb_absorbeur[TEXT_MAX_CHAR];
    char text_nb_reflecteur[TEXT_MAX_CHAR];

    /*pointeurs pour creer les objets de GLUI*/
    float zoom = 1;

    /*pointeurs pour creer les objets de GLUI*/
    GLfloat aspect_ratio ; 
    int main_window;
    int radio_live_var;
    float he;
    float wi;
    int z = 0;
    //Rectangle_t rectangle;
    Point_t P1, P2, P3, P4, Pf, Pi, Pai, Paf, Psc;
    int proj_spi = 0;
    int refl_spi = 0;
    int abs_spi = 0;
    Point_t * tab_point = (Point_t *)malloc(6 * sizeof(Point_t));

    GLUI_EditText *edittext[6];
    GLUI_RadioGroup *radiogroup[2];
    GLUI_RadioButton *radio[6];
    GLUI_Button *button[5];
    GLUI_Panel *check_panel[6];
}

/************************ control_cb() *******************************/
void init_ui();
void affichage(void);
void reshape(int w, int h);
void MouseZoom(int button, int state, int x, int y); //scroll
void motion_souris(int x, int y);
void keyboard(unsigned char key,int x, int y);

static int compteur_abs = 1;
static GLfloat gauche, droite;
static GLfloat bas, haut;
static int stateG = 1;
static int radio_button_state;
static int radio_button_sc;
static int state_rect = 0;
static int etat_abs = 1;
static int refl_sel,proj_sel,abs_sel;
static int etat_sc_ref = 0;
static int etat_sc_proj = 0;
static int etat_sc_abs = 0;
static int etat_start=-1;

/************************ simulation callback *************************/

void step ()
{
    //Ici vient la simulation
    //printf ("step de simulation\n");
	glutSetWindow(main_window);
    projecteurs_projettent();
    absorbeurs_absorber();
    photons_update();

    snprintf(text_nb_photon, TEXT_MAX_CHAR, "%d", get_nb_photon()); 
    edittext[3]->set_text(text_nb_photon);

    glutPostRedisplay();
}

//Called every time step
void simulation (int value)
{
	glutSetWindow(main_window);
	if(etat_start==1)
	{
        button[2]->name=("Stop");
        step();
        glutTimerFunc(DELTA_T * 100, simulation, 0);
    }
    if(etat_start==-1)
    {
		button[2]->name=("Start");
		glutTimerFunc(DELTA_T * 100, simulation, 0);
	}
}

void init (int argc, char* argv[])
{
    /*Initialisation Open GL */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(SCREEN_POSX, SCREEN_POSY);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    main_window = glutCreateWindow("Fiat Lux");

    glClearColor(1.0, 1.0, 1.0, 0.0); 
    glLineWidth(3);
    //fonction callback d'affichage
    glutDisplayFunc(affichage); 
    //fonction callback reshape 
    glutReshapeFunc(reshape); 
    glutMouseFunc(MouseZoom);
    glutMotionFunc(motion_souris);
    glutKeyboardFunc(keyboard);

    gauche = -DMAX;
    bas = -DMAX;
    droite = DMAX;
    haut = DMAX;

    snprintf(text_nb_photon, TEXT_MAX_CHAR, "%d", get_nb_photon()); 
    snprintf(text_nb_projecteur, TEXT_MAX_CHAR, "%d", get_nbre_projecteur()); 
    snprintf(text_nb_absorbeur, TEXT_MAX_CHAR, "%d", get_nbre_absorbeur()); 
    snprintf(text_nb_reflecteur, TEXT_MAX_CHAR, "%d", get_nbre_reflecteur()); 

    if(argc == 1)
        strcpy(text0, "test.txt");
    else
        strncpy(text0, argv[2], TEXT_MAX_CHAR);
    
    strcpy(text1, "save.txt");
    
    init_ui();
}

/*********************** main() ********************/
int main (int argc, char *argv[])
{
    if (argc == 1)
    {
        init (argc, argv);
        affichage(); 
        glutMainLoop();
        return EXIT_SUCCESS;       
    }
    if (argc == 3)
    {
        if (strcmp("Error",argv[1]) == 0)
        {
            modele_lecture(argv[2]);
            return EXIT_SUCCESS;
        } 
        else if (strcmp("Verification", argv[1]) == 0)
        { 
            modele_lecture(argv[2]);
            modele_verification_rendu2();
            return EXIT_SUCCESS;
        }

        else if (strcmp("Graphic", argv[1]) == 0)
        {
            modele_lecture(argv[2]);
            init (argc, argv);
            affichage(); 
            glutMainLoop();
            return EXIT_SUCCESS;  
        }
        else if (strcmp("Final", argv[1]) == 0)
        {
            modele_lecture(argv[2]);
            init (argc, argv);
            affichage(); 
            glutMainLoop();
            return EXIT_SUCCESS;
        }
        else 
            return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

void selection_process(int button,int x,int y,int *c)
{
    radio_button_state=radiogroup[1]->get_int_val();
    radio_button_sc=radiogroup[0]->get_int_val();
    if(stateG==GLUT_DOWN)
    {
        if((radio_button_state==1)&&(radio_button_sc==0))
        {
            Psc=pixel_to_coordonne(x,y,wi,he,gauche,droite,haut,bas); 
            refl_sel=refl_pls_prche(Psc);
            printf("%d\n",refl_sel);
            etat_sc_ref=1;
            etat_sc_abs=0;
            etat_sc_proj=0;
            glutPostRedisplay();
        }
        if((radio_button_state==0)&&(radio_button_sc==0))
        {
            int i=get_nbre_projecteur();
            if(i!=0)
            {
                Psc=pixel_to_coordonne(x,y,wi,he,gauche,droite,haut,bas);
                proj_sel=proj_pls_prche(Psc);
                etat_sc_ref=0;
                etat_sc_abs=0;
                etat_sc_proj=1;
                glutPostRedisplay();
            }
        }
        if((radio_button_state==2)&&(radio_button_sc==0))
        {
            int i=get_nbre_absorbeur();
            if(i!=0)
            {
                Psc=pixel_to_coordonne(x,y,wi,he,gauche,droite,haut,bas);
                abs_sel=abs_pls_prche(Psc);
                etat_sc_ref=0;
                etat_sc_abs=1;
                etat_sc_proj=0;
                glutPostRedisplay();
            }
        }
    }
    snprintf(text_nb_projecteur, TEXT_MAX_CHAR, "%d", get_nbre_projecteur()); 
    edittext[4]->set_text(text_nb_projecteur);
    snprintf(text_nb_absorbeur, TEXT_MAX_CHAR, "%d", get_nbre_absorbeur()); 
    edittext[5]->set_text(text_nb_absorbeur);
    snprintf(text_nb_reflecteur, TEXT_MAX_CHAR, "%d", get_nbre_reflecteur()); 
    edittext[6]->set_text(text_nb_reflecteur);
}

void creation_process(int button, int x, int y, int * c)
{
    radio_button_state = radiogroup[1]->get_int_val();
    radio_button_sc = radiogroup[0]->get_int_val();
    if((stateG == GLUT_DOWN) && (*c == 0) && (radio_button_sc == 1))
    {
        etat_sc_abs = 0;
        etat_sc_proj = 0;
        etat_sc_ref = 0;
        Pi.x = 0;
        Pi.y = 0;
        Pai.x = 0;
        Pai.y = 0;
        *c = *c + 1;

        if((radio_button_state == 0) || (radio_button_state == 1))
        {
            Pi = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
        }
        if(radio_button_state == 2)    
        {
            Pai = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
            ini_point(tab_point[0], tab_point[1], tab_point[2],
                      tab_point[3], tab_point[4], tab_point[5]);
            compteur_abs = 1;
            tab_point[0] = Pai;
        }
    }
    if((stateG == GLUT_UP) && (*c == 1))
    {
        *c = *c + 1;
    }	
    if((stateG == GLUT_DOWN) && (*c == 2) && (radio_button_state == 1) 
                                          && (radio_button_sc == 1))
    { 
        if(etat_abs == 1)
        { 
            ini_point(tab_point[0], tab_point[1], tab_point[2],
                      tab_point[3], tab_point[4], tab_point[5]);
            Pf = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas);
            //increment_nb_reflecteur();
            reflecteur_new(Pi, Pf);
            if (modele_verification_rendu2())
                destruc_last_refl();
            
            glutPostRedisplay();
            *c=0;
        }
        if(etat_abs == 2)
        {
        *c=2;
        Pi = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
        etat_abs = 1;
	    }
    }
    if((stateG == GLUT_DOWN) && (*c == 2) && (radio_button_state == 0) 
                                          && (radio_button_sc == 1))
    {
        if(etat_abs == 1)
        {
            ini_point(tab_point[0], tab_point[1], tab_point[2],
                      tab_point[3], tab_point[4], tab_point[5]);
            Point_t Ppi, Ppf;
            Ppi.x = 0;
            Ppi.y = 0;
            Ppf.x = 1;
            Ppf.y = 0;
            Pf = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas);
         
            Bipoint_t v1 = bipoint_new(Pf, Pi);
            Bipoint_t v2 = bipoint_new(Ppi, Ppf);
            double angle=atan2(Pf.y-Pi.y,Pf.x-Pi.x);
            projecteur_new(angle, Pi);
            *c=0;
            if (modele_verification_rendu2())
                destruc_last_proj();
            
            glutPostRedisplay();
        }
        if(etat_abs == 2)
        {
        *c = 2;
        Pi = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
        etat_abs = 1;
	}
    } 
    if((stateG == GLUT_DOWN) && (*c == 2) && (radio_button_state == 2) 
                                          && (radio_button_sc == 1))
    {
        if(etat_abs == 1)
        {
            Paf = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas);
            tab_point[compteur_abs] = Paf;
            compteur_abs ++;
            //increment_nb_absorbeur();
            absorbeur_new(tab_point, compteur_abs);

            if (modele_verification_rendu2())
                absorbeur_detruire_dernier();

            glutPostRedisplay();
        }
        if(etat_abs == 2)
        {
            if((compteur_abs < 6))
            {
				printf("compteur abs : %d",compteur_abs);
                Paf = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas);
                tab_point[compteur_abs] = Paf;
                compteur_abs ++;
                crush_absorbeur(tab_point, compteur_abs);

                if (modele_verification_rendu2())
                    absorbeur_detruire_dernier();

                glutPostRedisplay();
            }
            else
            {
                printf("trop de point \n");
                ini_point(tab_point[0], tab_point[1], tab_point[2],
                          tab_point[3], tab_point[4], tab_point[5]);
            } 
        }
        if(etat_abs==1)
        {
			etat_abs++;
		}
    }

    snprintf(text_nb_projecteur, TEXT_MAX_CHAR, "%d", get_nbre_projecteur()); 
    edittext[4]->set_text(text_nb_projecteur);
    snprintf(text_nb_absorbeur, TEXT_MAX_CHAR, "%d", get_nbre_absorbeur()); 
    edittext[5]->set_text(text_nb_absorbeur);
    snprintf(text_nb_reflecteur, TEXT_MAX_CHAR, "%d", get_nbre_reflecteur()); 
    edittext[6]->set_text(text_nb_reflecteur);

    glutPostRedisplay();
}

void zoom_process(int button, int x, int y, int * c)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if((*c == 0) && (stateG == GLUT_DOWN))
        {
            P1 = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
            *c = *c + 1;
            state_rect = 1;
        } 
        if(*c == 1 && stateG == GLUT_UP)
        {
            P2 = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
            P3.x = P1.x;
            P3.y = P2.y;
            P4.x = P2.x;
            P4.y = P1.y;
            //changer gauche droit haut bas
            if(P1.x < P2.x)
            {
                if(P1.y < P2.y)
                {
                    if(fabs(P1.x - P2.x) < fabs(P1.y - P2.y))
                    {
                        bas = P1.y;
                        haut = P2.y;
                        gauche = ((P1.x + P2.x) / 2) - fabs(P1.y - P2.y) / 2;
                        droite = ((P1.x + P2.x) / 2) + fabs(P1.y - P2.y) / 2;
                    }
                    else 
                    {
                        gauche = P1.x;
                        droite = P2.x;
                        haut = ((P1.y + P2.y) / 2) + fabs(P1.x - P2.x) / 2;
                        bas = ((P1.y + P2.y) / 2) - fabs(P1.x - P2.x) / 2;
                    }
                }
                if(P1.y > P2.y)
                {
                    if(fabs(P1.x - P2.x) < fabs(P1.y - P2.y))
                    {
                        bas = P2.y;
                        haut = P1.y;
                        gauche = ((P1.x + P2.x) / 2) - fabs(P1.y - P2.y) / 2;
                        droite = ((P1.x + P2.x) / 2) + fabs(P1.y - P2.y) / 2;
                    }
                    else 
                    {
                        gauche = P1.x;
                        droite = P2.x;
                        haut = ((P1.y + P2.y) / 2) + fabs(P1.x - P2.x) / 2;
                        bas = ((P1.y + P2.y) / 2) - fabs(P1.x - P2.x) / 2;
                    }
                }
            }
            if (P1.x > P2.x)
            {
                if(P1.y < P2.y)
                {
                    if(fabs(P1.x - P2.x) < fabs(P1.y - P2.y))
                    {
                        bas = P1.y;
                        haut = P2.y;
                        gauche = ((P1.x + P2.x) / 2) - fabs(P1.y - P2.y) / 2;
                        droite = ((P1.x + P2.x) / 2) + fabs(P1.y - P2.y) / 2;
                    }
                    else 
                    {
                        gauche = P1.x;
                        droite = P2.x;
                        haut = ((P1.y + P2.y) / 2) + fabs(P1.x - P2.x) / 2;
                        bas = ((P1.y + P2.y) / 2) - fabs(P1.x - P2.x) / 2;
                    }
                }
                if(P1.y > P2.y)
                {
                    if(fabs(P1.x - P2.x) < fabs(P1.y - P2.y))
                    {
                        bas=P2.y;
                        haut=P1.y;
                        gauche=((P1.x+P2.x)/2)-fabs(P1.y-P2.y)/2;
                        droite=((P1.x+P2.x)/2)+fabs(P1.y-P2.y)/2;
                    }
                    else 
                    {
                        gauche=P1.x;
                        droite=P2.x;
                        haut=((P1.y+P2.y)/2)+fabs(P1.x-P2.x)/2;
                        bas=((P1.y+P2.y)/2)-fabs(P1.x-P2.x)/2;
                    }
                } 
            }
            state_rect = 0;
            glutPostRedisplay();
            *c = 0;
        }
    }	
}

//static char zoom_state;
void MouseZoom (int button, int state, int x, int y) //scroll
{
    static int c = 0;
    stateG = state;
    if(button == GLUT_LEFT_BUTTON)
    {
        zoom_process(button, x, y, &c);
    }
    if(button == GLUT_RIGHT_BUTTON)
    {
        creation_process(button, x, y, &c);
        selection_process(button, x, y, &c);
    }
}

/* GLUI control callback */
/********** User IDs pour les fonctions callback ********/
void affichage(void)
{
    // variables qui définissent le domaine visualisé avec Ortho
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    // ré-initialisation de la matrice de transformation 
    glLoadIdentity();

    // ajustement du domaine visualisé selon la forme de la fenêtre
    // pour garantir d'avoir toujours au moins [-1, 1] selon X et Y
    // les derniers paramètres (profondeur) restent constants pour du 2D
    if (aspect_ratio <= 1.)
        glOrtho(gauche, droite, bas/aspect_ratio,
                haut/aspect_ratio, -1.0, 1.0);
    else 
        glOrtho(gauche*aspect_ratio,
                droite*aspect_ratio,
                bas, haut, -1.0, 1.0);

    printf ("redisplay\n");

    draw_projecteur();
    draw_reflecteur();
    draw_photon();
    draw_absorbeur();
    if(state_rect==1)
    {
        draw_rectangle(P1,P2,P3,P4);
    }
    if(etat_sc_ref==1)
    {
        draw_refl_sc( refl_sel);
    }
    if(etat_sc_proj==1)
    {
        draw_proj_sc(proj_sel);	
    }
    if(etat_sc_abs==1)
    {
        draw_abs_sc(abs_sel);
    }
    glutSwapBuffers(); //exchange back and front buffers
}  

void reshape(int w, int h)
{
    //le viewport prend toute la fenêtre
    glViewport(0, 0, w, h); 
    he=h;
    wi=w;
    // mise à jour pour usage futur par affichage()
    aspect_ratio = (GLfloat)w / (GLfloat)h ; 
} 

void load_cb (int control)
{
    reset_absorbeur ();
    reset_photon ();
    reset_reflecteur ();
    reset_projecteur ();
    reset_modele_etat ();

    modele_lecture((char*)edittext[0]->get_text ());

    glutPostRedisplay();
}

void save_cb (int control)
{
    FILE * fp = fopen (edittext[1]->get_text(), "w");
    projecteurs_vers_fichier(fp);
    reflecteurs_vers_fichier(fp);
    absorbeurs_vers_fichier(fp);
    photons_vers_fichier(fp);
    fclose(fp);
}

void motion_souris(int x, int y)
{
    if (stateG == 0)
    {
        P2 = pixel_to_coordonne(x, y, wi, he, gauche, droite, haut, bas); 
        P3.x = P1.x;
        P3.y = P2.y;
        P4.x = P2.x;
        P4.y = P1.y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key,int x, int y)
{
    if(key=='r')
    {
        gauche = -DMAX;
        bas = -DMAX;
        droite = DMAX;
        haut = DMAX;
        P1 = pixel_to_coordonne(0, 0, wi, he, gauche, droite, haut, bas); 
        P2 = pixel_to_coordonne(0, 0, wi, he, gauche, droite, haut, bas); 
        glutPostRedisplay();
    }
    if(key == 'd')
    {
        if(etat_sc_ref == 1)
        {
            printf("%d\n", refl_sel);
            destruc_refl(refl_sel);
            etat_sc_ref = 0;
        }
        if(etat_sc_abs == 1)
        {
            printf("%d\n", abs_sel);
            absorbeur_detruire(abs_sel);
            etat_sc_abs = 0;
        }
        if(etat_sc_proj == 1)
        {
            printf("%d\n", proj_sel);
            destruc_proj(proj_sel);
            etat_sc_proj = 0;
        }
        glutPostRedisplay();  
    }
    if(key=='k')
    {
        detruct_photon_hors_cadre(gauche,droite,haut,bas);	
        snprintf(text_nb_photon, TEXT_MAX_CHAR, "%d", get_nb_photon()); 
        edittext[3]->set_text(text_nb_photon);
        glutPostRedisplay();
    }
}

/********************************************************************
  imprimer la valeur du parametre pour indiquer qui a produit l'appel.
  utiliser les fonctions de type get_int_val() ou get_float_val()
  plutôt que de creer des live variables.
 ********************************************************************/
void control_cb(int control)
{
    printf("callback: %d\n", control );
    switch(control)
    {
        case 1:
            exit (1);
            break;
        case 2:
            etat_start=-etat_start;
            simulation(0);
            break;
        case 3:
            step();
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

/****************************************/
/* widgets GLUI */
/****************************************/
void init_ui ()
{
    GLUI *glui = GLUI_Master.create_glui((char*) "Fiat Lux"); 

    //File
    check_panel[0] = glui->add_panel((char*)"File");
    edittext[0] = glui->add_edittext_to_panel(check_panel[0], (char*)"FileName",
            GLUI_EDITTEXT_TEXT, text0, 0, control_cb);
    button[0] = glui->add_button_to_panel(check_panel[0], (char*)"Load", 0, load_cb);

    edittext[1] = glui->add_edittext_to_panel(check_panel[0], (char*)"FileName",
            GLUI_EDITTEXT_TEXT, text1, 0, control_cb);
    button[1] = glui->add_button_to_panel(check_panel[0], (char*)"Save", 0, save_cb);

    //Simulation
    check_panel[1] = glui->add_panel((char*)"Simulation");
    button[2] = glui->add_button_to_panel(check_panel[1], (char*)"Start !", 2,
            control_cb);
    button[3] = glui->add_button_to_panel(check_panel[1], (char*)"Step", 3,
            control_cb);

    //Quit
    button[4] = glui->add_button((char*)"Exit", 1, control_cb);

    //colonne
    glui-> add_column(true);

    //Information
    check_panel[2] = glui->add_panel((char*)"Information");
    edittext[3] = glui->add_edittext_to_panel(check_panel[2], (char*)"Nb Photons",
            GLUI_EDITTEXT_TEXT, text_nb_photon, 0, control_cb);
    edittext[4] = glui->add_edittext_to_panel(check_panel[2], (char*)"Nb Projecteurs",
            GLUI_EDITTEXT_TEXT, text_nb_projecteur, 0, control_cb);
    edittext[5] = glui->add_edittext_to_panel(check_panel[2], (char*)"Nb Absorbeurs",
            GLUI_EDITTEXT_TEXT, text_nb_absorbeur, 0, control_cb);
    edittext[6] = glui->add_edittext_to_panel(check_panel[2], (char*)"Nb Reflecteurs",
            GLUI_EDITTEXT_TEXT, text_nb_reflecteur, 0, control_cb);

    //Mouse Panel
    check_panel[3] = glui->add_panel((char*)"Mouse panel");
    check_panel[4] = glui->add_panel_to_panel(check_panel[3], "Action panel",
            GLUI_PANEL_EMBOSSED); 
    radiogroup[0] = glui->add_radiogroup_to_panel(check_panel[4]);
    radio[0] = glui->add_radiobutton_to_group(radiogroup[0], "Selection");
    radio[1] = glui->add_radiobutton_to_group(radiogroup[0], "Creation");

    //glui-> add_radiogroup_to_panel(check_panel[4]);
    check_panel[5] = glui->add_panel_to_panel(check_panel[3], "Entity selection",
            GLUI_PANEL_EMBOSSED); 
    radiogroup[1] = glui->add_radiogroup_to_panel(check_panel[5], NULL);
    radio[2] = glui->add_radiobutton_to_group(radiogroup[1], "Projecteur");
    radio[3] = glui->add_radiobutton_to_group(radiogroup[1], "Reflecteur");
    radio[4] = glui->add_radiobutton_to_group(radiogroup[1], "Absorbeur");

    //Start Glut Main Loop
    glui->set_main_gfx_window(main_window);
}

