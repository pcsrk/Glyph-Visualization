/*
 *  GVselectionMaster.cpp
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <QHBoxLayout>
#include <QPalette>
#include <QFileDialog>
#include <QTextStream>




#include "GVselectionMaster.h"
#include "GVfindingsObject.h"


GVselectionMaster::GVselectionMaster(QWidget      *parent, 
                                     GVfindings   *findings) : QWidget(parent) {
									 
	_findings = findings;
									 								 
// set up all selection widgets
    foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
        attributes[attributeType->name()] = attributeType;
    }
    
	QHBoxLayout *layout = new QHBoxLayout;

	QMapIterator<QString, GVattributeType*> i(attributes);
	while (i.hasNext()) {
		i.next(); 
		GVattributeSelectionWidget *sw = new GVattributeSelectionWidget (i.value(), this);
		layout->addWidget (sw);
		connect(sw,   SIGNAL(updateSelection()),         this, SLOT   (updateSelectionState()) );
		connect(sw,   SIGNAL(showMessage(QString, int)), this, SIGNAL (showMessage(QString, int))); 
		selectionWidgets[i.key()] = sw;
	}
    
	layout->setSpacing (10);
    setLayout(layout);		

	updateSelectionState ();																																																		
}







//------------------------------------------------------------------------------
void GVselectionMaster::setManualSelection (QList <long> list) {


	setNumberOfSelectedElements (list.size());

	long fn = _findings->numberOfRecords;
	for (long i = 0; i< fn; i++) _findings->el[i]->isSelected = false;	   

	QMapIterator<QString, GVattributeType*> attributeIterator (attributes);
	while (attributeIterator.hasNext()) {
		attributeIterator.next();
		attributeIterator.value()->setEmptyIndexRange();
		attributeIterator.value()->resetDistributionS();
	}
	
	for (long i= 0; i<list.size(); i++) {
		long index = list[i];
		GVfindingsObject *fo = _findings->el [index];
	    fo->isSelected = true;	 
	    fo->updateAttributeTypeIndex();
	}


	for (long i= 0; i<list.size(); i++) {
		long index = list[i];
		GVfindingsObject *fo = _findings->el [index];
        
        
#if 0
		GVfindingsObject::stagingTType->incDistributionS(fo->stagingTIndex);
		GVfindingsObject::stagingNType->incDistributionS(fo->stagingNIndex);		
		GVfindingsObject::stagingMType->incDistributionS(fo->stagingMIndex);		
     	GVfindingsObject::gradingType->incDistributionS(fo->gradingIndex);
		GVfindingsObject::ageType->incDistributionS(fo->ageIndex);
		GVfindingsObject::sexType->incDistributionS(fo->sexIndex);
		GVfindingsObject::diseaseFreeSurvivalType->incDistributionS(fo->diseaseFreeSurvivalIndex);
		GVfindingsObject::examYearType->incDistributionS(fo->examYearIndex);
        GVfindingsObject::einsenderType->incDistributionS (fo->einsenderIndex);
		GVfindingsObject::diedBecauseOfInitialDiagnoseType->incDistributionS(fo->diedBecauseOfInitialDiagnoseIndex);
#endif
	}


	updateDistributions();

	
	QMapIterator<QString, GVattributeSelectionWidget*> i(selectionWidgets);
	while (i.hasNext()) {
		i.next();
		i.value()->update();
	}
}



//------------------------------------------------------------------------------
void GVselectionMaster::selectAll () {

	long fn = _findings->numberOfRecords;
	for (long i = 0; i< fn; i++) _findings->el[i]->isSelected = true;	   

	updateDistributions();
	
 	showMessage (QString::number (fn) + " elements selected", 0);
	setNumberOfSelectedElements (fn);

	QMapIterator<QString, GVattributeType*> attributeIterator (attributes);
	while (attributeIterator.hasNext()) {
		attributeIterator.next();
		attributeIterator.value()->resetIndexRange();
	}

	QMapIterator<QString, GVattributeSelectionWidget*> i(selectionWidgets);
	while (i.hasNext()) {
		i.next();
		i.value()->update();
	}
}


//------------------------------------------------------------------------------
void GVselectionMaster::updateSelectionState () {

	long sn = 0;
	long fn = _findings->numberOfRecords;
	
	for (long i = 0; i< fn; i++) {
		GVfindingsObject *fo = _findings->el [i];	   
		int sel =  fo->updateSelectionState();
		sn += sel;
	}
	
		
 	showMessage (QString::number (sn) + " elements selected", 0);
	setNumberOfSelectedElements (sn);
		
	updateDistributions();
	
	QMapIterator<QString, GVattributeSelectionWidget*> i(selectionWidgets);
	while (i.hasNext()) {
		i.next();
		i.value()->update();
	}
}



//------------------------------------------------------------------------------
void GVselectionMaster::hightLightCurrentValue (int index) {
    if (index >= -1) {
		GVfindingsObject *fo = _findings->el [index];
        foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
            selectionWidgets[attributeType->name()]->highlightValue(fo->value[attributeType->name()]);
        }
        
	}
	else
	{
        foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
            selectionWidgets[attributeType->name()]->highlightValue(-99);
            
        }
	}
    
}


//------------------------------------------------------------------------------
void GVselectionMaster::updateDistributions () {


	QMapIterator<QString, GVattributeType*> attributeIterator (attributes);

	while (attributeIterator.hasNext()) {
		attributeIterator.next();
		attributeIterator.value()->resetDistribution();
		attributeIterator.value()->resetDistributionS();
	}


	if (!_findings) return;
	
	long s  = 0;	
	long fn = _findings->numberOfRecords;

	for (long i = 0; i< fn; i++) {
		GVfindingsObject *fo = _findings->el [i];

        foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
            attributeType->incDistribution(fo->value[attributeType->name()]);
        }
		if (fo->isSelected ) {
            foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
                attributeType->incDistributionS(fo->value[attributeType->name()]);
            }
		}
		s++;
	}
}


//------------------------------------------------------------------------------
QSize GVselectionMaster::minimumSizeHint() const {
	return QSize (400,100);
}

//------------------------------------------------------------------------------
QSize GVselectionMaster::sizeHint() const {
	return QSize (800,200);
	
}

