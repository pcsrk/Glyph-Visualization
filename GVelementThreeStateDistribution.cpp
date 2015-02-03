/*
 *  GVelementThreeStateDistribution.cpp
 *  isoProjTest
 *
 *  Created by mue on 19.06.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVelementThreeStateDistribution.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


GVelementThreeStateDistribution::GVelementThreeStateDistribution  (QGLContext *cx) : GVbasicElement ()    
{
    cx->makeCurrent();    

}

void GVelementThreeStateDistribution::generateOrUpdateElement ( GLUquadricObj* gq, double v1,  double v2, double v3, float transparency)
{
    
    this->w = w; 
    this->h = h; 
    
//    GLcontext->makeCurrent();
    
    float xmax = (float)1;
    float ymax = (float)1;
    float zmax = (float)3;
    
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
    
    glPushMatrix();
    glColor4f(1.0, 1.0, 1.0, transparency);
   
    glTranslated(0.5, 0.5, 0.0); 
    
    gluCylinder(     gq,
                     0.05,
                     0.05,
                     zmax,
                     25,
                     25);
    glTranslated(0.0, 0.0, 0.5); 
    glColor4f(0.0, 0.0, 1.0, transparency);
    gluSphere(gq, v1/2, 30, 30);

    glTranslated(0.0, 0.0, 1.0); 
    glColor4f(0.0, 0.6, 0.0, transparency);
    gluSphere(gq, v2/2, 30, 30);
        

    glTranslated(0.0, 0.0, 1.0); 
    glColor4f(0.6, 0.0, 0.0, transparency);
    gluSphere(gq, v3/2, 30, 30);         

    glPopMatrix();    
    
    glEndList();
    
}
