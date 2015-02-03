/*
 *  Gnumber.h
 *  sequencepaint
 *
 *  Created by mue on 05.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QGLWidget>
#include <QObject>

#include "GLmaster.h"
#include "G_number.h"

class Gnumber : public QGLWidget 
{
    
    Q_OBJECT
    
public:
    
    Gnumber  (QWidget        *parent = 0, 
              GLmaster       *shareWidget=0);
        
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
        
    void setNumber (int value);
    void repaint();
    void doResize();
   
protected:
        
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:        

    GLmaster     *gmaster;    
    G_number     *gnumber;
    
    bool         glInitOK;
    int          number;        
    GLuint       singleNumberDL[10];
    GLuint       crossDL;
    
    double       scaleCorrectionX;
    double       scaleCorrectionY;
    double       scaleCorrectionDx;
    double       scaleCorrectionDy;
    
};