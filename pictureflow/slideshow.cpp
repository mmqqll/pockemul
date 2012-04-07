/****************************************************************************
 **
 ** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the demonstration applications of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** GNU Lesser General Public License Usage
 ** This file may be used under the terms of the GNU Lesser General Public
 ** License version 2.1 as published by the Free Software Foundation and
 ** appearing in the file LICENSE.LGPL included in the packaging of this
 ** file. Please review the following information to ensure the GNU Lesser
 ** General Public License version 2.1 requirements will be met:
 ** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights. These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU General
 ** Public License version 3.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of this
 ** file. Please review the following information to ensure the GNU General
 ** Public License version 3.0 requirements will be met:
 ** http://www.gnu.org/copyleft/gpl.html.
 **
 ** Other Usage
 ** Alternatively, this file may be used in accordance with the terms and
 ** conditions contained in a signed written agreement between you and Nokia.
 **
 **
 **
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QBasicTimer>
 #include <QList>
 #include <QImage>
 #include <QDir>
 #include <QPainter>
 #include <QPaintEvent>

 #include <QDebug>

 #include "slideshow.h"

#define BACKGROUNDCOLOR Qt::white


 class SlideShowPrivate
 {
 public:
     SlideShowPrivate();

     int currentSlide;
     int slideInterval;
     QBasicTimer interSlideTimer;
     QStringList imagePaths;

     void showNextSlide();
 };

 SlideShowPrivate::SlideShowPrivate()
 {
     currentSlide = 0;
     slideInterval = 10000; // Default to 10 sec interval
 }

 void SlideShowPrivate::showNextSlide()
 {
     currentSlide++;
     qWarning("Current:%i / %i\n",currentSlide,imagePaths.size());
     if (currentSlide >= imagePaths.size())
       currentSlide = 0;
 }

 SlideShow::SlideShow(QWidget* parent) :
     QWidget(parent)
 {
     d = new SlideShowPrivate;

     setAttribute(Qt::WA_StaticContents, true);
     setAttribute(Qt::WA_OpaquePaintEvent, true);
     setAttribute(Qt::WA_NoSystemBackground, true);

     setMouseTracking(true);
 }

 SlideShow::~SlideShow()
 {
     delete d;
 }

 void SlideShow::addImageDir(QString dirName)
 {
     QDir dir(dirName);
     qWarning("dirname:%s",dirName.toAscii().data());
     QStringList fileNames = dir.entryList(QDir::Files | QDir::Readable, QDir::Name);

     for (int i=0; i<fileNames.count(); i++) {
         d->imagePaths << dir.absoluteFilePath(fileNames[i]);
                          qWarning("   file:%s\n",dir.absoluteFilePath(fileNames[i]).toAscii().data());
     }
 }

 void SlideShow::addImage(QString filename)
 {
     d->imagePaths << filename;
 }

 void SlideShow::clearImages()
 {
     d->imagePaths.clear();
 }

 void SlideShow::startShow()
 {
     qWarning("Start show\n");
     d->interSlideTimer.start(d->slideInterval, this);
     d->showNextSlide();
     update();
 }

 void SlideShow::stopShow()
 {
     d->interSlideTimer.stop();
 }

 int SlideShow::slideInterval()
 {
     return d->slideInterval;
 }

 void SlideShow::setSlideInterval(int val)
 {
     d->slideInterval = val;
 }

 void SlideShow::timerEvent(QTimerEvent* event)
 {
     Q_UNUSED(event);
     d->showNextSlide();
     update();
 }

 void SlideShow::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing, false);
     qWarning("PaintEvent \n");
     if (d->imagePaths.size() > 0) {
         QPixmap slide = QPixmap(d->imagePaths[d->currentSlide]);
         qWarning("Loading img:%s\n",d->imagePaths.at(d->currentSlide).toAscii().data());
         QSize slideSize = slide.size();
         QSize scaledSize = QSize(qMin(slideSize.width(), size().width()),
             qMin(slideSize.height(), size().height()));
         if (slideSize != scaledSize)
             slide = slide.scaled(scaledSize, Qt::KeepAspectRatio);

         QRect pixmapRect(qMax( (size().width() - slide.width())/2, 0),
                          qMax( (size().height() - slide.height())/2, 0),
                          slide.width(),
                          slide.height());

         if (pixmapRect.top() > 0) {
             // Fill in top & bottom rectangles:
             painter.fillRect(0, 0, size().width(), pixmapRect.top(), BACKGROUNDCOLOR);
             painter.fillRect(0, pixmapRect.bottom(), size().width(), size().height(), BACKGROUNDCOLOR);
         }

         if (pixmapRect.left() > 0) {
             // Fill in left & right rectangles:
             painter.fillRect(0, 0, pixmapRect.left(), size().height(), BACKGROUNDCOLOR);
             painter.fillRect(pixmapRect.right(), 0, size().width(), size().height(), BACKGROUNDCOLOR);
         }

         painter.drawPixmap(pixmapRect, slide);

     } else
         painter.fillRect(event->rect(), BACKGROUNDCOLOR);
 }

 void SlideShow::keyPressEvent(QKeyEvent* event)
 {
     Q_UNUSED(event);
     emit inputReceived();
 }

 void SlideShow::mouseMoveEvent(QMouseEvent* event)
 {
     Q_UNUSED(event);
     emit inputReceived();
 }

 void SlideShow::mousePressEvent(QMouseEvent* event)
 {
     Q_UNUSED(event);
     emit inputReceived();
 }

 void SlideShow::mouseReleaseEvent(QMouseEvent* event)
 {
     Q_UNUSED(event);
     emit inputReceived();
 }

 void SlideShow::showEvent(QShowEvent * event )
 {
     Q_UNUSED(event);
 #ifndef QT_NO_CURSOR
     setCursor(Qt::BlankCursor);
 #endif
 }
