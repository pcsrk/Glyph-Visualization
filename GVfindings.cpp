/*
 *  GVfindings.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVfindings.h"
#include "GVanimationMaster.h"

#include <iostream>

#include "xlsxdocument.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDate>

#include <QMessageBox>




#define lokal 0

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
GVfindings::GVfindings() {
    
    qDebug ("HELLO in GVfindings::GVfindings");
    QXlsx::Document xlsx("/Users/Shared/SWD/GlyphVisualization/config.xlsx");
    
    QStringList attributeNames = xlsx.sheetNames();

    foreach (QString sheetName, attributeNames){
        qDebug ("****  %s *****",qPrintable(sheetName));
        
        GVattributeType *attributeType =new GVattributeType (sheetName);
        xlsx.selectSheet(sheetName);
        QString attributeT = xlsx.read("B2").toString();
        
        QString visuMethod = xlsx.read("B4").toString();
        if (visuMethod.length() > 0) {
            GVfindingsObject::visu2attribute[visuMethod] = sheetName;
        }
        
        if (QString::compare(attributeT, "ORD", Qt::CaseInsensitive) == 0) {
            int n = xlsx.read("B5").toInt();
            for (int i = 0; i<n; i++) {
                QString cellV = "C" + QString::number(i+2);
                QString cellI = "D" + QString::number(i+2);
                QString value = xlsx.read(cellV).toString();
                float   index = xlsx.read(cellI).toDouble();
                attributeType->appendValue (value, index);
               
                qDebug ("Value / index %s = %f",qPrintable(value), index);
            }
            attributeType->resetIndexRange();
            GVfindingsObject::GVattributes.append (attributeType);
        }
    }
}


//------------------------------------------------------------------------------
void GVfindings::addFindings    () {
	
    
    QXlsx::Document xlsx("/Users/Shared/SWD/GlyphVisualization/data.xlsx");
    
    int n = GVfindingsObject::GVattributes.length();
    QMap<int,GVattributeType *> index2attribute;
    int atfound = 0;
    for (int i=0; i<n; i++) {
        QString attributeName = xlsx.read(1,i+1).toString();
        foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
            if (QString::compare(attributeType->name(), attributeName, Qt::CaseInsensitive) == 0) {
                index2attribute[i] = attributeType;
                atfound++;
            }
        }
    }

    if (atfound != n) {
        QMessageBox::critical(0, "Critical", "Data Format Error");
        
    }
    
    int nr = 0;
    for (int k= 1;k<100;k++) {
        int li = 1;
        while (1) {
            GVfindingsObject *finding = new  GVfindingsObject();
            int sum_length = 0;
            for (int i=0; i<n; i++) {
                QString value = xlsx.read(li,i+1).toString();
                GVattributeType *attributeType = index2attribute[i];
                finding->value[attributeType->name()] = attributeType->indexOf(value);
//                qDebug ("%s <= %s, %d", qPrintable(attributeType->name()),   qPrintable(value),  attributeType->indexOf(value) );
                sum_length += value.length();
            }
            if (sum_length==0) break;
            el.append (finding);
            nr++;
            li++;
            QCoreApplication::processEvents (QEventLoop::AllEvents, 5);
        }
    }
	numberOfRecords  = nr;
	qDebug ("(3) %d records read", numberOfRecords);		
}  


//------------------------------------------------------------------------------
void GVfindings::clearFindings () {

    for (int i= 0; i<el.size(); i++) {
		GVfindingsObject *f= el[i];
		delete (f);	
	}
	el.clear();
	eh.clear();	
	numberOfRecords = 0;
}

//------------------------------------------------------------------------------
GVfindings::~GVfindings() {
    clearFindings();
}


//------------------------------------------------------------------------------
int                      GVfindings::numberOfRecords = 0;

QHash <int, GVfindingsObject *>   GVfindings::eh;
QList <GVfindingsObject *>        GVfindings::el;
