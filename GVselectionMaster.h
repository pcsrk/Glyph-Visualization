/*
 *  GVselectionMaster.h
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GVselectionMaster_H
#define GVselectionMaster_H

#include <QWidget>

#include "GVattributeSelectionWidget.h"
#include "GVfindings.h"

class GVselectionMaster : public QWidget
{
    Q_OBJECT
    
public:
    GVselectionMaster (QWidget       *parent   = 0,
                       GVfindings    *findings = 0);
    
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:


	void updateDistributions ();
	void updateSelectionState ();
	void hightLightCurrentValue (int index);
	void setManualSelection (QList <long> list);
	void selectAll ();    


signals:

	void showMessage(QString m, int t);
	void setNumberOfSelectedElements (int sn);

protected:
    
private:

	GVfindings   *_findings;

	QMap <QString, GVattributeType*> attributes;
	QMap <QString, GVattributeSelectionWidget*> selectionWidgets;


 };

#endif
