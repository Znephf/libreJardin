#include "detail_parcelle.h"
#include "ui_detail_parcelle.h"
#include "graphic/parcelleitem.h"
#include "graphic/mygraphicsscene.h"
#include "graphic/background.h"
#include "dialogs/cultures.h"
#include "graphic/mypolygone.h"
#include "graphic/mypolyline.h"
#include "utilitaires/util.h"

#include <QDomDocument>
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include <QColorDialog>
#include <QUuid>

detail_parcelle::detail_parcelle(const int&IdParcelle, const QString&fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::detail_parcelle)
{
    // translator
    QTranslator translator;
    QString     fileContent = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fileContent);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    setFileNameXML(fileName); //name of the detail plan file
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1900, 1200);
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    ui->graphicsView->setScene(scene);
    // if the item selection changes, launch the command
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(Item_clicked()));
    m_ZoomRatio = 1;  //zoom factor variable (actual size =1 )
    ui->label_parcelle->setText(QString::number(IdParcelle));
    setIdParcelle(IdParcelle);
    if (fileName != "")
    {
        open_XMLFile(fileName);
    }
}

detail_parcelle::~detail_parcelle()
{
    delete ui;
}

void detail_parcelle::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    accept();
    close();
}

void detail_parcelle::open_XMLFile(QString fileName)
{
    QDomDocument document;
    QString      field;
    QFile        fileHandle(fileName);

    // clear existing plots list
    plotList.clear();
    qDebug() << " xml file " << fileName;
    if (fileHandle.exists())
    {
        QFile file(fileHandle.fileName());
        setFileNameXML(fileName);  //name of the detail plan file
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("READ ERROR");
            return;
        }
        else
        {
            if (!document.setContent(&file))
            {
                //Error reading file";
            }
            else
            {
                // Element root
                setFileNameXML(fileName);
                QDomElement  root  = document.firstChildElement();
                QDomNodeList base  = root.elementsByTagName("fichier_base");
                QDomNodeList background_node  = root.elementsByTagName("fond");
                QDomNodeList object_node_list = root.elementsByTagName("Objet");

                // write the background object to the array
                QDomNode fondnode = background_node.at(0);
                if (fondnode.isElement())
                {
                    // qDebug() << "background element loop";
                    QDomElement fond_element = fondnode.toElement();
                    setFileNameBackGround(fond_element.attribute("chemin"));
                    QString     currentFileName = getFileNameBackGround();
                    QPixmap     pim(currentFileName);
                    background *pixmap = new background();

                    scene->clear();
                    pixmap->setPixmap(pim);
                    scene->addItem(pixmap);
                    pixmap->setPixFile(currentFileName);
                    double sizeW = pixmap->boundingRect().width();
                    double sizeH = pixmap->boundingRect().height();
                    scene->setSceneRect(0, 0, sizeW, sizeH);
                }
                // loop to write each object to the array
                for (int i = 0; i < object_node_list.count(); i++)
                {
                    QDomNode objetnode = object_node_list.at(i);
                    if (objetnode.isElement())
                    {
                        QDomElement objet_element = objetnode.toElement();
                        if (objet_element.attribute("Type").toInt() == 65536) //parcelleItem
                        {
                            QColor        backgroundColor = QRgb(objet_element.attribute("Brush").toDouble());
                            QColor        penColor  = QRgb(objet_element.attribute("Pen").toDouble());
                            parcelleItem *item        = new parcelleItem(objet_element.attribute("SizeX").toInt(), objet_element.attribute(
                                                                             "SizeY").toInt());
                            QPen pen(Qt::blue, 1);
                            item->setColor(QColor(backgroundColor));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet_element.attribute("PositionX").toInt(), objet_element.attribute("PositionY").toInt());
                            item->setTexte(objet_element.attribute("Texte"));
                            item->setTypeShape(objet_element.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet_element.attribute("Commentaire"));
                            item->setPenColor(QColor(penColor));
                            item->setPixFile(objet_element.attribute("Chemin"));
                            item->setId(objet_element.attribute("Id").toInt());
                            item->setNom(objet_element.attribute("Nom"));
                            item->setEtat(objet_element.attribute("Etat").toInt());
                            item->setMode(objet_element.attribute("Mode").toInt());
                            item->setTypeLine(objet_element.attribute("TypeLine").toInt());
                            item->setWidthLine(objet_element.attribute("WidthLine").toInt());
                            item->setZValue(objet_element.attribute("ZValue").toDouble());
                            if (objet_element.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet_element.attribute("Uuid"));
                            }
                            scene->addItem(item);
                        }
                        if (objet_element.attribute("Type").toInt() == 65537) // MyPolygone
                        {
                            MyPolygone *item = new MyPolygone(convertStrToPoly(objet_element.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      backgroundColor = QRgb(objet_element.attribute("Brush").toDouble());
                            QColor      penColor  = QRgb(objet_element.attribute("Pen").toDouble());
                            item->setColor(QColor(backgroundColor));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet_element.attribute("PositionX").toInt(), objet_element.attribute("PositionY").toInt());
                            item->setTexte(objet_element.attribute("Texte"));
                            item->setTypeShape(objet_element.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet_element.attribute("Commentaire"));
                            item->setPenColor(QColor(penColor));
                            item->setStrPoly(objet_element.attribute("Chemin"));
                            item->setId(objet_element.attribute("Id").toInt());
                            item->setNom(objet_element.attribute("Nom"));
                            item->setEtat(objet_element.attribute("Etat").toInt());
                            item->setMode(objet_element.attribute("Mode").toInt());
                            item->setTypeLine(objet_element.attribute("TypeLine").toInt());
                            item->setWidthLine(objet_element.attribute("WidthLine").toInt());
                            item->setZValue(objet_element.attribute("ZValue").toDouble());
                            item->setUuid(objet_element.attribute("Uuid"));
                            if (objet_element.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet_element.attribute("Uuid"));
                            }
                            scene->addItem(item);
                        }
                        if (objet_element.attribute("Type").toInt() == 65539) // MyPolyline
                        {
                            MyPolyline *item = new MyPolyline(convertStrToPoly(objet_element.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      backgroundColor = QRgb(objet_element.attribute("Brush").toDouble());
                            QColor      penColor  = QRgb(objet_element.attribute("Pen").toDouble());
                            item->setColor(QColor(backgroundColor));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet_element.attribute("PositionX").toInt(), objet_element.attribute("PositionY").toInt());
                            item->setTexte(objet_element.attribute("Texte"));
                            item->setTypeShape(objet_element.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet_element.attribute("Commentaire"));
                            item->setPenColor(QColor(penColor));
                            item->setStrPoly(objet_element.attribute("Chemin"));
                            item->setId(objet_element.attribute("Id").toInt());
                            item->setNom(objet_element.attribute("Nom"));
                            item->setEtat(objet_element.attribute("Etat").toInt());
                            item->setMode(objet_element.attribute("Mode").toInt());
                            item->setTypeLine(objet_element.attribute("TypeLine").toInt());
                            item->setWidthLine(objet_element.attribute("WidthLine").toInt());
                            item->setZValue(objet_element.attribute("ZValue").toDouble());
                            if (objet_element.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet_element.attribute("Uuid"));
                            }
                            scene->addItem(item);
                        }
                    }
                }
            }
        }
        file.close();
    }
    on_toolButton_Usage_clicked();
}

