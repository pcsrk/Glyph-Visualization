/*
 *  Gnumber.cpp
 *  sequencepaint
 *
 *  Created by mue on 05.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Gnumber.h"
#include <QImage>
#include <math.h>

#include <iostream>

Gnumber::Gnumber  (QWidget        *parent, 
                   GLmaster       *shareWidget) : QGLWidget(parent, shareWidget)
{    
    gmaster = shareWidget;
    gmaster->fastTimer (this, SLOT(repaint()));
    glInitOK = false;
    number   = 0;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void Gnumber::setNumber (int value) {
    number = value;
    if (glInitOK) {
      gnumber->setNumber(number);
    }
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QSize Gnumber::minimumSizeHint() const
{
    return QSize(50, 5);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

QSize Gnumber::sizeHint() const
{
    return QSize(200, 50);
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void Gnumber::initializeGL() {
    
    QGLContext *cx = (QGLContext *)this->context();
    
    gnumber = new G_number(gmaster, cx);
    gnumber->setDigits(8);

    crossDL = glGenLists(1);
    glNewList(crossDL, GL_COMPILE); 
    {
        glDisable     ( GL_TEXTURE_2D);
        glDisable     ( GL_BLEND);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_STRIP);
        { glVertex3d   (0, 0, 0); glVertex3d   (0, 1, 0); 
          glVertex3d   (1, 1, 0); glVertex3d   (1, 0, 0); 
          glVertex3d   (0, 0, 0);  }
        glEnd();    
        glBegin(GL_LINES);
        {   glColor3f(1.0, 0.0, 0.0);  glVertex3d   (0, 0, 0);  glVertex3d   (1, 1, 0);	
        glColor3f(0, 0, 1);        glVertex3d   (1, 0, 0);	glVertex3d   (0, 1, 0); }
        glEnd();    
    }
    glEndList();

    glInitOK = true;   
    gnumber->setNumber(number);
}




//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void Gnumber::paintGL() {
    
    this->makeCurrent();
    
    glColor3f(1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    glCallList   (  GLmaster->GMA_2Dunit);
//    glCallList   (  GLmaster->GMA_2Dcross);

    
    glTranslated ( scaleCorrectionDx,  scaleCorrectionDy, 0.0);    
    glScaled     ( scaleCorrectionX,   scaleCorrectionY,  1.0 );
    //
    // TODO
    // make this generic
    // now the scale and translation are just hack
    //
    glScaled     ( 3.0,  3.0, 1.0 );
    glTranslated (-0.3,  0.0, 0.0);    

    gnumber->paint();
 }

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void Gnumber::doResize() {
    
    makeCurrent();
    glViewport(0, 0, this->width(), this->height());
    
    double canvasWidth  = (double)this->width();
    double canvasHeight = (double)this->height();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   
    glOrtho(0, 1, 0, 1, 0, 1);
    
    if (canvasWidth  <  canvasHeight) {
        scaleCorrectionX = 1.0;
        scaleCorrectionY = canvasWidth / canvasHeight; 
    } 
    else {        
        scaleCorrectionX = canvasHeight/  canvasWidth; 
        scaleCorrectionY = 1.0;
    }    
    scaleCorrectionDx = (1-scaleCorrectionX) / 2.0;
    scaleCorrectionDy = (1-scaleCorrectionY) / 2.0;
 //   updateGL();
}


void Gnumber::repaint() {
#if 0     
    QTime    t = GLmaster->getTime();
    int      h = t.hour();
    int      m = t.minute();
    int      s = t.second();
    int     ms = t.msec();
    
    setNumber (h* 1000000 + m*10000+ s *100+ ms/10);
#endif
    //    qDebug ("repaint in number - %d : %d", s, ms);
    updateGL();
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void Gnumber::resizeGL(int width, int height) {
     doResize();
}

