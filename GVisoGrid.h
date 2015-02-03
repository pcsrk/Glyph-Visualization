/*
 *  GVisoGrid.h
 *  isoProjTest
 *
 *  Created by mue on 08.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVisoGrid_H
#define GVisoGrid_H

#include <QList>
#include <QMutex>

#include "GVglobals.h"
#include "GVbasicElement.h"

// areas are oly used by the animation master


class Area {
public:
	int  movementType;
	int x;
	int y;
	int ex; 
	int ey;
	int cx; 
	int cy;
	int w;
	int h;
	QList <int> elementsToMoveIn;
	QList <int> lookForAPlace;
};





class GVisoGrid 
{
    
    
public:
    
    GVisoGrid  (QMutex *m,	
				int     width,      
                int     height);    
  
	void   resetAndSetSize (int width, int height);    
    
	int             width ()  { return w;}
	int             height () { return h;}

    int             setElement     (GVbasicElement* e);    
    int             moveElementTo  (GVbasicElement* e, int xi, int yi);    
    int             swapElements   (GVbasicElement* e1, GVbasicElement* e2);
    
	void            removeElement   (GVbasicElement* e);    
	
    int             count      ()          {   return elementList.count();}   
    GVbasicElement* elementAt  (int index) {   return elementList.at (index);}    
    
    bool            isTherePlace    (GVbasicElement* e, int atx, int aty);

    bool            isElementInArea (GVbasicElement* e, int area);
	bool            isCellTaken     (int x, int y);


    QList <QPoint>  allFreePlacesAround (GVbasicElement* e); 

    QList <GVbasicElement*> allNeighbors   (int x, int y); 
    QList <GVbasicElement*> allNeighbors   (GVbasicElement* e);
    QList <GVbasicElement*> upNeighbors    (GVbasicElement* e);
    QList <GVbasicElement*> downNeighbors  (GVbasicElement* e);
    QList <GVbasicElement*> leftNeighbors  (GVbasicElement* e);
    QList <GVbasicElement*> rightNeighbors (GVbasicElement* e);
    
    GVbasicElement* getElement  (int xi, int yi);
    int             getIndex    (int xi, int yi);

    bool            isSelected(int xi, int yi);
  

	QRect           minMax         ();
	QRect           minMaxSelected ();
    

    QList <GVbasicElement*>  elementList;

	Area    areas[4];

	int   defaultCellsPerYear;  

	
	bool  roundToYears;  
	int   cellsPerYear;  
	int   yearsPerStripe; 

private:

   QMutex *m;   
   
   int  w,h;   
     
   void setAreaRegions ( int h0, int w1, int w2, int w3);
   
   int *grid; 
	
	
	
  
};


#endif