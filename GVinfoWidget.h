/*
 *  GVinfoWidget.h
 *  diseaseViewer
 *
 *  Created by mue on 22.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVinfoWidget_H
#define GVinfoWidget_H

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QTextEdit>

#include  "GLmaster.h"
#include  "GVsimpleObjectFindingsglDisplay.h"
#include  "GVfindings.h"


class GVinfoWidget: public QWidget
{
    Q_OBJECT
    
public:
    
    GVinfoWidget ( QWidget *parent, GVfindings  *findings);
    
public slots:        

	void setSelectedElement (int v);
	void clear              ();

signals:
        

private:
        
	QTextEdit    *_textOutput;
    GVfindings   *_findings;  
    
};

#endif