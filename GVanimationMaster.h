/*
 *  GVanimationMaster.h
 *  diseaseViewer
 *
 *  Created by mue on 24.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GVanimationMaster_H
#define GVanimationMaster_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QHash>


#include "GVglobals.h"
#include "GVsettings.h"
#include "GVisoGrid.h"

class GVfindings;

class GVanimationMaster : public QThread
{
    Q_OBJECT
    
public:

    GVanimationMaster     ( GVsettings *settings,
							QMutex *m);
							    	
	void setDataGrid      ( GVisoGrid       *objectGrid,
                            GVfindings      *findings);
    
	
	void addNewElement      ( long findingRecord);
    void allElementsAdded   ();
	void moveSelectedToArea      ( int area);
	void reorderArea             ( int area);
	
	void finish()    {_f=true;}


public slots:

	void antSettingsChanged ();

signals:

	void updateDistributions();
	void updateGlyphs();

protected:
	
	void run();
	    
private:

	void  setElementPositions  (int area);
	void  initAnts             (int area); 
	void  moveAnts             ();
	qreal ffunc                (GVbasicElement *e,  int x, int y);
	qreal distance             (GVbasicElement *e1, GVbasicElement *e2 ) ;
	qreal genericDistance      (GVbasicElement *e1, GVbasicElement *e2 ) ;

	QList <long>    elementStack;
	QList <QPoint> freepositions;    
	QHash <int, bool>    fpHash;    


    GVsettings       *_settings;


	bool		   _antClustering;
    int            _antArea; 
	int            _numberOfAnts;
	int            __aCount;
	
	bool         _antparamsChanged;
	
	int _antminSpeed;
	int _antmaxSpeed;
	double _antspeedDelta;
	
	QList <QPoint>             antpositions; 
	QList <GVbasicElement *>   antelements; 

	QGLWidget	*_shareWidget; 
	QGLContext	*_cx;


    int          __nf;

	bool          _f;
	bool		  _readyToGenerateGLelements;

	QMutex		 *_m;

	GVisoGrid    *_objectGrid;
	GVfindings   *_findings;
	
	int          _noE;
 };

#endif
