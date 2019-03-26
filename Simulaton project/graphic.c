/*!
 \file graphic.c
 \brief gérer le graphique
 \author Mohamed Ramadane, My-Hanh Dang, Ziyu Zhu
 \date 22 mai 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include "constantes.h"
#include "utilitaire.h"

#define PI 3.1415926535898
#define r EPSIL_PROJ/2  //définer rayon du circle

int i;

void ligneBlue(float x, float y, float d, float a)
{
    glBegin(GL_LINES);
    glColor3f(0, 0, 1); //blue
    glVertex2d(x, y); //from Point1(x,y) to Point2(x,y)       
    glVertex2d(d, a); 
    glEnd();
}

void ligneRed(float x, float y, float d, float a)
{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); //red
    glVertex2f(x, y); //from Point1(x,y) to Point2(x,y)       
    glVertex2f(d, a); 
    glEnd();
}

void ligneGreen(float x, float y, float d, float a)
{
    glBegin(GL_LINES);
    glColor3f(0, 1, 0); //green
    glVertex2d(x, y); //from Point1(x,y) to Point2(x,y)       
    glVertex2d(d, a); 
    glEnd();
     
}
void ligneYellow(float x ,float y,float d, float a)
{
	glBegin(GL_LINES);
    glColor3f(1., 1., 0); //green
    glVertex2d(x, y); //from Point1(x,y) to Point2(x,y)       
    glVertex2d(d, a); 
    glEnd();
}

void circle (float x, float y, float a)
{      
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0); //noir
    for (i = 0; i < 200; i++)
    {
        glVertex2d(x + r * cos(a), y + r * sin(a)); //r fixé
        a += (PI / 100);  
    }
    glEnd(); 
} 
void draw_rectangle(Point_t P1,Point_t P2,Point_t P3,Point_t P4)
{
  if((P1.x!=P2.x)&&(P1.y!=P2.y))	
	{
		glBegin(GL_LINE_LOOP);
       glColor3f(0, 0, 0);
       glVertex2d(P1.x,P1.y);
       glVertex2d(P3.x,P3.y);
       glVertex2d(P2.x,P2.y);
       glVertex2d(P4.x,P4.y);
       glEnd();
   }
}