void detail_parcelle::on_pushButton_open_clicked()
{
    // open file for reading
    QString directory = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open object file"),
                                     directory,
                                     tr("XML Files (*.xml);; All files (*.*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        open_XMLFile(fileName);
    }
}

QPolygon detail_parcelle::convertStrToPoly(const QString MyString)
{
    //conversion of a string with QPoints separated by ; into QPolygon

    QStringList list_str = MyString.split(';', QString::SkipEmptyParts);
    QPolygon    poly(list_str.count());

    for (int i = 0; i < list_str.count(); i++)
    {
        QString     point_value = list_str[i];
        QStringList point_list  = point_value.split(',', QString::SkipEmptyParts);
        QString     strX         = point_list[0];
        QString     strY         = point_list[1];
        int         x            = strX.toInt();
        int         y            = strY.toInt();
        poly.setPoint(i, x, y);
    }
    return poly;
}

void detail_parcelle::Item_clicked()
{   //if the item is selected (selection changed)
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // parcelleItem
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());

                if (item->getTypeShape() == (parcelleItem::Rectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                if (item->getTypeShape() == (parcelleItem::RoundedRectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                if (item->getTypeShape() == (parcelleItem::Circle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
            }
            if (itemList[i]->type() == 65537) // MyPolygone
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
            }

            if (itemList[i]->type() == 65539) // MyPolyline
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                //    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
            }
        }
    }
}

