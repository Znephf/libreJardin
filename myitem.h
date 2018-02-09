#ifndef MYITEM_H
#define MYITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

// class for customization
class MyItem :public QGraphicsItem
{
public:

  MyItem(qreal wid,qreal hgt);

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
    QPainterPath shape()const override;

    void setColor(const QColor &color)
      {
        brushColor = color;
      }
    void setPenColor(const QColor &color)
      {
        penColor = color;
      }

    QColor getColor()
      {
        return brushColor;
      }
    QColor getPenColor()
      {
        return penColor;
      }

    void setTypeShape(const int &type)
      {
        typeShape = type;
      }

    int getTypeShape()
      {
        return typeShape;
      }

    void setEtat(const int &etat)
      {
        m_etat = etat;
      }

    int getEtat()
      {
        return m_etat;
      }

    void setMode(const int &mode)
      {
        m_mode = mode;
      }

    int getMode()
      {
        return m_mode;
      }
    int getWidth()
    {
      return m_width;
    }

    void setSizeShape(const qreal &W,const qreal &H)
      {
        m_width = W;
        m_height = H;
      }
    void setOldX(const qreal &X)
      {
        m_oldX = X;
      }
    void setOldY(const qreal &Y)
      {
        m_oldY = Y;
      }
    qreal getOldX()
    {
      return m_oldX;
    }
    qreal getOldY()
    {
      return m_oldY;
    }
    void setId(const int &id)
      {
        m_itemId = id;
      }

    int getId()
      {
        return m_itemId;
      }

    void setNom(const QString nom)
      {
        itemNom = nom;
      }

    QString getNom()
      {
        return itemNom;
      }
    void setOpac(const qreal &Opacity)
      {
        m_opacity = Opacity;
      }
    qreal getOpac()
    {
      return m_opacity;
    }



    void setTypeAction(int typeAction);
    int getTypeAction();
    void setAction(QString action);
    QString getAction();
    void setTexte(const QString text);
    QString getTexte();
    void setComment(const QString comment);
    QString getComment();
    void setPixFile(QString pixmapfileName);
    QString getPixFile();

    // item state
    bool Pressed;
protected:
    // overriding mouse events
    QPoint mousePressPt;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    int m_itemId;
    int typeAction;
    QString action;
    QColor brushColor;
    QColor penColor;
    qreal m_opacity;
    qreal m_width;
    qreal m_height;
    qreal m_oldX;
    qreal m_oldY;
    QString itemNom;
    QString itemText;
    QString itemComment;
    int typeShape;
    QString pixmapfileName;
    int m_etat;
    int m_mode;//0 modification - 1 utilisation
};

#endif // MYITEM_H
