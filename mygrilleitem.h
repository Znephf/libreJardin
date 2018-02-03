#ifndef MYGRILLEITEM_H
#define MYGRILLEITEM_H

#include <QtGui>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

class mygrilleitem:public QGraphicsRectItem
{
public:
  mygrilleitem(int caseL,int caseH);

  ~mygrilleitem();

	// Couleur de la grille
	QColor caseColor ;
	QColor penColor;
	QColor brushColor;
	// Coordonnées de la case de la grille
	int caseL;
	int caseH;
	QString itemText;

	/* Méthodes */

	// Retourne la forme de la case (item)
	QPainterPath shape() const;
	// Zone du rectangle à représenter
	QRectF boundingRect() const;
	// Fonction virtuelle de QGraphicsItem appelée par QGraphicsView
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

	protected:

	public:
	// Fonction qui renvoie la couleur de la case
	QColor getColor()
	{
		return caseColor;
	}
	// Fonction qui définie une nouvelle couleur pour la case et mets à jour l'affichage
	void setColor(const QColor &color)
	{
		caseColor = color;
		update();
	}
	// Fonction qui définie une nouvelle couleur du crayon de la case
	void setPenColor(const QColor &color)
	  {
	    penColor = color;
	  }
	// Fonction qui renvoie la couleur du crayon de la case
	QColor getPenColor()
	  {
	    return penColor;
	  }
	void setText(const QString texte)
	  {
	    itemText = texte;
	  }

        QString getText()
          {
            return itemText;
          }


};

#endif // MYGRILLEITEM_H
