/*
 *  GSlider.cpp
 *  isoProjTest
 *
 *  Created by mue on 26.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GSlider.h"
#include <QImage>
#include <QMouseEvent>

#include <iostream>
#include "gl.hpp"

#define MAINBUTTONNAME 4711
#define BACKGROUND       99
#define NOOBJECT          0

#define SELECTION_BUFSIZE 20    


//------------------------------------------------------------------------------------

GSlider::GSlider  (QWidget   *parent,  
                   QGLWidget *shareWidget, 
                   Qt::Orientation orientation) : QGLWidget(parent, shareWidget)
{
    //    m = glModel;
    
    this->orientation = orientation; 
    buttonImage   = new QImage ("/Users/mue/_working/programming/_genview/isoProjTest/buttonImage.png");
    valueImage    = new QImage ("/Users/mue/_working/programming/_genview/isoProjTest/dummyWert.png");

    
    setMouseTracking (true);
    startMovement = false; 
    border = 10.0;
    buttonScale = 0.1;
    msv = 0.0;
}
//------------------------------------------------------------------------------------

QSize GSlider::minimumSizeHint() const
{
    if (orientation == Qt::Horizontal) 
    {
        return QSize(200, 30);
    }
    else
    {
        return QSize(30, 200);
    }
}
//------------------------------------------------------------------------------------

QSize GSlider::sizeHint() const
{
    if (orientation == Qt::Horizontal) 
    {
        return QSize(400, 50);
    }
    else
    {
        return QSize(50, 400);
    }
}



void GSlider::setBorder (double borderInPerzent) {
    border = borderInPerzent;
}

//------------------------------------------------------------------------------------

void GSlider::initializeGL() {

    glDisable(GL_CULL_FACE);

 #if 0 
    
    glNewList(bgDL, GL_COMPILE); 
    {
        glColor3f(0.9, 0.3, 0.2);
        glDisable     ( GL_TEXTURE_2D);
        glDisable     ( GL_BLEND);        
        glBegin(GL_QUADS);
        { glVertex3d   (0, 0, 0); glVertex3d   (1, 0, 0);  glVertex3d   (1, 1, 0);  glVertex3d   (0, 1, 0);}
        glEnd();    
    }
    glEndList();
        
#endif
    haarlineDL = glGenLists(1);
    glNewList(haarlineDL, GL_COMPILE); 
    {
        glDisable     ( GL_TEXTURE_2D);
        glDisable     ( GL_BLEND);        
        glBegin(GL_LINES);
        {
            glVertex3d   (0, 0, 0);				    
            glVertex3d   (0, 1, 0);	
        }
        glEnd();    
    }
    glEndList();
    
    
    
    crossDL = glGenLists(1);
    glNewList(crossDL, GL_COMPILE); 
    {
        glDisable     ( GL_TEXTURE_2D);
        glDisable     ( GL_BLEND);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_STRIP);
        { glVertex3d   (0, 0, 0); glVertex3d   (0, 1, 0); glVertex3d   (1, 1, 0); glVertex3d   (1, 0, 0); glVertex3d   (0, 0, 0);  }
        glEnd();    
        glBegin(GL_LINES);
        {   glColor3f(1.0, 0.0, 0.0);  glVertex3d   (0, 0, 0);  glVertex3d   (1, 1, 0);	
            glColor3f(0, 0, 1);        glVertex3d   (1, 0, 0);	glVertex3d   (0, 1, 0); }
        glEnd();    
    }
    glEndList();
    
    
    // generate the buttons
    
    buttonTexture = bindTexture ( *buttonImage );   

    buttonDL = glGenLists(1);
    glNewList(buttonDL, GL_COMPILE); 
    {
        glEnable     ( GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, buttonTexture);
        glEnable     ( GL_BLEND);
        glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexEnvf      ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  );   
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );    
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBegin(GL_QUADS);
        {
            glTexCoord2d (0,  0);   glVertex3d   (0,  0,  0);				    
            glTexCoord2d (1,  0);   glVertex3d   (1,  0,  0);		
            glTexCoord2d (1,  1);   glVertex3d   (1,  1,  0);		
            glTexCoord2d (0,  1);   glVertex3d   (0,  1,  0);          	
        }
        glEnd();
 
    }
    glEndList();

    
    valueTexture =  bindTexture (*valueImage );   
    valueDL = glGenLists(1);
    glNewList(valueDL, GL_COMPILE); 
    {
        glEnable     ( GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, valueTexture);
        glEnable     ( GL_BLEND);
        glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glTexEnvf      ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  );   
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );    
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBegin(GL_QUADS);
        {
            glTexCoord2d (0,  0);   glVertex3d   (0,  0,  0);				    
            glTexCoord2d (1,  0);   glVertex3d   (1,  0,  0);		
            glTexCoord2d (1,  1);   glVertex3d   (1,  1,  0);		
            glTexCoord2d (0,  1);   glVertex3d   (0,  1,  0);          	
        }
        glEnd();
        
    }
    glEndList();
    
   scaleCorrectionX = 1.0;
   scaleCorrectionY = 1.0;
   
}


//------------------------------------------------------------------------------------

void GSlider::setBGtexture(QImage bgImage) {
    
    makeCurrent();     
    bgDL = glGenLists(1);

    if (!bgImage.isNull() ){
        bgTexture = bindTexture ( bgImage );   
        std::cout << "******* REPLACE BG DISPLAY LIST texture ID is =  " <<  bgTexture    << "\n"; 
    }
    
    glNewList(bgDL, GL_COMPILE); 
    {      
        if (bgImage.isNull()) {
            glColor3f(0.9, 0.2, 0.2);
            glDisable     ( GL_TEXTURE_2D);
            glDisable     ( GL_BLEND);        
            glBegin(GL_QUADS);
                { glVertex3d   (0, 0, 0); glVertex3d   (1, 0, 0);  glVertex3d   (1, 1, 0);  glVertex3d   (0, 1, 0);}
            glEnd();    
        }
        else
        {
            glEnable     ( GL_TEXTURE_2D);
            glBindTexture (GL_TEXTURE_2D, bgTexture);
            glEnable     ( GL_BLEND);
            glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST  );   
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );    
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            
            glColor3f(0.9, 0.2, 0.2);
            
            glBegin(GL_QUADS);
            {
                glTexCoord2d (0,  0);   glVertex3d   (0,  0,  0);				    
                glTexCoord2d (1,  0);   glVertex3d   (1,  0,  0);		
                glTexCoord2d (1,  1);   glVertex3d   (1,  1,  0);		
                glTexCoord2d (0,  1);   glVertex3d   (0,  1,  0);          	
            }
            glEnd();    
        }
    }
    glEndList();
    updateGL();
}


//------------------------------------------------------------------------------------

void GSlider::paintSlider(double position) {

    makeCurrent();
    glPushMatrix();
    glTranslated ( position, 0, 0); 
    glColor3f    ( 0.0, 0.0, 0.8);
    glCallList   ( haarlineDL);
    
    glTranslated (   -0.5,    -0.5,  0.0); 
    glTranslated (    0.5,     0.5,   0.0); 
    glScaled     (    buttonScale,   buttonScale,  1.0 );  
    glTranslated (   -0.5,    -0.5,  0.0); 
    
    if (orientation == Qt::Vertical) {
        glTranslated ( scaleCorrectionDy,  scaleCorrectionDx, 0.0);    
        glScaled     ( scaleCorrectionY,   scaleCorrectionX, 1.0 );
    }
    else
    {
        glTranslated ( scaleCorrectionDx,  scaleCorrectionDy, 0.0);    
        glScaled     ( scaleCorrectionX,   scaleCorrectionY, 1.0 );
    }
    
    glLoadName(MAINBUTTONNAME);    
    glCallList ( buttonDL);
    glPopMatrix();
}
//------------------------------------------------------------------------------------

void GSlider::paintGL() {
    
    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glInitNames();
    glPushName(NOOBJECT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    
    
    if (orientation == Qt::Vertical) {
        // rotate by 90 degrees
        glTranslated (   -sreenDxCorr,    -sreenDyCorr,  0.0); 
        glRotated(90,0,0,1);
        glTranslated (  sreenDyCorr,  -1-sreenDxCorr,  0.0);
        
    
        glEnable     ( GL_TEXTURE_2D);
        glCallList   ( bgDL );
        glDisable    ( GL_TEXTURE_2D);
        
//         glCallList ( crossDL);
         paintSlider(msv);
        
    }
    else
    {    
        glEnable     ( GL_TEXTURE_2D);
        glCallList   ( bgDL );
        glDisable    ( GL_TEXTURE_2D);
        
//        glCallList ( crossDL);
        paintSlider(msv);
        
        glPushMatrix();
        
        
        if (msv < 0.6) {
            glTranslated ( msv, 0.7, 0); 
        }
        else
        {
            glTranslated ( msv-0.1, 0.7, 0); 
        }
            
            
        glScaled     ( 0.4,   0.2, 1.0 );
 //       glTranslated ( scaleCorrectionDx,  scaleCorrectionDy, 0.0);    
      glScaled     ( scaleCorrectionX,   scaleCorrectionY, 1.0 );
        
         glCallList   ( valueDL);
        glPopMatrix();        
        
        paintSlider(msv);        
    }
    glPopMatrix();

}

//------------------------------------------------------------------------------------

void GSlider::mousePressEvent    (QMouseEvent *event) {
    
    makeCurrent();
    QPoint   screenPos = event->pos();

    static GLuint selectionBuffer[SELECTION_BUFSIZE];
    GLint  hits, viewport[4];
    
    glSelectBuffer (SELECTION_BUFSIZE, selectionBuffer);
    glGetIntegerv(GL_VIEWPORT,viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    glRenderMode(GL_SELECT);
    glLoadIdentity();
    gluPickMatrix(screenPos.x(),  viewport[3] - screenPos.y(), 5, 5, viewport);
    glOrtho(leftClippingPlane, rightClippingPlane ,topClippingPlane, bottomClippingPlane, 0, 1);
    
    updateGL();
//    glFlush();
    hits = glRenderMode(GL_RENDER);
//    std::cout << "hits"    << hits     << "\n";
    
    GLuint *ptr = (GLuint *) selectionBuffer;
    for (int i=0;i<hits;i++) {    
        GLuint names = *ptr; 
        ptr +=3;
        for (int j=0;j<names;j++) {
//            std::cout << "                      name = " << *ptr << "\n"; 
            if (*ptr == MAINBUTTONNAME) {
//                std::cout << " START MOVING\n";
                startMovement = true;
                lastEventPos = screenPos;
            }
            ptr++;
        }
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    //        QApplication::sendEvent(this->parentWidget(), event);
}


//------------------------------------------------------------------------------
void GSlider::mouseMoveEvent     (QMouseEvent *event) {
 
    
    
    if (startMovement) {
        if (orientation == Qt::Vertical) {
            double backTransform  = (bottomClippingPlane - topClippingPlane)/  canvasHeight;
            msv = (canvasHeight - (double)event->pos().y()) * backTransform - sreenDyCorr;
        }
        else {
            double backTransform  = (rightClippingPlane - leftClippingPlane)/  canvasWidth;
            msv = (double)event->pos().x() * backTransform - sreenDxCorr;
        }
        
        if (msv < 0.00) msv = 0.0;        
        if (msv > 1.00) msv = 1.0;
       
        valueChanged (msv);
        updateGL();
    }
}
//------------------------------------------------------------------------------
void GSlider::mouseReleaseEvent    (QMouseEvent *event) {
    startMovement = false;
}
//------------------------------------------------------------------------------------
void GSlider::repaint() {
    updateGL();
}

//------------------------------------------------------------------------------------
void GSlider::doResize() {
    
    makeCurrent();
    glViewport(0, 0, this->width(), this->height());
    
    canvasWidth  = (double)this->width();
    canvasHeight = (double)this->height();
        
    double bx = border/100;
    double by = border/100;
    
    if ( (bx * canvasWidth)  > 12)   bx = 12 / canvasWidth;
    if ( (by * canvasHeight) > 12)   by = 12 / canvasHeight;

    if ( (bx * canvasWidth)  < 10)   bx = 10 / canvasWidth;
    if ( (by * canvasHeight) < 10)   by = 10 / canvasHeight;

    
    leftClippingPlane      = 0-bx;
    rightClippingPlane     = 1+bx;
    topClippingPlane       = 0-by;
    bottomClippingPlane    = 1+by;
    
    sreenDxCorr   = bx;
    sreenDyCorr   = by;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   
    glOrtho(leftClippingPlane, rightClippingPlane ,topClippingPlane, bottomClippingPlane, 0, 1);
     
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

    int bw = canvasWidth * buttonScale * scaleCorrectionX;
    
    if  (bw < 15 ) buttonScale = 15  / canvasWidth / scaleCorrectionX;
    if  (bw > 30) buttonScale =  30  / canvasWidth / scaleCorrectionX;
    
    updateGL();
}


//------------------------------------------------------------------------------------
void GSlider::resizeGL(int width, int height) {
    doResize() ;
}

//------------------------------------------------------------------------------------
        
void GSlider::setMainSliderPos (double v) 
{
    msv = v;
    updateGL();
}
//------------------------------------------------------------------------------------

void GSlider::setMinSliderPos  (double v) 
{
}
//------------------------------------------------------------------------------------

void GSlider::setMaxSliderPos  (double v) 
{
}
    

   
