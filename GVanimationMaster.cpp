/*
 *  GVanimationMaster.cpp
 *  diseaseViewer
 *
 *  Created by mue on 24.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVanimationMaster.h"
#include "GVsimpleObjectFindings.h"
#include "GVfindings.h"
#include <QTime>
#include "math.h" 

//------------------------------------------------------------------------------
GVanimationMaster::GVanimationMaster (  GVsettings *settings,
										QMutex *m) {


	_settings = 	settings;	
	
	qDebug ("create animation thread");

	_antArea = 0;
	_objectGrid = 0;
	_findings   = 0;
	_readyToGenerateGLelements = false;
	_m=m;
	_numberOfAnts = 100;
	_antparamsChanged = false;
	elementStack.clear();
}


//------------------------------------------------------------------------------
void GVanimationMaster::setDataGrid      (     GVisoGrid       *objectGrid,
                                               GVfindings      *findings) {
	_objectGrid = objectGrid;
	_findings   = findings;			
	
	qDebug ("start position = %d, %d", _objectGrid->width()/2, _objectGrid->height()/2 );
	
	freepositions.append (QPoint (_objectGrid->width()/2, _objectGrid->height()/2 ));	
	fpHash [_objectGrid->width()  + _objectGrid->height()/2 *  _objectGrid->width() ] = true;								  
	
	
	_numberOfAnts = _objectGrid->width() *  _objectGrid->height() * 0.01;
	
	if (_numberOfAnts < 10)  _numberOfAnts =  10;
	if (_numberOfAnts > 300) _numberOfAnts = 300;
	
}


//------------------------------------------------------------------------------
void GVanimationMaster::initAnts ( int area) {

	_numberOfAnts = _settings->numberOfAnts;

	_antminSpeed = int ( (double)_objectGrid->areas[area].w * _settings->antMinSpeed / 200);
	_antmaxSpeed = int ( (double)_objectGrid->areas[area].w * _settings->antMaxSpeed / 200);
	
	  
		  
    if (_antminSpeed <1) _antminSpeed = 1;
    if (_antmaxSpeed <1) _antminSpeed = 1;

	_antspeedDelta = _antmaxSpeed - _antminSpeed;
	
	antpositions.clear();
	antelements.clear();
	int x,y;

	for (int k=0; k< _numberOfAnts; k++) {
	
		x = _objectGrid->areas[area].x + rand()%_objectGrid->areas[area].w -1;
		y = _objectGrid->areas[area].y + rand()%_objectGrid->areas[area].h -1;
		antpositions.append (QPoint (x,y));
		antelements.append(0);
	}	
	_antArea = area;	
	__aCount = 0;

	qDebug ("INIT ANTS");

 
}


void GVanimationMaster::antSettingsChanged () {_antparamsChanged = true; }


//------------------------------------------------------------------------------
void GVanimationMaster::moveAnts         ( ) {

	qreal k1 = _settings->antK1;
	qreal k2 = _settings->antK2;
	qreal f; 
	qreal ppick; 
	qreal pdrop; 
 //   qDebug ("AA %d ", __aCount++);

	for (int k=0; k< _numberOfAnts; k++) {

		GVbasicElement *ae = antelements[k];
		
			
		if ( (ae == 0) &&  _objectGrid->isCellTaken (antpositions[k].x(), antpositions[k].y()) ) {

			GVbasicElement *ne = _objectGrid->getElement  (antpositions[k].x(), antpositions[k].y() );

	//	    qDebug ("ELEMENT met at %d %d  ", antpositions[k].x(), antpositions[k].y());

			f = ffunc (ne, antpositions[k].x(), antpositions[k].y());

			ppick = 1000 * ( k1 / (k1+f) ) * ( k1 / (k1+f) );
//			qDebug ("    ne->fo->sV  = %f, f = %f ppik = %f", ne->fo->sV, f, ppick);

//			if (ne->fo->sV > 2.0) ppick = 0;
			if ( rand()%1000 < ppick) {
				antelements[k] = ne;
	//			_objectGrid->removeElement (ne);
	//			qDebug ("    PICKED");
			} 

			
		}
		if (  (ae !=  0) &&  ! _objectGrid->isCellTaken (antpositions[k].x(), antpositions[k].y()) ) {
	//		qDebug ("empty ELEMENT met at %d %d  ", antpositions[k].x(), antpositions[k].y());

			f = ffunc (ae, antpositions[k].x(), antpositions[k].y());			
			if (f < k2) pdrop = 2000 * f; else pdrop = 1000;
			if ( rand()%1000 < pdrop) {
			    ae->x_togo = antpositions[k].x();
				ae->y_togo = antpositions[k].y();
				antelements[k] = 0;
				int index = _objectGrid->getIndex  (ae->x , ae->y );
				if (index != 1)  {
					_objectGrid->areas[_antArea].elementsToMoveIn.append (index);
//					qDebug ("    DROPPED element with index %d", index);
				}
			} 		
		}
	}

	for (int k=0; k< _numberOfAnts; k++) {
	
	  double randV = 	rand()%500 / 500.0;
	  int speed = _antminSpeed +_antspeedDelta * randV;	
	  int border = 10;	  
	  
//	  qDebug ("ANTESPEED = %d", speed);
	  
      int nx = antpositions[k].x() + (rand()%3 -1) * speed;
	  int ny = antpositions[k].y() + (rand()%3 -1) * speed;
	  if  (nx < _objectGrid->areas[_antArea].x+border)  nx = _objectGrid->areas[_antArea].x+border;
	  if  (ny < _objectGrid->areas[_antArea].y+border)  ny = _objectGrid->areas[_antArea].y+border;
	  if  (nx > _objectGrid->areas[_antArea].ex-border) nx = _objectGrid->areas[_antArea].ex-border;
	  if  (ny > _objectGrid->areas[_antArea].ey-border) ny = _objectGrid->areas[_antArea].ey-border;

	  antpositions[k].setX (nx);
	  antpositions[k].setY (ny);
	 
	}
//	qDebug ("new Position of Ant 0 -> %d,%d", antpositions[0].x(), antpositions[0].y());
}


//------------------------------------------------------------------------------
qreal GVanimationMaster::ffunc         (GVbasicElement *e, int x, int y)  {

   qreal ftemp = 0.0;	
   qreal alpha = 1; // maximal dissimilarity
   QList <GVbasicElement*> nh = _objectGrid->allNeighbors  (x,y);
   if ( nh.size() == 0) return 0;
   
   for (int i= 0; i< nh.size(); i++) {
   
	
        if (e != nh[i]) {
		
//			qreal dist = distance (nh[i], e) / alpha;
			qreal dist = genericDistance (nh[i], e);
			if (dist>1) dist = 1;
			ftemp = ftemp + ( 1 - dist);
				
  //       	qDebug (" distance sv1=%f, sv2=%f dist= %f", e->fo->sV , nh[i]->fo->sV , dist);
	  }

   }

   ftemp = ftemp / 8;  // wir haben maximal 8 nachbarn
  // qDebug ("-------- f = %f", ftemp);
   return ftemp;
}

//------------------------------------------------------------------------------
qreal GVanimationMaster::distance         (GVbasicElement *e1, GVbasicElement *e2 ) {
#if 0
    	qreal t1 =   (e1->fo->sV - e2->fo->sV) /2 ;
		qreal t2 =   (e1->fo->survival - e2->fo->survival)/10 ;

		if (t1<0) t1 = -t1;
		if (t2<0) t2 = -t2;

		return 0.8*t1+ 0.2*t2;
#endif
    return 0;
}

//------------------------------------------------------------------------------

qreal GVanimationMaster::genericDistance         (GVbasicElement *e1, GVbasicElement *e2 ) {
	qreal v1;
	qreal v2;
	qreal dtemp = 0;
	qreal dl = 0;
#if 0
	if ( e1->fo->stagingTType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->stagingTType->nominalValue (e1->fo->stagingTIndex);
		v2 = e2->fo->stagingTType->nominalValue (e2->fo->stagingTIndex);
		dl = (v1-v2) / e1->fo->stagingTType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->stagingTType->distanceWeight();
	}
	if ( e1->fo->stagingNType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->stagingNType->nominalValue (e1->fo->stagingNIndex);
		v2 = e2->fo->stagingNType->nominalValue (e2->fo->stagingNIndex);
		dl = (v1-v2) / e1->fo->stagingNType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->stagingNType->distanceWeight();
	}
	if ( e1->fo->stagingMType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->stagingMType->nominalValue (e1->fo->stagingMIndex);
		v2 = e2->fo->stagingMType->nominalValue (e2->fo->stagingMIndex);
		dl = (v1-v2) / e1->fo->stagingMType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->stagingMType->distanceWeight();
	}
	if ( e1->fo->gradingType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->gradingType->nominalValue (e1->fo->gradingIndex);
		v2 = e2->fo->gradingType->nominalValue (e2->fo->gradingIndex);
		dl = (v1-v2) / e1->fo->gradingType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->gradingType->distanceWeight();
	}
	if ( e1->fo->sexType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->sexType->nominalValue (e1->fo->sexIndex);
		v2 = e2->fo->sexType->nominalValue (e2->fo->sexIndex);
		dl = (v1-v2) / e1->fo->sexType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->sexType->distanceWeight();
	}
	if ( e1->fo->ageType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->ageType->nominalValue (e1->fo->ageIndex);
		v2 = e2->fo->ageType->nominalValue (e2->fo->ageIndex);
		dl = (v1-v2) / e1->fo->ageType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->ageType->distanceWeight();
	}
	if ( e1->fo->diseaseFreeSurvivalType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->diseaseFreeSurvivalType->nominalValue (e1->fo->diseaseFreeSurvivalIndex);
		v2 = e2->fo->diseaseFreeSurvivalType->nominalValue (e2->fo->diseaseFreeSurvivalIndex);
		dl = (v1-v2) / e1->fo->diseaseFreeSurvivalType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->diseaseFreeSurvivalType->distanceWeight();
	}
	if ( e1->fo->examYearType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->examYearType->nominalValue (e1->fo->examYearIndex);
		v2 = e2->fo->examYearType->nominalValue (e2->fo->examYearIndex);
		dl = (v1-v2) / e1->fo->examYearType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->examYearType->distanceWeight();
	}
	if ( e1->fo->einsenderType->isPartOfDistanceFunction() ) {
		v1 = e1->fo->einsenderType->nominalValue (e1->fo->einsenderIndex);
		v2 = e2->fo->einsenderType->nominalValue (e2->fo->einsenderIndex);
		dl = (v1-v2) / e1->fo->einsenderType->nominalRange();
		if (dl<0) dl = -dl;
		dtemp = dtemp + dl * e1->fo->einsenderType->distanceWeight();
	}
#endif
	return dtemp;
}





//------------------------------------------------------------------------------
void GVanimationMaster::run() {
	
	qDebug ("******  start animation thread");
	_f = false;
	
	long  t = 0;
	long ct = 0;
	long cycles = 10000000;
	QTime stopuhr;
	stopuhr.start();
	qreal mist = 8.0*8.0*8.0*8.0;
	int as = 0;
	int  gylphStepper = 0;	
	int nf = 0;
	int maxSpeed; 


	while (1) {
		
		
	    _m->lock();	
		
		
		if (_antClustering) {
		    if (_antparamsChanged) { 
					initAnts(_antArea);
					_antparamsChanged=false;
			}
			moveAnts ();
		}
		{
	    as=0;
		if (_f && !_antClustering) goto end;
		
		//		qDebug ("animation step %d", as++);		
		
		int w = _objectGrid->width();
		int h = _objectGrid->height();
		
		for (int ai = 0; ai < 4; ai++) {
			
			int eltm =  _objectGrid->areas[ai].elementsToMoveIn.size();
			//			if (eltm) qDebug ("area %d - %d to move", ai, eltm);
			

//#define MOVEFAST

#ifdef MOVEFAST
            for (int ei = 0; ei < _objectGrid->areas[ai].elementsToMoveIn.size() ; ei++) {
				GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[ai].elementsToMoveIn[ei]);                
                _objectGrid->moveElementTo    (e, e->x_togo, e->y_togo );
				_objectGrid->areas[ai].elementsToMoveIn.removeAt (ei);
			
			}
#else            
            
			for (int ei = 0; ei < _objectGrid->areas[ai].elementsToMoveIn.size() ; ei++) {
				GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[ai].elementsToMoveIn[ei]);
				if  ( ( e->x ==  e->x_togo ) && ( e->y ==  e->y_togo )){
					e->positionFound = true;
					__nf++;
					//				    qDebug ("FFFound - %d", __nf);
					_objectGrid->areas[ai].elementsToMoveIn.removeAt (ei);
				
               }
            }
#endif					
			for (int ei = 0; ei < _objectGrid->areas[ai].elementsToMoveIn.size(); ei++) {
			    as++;
				//				if ((ei) < (as/4+100)) {
				if (1) {
					
					//					qDebug ("as= %d  ei = %d", as, ei);
					GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[ai].elementsToMoveIn[ei]);
					
					
					maxSpeed = 24 + rand()%8;
					
					int dx =  e->x_togo - e->x;
					int dy =  e->y_togo - e->y;
					if (dx >  maxSpeed)  dx =  maxSpeed;
					if (dx < -maxSpeed)  dx = -maxSpeed;
					if (dy >  maxSpeed)  dy =  maxSpeed;
					if (dy < -maxSpeed)  dy = -maxSpeed;
					
					int xnn = e->x + dx;
					int ynn = e->y + dy;
					int x_start = e->x;
					int y_start = e->y;
					bool freePlaceFound;
					
					freePlaceFound = _objectGrid->isTherePlace (e, xnn, ynn);
					
					if  (freePlaceFound) {
						e->rc = 0;
						_objectGrid->moveElementTo    (e, xnn, ynn );
						
					}
					else {
						if ((xnn ==  e->x_togo ) && ( ynn ==  e->y_togo ) ) {
							GVbasicElement *e2 = _objectGrid->getElement   ( e->x_togo,  e->y_togo);
							if (e2) _objectGrid->swapElements (e,e2);
						}
						else {
							if (e->rc > 20) {
								int c = 0;
								int r = 1;
								while ( !_objectGrid->isTherePlace (e, xnn, ynn) )  {            
									xnn = xnn + rand()%(2*r+1) -r;
									ynn = ynn + rand()%(2*r+1) -r;
									if (xnn < 0)  xnn = 0;
									if (ynn < 0)  ynn = 0;
									if (xnn > w)  xnn = w;
									if (ynn > h)  ynn = h;
									if (c > 100 )   {c=0; r++;}
									if (r > 500)    {break;}
									c++;
								}
								if (r<500) _objectGrid->moveElementTo    (e, xnn, ynn );
							}
							else {
								e->rc++;
							}
						}			
					}
				}
				}	
			
			}
			}
		if (t++ > cycles)  {
			int d = stopuhr.restart();
//			qDebug ("animation ticker - %d for %d cyles %d msec", ct, cycles, d);
			t = 0;
			ct++;
		}
	    _m->unlock();	
		//    usleep (100*1000);
	}
end:
	for (int ai = 0; ai < 4; ai++) {
	    _objectGrid->areas[ai].elementsToMoveIn.clear();
    }
	 _m->unlock();
	qDebug ("-----------------  end of animation thread");
}



//------------------------------------------------------------------------------------
void  GVanimationMaster::setElementPositions  (int area) {
	
	int mCode = _objectGrid->areas[area].movementType;
	
	
	//qDebug ("mocde = %d", mCode);
	_antClustering = false;
	
    switch (mCode)  {	
		
		case kAntAnimtion:	{
		
			if (mCode == kAntAnimtion) {
				_antClustering = true;
				initAnts (area);
			}
		
		}
		
	    break;
		case kRandomPosition:
		{
		
			
			int x,y;
			 
			QHash <int, bool> elementIsTaken;
			for (int ei = 0; ei < _objectGrid->areas[area].elementsToMoveIn.size(); ei++) {
				
				GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[ei]);
				int hashIndex;
				int border = 20;
				_objectGrid->areas[area].cx + rand()%_objectGrid->areas[area].w;
				x = _objectGrid->areas[area].x+border + rand()%(_objectGrid->areas[area].w-2*border);
				y = _objectGrid->areas[area].y+border + rand()%(_objectGrid->areas[area].h-2*border);
				hashIndex = x * _objectGrid->areas[area].w + y;
				
				while (elementIsTaken.contains (hashIndex) ) {
					x = _objectGrid->areas[area].x+border + rand()%(_objectGrid->areas[area].w-2*border);
					y = _objectGrid->areas[area].y+border + rand()%(_objectGrid->areas[area].h-2*border);

					hashIndex = x * _objectGrid->areas[area].w + y;
				}
				elementIsTaken[hashIndex] = true;
			
				e->x_togo = x;			
				e->y_togo = y;
			}
			break;
	     }		
			
		
		case kMoveToRectange:	{
			int x,y;
			int side = sqrt (  _objectGrid->areas[area].elementsToMoveIn.size());
			int x_start = _objectGrid->areas[area].cx - side  / 2;
			int y_start = _objectGrid->areas[area].cy - side  / 2;
			int x_end   =  x_start + side;
			int y_end   =  y_start + side;


			x = x_start;
			y = y_start;
 
			for (int ei = 0; ei < _objectGrid->areas[area].elementsToMoveIn.size(); ei++) {
				GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[ei]);
				e->x_togo = x;			
				e->y_togo = y;
				if (x++ > x_end) { x = x_start; y++; }
			}
			break;
	     }	
			
		case kMoveToCenter:	{
			
			int k =  0;
			int d  = 0;
			int cm = 1;
			int c  = 0;
			int x = _objectGrid->areas[area].cx;
			int y = _objectGrid->areas[area].cy;
			
			// the center part = eien spirale	
#if 0	
			    0 1 2 3
				r d l u
				1 1 2 2
				3 3	4 4 
#endif		
				for (int ei = 0; ei < _objectGrid->areas[area].elementsToMoveIn.size(); ei++) {
					GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[ei]);
					
					e->x_togo = x;			
					e->y_togo = y;
					__nf++;
//					qDebug ("spiral point = %3d %3d", x,y);
					
					switch (d%4) {
						case 0:  x++; break;
						case 1:  y--; break;
						case 2:  x--; break;
						case 3:  y++; break;				
					}
					c++;
					if (c == cm) {
						d++;  
						c=0;
						k++;
						if (k== 2)   {cm++; k=0;}
					}		
					
				}
			
			
			break;
		}
		case kMoveToYAS_Single:
		case kMoveToYAS_Multiple:
		
		{	
#if 0
			if (mCode==kMoveToYAS_Single)   _objectGrid->cellsPerYear = _settings->defaultCellsPerYear; 					
			if (mCode==kMoveToYAS_Multiple) _objectGrid->cellsPerYear = 1;
			
			
			_objectGrid->yearsPerStripe = _settings->defaultAgeBands; 
			
			
			QHash <int, QMap <int,  int>* > maxPerYearPearAge;
			QMap  <int, int> *ageD; 
			
			int minYear =  10000;
			int maxYear = -10000;
			
			int hMax = 8 * _objectGrid->areas[area].h / 10;
			int ppa  = hMax / 120;
			
//			ppa =  std::min (1,ppa);
//			ppa =  std::max (1,ppa);
			ppa = _objectGrid->cellsPerYear;
			
			for (int ei = 0; ei < _objectGrid->areas[area].elementsToMoveIn.size(); ei++) {
				
				GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[ei]);

				int    y;
				if (mCode==kMoveToYAS_Single)    y = 1990;
				if (mCode==kMoveToYAS_Multiple)  y = e->fo->year;
				int    a = (ppa * _objectGrid->yearsPerStripe * (e->fo->age) + 0.5);
				
				if ( maxPerYearPearAge.contains (y) ) {
					ageD = maxPerYearPearAge[y]; 
				}
				else  {
					ageD = new QMap  <int, int>;
					maxPerYearPearAge[y] = ageD ;
				}		
				if ( ageD->contains (a) ) ageD->insert(a, ageD->value(a)+1); else   ageD->insert(a, 1);
				minYear = std::min (minYear, y);
				maxYear = std::max (maxYear, y);
			}
			
			QHash <int, int> maXperYear;
			
			int maxD = 0;
			for (int y = minYear; y<=maxYear; y++) {
				ageD =  maxPerYearPearAge[y];
				if (ageD) {
					QMapIterator<int, int> i(*ageD);
					while (i.hasNext()) {
						i.next();
						maxD =  std::max  (maxD,i.value());
					}
				}	
			}
			
            maxD = 2* maxD ;				
			int maxmaxD = _objectGrid->areas[area].w / (maxYear-minYear+1);
			qDebug ("minMax Year = %d , %d,   maxD = %d, maxmaxD= %d", minYear, maxYear, maxD, maxmaxD);
			
			if (maxD < maxmaxD) {
				
				
				int xmin = _objectGrid->areas[area].x + ( _objectGrid->areas[area].w - (maxYear-minYear+1) * maxD) / 2;
				int ymin = _objectGrid->areas[area].y + _objectGrid->areas[area].h / 10;
				
				qDebug ("**** xmin - ymin %d, %d", xmin, ymin);
				
				QHash <int, int> elementIsTaken;
				for (int ei = 0; ei < _objectGrid->areas[area].elementsToMoveIn.size(); ei++) {
					
					GVbasicElement *e = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[ei]);
					__nf++;
					
					
					int ageInt = (e->fo->age/_objectGrid->yearsPerStripe) + 0.5;
					
					
					ageInt = ageInt*_objectGrid->yearsPerStripe;

//					e->y_togo =  ymin + cellsPerYear * e->fo->age;
					
					
					if (_objectGrid->roundToYears)
						e->y_togo =  ymin + _objectGrid->cellsPerYear * ageInt;
					else
						e->y_togo =  ymin + _objectGrid->cellsPerYear * e->fo->age;
					
					
					if (mCode==kMoveToYAS_Single)   e->x_togo =  xmin +	maxD * (1990 - minYear) +  maxD/2; 					
					if (mCode==kMoveToYAS_Multiple) e->x_togo =  xmin +	maxD * (e->fo->year- minYear) +  maxD/2; 
					
					
					
					if (e->fo->isMale) e->x_togo --; else e->x_togo++;
					int hashIndex = e->y_togo * _objectGrid->areas[area].w + e->x_togo;
					
					int startX = e->x_togo;
					int delta;
					bool   doSort = false;
					
					while (elementIsTaken.contains (hashIndex) ) {
						
						int streifenIndex = 0;
						
						while ( elementIsTaken.contains (hashIndex) && (streifenIndex<(_objectGrid->cellsPerYear*_objectGrid->yearsPerStripe)) ) {
							streifenIndex++;
							e->y_togo =  ymin + _objectGrid->cellsPerYear * ageInt + streifenIndex;
							hashIndex = e->y_togo * _objectGrid->areas[area].w + e->x_togo;
						}
						
						if (elementIsTaken.contains (hashIndex)) {
						
						  doSort = true;
						  if (e->fo->isMale)  {
						  	 e->x_togo--;
						  	 delta = -1;
						   } 
						   else  {
							  e->x_togo++;
							  delta = +1;

						   }
						}
						hashIndex = e->y_togo * _objectGrid->areas[area].w + e->x_togo;
					}
					
					elementIsTaken[hashIndex] = ei;
					
                    doSort = 0;
					if (doSort) {
						int endX = e->x_togo;
						qDebug ("SORT from %d to %d", startX, endX);

						int nOfSwaps = 1; 
						
						while (nOfSwaps > 0) {
							nOfSwaps = 0;
							for (int xi = startX; xi< endX; xi +=delta) {
							
								int hi1 = e->y_togo * _objectGrid->areas[area].w + xi;
								int hi2 = e->y_togo * _objectGrid->areas[area].w + xi+1;
							
								GVbasicElement *e1 = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[elementIsTaken[hi1]]);
								GVbasicElement *e2 = _objectGrid->elementAt   (_objectGrid->areas[area].elementsToMoveIn[elementIsTaken[hi2]]);
								
                                int pp;
                                int sT1 = (int) e1->fo->sV;
                                pp =  (e1->fo->isDeath)?500:0;
                                int value1 = sT1 * 1000 + pp +  e1->fo->survival;

                                int sT2 = (int) e2->fo->sV;
                                pp =  (e2->fo->isDeath)?500:0;
                                int value2 = sT2 * 1000 + pp + e2->fo->survival;
                                
                                qDebug("Sorting V1=%d, V2=%d", value1, value2);
                                
								if (delta > 0) {
									if ( ( value1 > value2) ) {
										int xtemp = e1->x_togo;
										e1->x_togo = e2->x_togo;
										e2->x_togo = xtemp;
										
										int itemp = elementIsTaken[hi1];
										elementIsTaken[hi1] = elementIsTaken[hi2];
										elementIsTaken[hi2] = itemp;									
										nOfSwaps++;
									}
								}
								else
								{
									if (value1 < value2 ) {
										int xtemp = e1->x_togo;
										e1->x_togo = e2->x_togo;
										e2->x_togo = xtemp;
										
										int itemp = elementIsTaken[hi1];
										elementIsTaken[hi1] = elementIsTaken[hi2];
										elementIsTaken[hi2] = itemp;									
										nOfSwaps++;
									}								
								}
							}
							if (nOfSwaps > 0) qDebug ("nOfSwaps = %d", nOfSwaps  );
						}
						
					}
					// end of sort
					
					
				}
			}
			else
			{
				;
				
			}
			
			
			
			
			
			break;
#endif
		}

	}
	
	
	//qDebug ("**** position of %d elements fixed", __nf);
	__nf = 0;
}



//------------------------------------------------------------------------------------

void  GVanimationMaster::moveSelectedToArea  (int area) {
	
	
	qDebug ("GVmainGlVisuPlane::moveSelectedToArea  %d  cx,y = %d, %d", area, _objectGrid->areas[area].cx, _objectGrid->areas[area].cy);   
	
    _m->lock();	
    
	__nf = 0;
	
	for (int i = 0; i<4; i++) {
		_objectGrid->areas[i].elementsToMoveIn.clear();
	}
	
	if (!_objectGrid) return;
	
	
	for (int i = 0; i< _objectGrid->count(); i++) {
		if (_objectGrid->elementAt(i)->isSelected() || (_objectGrid->elementAt(i)->belongToGroup == area) ) {
			_objectGrid->elementAt(i)->belongToGroup  = area;
			_objectGrid->elementAt(i)->positionFound  = false;
			_objectGrid->elementAt(i)->fo->isSelected = true;
			_objectGrid->elementAt(i)->rc             = 0;
			_objectGrid->areas[area].elementsToMoveIn.append(i);
		}
	}


//#define SORT
#ifdef SORT



	// sort by SV value
	QMultiMap <qreal, int> tempMap;	
	for (int i = 0; i< _objectGrid->areas[area].elementsToMoveIn.count(); i++) {
		GVbasicElement *e = _objectGrid->elementAt(_objectGrid->areas[area].elementsToMoveIn[i]);
		int dvI = e->fo->sV;

        int pp  =  (e->fo->isDeath)?500:0;
    //    pp = 0;
        int sT =   (int) e->fo->sV;
        dvI = sT * 1000 + pp +  e->fo->survival;



		tempMap.insert (dvI, _objectGrid->areas[area].elementsToMoveIn[i]);
	}		

	QList<qreal> svValues = tempMap.uniqueKeys(); 
	QMultiMap <double, int> tempMapSurvival;	

	_objectGrid->areas[area].elementsToMoveIn.clear();

	for (int i = 0; i< svValues.size(); i++) {
	
	
		qreal key = svValues[i];
		tempMapSurvival.clear();
		
		QMap<qreal, int>::const_iterator iforOneKey = tempMap.find(key);
		
		 QList<int> elementsForOneKey = tempMap.values(key);

		for (int eii = 0; eii < elementsForOneKey.size(); ++eii) {
			GVbasicElement *e = _objectGrid->elementAt( elementsForOneKey.at(eii) );
		
			tempMapSurvival.insert (1000000 - 10 * e->fo->survival, elementsForOneKey.at(eii)) ;

		
		}	
		
		QMapIterator <double, int>iterSurvival (tempMapSurvival);
		while (iterSurvival.hasNext()) {
			iterSurvival.next();
			GVbasicElement *e = _objectGrid->elementAt( iterSurvival.value() );
			_objectGrid->areas[area].elementsToMoveIn.append (iterSurvival.value() );
		}
	}
#endif
	
	setElementPositions  (area);
	
	_m->unlock();
}

//------------------------------------------------------------------------------------

void  GVanimationMaster::reorderArea  ( int area) {
	//qDebug ("GVmainGlVisuPlane::reorderArea  %d", area);
	  _m->lock();	
#if 0
  
	
	for (int i = 0; i<4; i++) {
		_objectGrid->areas[i].elementsToMoveIn.clear();
	}
	
	if (!_objectGrid) return;
	
	for (int i = 0; i< _objectGrid->count(); i++) {
		if ((_objectGrid->elementAt(i)->belongToGroup == area) ) {
			_objectGrid->elementAt(i)->positionFound  = false;
			_objectGrid->elementAt(i)->rc             = 0;
			_objectGrid->areas[area].elementsToMoveIn.append(i);
		}
	}	
	
	
	// sort by SV value
	QMultiMap <qreal, int> tempMap;	
	for (int i = 0; i< _objectGrid->areas[area].elementsToMoveIn.count(); i++) {
		GVbasicElement *e = _objectGrid->elementAt(_objectGrid->areas[area].elementsToMoveIn[i]);
		int dvI = e->fo->sV;
		tempMap.insert (dvI, _objectGrid->areas[area].elementsToMoveIn[i]);
	}		

	QList<qreal> svValues = tempMap.uniqueKeys(); 
	QMultiMap <double, int> tempMapSurvival;	

	_objectGrid->areas[area].elementsToMoveIn.clear();

	for (int i = 0; i< svValues.size(); i++) {
	
	
		qreal key = svValues[i];
		tempMapSurvival.clear();
		QMap<qreal, int>::const_iterator iforOneKey = tempMap.find(key);
		
		 QList<int> elementsForOneKey = tempMap.values(key);

		for (int eii = 0; eii < elementsForOneKey.size(); ++eii) {
			GVbasicElement *e = _objectGrid->elementAt( elementsForOneKey.at(eii) );
			
			
			
			int dindex = e->fo->diedBecauseOfInitialDiagnoseIndex * 10000 + 200 - e->fo->sV;
			tempMapSurvival.insert (dindex, elementsForOneKey.at(eii)) ;
			
//			if (e->fo->isDeath)  tempMapSurvival.insert (10000 - 20 * e->fo->survival, elementsForOneKey.at(eii)) ;
//			if (!e->fo->isDeath) tempMapSurvival.insert (e->fo->sV,   elementsForOneKey.at(eii)) ;

			//			tempMapSurvival.insert (e->fo->survival, elementsForOneKey	.at(eii)) ;

		}	
		
		QMapIterator <double, int>iterSurvival (tempMapSurvival);
		while (iterSurvival.hasNext()) {
			iterSurvival.next();
			GVbasicElement *e = _objectGrid->elementAt( iterSurvival.value() );
			_objectGrid->areas[area].elementsToMoveIn.append (iterSurvival.value() );
		}
	}
#endif
	setElementPositions  (area);
	_m->unlock();
}





#if 0





///------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
int GVmainGlVisuPlane::sortElementsInYears(int ymin, int ymax, int xmin, int xmax, int xdelta) {
	
	GVsimpleObjectFindings *so;
	GVsimpleObjectFindings *soLEFT;
	GVsimpleObjectFindings *soRIGHT;
	
	GVsimpleObjectFindings *soLEFT1;
	GVsimpleObjectFindings *soLEFT2;
	GVsimpleObjectFindings *soLEFT3;
	
	
	GVsimpleObjectFindings *soRIGHT1;
	GVsimpleObjectFindings *soRIGHT2;
	GVsimpleObjectFindings *soRIGHT3;
	
	QString loc;  
	
	
	int swapCount = 0;
	if (objectGrid->count() == 0) return 0;
	
	
	for (int xo = xmin; xo < xmax; xo = xo + xdelta){
		
		for (int yi = ymin; yi< ymax; yi++) {
			for (int xi = xo+1; xi< xo+xdelta/2+1; xi++) {
				
				so =   (GVsimpleObjectFindings*) objectGrid->getElement(xi,yi); 
				if (so) {
					QList <GVbasicElement*> neighbors =  objectGrid->leftNeighbors (so);
					
					int cId = so->recordID;
					bool doSwap = true;
					for (int i = 0; i< neighbors.count(); i++) {
						int nId = neighbors[i]->recordID;
						float stl = findings->el.value(cId)->sV;
						float str = findings->el.value(nId)->sV;

#if 0						
						int yl = findings->el.value(cId)->year;
						int yr = findings->el.value(nId)->year;
						
						if (yl!=yr) doSwap = false;
#endif						
						if (doSwap && (str > stl)) {
							objectGrid->swapElements (so,neighbors[i]);
							doSwap = false;
							swapCount++;
						}
					}
				}
			}
			for (int xi = xo; xi>xo-xdelta/2-1; xi--) {
				
				so =   (GVsimpleObjectFindings*) objectGrid->getElement(xi,yi); 
				if (so) {
					QList <GVbasicElement*> neighbors =  objectGrid->leftNeighbors (so);
					
					int cId = so->recordID;
					bool doSwap = true;
					for (int i = 0; i< neighbors.count(); i++) {
						int nId = neighbors[i]->recordID;
						float stl = findings->el.value(cId)->sV;
						float str = findings->el.value(nId)->sV;
						
						int yl = findings->el.value(cId)->year;
						int yr = findings->el.value(nId)->year;
						
						if (yl!=yr) doSwap = false;
						if (doSwap && (str < stl)) {
							objectGrid->swapElements (so,neighbors[i]);
							doSwap = false;
							swapCount++;
						}
					}
				}
			}
			
		}
	}
	// qDebug ("ANT ANIM %d swaps", swapCount);        
	return swapCount;
	
}


//
sort elements
int index = 0;

QList <bool> lr;
bool  v = true;
for (int i = 0; i< _objectGrid->width(); i++) {
	lr.append (v);
}


QList < GVsimpleObjectFindings *> objects;

#if 1   
for (int i = 0; i< findings->numberOfRecords; i++) {
	
	int     year  =   findings->el.value(i)->year;
	int     month =   findings->el.value(i)->month;
	double  age   =   findings->el.value(i)->age;
	
	//      qDebug ("VISU PLANE - setup elements %d - year %d, mont=%d age = %f", i, year, month, age);
	// RR_CONFIG   
	
	const int MINYEAR  = 1984;    
	const int MAXYEAR  = 2005;    
	
	//       const int MINYEAR  = 1990;    
	//	   const int MAXYEAR  = 1995;    
	
	const int BORDER   = 20;    
	
	TREESIZE = (objectGrid->w- 2 * BORDER)   / (MAXYEAR - MINYEAR+1) / 2;
	
	
	ymin =     objectGrid->h/4;
	ymax =  3* objectGrid->h/4;
	xmin =  BORDER   + TREESIZE;
	xmax =  objectGrid->w -  BORDER;
	
	
	if (( year >= MINYEAR) && (year <= MAXYEAR))  {
		
		
		GVsimpleObjectFindings *so = new GVsimpleObjectFindings (gmaster, findings, cx);
		
		so->objectGrid->h = i;
		so->w = 1;
		so->h = 1;
		
		// RR_CONFIG 
		//		   hier wird die initiale Verteilung der Elemente bestimmt
		
		double agedelta = double(  rand()%400 / 400.0);
		
		so->y =  ymin  +  (int) (   (double)(objectGrid->h ) / 2 *  double (age + agedelta) / 160.0     );
		so->x =  BORDER + TREESIZE + 2 * TREESIZE *(year - MINYEAR );
		
		//		   qDebug ("adding a element for a %s", findings->el.value(i)->isMale?"m‰nnlich":"weiblich");
		if (findings->el.value(i)->isMale) so->x--; else so->x++;
		
		if (so->x > objectGrid->w  -1) so->x = objectGrid->w  -1;
		
		
		int leftOrRight = rand()%2;
		int upOrDown    = rand()%2;
		
		//            so->generateOrUpdateObject (i);
		int xstart = so->x;
		
		
		if (! (objectGrid->isTherePlace (so, so->x, so->y))) {
#if 1
			while (! (objectGrid->isTherePlace (so, so->x, so->y))) {
				
				//				   if (lr[so->y]) so->x++; else so->x--;
				
				if (findings->el.value(i)->isMale) so->x--; else so->x++;
				if ((so->x > (xstart + TREESIZE)) || (so->x < (xstart -  TREESIZE)) ) { 
					so->x  = xstart;
					if (upOrDown)  so->y++; else so->y--;    
					if (so->y < 0)            so->y = 0;
					if (so->y > objectGrid->h  -1) so->y = objectGrid->h -1;
					//                       qDebug ("UPDOWN for %d,%d (ae %f, year %d)",  so->x, so->y, age, year );
					
				}               
			}
			//     lr[so->y] = !lr[so->y];
			
#endif
		}
		
		//          qDebug ("ok for %d", index);
		
		objects.append (so);
		objectGrid->setElement (so);
		
		findings->el.value(i)->x = so->x;
		findings->el.value(i)->y = so->y;
		index++;
		
	}     
	
}
#endif   
#if 0  


for (int i = 0; i< 20; i++) {
	GVsimpleObjectFindings *so = new GVsimpleObjectFindings (gmaster, findings, cx);
	findings->el.value(i)->sV = i/10 + 1;
	objects.append (so);	
}
#if 0   
objects[0]->x = 0;
objects[0]->y = 0;

objects[1]->x = 1;
objects[1]->y = 0;

objects[2]->x = 2;
objects[2]->y = 0;

objects[3]->x = 3;
objects[3]->y = 0;

objects[4]->x = 4;
objects[4]->y = 0;

objects[5]->x = 5;
objects[5]->y = 0;

objects[6]->x = 6;
objects[6]->y = 0;

objects[7]->x = 7;
objects[7]->y = 0;


objects[8]->x =  8;
objects[8]->y =  0;

objects[9]->x = 9;
objects[9]->y = 0;
#endif
for (int i = 0; i< 10; i++) {
	objects[i]->x = 0;
	objects[i]->y = i;
}
for (int i = 10; i< 20; i++) {
	objects[i]->x = 2;
	objects[i]->y = i-10;
}


for (int i = 0; i<19; i++) {
	objectGrid->setElement (objects[i]);
}
#if 0   
QList <GVbasicElement*> neighbors =  objectGrid->rightNeighbors (objects[0]);

qDebug (" %d neighbors found", neighbors.count());


for (int i = 0; i< neighbors.count(); i++) {
	objectGrid->moveElementTo(neighbors[i],neighbors[i]->x, neighbors[i]->y+8);
}


int ny = neighbors[0]->y+1;

for (int i = 0; i< 9; i++) {
	
	while ( !objectGrid->isTherePlace (neighbors[0], neighbors[0]->x, ny) )  {            
		ny++;
	}
	objectGrid->moveElementTo(neighbors[0], neighbors[0]->x, ny);
}

#endif   

#endif

numberOfObjects = objectGrid->count(); 
currentObject = 0;
updateObjectTexture();

#if 0


// RR_CONFIG 
//      hier werden die Elemente innerhalb des Jahrs sortiert, kleine Stagings Innen


int sortcount = 999;

while (sortcount > 0) {
	sortcount = sortElementsInYears (ymin, ymax, xmin, xmax, 2*TREESIZE);
	qDebug ("YEAR SORT with  %d swaps", sortcount);
}
#endif

// store the initial positions

for (int i = 0; i<objects.count(); i++) {
	objects[i]->x_i = objects[i]->x;
	objects[i]->y_i = objects[i]->y;
}


qDebug ("END OF initializeGL %d elements", numberOfObjects);

// end sort elements


#endif


