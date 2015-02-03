/*
 *  window.h
 *  paintTest
 *
 *  Created by mue on 11.11.05.
 *  Copyright 2005 __hmmc__. All rights reserved.
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QAction>

#include  <QObject>
#include  <QStatusBar>

#include  "GLmaster.h"
#include  "GVfindings.h"

#include  "GVsettings.h"
#include  "GVoptionsDialog.h"

#include "GVinfoWidget.h"

#include  "GVselectionMaster.h"
#include  "GVmainGlVisuPlane.h"
#include  "GVelementViewFindings.h"
#include  "GVmainToolbar.h"
#include  "GVanimationMaster.h"



class QSlider;
class GVmainGlVisuPlane;

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
 
	Window();
    
public slots:	

	void setNumberOfSelectedElements (int n);
	void moveSelectedToArea();


	void open ();
	void read ();	
	void setMaxElements (int n);
	
	void defineWhere      (const QString & v);
	void defineSearchTerm (const QString & v);

	void setAreaMovementStrategie (int a, int s);
    void updateAreaMovementStrategie();
	
	void editSettings ();	
	
private:
 
    GVsettings						  *_settings;
 
 	QString							   whereStatement;

	QString							   searchTerm;
	enum SERACHFIELD 				   {TypologieID, Diagnose};	
	
	
	int							       maxNumberOfElements;
 
 
	QString								_lastUsedDirectory;


	QAction					           *exitA;
    QAction					           *openA;
    QAction					           *readA;
	
    QAction					           *saveToFileA;
	
	QAction					           *editSettingsA;

 	QAction					           *selectAllA;
 
    QAction					           *viewAll;
    QAction					           *viewSelected;
    QAction					           *viewFull;
    
    
 
	QList<QAction *>				   moveSelectedToAreaA;	 
	QList<QAction *>				   removeSelectedFromAreaA;	
 
    GLmaster                           *gmaster;    
    GVfindings                         *findings; 
	
	GVisoGrid				           *objectGrid;
	
    GVmainGlVisuPlane                  *glView;
    GVelementViewFindings              *elementView;


	GVmainToolbar					   *mainToolBarWidget;
	QDockWidget                        *mainToolBarDock;
	
	GVselectionMaster                  *selectionWidget;
	QDockWidget                        *selectionDock;

	GVinfoWidget					   *infoWidget;
	QDockWidget                        *infoDock;

	GVanimationMaster				   *animationMaster;	
	QMutex							   *m;
	QMutex							   *rm;
	QMutex							   *glm;
	
	QStatusBar						   *myStatusBar;
	
	QHash<int, int>                    areaMovementStrategie;
};

#endif
