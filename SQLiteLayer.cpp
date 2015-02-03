/*
 *  SQLiteLayer.cpp
 *  patientDBdebugger
 *
 *  Created by mue on 11.07.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "SQLiteLayer.h"


void SQLiteLayer::open(QString databaseName)
{
  if(db_)close();
  char *errmsg = NULL;
  sqlite3_open(databaseName.toUtf8(), &db_);
  
  if(db_ == 0)
  {
    qDebug ("Database %s coud not be opened, error code %s", qPrintable (databaseName), errmsg); 
//	sqlite_freemem(errmsg);
  }	
}

void SQLiteLayer::close()
{
  if(db_)sqlite3_close(db_);
  db_ = NULL;
}

sqlite_result SQLiteLayer::exec_sql(QString query)
{
 
  sqlite_result result;
  
  char *errmsg = NULL;

  
  int res = sqlite3_exec(db_, query.toUtf8(), sqlite_callback, (void*)&result, &errmsg);
  if(res != SQLITE_OK)
  {
      qDebug ("Query %s failed,  error code %s", qPrintable (query), errmsg); 
//      sqlite3_freemem(errmsg);
  }
  return result;
}

void SQLiteLayer::exec_dml(QString query)
{

  char *errmsg = NULL;

  int res = sqlite3_exec(db_,query.toUtf8(),sqlite_callback,(void*)NULL,&errmsg);
  if(res != SQLITE_OK)
  {
      qDebug ("Query %s failed,  error code %s", qPrintable (query), errmsg); 
//      sqlite3_freemem(errmsg);
  }
}

int SQLiteLayer::sqlite_callback(void *param,int colcount,char **cols,char **colnames)
{
  sqlite_result* result = reinterpret_cast<sqlite_result*>(param);
  sqlite_fields f;
  for(int i = 0;i < colcount;++i)
  {
    f[colnames[i]] = cols[i];
  }
  result->append (f);
  return 0;
}