void detail_parcelle::on_toolButton_save_as_clicked()
{
    //save the plan
    QString cellvalue;
    QString field;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background_element = document.createElement("background");
    root.appendChild(background_element);
    QDomElement objects_element = document.createElement("objets");
    root.appendChild(objects_element);
    QDomElement file_base_element = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    field     = "fichier";
    file_base_element.setAttribute(field, cellvalue);
    base.appendChild(file_base_element);
    //background
    QDomElement fond_element_xml = document.createElement("fond");

    fond_element_xml.setAttribute("chemin", getFileNameBackGround());
    fond_element_xml.setAttribute("Type", "7");
    fond_element_xml.setAttribute("PositionX", "0");
    fond_element_xml.setAttribute("PositionY", "0");
    background_element.appendChild(fond_element_xml);

    //Write objects
    QString ObjectId;
    QString ObjectName;
    QString ObjectType;
    QString ObjectShape;
    QString ObjectPosX;
    QString ObjectPosY;
    QString ObjectWidth;
    QString ObjectHeight;
    QString ObjectBrush;
    QString ObjectPen;
    QString ObjectComment;
    QString ObjectText;
    QString ObjectPath;
    QString ObjectState;
    QString ObjectMode;
    QString ObjectTypeLine;
    QString ObjectWidthLine;
    QString ObjectZValue;
    QString ObjectUuid;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet_xml_element = document.createElement("Objet");

            if (itemList[i]->type() == 65536)       // MyItem
            {
                parcelleItem *item     = dynamic_cast <parcelleItem *> (itemList[i]);
                QPointF       location = itemList[i]->pos();
                int           valX     = int(location.x());
                int           valY     = int(location.y());
                int           sizeW    = int(itemList[i]->boundingRect().width());
                int           sizeH    = int(itemList[i]->boundingRect().height());
                QColor        clr      = item->getColor();
                QColor        clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65536);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getPixFile();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65537)       // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65537);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getStrPoly();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65539)       // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65539);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getStrPoly();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }



            objet_xml_element.setAttribute("Id", ObjectId);
            objet_xml_element.setAttribute("Nom", ObjectName);
            objet_xml_element.setAttribute("Type", ObjectType);
            objet_xml_element.setAttribute("PositionX", ObjectPosX);
            objet_xml_element.setAttribute("PositionY", ObjectPosY);
            objet_xml_element.setAttribute("SizeX", ObjectWidth);
            objet_xml_element.setAttribute("SizeY", ObjectHeight);
            objet_xml_element.setAttribute("Brush", ObjectBrush);
            objet_xml_element.setAttribute("Pen", ObjectPen);
            objet_xml_element.setAttribute("Forme", ObjectShape);
            objet_xml_element.setAttribute("Commentaire", ObjectComment);
            objet_xml_element.setAttribute("Texte", ObjectText);
            objet_xml_element.setAttribute("Chemin", ObjectPath);
            objet_xml_element.setAttribute("Mode", ObjectMode);
            objet_xml_element.setAttribute("Etat", ObjectState);
            objet_xml_element.setAttribute("TypeLine", ObjectTypeLine);
            objet_xml_element.setAttribute("WidthLine", ObjectWidthLine);
            objet_xml_element.setAttribute("ZValue", ObjectZValue);
            objet_xml_element.setAttribute("Uuid", ObjectUuid);
            objects_element.appendChild(objet_xml_element);
        }
    }
    // Write to file

    QFileInfo fileInfoXML(getFileNameXML());
    QDir      directoryXML = fileInfoXML.absoluteDir();
    QString   directory    = QDir::homePath() + "/openjardin/";
    QString   fileName      =
        QFileDialog::getSaveFileName(this, tr("Save detail plan"),
                                     directory,
                                     tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("WRITE ERROR");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        setFileNameXML(fileName);
    }
    close();
}

