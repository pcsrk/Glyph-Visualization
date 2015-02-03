/*
 *  GVsettings.h
 *  dv
 *
 *  Created by mue on 09.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef GVsettings_H
#define GVsettings_H


#include <QObject>

class GVsettings : public QObject
{

	Q_OBJECT  
public:
    
int    flipPBuffer;

	
int    defaultCellsPerYear;
int	   defaultAgeBands;
	
int		numberOfAnts;
double  antMinSpeed;
double  antMaxSpeed;

double antK1;
double antK2;

private:
  
};

#endif