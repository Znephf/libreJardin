#include "mygraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPoint>
#include <QtWidgets>
#include "myitem.h"
#include "mygrilleitem.h"
#include "mypolygone.h"
#include "mypolyline.h"
#include "myvertex.h"


//MyGraphicsScene::MyGraphicsScene(QWidget *parent)
 MyGraphicsScene::MyGraphicsScene(QObject *parent)
  : QGraphicsScene(parent)
{
   setMode(MyGraphicsScene::InsertPolygon);

}


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(mouseEvent->button() == Qt::LeftButton)
    {

    if (myMode==InsertPolygon)
      {
         //création de polygones
         MyVertex *item = new MyVertex(10,10);
         item->setColor(QColor(Qt::yellow));
         item->setPenColor(QColor(Qt::black));
         item->setOpac(0.8); //opacité de l'item
         item->setTypeShape(MyVertex::Rectangle);
         item->setPos(mouseEvent->buttonDownScenePos(Qt::LeftButton).x() ,mouseEvent->buttonDownScenePos(Qt::LeftButton).y());
         item->setNom(" ");
         item->setTexte("xx");
         item->setEtat(1);
         item->setMode(0);
         addItem(item);
      }
    }
  if(mouseEvent->button() == Qt::MiddleButton)
    {

    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    QGraphicsScene::mouseMoveEvent(mouseEvent);

}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(mouseEvent->button() == Qt::MiddleButton)
    {


    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MyGraphicsScene::setMode(Mode mode)
{
    myMode = mode;
}