void detail_parcelle::on_toolButton_Save_clicked()
{   //save the plan
    QString cellvalue;
    QString field;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background_element = document.createElement("background");
    root.appendChild(background_element);
    QDomElement objects_element = document.createElement("objets");
    root.appendChild(objects_element);
    QDomElement file_base_element = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    field     = "fichier";
    file_base_element.setAttribute(field, cellvalue);
    base.appendChild(file_base_element);
    //background
    QDomElement fond_element_xml = document.createElement("fond");

    fond_element_xml.setAttribute("chemin", getFileNameBackGround());
    fond_element_xml.setAttribute("Type", "7");
    fond_element_xml.setAttribute("PositionX", "0");
    fond_element_xml.setAttribute("PositionY", "0");
    background_element.appendChild(fond_element_xml);

//Write objects
    QString ObjectId;
    QString ObjectName;
    QString ObjectType;
    QString ObjectShape;
    QString ObjectPosX;
    QString ObjectPosY;
    QString ObjectWidth;
    QString ObjectHeight;
    QString ObjectBrush;
    QString ObjectPen;
    QString ObjectComment;
    QString ObjectText;
    QString ObjectPath;
    QString ObjectState;
    QString ObjectMode;
    QString ObjectTypeLine;
    QString ObjectWidthLine;
    QString ObjectZValue;
    QString ObjectUuid;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet_xml_element = document.createElement("Objet");

            if (itemList[i]->type() == 65536)   // MyItem
            {
                parcelleItem *item     = dynamic_cast <parcelleItem *> (itemList[i]);
                QPointF       location = itemList[i]->pos();
                int           valX     = int(location.x());
                int           valY     = int(location.y());
                int           sizeW    = int(itemList[i]->boundingRect().width());
                int           sizeH    = int(itemList[i]->boundingRect().height());
                QColor        clr      = item->getColor();
                QColor        clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65536);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getPixFile();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65537)   // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65537);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getStrPoly();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65539)   // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjectId        = QString::number(item->getId());
                ObjectName       = item->getNom();
                ObjectType      = QString::number(65539);
                ObjectShape     = QString::number(item->getTypeShape());
                ObjectPosX      = QString::number(valX);
                ObjectPosY      = QString::number(valY);
                ObjectWidth     = QString::number(sizeW);
                ObjectHeight    = QString::number(sizeH);
                ObjectBrush     = QString::number(clr.rgb());
                ObjectPen       = QString::number(clrPen.rgb());
                ObjectComment   = item->getComment();
                ObjectText     = item->getTexte();
                ObjectPath    = item->getStrPoly();
                ObjectState      = QString::number(item->getEtat());
                ObjectMode      = QString::number(item->getMode());
                ObjectTypeLine  = QString::number(item->getTypeLine());
                ObjectWidthLine = QString::number(item->getWidthLine());
                ObjectZValue    = QString::number(item->zValue());
                ObjectUuid      = item->getUuid().toString();
            }



            objet_xml_element.setAttribute("Id", ObjectId);
            objet_xml_element.setAttribute("Nom", ObjectName);
            objet_xml_element.setAttribute("Type", ObjectType);
            objet_xml_element.setAttribute("PositionX", ObjectPosX);
            objet_xml_element.setAttribute("PositionY", ObjectPosY);
            objet_xml_element.setAttribute("SizeX", ObjectWidth);
            objet_xml_element.setAttribute("SizeY", ObjectHeight);
            objet_xml_element.setAttribute("Brush", ObjectBrush);
            objet_xml_element.setAttribute("Pen", ObjectPen);
            objet_xml_element.setAttribute("Forme", ObjectShape);
            objet_xml_element.setAttribute("Commentaire", ObjectComment);
            objet_xml_element.setAttribute("Texte", ObjectText);
            objet_xml_element.setAttribute("Chemin", ObjectPath);
            objet_xml_element.setAttribute("Mode", ObjectMode);
            objet_xml_element.setAttribute("Etat", ObjectState);
            objet_xml_element.setAttribute("TypeLine", ObjectTypeLine);
            objet_xml_element.setAttribute("WidthLine", ObjectWidthLine);
            objet_xml_element.setAttribute("ZValue", ObjectZValue);
            objet_xml_element.setAttribute("Uuid", ObjectUuid);
            objects_element.appendChild(objet_xml_element);
        }
    }
    // Write to file


    QString fileName = getFileNameXML();

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("WRITE ERROR");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        setFileNameXML(fileName);
    }
    close();
}

void detail_parcelle::on_toolButton_AddRectangle_clicked()
{   //create a rectangle
    parcelleItem *item = new parcelleItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);       //item opacity
    item->setId(get_MaxId()); // id value in last position in the table
    item->setUuid(QUuid::createUuid());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setTexte("Plot");
    item->setNom("Plot " + QString::number(get_MaxId()));
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(parcelleItem::Rectangle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
}

void detail_parcelle::on_toolButton_AddCircle_clicked()
{   // create a circle
    parcelleItem *item = new parcelleItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);                               //item opacity
    item->setId(get_MaxId());
    item->setUuid(QUuid::createUuid());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setNom("Plantation " + QString::number(get_MaxId()));
    item->setTexte("");
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(parcelleItem::Circle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
}

