/*
 *  zoomWindow.cpp
 *  paintTest
 *
 *  Created by mue on 10.11.05.
 *  Copyright 2005 __hmmc__. All rights reserved.
 *
 */

#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>


#include "math.h"
#include "GVmainGlVisuPlane.h"
#include "GVsettings.h"

#include "window.h"
// RR_CONFIG   
// bei ca. 12000 F‰llen (mamma) ist eine Gridsize von 500 ideal
#define     GRIDSIZE 1000


Window::Window()
{
	
    setWindowTitle(tr("GENVIEW - Disease Viewer"));
	
	_settings = new GVsettings ();	

	_settings->numberOfAnts =  10;
	_settings->antMinSpeed  =   1;
	_settings->antMaxSpeed  =  50;
	_settings->antK1		=  0.1;
	_settings->antK2		=  0.15;

	_settings->defaultCellsPerYear		=  3;
	_settings->defaultAgeBands          =  5;
	
	_settings->flipPBuffer   =  0;
	
	myStatusBar = statusBar();
    gmaster               = new GLmaster   ();
	//qDebug ("01 - init the findings object");
	findings              = new GVfindings ();       
	
	connect(findings, SIGNAL( showMessage(QString, int)), myStatusBar,    SLOT ( showMessage(QString, int)));
	
	m    = new QMutex();
	rm   = new QMutex();
	
	objectGrid = new GVisoGrid (rm, 9, 9);
	
	animationMaster = new GVanimationMaster (_settings, m);
	animationMaster->setDataGrid (objectGrid, findings); 
	
	glView          = new GVmainGlVisuPlane      ( _settings, this, gmaster, findings, objectGrid, m);    	
	
	connect(glView, SIGNAL( showMessage(QString, int)), myStatusBar,    SLOT ( showMessage(QString, int)));
	
	
	setCentralWidget (glView);
	
	//	make all connectios
	
	selectionWidget        = new GVselectionMaster (this, findings);
	selectionDock          = new QDockWidget (tr("Control Panel"), this);
    selectionDock->setAllowedAreas  ( Qt::BottomDockWidgetArea);
    selectionDock->setFeatures      ((QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
	
 	selectionDock->setWidget        ( selectionWidget);
	addDockWidget                   ( Qt::BottomDockWidgetArea, selectionDock);
	connect(selectionWidget, SIGNAL(showMessage(QString, int)), myStatusBar,    SLOT ( showMessage(QString, int)));
	connect(selectionWidget, SIGNAL(setNumberOfSelectedElements(int)), this,    SLOT ( setNumberOfSelectedElements(int)));
    
	
	connect(glView,              SIGNAL(setManualSelection(QList <long>)),   selectionWidget,    SLOT ( setManualSelection(QList <long> )));
	connect(glView,              SIGNAL(reportObject (int)),                 selectionWidget,    SLOT ( hightLightCurrentValue (int)) );
	connect(animationMaster,     SIGNAL(updateDistributions ()),             selectionWidget,    SLOT ( selectAll()));
	
	
	infoWidget = new GVinfoWidget (this, findings);
	infoDock = new QDockWidget    (tr("Info View"), this);
    infoDock->setAllowedAreas     ( Qt::BottomDockWidgetArea);
    infoDock->setFeatures         ( (QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
 	infoDock->setWidget           ( infoWidget);
	addDockWidget                 ( Qt::BottomDockWidgetArea, infoDock);
    connect(glView, SIGNAL(reportObject (int)),       infoWidget, SLOT ( setSelectedElement (int)) );
	
	
	myStatusBar->showMessage ("hello");
	
	//qDebug ("04 -selection widget initialzed");
	
	show();
	
	readA = new QAction (tr("Read Elements"), this);
	readA->setShortcut(tr("Ctrl+R"));
	connect(readA, SIGNAL(triggered()), this, SLOT(read()));
	
	exitA = new QAction (tr("Quit"), this);
	exitA->setShortcut(tr("Ctrl+Q"));
	connect(exitA, SIGNAL(triggered()), this, SLOT(exit()));
	
	
	
	editSettingsA  = new QAction ("Preferences", this);
	editSettingsA->setEnabled (true);
	connect        ( editSettingsA, SIGNAL(triggered()), this, SLOT(editSettings()));



	
	
	QMenu *fileMenu = menuBar()->addMenu ("File");
	QMenu *viewMenu = menuBar()->addMenu ("View");
	QMenu *selectionMenu = menuBar()->addMenu ("Selection");
	QMenu *sortMenu = menuBar()->addMenu ("Sorting");
	
	
	
	saveToFileA = new QAction (tr("Save Screenshot"), this);
	saveToFileA->setShortcut(tr("Ctrl+S"));
	connect(saveToFileA, SIGNAL(triggered()), glView, SLOT(saveToFile()));

	connect(glView, SIGNAL( showMessage(QString, int)), myStatusBar,    SLOT ( showMessage(QString, int)));
	
	
	fileMenu->addAction(openA);
	fileMenu->addAction(readA);
	fileMenu->addAction(editSettingsA);
	fileMenu->addAction(saveToFileA);

	fileMenu->addSeparator();
	fileMenu->addAction(exitA);
		
	
	
	viewAll = new QAction (tr("View All"), this);
	viewAll->setShortcut(tr("A"));
	connect(viewAll, SIGNAL(triggered()), glView, SLOT(zoomAllElements()));
	
	viewSelected = new QAction (tr("View Selected Elements"), this);
	viewSelected->setShortcut(tr("S"));
	connect(viewSelected, SIGNAL(triggered()), glView, SLOT(zoomSelectedElements()));
	
	viewFull = new QAction (tr("View Full Plane"), this);
	viewFull->setShortcut(tr("F"));
	connect(viewFull, SIGNAL(triggered()), glView, SLOT(zoomFull()));
	
	
	
	selectAllA = new QAction (tr("Select All"), this);
	selectAllA->setShortcut(tr("Ctrl+A"));
	connect(selectAllA, SIGNAL(triggered()), selectionWidget, SLOT(selectAll()));
	
	

	for (int i = 0; i<4; i++) {
	    QString actionName = "Move Selecet Objects to Area " + QString::number (i);
		QAction *ta = new QAction (actionName, this);
	 	ta->setData(i);
		ta->setVisible(true);
		connect(ta, SIGNAL(triggered()), this, SLOT(moveSelectedToArea()));
	    sortMenu->addAction(ta);
		moveSelectedToAreaA.append (ta);
	}	
	
	
	
	
	mainToolBarWidget	= new GVmainToolbar (this, moveSelectedToAreaA);
	mainToolBarDock     = new QDockWidget (this);
    mainToolBarDock->setAllowedAreas  ( Qt::TopDockWidgetArea);
    mainToolBarDock->setFeatures      ((QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
	mainToolBarDock->setWidget        ( mainToolBarWidget);
	addDockWidget                         ( Qt::TopDockWidgetArea, mainToolBarDock);
	
	
	connect(mainToolBarWidget, SIGNAL(setMaxElementsS(int)), this, SLOT(setMaxElements(int)));
	
	connect(mainToolBarWidget, SIGNAL(open()), this, SLOT(open()));
	connect(mainToolBarWidget, SIGNAL(read()), this, SLOT(read()));
	connect(mainToolBarWidget, SIGNAL(defineWhere(const QString &)), this, SLOT(defineWhere(const QString &)));	
	connect(mainToolBarWidget, SIGNAL(setAreaMovementStrategie(int, int)), this, SLOT(setAreaMovementStrategie(int,int)));


	connect(mainToolBarWidget, SIGNAL(search(const QString &)), selectionWidget, SLOT(search(const QString &)));	

	mainToolBarWidget->updateAreaMovementStrategieInMainWindow();		

	
	
    viewMenu->addAction(viewFull);
    viewMenu->addAction(viewAll);
    viewMenu->addAction(viewSelected);
	
	selectionMenu->addAction(selectAllA);
	
	
	mainToolBarWidget->enableReadButton   (false);
	mainToolBarWidget->enableSelectinMenu (false);
	selectionWidget->setEnabled (false);
}



void Window::moveSelectedToArea()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		animationMaster->moveSelectedToArea ( action->data().toInt() );
}





void Window::setNumberOfSelectedElements (int n) {
	qDebug ("%s", qPrintable (QString::number(n) + " elements selected"));
	selectionDock->setWindowTitle (QString::number(n) + " elements selected");
}

void  Window::setMaxElements (int n) {
	maxNumberOfElements = n;
}


void  Window::open () {
	qDebug ("GVmainToolbar::open");
	
	
	QString s;
	mainToolBarWidget->enableReadButton (true);
}

void  Window::read () {
	
	//qDebug ("Window::read <<%s>> max %d elements", qPrintable (whereStatement), maxNumberOfElements);

	
	
	if (animationMaster) { 
		animationMaster->finish();
		while ( animationMaster->isRunning () ) {
		   ;
		}
	}
	
	if (findings->numberOfRecords > 0) findings->clearFindings();
	glView->update ();	
	glView->resetFindingsToUpdate();
	selectionWidget->setEnabled (false);
	selectionWidget->updateSelectionState ();
	infoWidget->clear();
	
	
	QCoreApplication::processEvents (QEventLoop::AllEvents, 100);
    
    
    findings->addFindings        ();	

	
	qDebug ("findings added");
	
	int side = 25 * sqrt (findings->numberOfRecords);
	objectGrid->resetAndSetSize (side, side);
	updateAreaMovementStrategie();
	
	glView->resizeView         ();	
	glView->zoomFull           ();	
	glView->generateGlyphs     ();
	selectionWidget->selectAll ();	

	animationMaster->reorderArea (0);
	mainToolBarWidget->enableSelectinMenu (true);
	selectionWidget->setEnabled (true);
	selectionWidget->updateSelectionState ();

	animationMaster->start();
	
}	


void  Window::defineWhere (const QString & v) {
	qDebug ("Window::setWhereString  %s", qPrintable (v));
	whereStatement = v;
}

void  Window::defineSearchTerm (const QString & v) {
	qDebug ("Window::searchTerm =  %s", qPrintable (v));
	searchTerm = v;
}

void  Window::setAreaMovementStrategie (int a, int s) {
	areaMovementStrategie[a] = s; 
	updateAreaMovementStrategie();
	animationMaster->reorderArea (a);
	//qDebug ("Window::setAreaMovementStrategie REORDER");
}

void  Window::updateAreaMovementStrategie() {

	if (!objectGrid) return;
	QHashIterator<int, int> i(areaMovementStrategie);
	while (i.hasNext()) {
		i.next();
		objectGrid->areas[i.key()].movementType = i.value();
	}
}


void  Window::editSettings() {
	GVoptionsDialog  *optionsDialog = new GVoptionsDialog(_settings, this);
	connect(optionsDialog, SIGNAL(antSettingsChanged()), animationMaster, SLOT(antSettingsChanged ()));
    optionsDialog->show();
}




