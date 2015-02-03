#include <math.h>
#include <iostream>

#include "GVdbConnect.h"

#include <QtGlobal>

#include <QString>
#include <QLibrary>
#include <QPluginLoader>

#include <QApplication>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>


#define lokal 0


GVdbConnect::GVdbConnect()
{
    
    
    QSqlError    dberr;
    QSqlDatabase dbpg; 
    
    qDebug("Open the default SQL LITE Database");
    dbpg= QSqlDatabase::addDatabase ("QSQLITE");
    
	
	dbpg.setDatabaseName ( "/Users/mue/_working/genview/sqliteDB/tes2.db3");

    bool ok = dbpg.open();
    
    
    if (!ok) {
        dberr = dbpg.lastError();
        std::cout << "db error:       "  << (const char*)dberr.databaseText().toLatin1()<< "\n";
        std::cout << "driver error :  "  << (const char*)dberr.driverText().toLatin1()<< "\n";
        
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"),
                              QMessageBox::Cancel,
                              QMessageBox::NoButton);
        
    }
    qDebug("SQL Database found");
    
}    


GVdbConnect::~GVdbConnect() 
{
    // todo close DB 
}