int detail_parcelle::get_MaxId()
{   // search for the value of the last Id
    int Max = 0;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)   // parcelleItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65537)   // MyPolygone
        {
            MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65539)   // MyPolyline
        {
            MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
    }
    m_MaxId = Max + 1;
    return m_MaxId;
}

void detail_parcelle::on_comboBox_lineWidth_P_currentIndexChanged(const QString&arg1)
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
                //   ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(arg1.toInt())));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
        }
    }

    scene->update();
}

void detail_parcelle::on_comboBox_lineType_P_currentIndexChanged(int index)
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
//             ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(index+1)));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
        }
    }
    scene->update();
}

void detail_parcelle::on_toolButton_PenColor_clicked()
{
    // choose pen color
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_Pen->setText(rgb);
        ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor        color   = dialog->currentColor();
                QVariant      variant = color;
                QString       rgb     = variant.toString();
                parcelleItem *item    = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65539)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolyline *item    = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setPenColor(color);
            }
        }
    }
    //items_to_plotlist();

    delete dialog;

    scene->update();
}

void detail_parcelle::on_toolButton_BackgroundColor_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_P_background->setText(rgb);
        ui->label_couleurPolyline_P_background->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor        color   = dialog->currentColor();
                QVariant      variant = color;
                QString       rgb     = variant.toString();
                parcelleItem *item    = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setColor(color);
            }
        }
    }
    delete dialog;
    scene->update();
}

void detail_parcelle::on_pushButton_zoomIn_clicked()
{
    zoomGraphicsView(2);
}

void detail_parcelle::on_pushButton_ZoomOut_clicked()
{
    zoomGraphicsView(3);
}

void detail_parcelle::zoomGraphicsView(int ratio)
{   // ratio value in-> 2  out-> 3)
    if (ratio == 3 && m_ZoomRatio > 0.5)
    {
        m_ZoomRatio = m_ZoomRatio * 0.8;
        ui->graphicsView->scale(0.8, 0.8);
    }
    if (ratio == 2 && m_ZoomRatio < 3)
    {
        m_ZoomRatio = m_ZoomRatio * 1.25;
        ui->graphicsView->scale(1.25, 1.25);
    }
    ui->label_zoom->setText(QString::number(m_ZoomRatio, 'f', 2));
}

void detail_parcelle::on_toolButton_modification_clicked()
{
    m_mode = 0;
    ui->toolButtonUtilisation->setChecked(false);
    ui->toolButton_modification->setChecked(true);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)  // MyItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            item->setMode(0);
        }
        if (itemList[i]->type() == 65537)  // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(0);
        }
        if (itemList[i]->type() == 65539)  // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(0);
        }
    }
}

void detail_parcelle::on_toolButton_Usage_clicked()
{
    m_mode = 1;

    ui->toolButtonUtilisation->setChecked(true);
    ui->toolButton_modification->setChecked(false);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536) // MyItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            item->setMode(1);
        }
        if (itemList[i]->type() == 65537) // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(1);
        }
        if (itemList[i]->type() == 65539) // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(1);
        }
    }
}

void detail_parcelle::on_toolButton_clicked()
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // MyItem
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setNom(util::apos(ui->lineEdit_Nom_item->text()));
                item->setTexte(util::apos(ui->textEdit_plan_commentaires->document()->toPlainText()));
            }
        }
    }
    scene->update();
}

void detail_parcelle::on_toolButton_2_clicked()
{
    close();
}

void detail_parcelle::on_toolButton_delete_clicked()
{   // delete object
    int ret = QMessageBox::warning(this, tr("Delete"),
                                   tr("This object can be deleted.\n"
                                      "Confirm object deletion?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Ok:
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65536) // MyItem
                {
                    parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                    scene->removeItem(item);
                }
            }
        }
        scene->update();
        break;
    }

    case QMessageBox::Cancel:
    {
        break;
    }

    default:
        break;
    }
}

void detail_parcelle::on_toolButton_front_clicked()
{   // bring to front
    if (scene->selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem *         selectedItem = scene->selectedItems().first();
    QList <QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;

    foreach(QGraphicsItem * item, overlapItems)
    {
        zValue = item->zValue() + 0.1;
        qDebug() << zValue;
    }
    selectedItem->setZValue(zValue);
}

void detail_parcelle::on_pushButton_DisplaySheet_clicked()
{
    int Id = ui->lineEdit_Id_Item->text().toInt();

    // int Id = ui->lineEditUuid->text().toInt();

    if (Id >= 0)
    {
        QString   fileName       = getfileNameSQL();
        Cultures *cropSheet = new Cultures(Id, 0);
        cropSheet->show();
    }
}
