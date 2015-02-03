/*
 *  GVisoGrid.cpp
 *  isoProjTest
 *
 *  Created by mue on 08.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVisoGrid.h"
#include <iostream>

//------------------------------------------------------------------------------------
GVisoGrid::GVisoGrid  ( QMutex *m,
						int     width,      
                        int     height) {
	this->m = m;				   
	grid = 0;
	resetAndSetSize (width, height);
	
	
	roundToYears    = true;
	cellsPerYear    = 5;
	yearsPerStripe  = 5;
}    


//------------------------------------------------------------------------------------
void   GVisoGrid::resetAndSetSize (int width, int height) {
	
	m->lock();
    w = width;
    h = height;
	elementList.clear();
	if (grid) delete (grid);
    grid =   (int *) malloc(sizeof (int) * w * h);  
    for (int yo = 0; yo < h; yo++) {
        for (int xo = 0; xo < w; xo++) {
            *(grid + xo + (yo) * w) = -1;         
        }
    }
	m->unlock();
	
	setAreaRegions ( 2*h/3, w/3, w/3, w- w/3 - w/3);

}


//------------------------------------------------------------------------------------
void  GVisoGrid::setAreaRegions ( int h0, int w1, int w2, int w3) {


//	qDebug ("  GVisoGrid::setAreaRegions h0, w1, w2, w3 - %d %d %d %d", h0, w1,w1,w3);
	
	areas[0].w  = w;
	areas[0].h  = h0;
	areas[0].x  = 0;
	areas[0].y  = 0;
	
	areas[1].w = w1;
	areas[1].h = h-h0;
	areas[1].x = 0;
	areas[1].y = h0;

	areas[2].w = w2;
	areas[2].h = h-h0;
	areas[2].x = w1;
	areas[2].y = h0;

	areas[3].w = w3;
	areas[3].h = h-h0;
	areas[3].x = w1 + w2;
	areas[3].y = h0;
	
	
	for (int i = 0; i<4; i++) {
		areas[i].ex = areas[i].x + areas[i].w  - 1;
		areas[i].ey = areas[i].y + areas[i].h  - 1;
		areas[i].cx	= (areas[i].x + areas[i].w/2);
		areas[i].cy	= (areas[i].y + areas[i].h/2);
	}
}

//------------------------------------------------------------------------------------
int GVisoGrid::setElement  (GVbasicElement* e) {


    if (e->x < 0) return -1;
    if (e->y < 0) return -1;
    if (e->x + e->w > w) return -1;
    if (e->y + e->h > h) return -1;
 
	m->lock();
   
    elementList.append (e);
	
    for (int yo = 0; yo < e->h; yo++) {
        for (int xo = 0; xo < e->w; xo++) {
            *(grid + e->x + xo + (e->y +yo) * w) = elementList.indexOf (e);         
        }
    }
	m->unlock();
    return (e->w * e->h);


}    

//------------------------------------------------------------------------------------
bool GVisoGrid::isCellTaken  (int x, int y) {
   return  ( *(grid + x +  y * w) != -1);
}


//------------------------------------------------------------------------------------
bool GVisoGrid::isTherePlace  (GVbasicElement* e, int atx, int aty) {
    
    bool p = false;
    
    
 //   qDebug ("check of Element at %d, %d, size %d, %d", atx, aty, e->w, e->h); 

	
    if (atx< 0)         return p;
    if (aty < 0)        return p;
    if (atx+ e->w > w)  return p;
    if (aty+ e->h > h)  return p;

   	m->lock();

    p = true;
    for (int yo = 0; yo < e->h; yo++) {
        for (int xo = 0; xo < e->w; xo++) {
            if ( *(grid +atx + xo + (aty +yo) * w) != -1) {
                p = false;
                break;
            }         
        }
    }
	m->unlock();
    return p;

}    


//------------------------------------------------------------------------------------
bool       GVisoGrid::isElementInArea (GVbasicElement* e, int area) {
   
   return  (  ( e->x >= areas[area].x)  &&  
              ( e->x <= areas[area].ex) &&
			  ( e->y >= areas[area].y)  &&
			  ( e->y >= areas[area].ey) );
			  
}


//------------------------------------------------------------------------------------
QList <QPoint>  GVisoGrid::allFreePlacesAround (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <QPoint> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y+2); if (!n) ll.append(QPoint(x,   y+2)); // 1
        n = getElement (x,   y+1); if (!n) ll.append(QPoint(x,   y+1)); // 2
        n = getElement (x+1, y  ); if (!n) ll.append(QPoint(x+1, y  )); // 3
        n = getElement (x,   y-1); if (!n) ll.append(QPoint(x,   y-1)); // 4
        n = getElement (x,   y-2); if (!n) ll.append(QPoint(x,   y-2)); // 5
        n = getElement (x-1, y-1); if (!n) ll.append(QPoint(x-1, y-1)); // 6
        n = getElement (x-1, y  ); if (!n) ll.append(QPoint(x-1, y  )); // 7
        n = getElement (x-1, y+1); if (!n) ll.append(QPoint(x-1, y+1)); // 8
    }
    else {
        n = getElement (x,    y+2); if (!n) ll.append(QPoint(x,    y+2)); // 1
        n = getElement (x+1,  y+1); if (!n) ll.append(QPoint(x+1,  y+1)); // 2
        n = getElement (x+1,  y  ); if (!n) ll.append(QPoint(x+1,  y  )); // 3
        n = getElement (x+1,  y-1); if (!n) ll.append(QPoint(x+1,  y-1)); // 4
        n = getElement (x,    y-2); if (!n) ll.append(QPoint(x,    y-2)); // 5
        n = getElement (x,    y-1); if (!n) ll.append(QPoint(x,    y-1)); // 6
        n = getElement (x-1,  y  ); if (!n) ll.append(QPoint(x-1,  y  )); // 7
        n = getElement (x,    y+1); if (!n) ll.append(QPoint(x,    y+1)); // 8
    }
    return ll;
}


//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::allNeighbors (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y+2); if (n) ll.append(n); // 1
        n = getElement (x,   y+1); if (n) ll.append(n); // 2
        n = getElement (x+1, y  ); if (n) ll.append(n); // 3
        n = getElement (x,   y-1); if (n) ll.append(n); // 4
        n = getElement (x,   y-2); if (n) ll.append(n); // 5
        n = getElement (x-1, y-1); if (n) ll.append(n); // 6
        n = getElement (x-1, y  ); if (n) ll.append(n); // 7
        n = getElement (x-1, y+1); if (n) ll.append(n); // 8
    }
    else
    {
        n = getElement (x,    y+2); if (n) ll.append(n); // 1
        n = getElement (x+1,  y+1); if (n) ll.append(n); // 2
        n = getElement (x+1,  y  ); if (n) ll.append(n); // 3
        n = getElement (x+1,  y-1); if (n) ll.append(n); // 4
        n = getElement (x,    y-2); if (n) ll.append(n); // 5
        n = getElement (x,    y-1); if (n) ll.append(n); // 6
        n = getElement (x-1,  y  ); if (n) ll.append(n); // 7
        n = getElement (x,    y+1); if (n) ll.append(n); // 8
    }
    return ll;
}


//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::allNeighbors (int x, int y) 
{
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y+2); if (n) ll.append(n); // 1
        n = getElement (x,   y+1); if (n) ll.append(n); // 2
        n = getElement (x+1, y  ); if (n) ll.append(n); // 3
        n = getElement (x,   y-1); if (n) ll.append(n); // 4
        n = getElement (x,   y-2); if (n) ll.append(n); // 5
        n = getElement (x-1, y-1); if (n) ll.append(n); // 6
        n = getElement (x-1, y  ); if (n) ll.append(n); // 7
        n = getElement (x-1, y+1); if (n) ll.append(n); // 8
    }
    else
    {
        n = getElement (x,    y+2); if (n) ll.append(n); // 1
        n = getElement (x+1,  y+1); if (n) ll.append(n); // 2
        n = getElement (x+1,  y  ); if (n) ll.append(n); // 3
        n = getElement (x+1,  y-1); if (n) ll.append(n); // 4
        n = getElement (x,    y-2); if (n) ll.append(n); // 5
        n = getElement (x,    y-1); if (n) ll.append(n); // 6
        n = getElement (x-1,  y  ); if (n) ll.append(n); // 7
        n = getElement (x,    y+1); if (n) ll.append(n); // 8
    }
    return ll;
}

//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::rightNeighbors (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y+1); if (n) ll.append(n); // 2
        n = getElement (x+1, y  ); if (n) ll.append(n); // 3
        n = getElement (x,   y-1); if (n) ll.append(n); // 4
    }
    else
    {
        n = getElement (x+1,  y+1); if (n) ll.append(n); // 2
        n = getElement (x+1,  y  ); if (n) ll.append(n); // 3
        n = getElement (x+1,  y-1); if (n) ll.append(n); // 4
    }
    return ll;
}
//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::leftNeighbors (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x-1, y-1); if (n) ll.append(n); // 6
        n = getElement (x-1, y  ); if (n) ll.append(n); // 7
        n = getElement (x-1, y+1); if (n) ll.append(n); // 8
    }
    else
    {

        n = getElement (x,    y-1); if (n) ll.append(n); // 6
        n = getElement (x-1,  y  ); if (n) ll.append(n); // 7
        n = getElement (x,    y+1); if (n) ll.append(n); // 8
    }
    return ll;
}

//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::downNeighbors (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y-1); if (n) ll.append(n); // 4
        n = getElement (x,   y-2); if (n) ll.append(n); // 5
        n = getElement (x-1, y-1); if (n) ll.append(n); // 6
          }
    else
    {
        n = getElement (x+1,  y-1); if (n) ll.append(n); // 4
        n = getElement (x,    y-2); if (n) ll.append(n); // 5
        n = getElement (x,    y-1); if (n) ll.append(n); // 6
    }
    return ll;
}


//------------------------------------------------------------------------------------
QList <GVbasicElement*>  GVisoGrid::upNeighbors (GVbasicElement* e) 
{
    int x = e->x;
    int y = e->y;
    GVbasicElement* n;    
    QList <GVbasicElement*> ll;
    
    if ( (y%2) == 0) {
        n = getElement (x,   y+2); if (n) ll.append(n); // 1
        n = getElement (x,   y+1); if (n) ll.append(n); // 2
        n = getElement (x-1, y+1); if (n) ll.append(n); // 8
    }
    else
    {
        n = getElement (x,    y+2); if (n) ll.append(n); // 1
        n = getElement (x+1,  y+1); if (n) ll.append(n); // 2
        n = getElement (x,    y+1); if (n) ll.append(n); // 8
    }
    return ll;
}


void           GVisoGrid::removeElement   (GVbasicElement* e) {
   bool removeOK = true;
   
	if (e->x < 0)         removeOK = false;
    if (e->y < 0)         removeOK = false;
    if (e->x + e->w > w)  removeOK = false;
    if (e->y + e->h > h)  removeOK = false;
   	m->lock(); 
    if (removeOK) {
        for (int yo = 0; yo < e->h; yo++) {
            for (int xo = 0; xo < e->w; xo++) {
               *(grid + e->x + xo + (e->y +yo) * w) = -1;         
            }
        }
    }
	m->unlock();
}
	

//------------------------------------------------------------------------------------
int GVisoGrid::moveElementTo  (GVbasicElement* e, int xi, int yi) {
    
	

    if (xi < 0) return -1;
    if (yi < 0) return -1;
    if (xi + e->w > w) return -1;
    if (yi + e->h > h) return -1;

	m->lock();
    
    int index = elementList.indexOf (e);
    
    
    bool removeOK = true;
   
	if (e->x < 0)         removeOK = false;
    if (e->y < 0)         removeOK = false;
    if (e->x + e->w > w)  removeOK = false;
    if (e->y + e->h > h)  removeOK = false;
    
    if (removeOK) {
        for (int yo = 0; yo < e->h; yo++) {
            for (int xo = 0; xo < e->w; xo++) {
               *(grid + e->x + xo + (e->y +yo) * w) = -1;         
            }
        }
    }

    e->x = xi;
    e->y = yi;
    
    
    for (int yo = 0; yo < e->h; yo++) {
        for (int xo = 0; xo < e->w; xo++) {
            *(grid + e->x + xo + (e->y +yo) * w) = index;         
        }
    }
   	m->unlock();

    return (e->w * e->h);
}

//------------------------------------------------------------------------------------
int GVisoGrid::swapElements  (GVbasicElement* e1, GVbasicElement* e2) {
    
    int x1temp = e1->x;
    int y1temp = e1->y;
    int x2temp = e2->x;
    int y2temp = e2->y;
    
    int r1, r2;

	m->lock();
    e1->x = -1;
    e1->y = -1;
	m->unlock();
    
    r1 = moveElementTo (e2, x1temp, y1temp);
    r2 = moveElementTo (e1, x2temp, y2temp);
    return (r1+r2);
}



//------------------------------------------------------------------------------------
GVbasicElement* GVisoGrid::getElement  (int xi, int yi) {

	GVbasicElement *te = 0; 
    if (xi < 0) return 0;
    if (yi < 0) return 0;
    if (xi > w) return 0;
    if (yi > h) return 0;    
    int index =  *(grid + xi + yi * w);    
    if (index != -1) te = elementList.at(index);
    return te;
}


//------------------------------------------------------------------------------------
int GVisoGrid::getIndex (int xi, int yi) {
    
    if (xi < 0) return -1;
    if (yi < 0) return -1;
    if (xi > w) return -1;
    if (yi > h) return -1;    
    return  *(grid + xi + yi * w);  
}


//------------------------------------------------------------------------------------
QRect   GVisoGrid::minMax         () {
	
	int xmin =  w+1;
	int ymin =  h+1;
	int xmax = -1;
	int ymax = -1;	
	m->lock();
	for (long i= 0; i<elementList.size(); i++) {
		if 	(xmin >  elementList[i]->x ) xmin = elementList[i]->x;
		if 	(ymin >  elementList[i]->y ) ymin = elementList[i]->y;
		if 	(xmax <  elementList[i]->x ) xmax = elementList[i]->x;
		if 	(ymax <  elementList[i]->y ) ymax = elementList[i]->y;
	}
	m->unlock();
	return QRect (xmin, ymin, xmax-xmin, ymax-ymin);
}

//------------------------------------------------------------------------------------
QRect   GVisoGrid::minMaxSelected () {
	
	int xmin =  w+1;
	int ymin =  h+1;
	int xmax = -1;
	int ymax = -1;	
	
	for (long i= 0; i<elementList.size(); i++) {
		if (isSelected ( elementList[i]->x, elementList[i]->y) ) {
			m->lock();
			if 	(xmin >  elementList[i]->x ) xmin = elementList[i]->x;
			if 	(ymin >  elementList[i]->y ) ymin = elementList[i]->y;
			if 	(xmax <  elementList[i]->x ) xmax = elementList[i]->x;
			if 	(ymax <  elementList[i]->y ) ymax = elementList[i]->y;
			m->unlock();
		}
	}
	return QRect (xmin, ymin, xmax-xmin, ymax-ymin);
}
//------------------------------------------------------------------------------------
bool GVisoGrid::isSelected (int xi, int yi) { 

	GVbasicElement *e = getElement (xi, yi);
	
    if (e) {
		return  e->fo->isSelected;
	}
	else {
		return false;
	}
}
