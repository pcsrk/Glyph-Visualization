/*
 *  GVhistogramm.h
 *  isoProjTest
 *
 *  Created by mue on 31.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVHISTOGRAM_H
#define GVHISTOGRAM_H

#include <QImage>


class GVarrayQuantCont
{
    
public:
    
     GVarrayQuantCont(int numberOfRecords);
    ~GVarrayQuantCont();
    
    // no range check is done ! 
    void   setValue        (int atIndex, double value);
    double value           (int atIndex);
    int    numberOfRecords ();    
    
    double  minValue     ();
    double  maxValue     ();
    double  averageValue ();
    
    // add other statistcal parameters
    
    QImage histogramImage(int width, int height, double min, double max) ;
         
private:
    
    int     nor;    
    
    double *values;
    double  min_c;
    double  max_c;
    double  avg_c;
    
    QImage  *histogram; 
    
    bool    statvalid;
    bool    minvalid;
    bool    maxvalid;
    bool    avgvalid;
    bool    hisvalid;

};
#endif