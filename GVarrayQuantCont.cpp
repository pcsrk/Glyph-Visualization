/*
 *  GVhistogramm.cpp
 *  isoProjTest
 *
 *  Created by mue on 31.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVarrayQuantCont.h"
#include <QVector>
#include <QColor>
#include <iostream>


GVarrayQuantCont::GVarrayQuantCont(int numberOfRecords) {
    nor = numberOfRecords;
    values = (double  *) malloc (nor * sizeof(double));    
    
    statvalid  = false;
    minvalid   = false;
    maxvalid   = false;
    avgvalid   = false;
    hisvalid   = false;
}

//------------------------------------------------------------------------------------
GVarrayQuantCont::~GVarrayQuantCont() {
//    if (values) free values;
}

//------------------------------------------------------------------------------------
void   GVarrayQuantCont::setValue        (int atIndex, double value) {
    *(values+atIndex) = value;
    statvalid = false;
}

//------------------------------------------------------------------------------------
double GVarrayQuantCont::value           (int atIndex) {
    return  *(values+atIndex);
}

//------------------------------------------------------------------------------------
int    GVarrayQuantCont::numberOfRecords () {
    return nor;
}

//------------------------------------------------------------------------------------
double  GVarrayQuantCont::minValue     () {
    if (!statvalid || !minvalid) {
        double *p = values;
        min_c = *p;
        for (int i=0;i<nor;i++) {
            if (min_c > *p) {
                min_c = *p;
            }
            p++;
        }
        minvalid  = true;
        statvalid = true;
    }
    return min_c;
}
//------------------------------------------------------------------------------------
double  GVarrayQuantCont::maxValue     () {
    if (!statvalid || !maxvalid) {
        double *p = values;
        max_c = *p;
        for (int i=0;i<nor;i++) {
            if (max_c < *p) {
                max_c = *p;
            }
            p++;
        }
        maxvalid  = true;
        statvalid = true;
    }
    return max_c;
}

//------------------------------------------------------------------------------------
double  GVarrayQuantCont::averageValue () {
    if (!statvalid || !avgvalid) {
        double *p = values;
        avg_c = 0.0;
        for (int i=0;i<nor;i++) {
            avg_c = avg_c + *p++;
        }
        if (nor>0) {
            avg_c = avg_c / nor;
        }
        avgvalid  = true;
        statvalid = true;
    }
    return avg_c;
}

// add other statistcal parameters
//------------------------------------------------------------------------------------
QImage GVarrayQuantCont::histogramImage(int width, int height, double min, double max) {

    int index; 
    double *p=values;
    double scale;
    QVector<int> histV(width);    
    histV.fill (0);
        
    p=values;
    for (int i=0;i<nor;i++) {
        if ((*p >= min) && (*p <= max)) {
            scale = (*p - min) / (max-min);        
            index = (width-1) * scale;
            histV[index]++;
        }
        p++;
        
    }
    
    int maxCount = 0;
    int maxW = 0;
    for (int i=0;i<width;i++) {
        if (maxCount < histV[i]) 
            maxCount = histV[i];
            maxW = i;
    }
    
    
    std::cout << "max "    << maxCount << " at nr " << maxW << " age " << (min + (max-min) * maxW / width)     << "\n";

    
//    if (histogram)
//        delete histogram;
    
    histogram = new QImage (width, height, QImage::Format_ARGB32);
    histogram->fill(0);
    QColor fgColor = QColor (255,255,255,255);
    QRgb   fgpixel   = fgColor.rgba();
    int end;
    for (int i=0;i<width;i++) {
        end = height * (double(histV[i]) / double (maxCount));
        for (int j = height-1; j>(height-end); j--){
            histogram->setPixel (i,j,fgpixel);        
        }
    }        
    return *histogram;
}



