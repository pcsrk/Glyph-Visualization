/*
 *  GVsliderWindow.cpp
 *  isoProjTest
 *
 *  Created by mue on 26.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVsliderWindow.h"
#include <QVBoxLayout>
#include "QImage"


#include <iostream>

GVsliderWindow::GVsliderWindow () 
{
    
    gmaster = new GLmaster();

    
    qtSlider  = new QSlider(Qt::Horizontal);
    
    glSilder  = new GSlider(this, gmaster,        Qt::Horizontal);
    glSilder2 = new GSlider(this, gmaster, Qt::Horizontal);
    glSilder3 = new GSlider(this, gmaster, Qt::Horizontal);
    glSilder4 = new GSlider(this, gmaster, Qt::Vertical);  
    
    
    qtSlider->setRange(0, 1024);
    qtSlider->setValue(0);

    connect(qtSlider,   SIGNAL(valueChanged(int)),              this, SLOT(updateGlSlider(int)) );
    connect(glSilder,   SIGNAL(valueChanged(double)),           this, SLOT(updateQtSlider(double)) );
   
    connect(glSilder2,   SIGNAL(valueChanged(double)),          glSilder3, SLOT(setMainSliderPos(double)) );
    connect(glSilder2,   SIGNAL(valueChanged(double)),          glSilder4, SLOT(setMainSliderPos(double)) );
    
    connect(glSilder4,   SIGNAL(valueChanged(double)),          glSilder, SLOT(setMainSliderPos(double)) );
    connect(glSilder4,   SIGNAL(valueChanged(double)),          glSilder2, SLOT(setMainSliderPos(double)) );
    connect(glSilder4,   SIGNAL(valueChanged(double)),          glSilder3, SLOT(setMainSliderPos(double)) );
   
    connect(glSilder4,   SIGNAL(valueChanged(double)),          this, SLOT(updateNumberValue(double)) );

    
    QHBoxLayout   *glSliderhorizontalLayout = new QHBoxLayout;

    glSliderhorizontalLayout->addWidget(glSilder2);
    glSliderhorizontalLayout->addWidget(glSilder3);
    
    
    QVBoxLayout   *glSliderverticalLayout   = new QVBoxLayout;
    glSliderverticalLayout->addWidget(qtSlider);
    glSliderverticalLayout->addWidget(glSilder);
    glSliderverticalLayout->addLayout(glSliderhorizontalLayout);
//    glSliderverticalLayout->addWidget(number);
    
    
     QHBoxLayout   *finalLayout = new QHBoxLayout;
     finalLayout->addLayout(glSliderverticalLayout);
     finalLayout->addWidget(glSilder4);
   
     setLayout(finalLayout);     

}


void GVsliderWindow::updateNumberValue(double v) {
    int intValue = 10000 * v;
    number->setNumber(intValue);
    number->repaint();
}


void GVsliderWindow::updateGlSlider(int v) {
    glSilder->setMainSliderPos (double(v) / 1024.0);
}

void GVsliderWindow::updateQtSlider(double v) {
    int qtValue = 1024 * v;
    if (qtValue != oldQtValue) {
        oldQtValue = qtValue;
        qtSlider->setValue(qtValue);
    }
}