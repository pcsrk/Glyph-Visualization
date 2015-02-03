/*
 *  G_number.cpp
 *  isoProjTest
 *
 *  Created by mue on 06.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "G_number.h"
#include <QImage>
#include <math.h>
#include <iostream>

#define  DELTA  0.4

G_number::G_number (GLmaster *gmaster, QGLContext *cx) {

    this->gmaster = gmaster;
    this->cx      = cx;
    cx->makeCurrent();
    
    textureManager = new G__texture (cx);

    //------------------------------------------------------------------------------------    
    // debug display lists
    bbDL = glGenLists(1);
    glNewList(bbDL, GL_COMPILE); {
        glDisable ( GL_TEXTURE_2D);   glDisable ( GL_BLEND); glColor3f(0.0, 0.0, 0.5);
        glBegin(GL_LINE_STRIP);
        { glVertex3d(0, 0, 0); glVertex3d(0, 1, 0); glVertex3d(1, 1, 0); glVertex3d(1, 0, 0); glVertex3d(0, 0, 0);  }
        glEnd();    
    }
    glEndList();
    bbHalfDL = glGenLists(1);
    glNewList(bbHalfDL, GL_COMPILE); {
        glDisable ( GL_TEXTURE_2D);   glDisable ( GL_BLEND);  glColor3f(0.0, 0.0, 0.5);
        glBegin(GL_LINE_STRIP);
        { glVertex3d(0, 0, 0); glVertex3d(0, 1, 0); glVertex3d(0.5, 1, 0); glVertex3d(0.5, 0, 0); glVertex3d(0, 0, 0);  }
        glEnd();    
    }
    glEndList();
    //------------------------------------------------------------------------------------    
    
    //------------------------------------------------------------------------------------    
    // fonts DL
    
    GLuint singleNumbertexture;

    for (int i = 0; i<10; i++) {
        
        singleNumberDL[i] = glGenLists(1);
        QString fontname = ":/fonts/";  fontname.append (QString::number (i,10)); fontname.append (".png");        
        singleNumbertexture = textureManager->generateTexture (fontname);
       
        glNewList(singleNumberDL[i], GL_COMPILE); 
        {
            glEnable     ( GL_TEXTURE_2D);
            glBindTexture (GL_TEXTURE_2D, singleNumbertexture);
            glEnable     ( GL_BLEND);
            glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glTexEnvf      ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_REPLACE );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );    
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glBegin(GL_QUADS);
            {
                glTexCoord2d (0,  0);   glVertex3d   (0,    0,  0);				    
                glTexCoord2d (1,  0);   glVertex3d   (0.5,  0,  0);		
                glTexCoord2d (1,  1);   glVertex3d   (0.5,  1,  0);		
                glTexCoord2d (0,  1);   glVertex3d   (0,    1,  0);          	
            }
            glEnd();
        }
        glEndList();        
    }
 
    setDigits (5);
    digits_old = 5;
}

//------------------------------------------------------------------------------------    
void G_number::setNumber (int value) {
    number = value;
}

//------------------------------------------------------------------------------------    
void G_number::setDigits   (int n) {
    digits = n;
    if (digits == 1) 
        {width = 0.5;  height = 1.0;} 
    else
        {width = 1.0;  height = 1 / (digits * DELTA); } 
}

//------------------------------------------------------------------------------------    
void G_number::paint() {
    
    cx->makeCurrent();
    
    int stellen;
    
    if (number == 0) {
        stellen = 1;
    }
    else {
		//STS
        stellen = log10((double)number) + 1; 
    }
    
    int    tmp     = number;
    double scale   = 1 / (digits * DELTA );
        
    glPushMatrix();     
    glScaled    ( scale, scale,  1.0 );
    glTranslated ( DELTA * digits, 0.0, 0.0); 
    for (int j=0;j<stellen;j++) {
        int n = tmp%10;
        glTranslated ( -DELTA,  0, 0.0);    
        glCallList   ( singleNumberDL[n] );
//        glCallList   ( bbHalfDL );
        tmp = tmp / 10;      
    }
    glPopMatrix();
}


double G_number::getWidth() {return width;}
double G_number::getHeight(){return height;}
double G_number::getAspect(){return width/height;}
int    G_number::getDigits(){return digits;}
