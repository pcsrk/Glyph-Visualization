/*
 *  GVinfoWindow.cpp
 *  diseaseViewer
 *
 *  Created by mue on 22.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVinfoWidget.h"

#include <QFont>
#include <QVBoxLayout>
#include <QPalette>

GVinfoWidget::GVinfoWidget ( QWidget *parent, GVfindings  *findings) : QWidget(parent) {
	_findings = findings;
	
	_textOutput = new QTextEdit( );
	_textOutput->setReadOnly( true );
	_textOutput->setUndoRedoEnabled(false);
	_textOutput->setAcceptDrops(false);
	_textOutput->setAutoFillBackground(false);
	_textOutput->setAcceptRichText( true );
	_textOutput->setLineWrapMode( QTextEdit::NoWrap );
	_textOutput->setWordWrapMode( QTextOption::NoWrap );
	_textOutput->setTextColor( QColor( 161,161,161 ) );

#ifdef WIN32
	QFont logfont ("Helvetica", 9, QFont::Normal);
#else
	QFont logfont ("Helvetica", 11, QFont::Normal);
#endif

	_textOutput->setCurrentFont (logfont);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(_textOutput);
	setLayout (vbox);

	QPalette ptemp;
    ptemp.setColor(QPalette::Window,  QColor(70,70,70));
    ptemp.setColor(QPalette::Base,    QColor(75,75,75));
	ptemp.setColor(QPalette::Normal,   QPalette::Text,           QColor(164,164,164));
	ptemp.setColor(QPalette::Normal,   QPalette::WindowText,     QColor(164,164,164));
	ptemp.setColor(QPalette::Normal,   QPalette::ButtonText,     QColor(164,164,164));
	_textOutput->setPalette(ptemp);

	_textOutput->clear();
	_textOutput->append("welcome");
}

//------------------------------------------------------------------------------
void GVinfoWidget::setSelectedElement (int v) {

	_textOutput->clear();
}

//------------------------------------------------------------------------------
void GVinfoWidget::clear () {
	_textOutput->clear();
}

