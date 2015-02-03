/*
 *  SQLiteLayer.h
 *  patientDBdebugger
 *
 *  Created by mue on 11.07.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef SQLiteLayer_H
#define SQLiteLayer_H

extern "C" {
#include "sqlite3.h"
}


#include <QString>
#include <QHash>
#include <QList>
#include <QStringList>


typedef QHash <QString, QString>   sqlite_fields;
typedef QList <sqlite_fields>      sqlite_result;

class SQLiteLayer
{
  public:

    SQLiteLayer() : db_ (NULL) { }

    virtual ~SQLiteLayer() {if(db_) close(); }

    const char *version() const { return SQLITE_VERSION;} 

    void open (QString databaseName);

    void close ();

    sqlite_result exec_sql  (QString query);

    void exec_dml           (QString query);

  private:
    
	sqlite3* db_;
    static int sqlite_callback(void *param,int colcount,char **cols,char **colnames);


};

#endif

