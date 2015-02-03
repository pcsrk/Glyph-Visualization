/*
 *  GVoptionsDialog.h
 *  dv
 *
 *  Created by mue on 09.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVoptionsDialog_H__
#define GVoptionsDialog_H__


#include <QObject>
#include <QDialog>
#include <QSizeGrip>
#include <QTableWidget>
#include "GVsettings.h"


class GVoptionsDialog : public QWidget
{
    Q_OBJECT
    
public:
    
    GVoptionsDialog  ( GVsettings    *settings,   
                       QWidget        *parent = 0);
    
    QSize minimumSizeHint() const;
    
    QSize sizeHint() const;
    
  
protected slots:

    void  setNumberOfAnts           ( double v);
    void  setMinAntSpeed            ( double v);	
    void  setMaxAntSpeed            ( double v);
    void  setAntK1					( double v);
    void  setAntK2					( double v);

	void  setDefaultCellsPerYear	( double v);
	void  setDefaultAgeBand     	( double v);

	
	
    void  setflipPBuffer			( int v);
		
//    void  close();
//    void  resizeEvent  ( QResizeEvent * event );

    private slots:
        
signals: 

	void antSettingsChanged();    

private:
        
    GVsettings       *_settings;
	
};

#endif