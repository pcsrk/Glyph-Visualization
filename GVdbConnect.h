/*
 *  GVdbConnect.h
 *  isoProjTest
 *
 *  Created by mue on 23.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVdbConnect_H
#define GVdbConnect_H

#include <QSqlDatabase>
#include <QString>
#include <QHash>


class GVdbConnect 
{
    
public:
    
     GVdbConnect();
    ~GVdbConnect();

     
private:

    QSqlDatabase *db;
};
#endif