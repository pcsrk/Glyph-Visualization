/*
 *  G_number.h
 *  isoProjTest
 *
 *  Created by mue on 06.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef G_NUMBER_H
#define G_NUMBER_H


#include <QObject>
#include <QGLContext>

#include "GLmaster.h"
#include "G__texture.h"

class G_number : public QObject 
{
    Q_OBJECT    
public:
    
    G_number  (GLmaster *gmaster, QGLContext *cx);    
  
    double getWidth();
    double getHeight();
    double getAspect();
    int    getDigits();
    
public slots:
        
    void setNumber   (int value);
    void setDigits   (int n);
    void paint();
    
protected:
        
    
private:        

    GLmaster     *gmaster;
    QGLContext   *cx;
    G__texture   *textureManager;    
    
    int          number;        
    int          digits;
    int          digits_old; 
    
    double       width;
    double       height;
        
    GLuint       singleNumberDL[10];
    GLuint       bbHalfDL;
    GLuint       bbDL;
};
#endif