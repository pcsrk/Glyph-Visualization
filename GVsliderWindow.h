/*
 *  GVsliderWindow.h
 *  isoProjTest
 *
 *  Created by mue on 26.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QWidget>
#include <QSlider>
#include "GSlider.h"

#include "GLmaster.h"
#include "Gnumber.h"


class GVsliderWindow: public QWidget
{
    Q_OBJECT
    
    
public:    
    GVsliderWindow ();
    
public slots:
    
    
    void updateGlSlider(int v);
    void updateQtSlider(double v); 
    void updateNumberValue(double v); 

signals:

    
private:
    int                     oldQtValue;    
    QSlider                *qtSlider;

    GLmaster               *gmaster;
    Gnumber                *number;
    
    GSlider                *glSilder;
    GSlider                *glSilder2;
    GSlider                *glSilder3;
    GSlider                *glSilder4;
    GSlider                *glSilder5;
    GSlider                *glSilder6;
    GSlider                *glSilder7;
    GSlider                *glSilder8;
    GSlider                *glSilder9;
    GSlider                *glSilder2v;
    GSlider                *glSilder3v;
    GSlider                *glSilder4v;
    GSlider                *glSilder5v;
    GSlider                *glSilder6v;
    GSlider                *glSilder7v;
    GSlider                *glSilder8v;
    GSlider                *glSilder9v;
    
};

    