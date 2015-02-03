/*
 *  GVsimpleObjectFindingsglDisplay.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVsimpleObjectFindingsglDisplay_H
#define GVsimpleObjectFindingsglDisplay_H

#include <QtGui>

#include <QGLWidget>
#include <QObject>
#include "GVfindings.h"
#include "GV__simpleObjectFindings.h"

class GVsimpleObjectFindingsglDisplay : public QGLWidget
{
    Q_OBJECT
    
public:
    
    GVsimpleObjectFindingsglDisplay (QWidget *parent=0,  QGLWidget *shareWidget = 0, GVfindings *findings = 0);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    void   generate3DdisplayList  ( int elementID);
    void   setScale               ( float s);
    void   setIsoShift            ( float x, float y);
    void   setBGcolor             ( float r, float g, float b, float a);
    void   setShowRaster          ( bool v);
    
    
    
protected:
        
    void  initializeGL();
    void  paintGL();
    void  resizeGL(int width, int height);
    
    
    void  mouseDoubleClickEvent(QMouseEvent *event);
    void  mousePressEvent(QMouseEvent *event);
    void  mouseMoveEvent(QMouseEvent *event);
    
    
private:
        
     QPoint lastPos;
    
    GLclampf bgr, bgg, bgb, bga;
    
    GV__simpleObjectFindings     *om;
    GVfindings   *findings;   
    
    void   initLights();
    GLuint makeRaster();
    
    int    elementID;
    GLuint currentElement;
    
    float isy;
    float isx;
    
    bool showRaster;
    
    float xrot;
    float zrot;
    
    int   w,h;  
    float scale;
    
    GLuint rasterDl;
    
    
    GLuint backgroundGrid;
    GLuint selectionGrid;
    
};

#endif