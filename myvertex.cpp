//myvertex.cpp
/****************************************************************************
**
** Copyright (C) 2017 openjardin.eu.
** Contact: https://openjardin.eu
**
** You may use this file under the terms of the GNU GENERAL PUBLIC LICENSE
** Version 3, 29 June 2007  as follows:
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "myvertex.h"
#include "mainwindow.h"
#include "mygraphicsscene.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets>

MyVertex::MyVertex(qreal wid,qreal hgt)
{
  m_width=wid;m_height=hgt; //initialisation des valeurs largeur - hauteur
  brushColor = Qt::blue;
  penColor = Qt::black;
  m_opacity=1.0; //transparence
 // setFlag(QGraphicsItem::ItemIsFocusable);
  setFlag(QGraphicsPathItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setAcceptHoverEvents(true);
}

QRectF MyVertex::boundingRect() const
{
 return QRectF(-m_width/2-1,-m_height/2-1,m_width,m_height);
}
QPainterPath MyVertex::shape()const
{
   QPainterPath path;
   path.addRect(QRectF(-m_width/2,-m_height/2,m_width,m_height));
   return path;
}

void MyVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QRectF rect = boundingRect();
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setBrush(brushColor);
  QPen pen(penColor, 1);
  painter->setPen(pen);
  painter->setOpacity(m_opacity);

  switch (typeShape)
     {
             case MyVertex::Rectangle:
                     painter->drawRect(QRectF(-m_width/2,-m_height/2,m_width,m_height));
                     break;
             case MyVertex::RoundedRectangle:
                     painter->drawRoundedRect(QRectF(-m_width/2,-m_height/2,m_width,m_height), 10, 10);
                     break;
             case MyVertex::Circle:
                     painter->drawEllipse(QRectF(-m_width/2,-m_height/2,m_width,m_height));  //à verifier
                     break;
             case MyVertex::Image:
                     QImage pixm(pixmapfileName);
                     painter->drawImage(QRectF(-m_width/2,-m_height/2,m_width,m_height),pixm);  //test pixmap
                     break;
     }

    painter->drawText(rect, Qt::AlignCenter, getNom());
}

void MyVertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
 if(getMode()==1)
   {
         scene()->clearSelection();
         setSelected(true);
         // mode utilisation
   }
 else
   {
     if(getMode()==2) //mode modification planning
      {
         scene()->clearSelection();
         setSelected(true);
         setOldX(pos().x()-(m_width/2));
         setOldY(pos().y()-(m_height/2));
         const QPointF event_pos = event->pos();
     //    const QPointF event_scene_pos = event->scenePos();
         if(event_pos.x()<(m_width/2)-2 && event_pos.y()<(m_height/2)-2)
           {
              setCursor(Qt::ClosedHandCursor);
              mousePressPt.setX(pos().x() - event->scenePos().x());
           }
         else
           {
             setCursor(Qt::SizeFDiagCursor);
           }
          update();
       }
     else  //mode modification plan
       {
          scene()->clearSelection();
          setOldX(pos().x()-(m_width/2));
          setOldY(pos().y()-(m_height/2));
          setSelected(true);
          const QPointF event_pos = event->pos();
          setCursor(Qt::ClosedHandCursor);
           mousePressPt.setX(pos().x() - event->scenePos().x());
           mousePressPt.setY(pos().y() - event->scenePos().y());


          update();
         }
   }
 QGraphicsItem::mousePressEvent(event);

}

void MyVertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
 if(getMode()==1)
   {
     //mode utilisation
   }
 else
   {
      if(getMode()==2) //mode modification planning
        {
          if(cursor().shape()==Qt::ClosedHandCursor)
            {
          setX(mousePressPt.x() + event->scenePos().x());
            }
            event->accept();
            update();
         }
        else   //mode modification plan
         {
           if(cursor().shape()==Qt::ClosedHandCursor)
             {
           setX(mousePressPt.x() + event->scenePos().x());
           setY(mousePressPt.y() + event->scenePos().y());
              }
            event->accept();
            update();
         }
    }
}

void MyVertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
 if(getMode()==1)
   {
    // mode utilisation
   }
 else
   {
     if(getMode()==2) //mode modification planning
       {
         if(cursor().shape()==Qt::ClosedHandCursor)
           {
             setCursor(Qt::ArrowCursor);
           }
           scene()->clearSelection();
           setSelected(true);
           update();
      }
     else  //mode modification plan
       {
           if(cursor().shape()==Qt::ClosedHandCursor)
             {
                setCursor(Qt::ArrowCursor);
             }
             update();
         }
    }
   QGraphicsItem::mouseReleaseEvent(event);
}

void MyVertex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   setToolTip(getComment());
}
void MyVertex::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

 if(getMode() != 1 &&
      getMode() != 2)
   {
       if(event->pos().x()<(m_width/2)-2 && event->pos().y()<(m_height/2)-2)
           setCursor(Qt::OpenHandCursor);
   }
 else
   {
      setCursor(Qt::ArrowCursor);
   }
}

void MyVertex::setTexte(const QString text)
 {
   itemText = text;
 }

QString MyVertex::getTexte()
 {
   return itemText;
 }

void MyVertex::setComment(const QString comment)
 {
   itemComment = comment;
 }

QString MyVertex::getComment()
 {
   return itemComment;
 }

int MyVertex::getTypeAction()
 {
   return typeAction;
 }

void MyVertex::setTypeAction(int typeAct)
 {
   typeAction =typeAct;
 }

QString MyVertex::getAction()
 {
   return action;
 }

void MyVertex::setAction(QString act)
 {
   action =act;
 }

QString MyVertex::getPixFile()
 {
   return pixmapfileName;
 }

void MyVertex::setPixFile(QString file)
 {
   pixmapfileName =file;
 }
