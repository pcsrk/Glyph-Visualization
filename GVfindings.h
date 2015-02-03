/*
 *  GVfindings.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVfindings_H
#define GVfindings_H

#include "GVarrayQuantCont.h"


#include "GVfindingsObject.h"

#include <QStringList>
#include <QHash>

extern "C" {
#include "sqlite3.h"
}

class GVanimationMaster;

class GVfindings  :public QObject
{
    Q_OBJECT
        
public:
    
     GVfindings();
    ~GVfindings();
    
	void openDatabase   (QString dbName);    
	void closeDatabase  ();

	long approxNumberOfElements    ( QString whereStatment);
	
	void addFindings           ();
				
	void clearFindings ();

    static int   numberOfRecords;        
    
	static QHash  <int, GVfindingsObject *> eh;
    static QList  <GVfindingsObject *>      el;

signals:

	void showMessage(QString m, int t);
    
private:        
        
    sqlite3		*_db;				
	QHash <QString, int > CI; 

};

#endif