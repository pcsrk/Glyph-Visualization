/*
 *  GVelemetBox.cpp
 *  isoProjTest
 *
 *  Created by mue on 07.06.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVelemetBox.h"


GVelemetBox::GVelemetBox  (QGLContext *cx) : GVbasicElement ()    
{
    
}

void GVelemetBox::generateOrUpdateBox (int w, int h, int z, float transparency) 
{
    
    this->w = w; 
    this->h = h; 
    
//    GLcontext->makeCurrent();
    
    float xmax = (float)w;
    float ymax = (float)h;
    float zmax = (float)z;
    
    float black[4]={0,0,0,0};
    
    if (object3dDl) glDeleteLists(object3dDl,1);
    object3dDl = glGenLists(1);
    glNewList(object3dDl, GL_COMPILE);	
    
    glEnable(GL_BLEND);		// Turn Blending On    
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
     
    GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );   
 
    
    glColor4f(0.6, 0.0, 0.0, transparency);
    // der Boden
    glBegin(GL_QUADS);
    glNormal3i (0, 0, -1);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(xmax, 0.0f, 0.0f);    
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(0.0f, ymax, 0.0f);
    glEnd( );
    
    glBegin(GL_QUADS);
    glNormal3i (0, 1, 0);
    glVertex3f(0.0f, ymax, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(0.0f, ymax, 0.0f);
    glEnd( );
    
    glBegin(GL_QUADS);
    glNormal3i (1, 0, 0);
    glVertex3f(xmax, 0.0f, zmax);
    glVertex3f(xmax, 0.0f, 0.0f);
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(xmax, ymax, zmax);
    glEnd( );
    
    glBegin(GL_QUADS);
    glNormal3i (0, -1, 0);
    glVertex3f(0.0f, 0.0f, zmax);
    glVertex3f(xmax, 0.0f, zmax);
    glVertex3f(xmax, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd( );
    
    
    glBegin(GL_QUADS);
    glNormal3i (-1, 0, 0);
    glVertex3f(0.0f, 0.0f, zmax);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, ymax, 0.0f);
    glVertex3f(0.0f, ymax, zmax);
    glEnd( );
    
    
    glBegin(GL_QUADS);
    glNormal3i (0, 0, 1);
    glVertex3f(0.0f, 0.0f, zmax);
    glVertex3f(xmax, 0.0f, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(0.0f, ymax, zmax);
    glEnd( );
    
    glEndList();
    
}
