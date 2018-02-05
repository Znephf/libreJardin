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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"
#include "ui_database.h"
#include "planning_item.h"

#include <QMessageBox>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsGridLayout>
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>
#include <QtWidgets>
#include <QDialog>
#include <QDir>
#include <QTimerEvent>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

//xml
#include <QDomDocument>

#include "myitem.h"
#include "mybutton.h"
#include "mygrilleitem.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  mode_modifier=false;
  //connexion à la base de données
  QString fileName = QDir::homePath()+"/openjardin/jardin.sqli";     //emplacement de la base de données utilisée
  QString fileName_usr = "/usr/share/openjardin/jardin.sqli";        //emplacement base de données copiées à l'installation

  //test existance du fichier de la base dans homePath()/openjardin pour la première utilisation

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    {
          qDebug() << "création du dossier openJardin en cours";
          QString path = QDir::homePath()+"/openjardin";
          QDir dir(path);
          dir.mkdir(path);
          //copie des fichiers dans le repertoire personnel
          QFile::copy(fileName_usr, fileName);
          QFile::copy("/usr/share/openjardin/jardin.xml",path+"/jardin.xml");
          QFile::copy("/usr/share/openjardin/message.png", path+"/message.png");
          QFile::copy("/usr/share/openjardin/jardin_type.png", path+"/jardin_type.png");
          QFile::copy("/usr/share/openjardin/notice_openJardin.pdf", path+"/notice_openJardin.pdf");
          QFile::copy("/usr/share/openjardin/jardin_exemple.xml",path+"/jardin_exemple.xml");
     }

  createConnection(fileName);

  ui->setupUi(this);
  ui->lineEdit_config_nom_base->setText(fileName);
  scene = new QGraphicsScene(this);
  scene->setSceneRect(0, 0,1900, 1200);
  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex); // indexation des items de la scene

  scene_planning = new QGraphicsScene(this);
  scene_planning->setSceneRect(0, 0,5205, 1200);//5320
  scene_planning->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

  scene_planning2 = new QGraphicsScene(this);
  scene_planning2->setSceneRect(0, 0,100, 1200);
  scene_planning2->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

  scene_planning3 = new QGraphicsScene(this);
  scene_planning3->setSceneRect(0, 0,5320, 100);
  scene_planning3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);


  scene_rotation = new QGraphicsScene(this);
  scene_rotation->setSceneRect(0, 0,5045, 1200);//5140
  scene_rotation->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

  scene_rotation2 = new QGraphicsScene(this);
  scene_rotation2->setSceneRect(0, 0,100, 1200);
  scene_rotation2->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

  scene_rotation3 = new QGraphicsScene(this);
  scene_rotation3->setSceneRect(0, 0,5160, 50);
  scene_rotation3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);


  // stylesheets
  ui->pushButton_Enregistrer_modif_item->setStyleSheet("background-color : #ffff00");
  ui->pushButton_Affiches_fiche->setStyleSheet("background-color : #ffff00");

  qApp->setPalette(this->style()->standardPalette());
  qApp->setStyle(QStyleFactory::create("linux"));
  qApp->setStyleSheet("QLineEdit ,QTextEdit,QTableView { background-color: rgb(255,255,217) }");

  ui->label_16->setStyleSheet("background-color : #ffff00");
  QPalette pal7 = ui->label_16->palette();
  pal7.setColor(QPalette::WindowText , QColor(Qt::blue));
  ui->label_16->setAutoFillBackground(true);
  ui->label_16->setPalette(pal7);
  ui->label_16->update();

  ui->label_17->setStyleSheet("background-color : #ffff00");
  QPalette pal8 = ui->label_17->palette();
  pal8.setColor(QPalette::WindowText , QColor(Qt::red));
  ui->label_17->setAutoFillBackground(true);
  ui->label_17->setPalette(pal8);
  ui->label_17->update();

  QPalette pal9 = ui->label_19->palette();
  pal9.setColor(QPalette::WindowText , QColor(Qt::blue));
  ui->label_19->setPalette(pal9);
  ui->label_19->update();

  QPalette pal10 = ui->label_20->palette();
  pal10.setColor(QPalette::WindowText , QColor(Qt::red));
  ui->label_20->setPalette(pal10);
  ui->label_20->update();

  QList<QColor> colors;
  colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;

  ui->graphicsView->setScene(scene);
  ui->graphicsView_planning->setScene(scene_planning);
  ui->graphicsView_Planning2->setScene(scene_planning2);
  ui->graphicsView_planning3->setScene(scene_planning3);

  ui->graphicsView_rotation->setScene(scene_rotation);
  ui->graphicsView_rotation2->setScene(scene_rotation2);
  ui->graphicsView_rotation3->setScene(scene_rotation3);

  QDate currentDate= QDate::currentDate();
  QString year=QString::number(currentDate.year());
  QString strDate = "01/01/"+year;
  QDate date = QDate::fromString(strDate ,"dd/MM/yyyy");
  int jour=date.dayOfWeek();
  qDebug() << "date "<<currentDate<<" strDate "<<strDate<<" jour"<<jour;
  ui->comboBox_AnneeEnCours->setCurrentText(year);
  affiche_rotation(year.toInt());

  /*******************************************************************************/

  //Inialisation de la table fond d'écran
  ui->tableBackground->setRowCount(1);
  ui->tableBackground->setColumnCount(4); //nb de colonnes
  QStringList headersFond;
  headersFond << "Type" << "PositionX" << "PositionY"  << "chemin";
  ui->tableBackground->setHorizontalHeaderLabels(headersFond);

  //Inialisation de la table des objets
  ui->tableObjets->setRowCount(1);
  ui->tableObjets->setColumnCount(17); //nb de colonnes
  QStringList headers;
  headers << "Id" << "Nom" << "Type" << "PositionX" << "PositionY"  << "SizeX" << "SizeY" << "Brush"
          << "Pen" << "Forme" << "TypeAction"<< "Action"<< "Commentaire" << "Etat" << "Texte"<< "Mode"<< "Chemin";
  ui->tableObjets->setHorizontalHeaderLabels(headers);

  //initialisation de la table des parcelles
  ui->tableWidget_parcelles->setRowCount(0);
  ui->tableWidget_parcelles->setColumnCount(2); //nb de colonnes
  QStringList headersParcelles;
  headersParcelles << "Id" << "Nom";
  ui->tableWidget_parcelles->setHorizontalHeaderLabels(headersParcelles);

  //initialisation de la table des fiches taches
  ui->tableWidget_taches->setRowCount(0);
  ui->tableWidget_taches->setColumnCount(6); //nb de colonnes
  QStringList headersTaches;
  headersTaches << "Id" << "Designation"<<"id_Parcelle"<<"dateDébut"<<"duree"<<"type_tache";
  ui->tableWidget_taches->setHorizontalHeaderLabels(headersTaches);

  // positionnement des scrollbar en haut et à gauche
  ui->graphicsView->horizontalScrollBar()->setSliderPosition(1);

  // si la selection de l'item change lancer la commande
  connect(scene, SIGNAL(selectionChanged()),this, SLOT(Item_clicked()));
  connect(scene_planning, SIGNAL(selectionChanged()),this, SLOT(ItemPlanning_clicked()));
  // passer en mode utilisation
  on_actionUtilisation_triggered();
  get_id();//initialisation du nombre d'objets hors grille

  /***********************initialisation des bases de données sqlite****************/
  init_base();

  /**********************fond d'écran standard**************************************/
  QString fileName2 = QDir::homePath()+"/openjardin/message.png";
  QPixmap pim(fileName2);
  FondItem *pixmap = new FondItem();
  pixmap->setPixmap(pim);
  scene->addItem(pixmap);
  //dimensionnement de la scene selon la taille de pixmap
  double sizeW = pixmap->boundingRect().width();
  double sizeH = pixmap->boundingRect().height();
  scene->setSceneRect(0, 0, sizeW, sizeH);
  // positionnement des scrollbar en haut et à gauche
  QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
  ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
  ui->graphicsView->verticalScrollBar()->setValue(zero.y());
  pixmap->setPixFile(fileName2);
  insererFond(0,0,fileName2,7); //ecriture des valeurs dans tableBackground
  //dessin de la grille (taille,transparence)
  setGrille(0);
  dessine_grille(50,0.3);

  /***********************************************************************************/

  // synchronisation des deux graphicsviews du planning
  connect(ui->graphicsView_planning->verticalScrollBar(), SIGNAL(valueChanged(int)),ui->graphicsView_Planning2->verticalScrollBar(),SLOT(setValue(int)));
  connect(ui->graphicsView_planning->horizontalScrollBar(), SIGNAL(valueChanged(int)),ui->graphicsView_planning3->horizontalScrollBar(),SLOT(setValue(int)));
  connect(ui->graphicsView_rotation->verticalScrollBar(), SIGNAL(valueChanged(int)),ui->graphicsView_rotation2->verticalScrollBar(),SLOT(setValue(int)));
  connect(ui->graphicsView_rotation->horizontalScrollBar(), SIGNAL(valueChanged(int)),ui->graphicsView_rotation3->horizontalScrollBar(),SLOT(setValue(int)));
  affiche_planning(jour);
  //cacher l'onglets "objets" qui contient les tables des objets
  ui->tabWidget_taches->removeTab(4);
}


MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::createConnection(QString fileName)
  {
      QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
      db.setHostName("localhost");
      db.setUserName("root");
      db.setPassword("");
      db.setDatabaseName(fileName);
      if (!db.open())
        {
            qDebug() << "connection database erreur ";
        }
      else
        {
            qDebug() << "database open ";
        }
  }

void MainWindow::init_base()
{  /***********************initialisation des bases de données sqlite******************/

    //ESPECES
    QSqlQueryModel *model1 = new QSqlQueryModel;
    model1->setQuery("SELECT id, designation, famille, positif, negatif, commentaires, compost, rotation_ans,rotation_familles FROM especes ORDER BY designation ASC");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(3, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);
    ui->tableView_especes->setColumnWidth(0,30);
    ui->tableView_especes->setColumnWidth(1,200);
    for(int i=2;i<9;i++)
       {
          ui->tableView_especes->setColumnHidden(i,true);
       }

    //FAMILLES
    QSqlQueryModel *model2 = new QSqlQueryModel;
    model2->setQuery("SELECT id, designation FROM FAMILLES ORDER BY designation ASC");
    model2->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model2->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    ui->tableView_familles->setModel(model2);
    //PLANTES
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation,type_lune,nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes ORDER BY designation ASC");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("commentaires"));
    ui->tableView_plantes->setModel(model3);
    ui->tableView_plantes->setColumnWidth(0,30);
    ui->tableView_plantes->setColumnWidth(1,200);
    for(int i=2;i<9;i++)
       {
          ui->tableView_plantes->setColumnHidden(i,true);
       }

    //TACHES
    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id,designation FROM taches ORDER BY designation ASC");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    ui->tableView_taches->setModel(model4);

    //remplissage combobox familles
    QSqlQueryModel *modelF = new QSqlQueryModel;
    modelF->setQuery("SELECT designation FROM familles ORDER BY designation ASC ");
    ui->comboBox_familles_de_plantes ->setModel(modelF);
    ui->comboBox_familles_de_especes ->setModel(modelF);
    //remplissage combobox especes
    QSqlQueryModel *modelE = new QSqlQueryModel;
    modelE->setQuery("SELECT designation FROM especes ORDER BY designation ASC");
    ui->comboBox_especes_de_plantes->setModel(modelE);

    /**************initialisation des tables des semis récoltes********************/

    for(int x=0;x<ui->tableWidget_culture_printemps->rowCount();x++)
      {
        for(int y=0;y<ui->tableWidget_culture_printemps->columnCount();y++)
          {
            ui->tableWidget_culture_printemps->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_culture_printemps->item(x,y)->setBackgroundColor(Qt::white);
          }
      }
    for(int x=0;x<ui->tableWidget_culture_ete->rowCount();x++)
      {
        for(int y=0;y<ui->tableWidget_culture_ete->columnCount();y++)
          {
            ui->tableWidget_culture_ete->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_culture_ete->item(x,y)->setBackgroundColor(Qt::white);
          }
      }
    for(int x=0;x<ui->tableWidget_culture_automne->rowCount();x++)
      {
        for(int y=0;y<ui->tableWidget_culture_automne->columnCount();y++)
          {
            ui->tableWidget_culture_automne->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_culture_automne->item(x,y)->setBackgroundColor(Qt::white);
          }
      }
    for(int x=0;x<ui->tableWidget_rotation->rowCount();x++)
      {
        for(int y=0;y<ui->tableWidget_rotation->columnCount();y++)
          {
            ui->tableWidget_rotation->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_rotation->item(x,y)->setBackgroundColor(Qt::white);
          }
      }
    for(int x=0;x<ui->tableWidget_rotation_2->rowCount();x++)
      {
        for(int y=0;y<ui->tableWidget_rotation_2->columnCount();y++)
          {
            ui->tableWidget_rotation_2->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(x,y)->setBackgroundColor(Qt::white);
          }
      }
}

/****************remplacement des apostrophes dans les QString pour les requêtes sql*************/

QString MainWindow::apos(QString texte)
{
  texte.replace(QString("'"), QString("''"));//remplacement des apostrophes
  QString texteModif = texte.toUtf8()  ;//conversion
  return texteModif;
}

/*******************************************************************************************/
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressPt.setX(pos().x() - event->x());
    mousePressPt.setY(pos().y() - event->y());
}
// **********************GRILLE **************************//

void MainWindow::dessine_grille(const int espace,const qreal &Opacite)
{
  if(getGrille()==0)
  {
    int space=espace; //espacement de la grille
   //définition du nombre de carrés selon la taille de la scene
   int pasH=(scene->sceneRect().width()/space)+1;
   int pasV=(scene->sceneRect().height()/space)+1;
   for(int h=1;h<pasH;h++)
     {
       for(int v=1;v<pasV;v++)
         {
           mygrilleitem *case1 = new mygrilleitem(space,space);
           case1->setColor(QColor(Qt::white));
           case1->setPenColor(QColor(Qt::blue));
           case1->setPos((h*space)-(space/2),(v*space)-(space/2));
           case1->setOpacity(Opacite);
           scene->addItem(case1);
         }
     }
  }
}

void MainWindow::on_comboBox_AnneeEnCours_currentIndexChanged(const QString &arg1)
{ //le changement de l'année du combobox entraine l'affichage du planning calé au 1er janvier
  QString strDate = "01/01/"+arg1;
  QDate date = QDate::fromString(strDate ,"dd/MM/yyyy");
  int jour=date.dayOfWeek();
  affiche_planning(jour);
}


void MainWindow::on_comboBox_AnneeEnCours_2_currentIndexChanged(const QString &arg1)
{//le changement de l'année du combobox cale le tableau des rotations sur l'année sélectionnée
  QString strDate = "01/01/"+arg1;
  QDate date = QDate::fromString(strDate ,"dd/MM/yyyy");
  int an=date.year();
  affiche_rotation(an);
}

void MainWindow::on_pushButton_color_clicked()
{  //actualiser rotation
  QString strDate = "01/01/"+ui->comboBox_AnneeEnCours_2->currentText();
  QDate date = QDate::fromString(strDate ,"dd/MM/yyyy");
  int an=date.year();
  affiche_rotation(an);
}

/********************************************************************************/
/************************  dessin du tableau rotation  **************************/
/********************************************************************************/
void MainWindow::affiche_rotation(int year)
{
  scene_rotation->clear();
  scene_rotation3->clear();
  // DESSIN DE LA GRILLE DU TABLEAU ROTATION
  int spaceCase=14; //espacement de la grille
  //définition du nombre de carrés selon la taille de la scene
  int pasHCase=(scene_rotation->sceneRect().width()/spaceCase)+1;
  int pasVCase=(scene_rotation->sceneRect().height()/spaceCase)+1;
  //remplissage des cases
  for(int h=1;h<pasHCase;h++)
     {
      for(int v=1;v<pasVCase;v++)
        {
          mygrilleitem *caseP = new mygrilleitem(spaceCase,spaceCase);
          caseP->setColor(QColor(Qt::white));
          caseP->setPenColor(QColor(Qt::blue));
          caseP->setPos((h*spaceCase)-(spaceCase/2),(v*spaceCase)-(spaceCase/2));
          scene_rotation->addItem(caseP);
        }
     }

  int decalage=0; //98
  int mois=1;
  for(int i=1;i<73;i++)
    {
      if (mois==1)
        {
      ajouter_vignette_haut_rotation("Janvier",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==2)
        {
      ajouter_vignette_haut_rotation("Février",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==3)
        {
      ajouter_vignette_haut_rotation("Mars",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==4)
        {
      ajouter_vignette_haut_rotation("Avril",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==5)
        {
      ajouter_vignette_haut_rotation("Mai",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==6)
        {
      ajouter_vignette_haut_rotation("Juin",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==7)
        {
      ajouter_vignette_haut_rotation("Juillet",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==8)
        {
      ajouter_vignette_haut_rotation("Aout",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==9)
        {
      ajouter_vignette_haut_rotation("Septembre",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==10)
        {
      ajouter_vignette_haut_rotation("Octobre",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==11)
        {
      ajouter_vignette_haut_rotation("Novembre",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
        }
      if (mois==12)
        {
      ajouter_vignette_haut_rotation("Décembre",2,decalage,5,Qt::lightGray);
      decalage=decalage+5;
      mois=0;
        }
      mois++;
    }

  decalage=0;
  for(int i=1;i<7;i++)
    {
       ajouter_vignette_haut_rotation(QString::number(year-1+i),1,decalage,60,Qt::lightGray);
       decalage=decalage+60;
    }

  // etiquettes horinzontales des parcelles
  int spaceE=28; //espacement de la grille
  // ajouter_vignette_horizontale("PARCELLES",1*spaceE,97,spaceE/2,Qt::white);
    QString nom_parcelle="";
    int init_pos_vignette=1;
    for(int h=0;h<ui->tableWidget_parcelles->rowCount();h++)
      {
        if(ui->tableWidget_parcelles->rowCount()>-1)
          {
            nom_parcelle=ui->tableWidget_parcelles->item(h,1)->text();

            ajouter_vignette_horiz_rotation(nom_parcelle,((init_pos_vignette+h)*spaceE),97,spaceE,Qt::white);
          }
      }

   //positionner les scrollbar sur le coin haut gauche
   ui->graphicsView_rotation->horizontalScrollBar()->setSliderPosition(1);
   ui->graphicsView_rotation2->horizontalScrollBar()->setSliderPosition(1);
   ui->graphicsView_rotation3->verticalScrollBar()->setSliderPosition(1);
   /*************MISE A JOUR AFFICHAGE DES TACHES SUR LA ROTATION**************/
    mise_a_jour_table_taches(1);// preparation affichage tâches


}

/*********************************************************************************/
/************************  dessin du tableau planning  ***************************/
/*********************************************************************************/
void MainWindow::affiche_planning(int day)
{
  //affiche le planning en démarrant au premier jour de la semaine selon la valeur de "day"
  //en fonction de l'année
  scene_planning->clear();
  scene_planning3->clear();
  QStringList jours_semaine;
  jours_semaine << "L" << "M" << "M" << "J" << "V"  << "S" << "D"<< "L" << "M" << "M" << "J" << "V"  << "S" << "D";

   // DESSIN DE LA GRILLE DU PLANNING
   int spaceCase=14; //espacement de la grille
   //définition du nombre de carrés selon la taille de la scene
   int pasHCase=(scene_planning->sceneRect().width()/spaceCase)+1;
   int pasVCase=(scene_planning->sceneRect().height()/spaceCase)+1;
   //remplissage des cases
   for(int h=1;h<pasHCase;h++)
      {
       for(int v=1;v<pasVCase;v++)
         {
           mygrilleitem *caseP = new mygrilleitem(spaceCase,spaceCase);
           caseP->setColor(QColor(Qt::white));
           caseP->setPenColor(QColor(Qt::blue));
           caseP->setPos((h*spaceCase)-(spaceCase/2),(v*spaceCase)-(spaceCase/2));
           scene_planning->addItem(caseP);
         }
      }
   // etiquettes horinzontales des parcelles
   int spaceE=28; //espacement de la grille
   // ajouter_vignette_horizontale("PARCELLES",1*spaceE,97,spaceE/2,Qt::white);
     QString nom_parcelle="";
     int init_pos_vignette=1;
     for(int h=0;h<ui->tableWidget_parcelles->rowCount();h++)
       {
         if(ui->tableWidget_parcelles->rowCount()>-1)
           {
             nom_parcelle=ui->tableWidget_parcelles->item(h,1)->text();
             ajouter_vignette_horizontale(nom_parcelle,((init_pos_vignette+h)*spaceE),97,spaceE,Qt::white);
           }
       }
   // remplissage des étiquettes semaines
   QString texte_date="semaine ";
   int WidthSemaine=spaceCase*7;
   int HeightSemaine=35;
   int NbSemaines=((scene_planning3->sceneRect().width()-100)/WidthSemaine)+1;
   int jour=1; //lundi
   int decal=101-((day-1)*14); //décalage des étiquettes semaines
   //ajout semaine 52 année précédente
   ajouter_vignette_verticale("S "+QString::number(52),WidthSemaine+4,WidthSemaine,HeightSemaine-14,Qt::white);
   for(int i=1;i<8;i++)
     ajouter_vignette_jour("",((i-1)*spaceCase)+4,HeightSemaine,spaceCase,spaceCase,Qt::lightGray);

   for(int v=2;v<NbSemaines+2;v++)
     {
         ajouter_vignette_verticale(texte_date+QString::number(v-1),(v*WidthSemaine-((day-1)*14))+3,WidthSemaine,HeightSemaine-14,Qt::white);
         for(int i=jour;i<8;i++)
           {
               QString Jour=jours_semaine[i+-1];
               ajouter_vignette_jour(Jour,decal+((i-1)*spaceCase),HeightSemaine,spaceCase,spaceCase,Qt::yellow);
           }
         decal=decal+(WidthSemaine);
     }
   // Etiquettes mois
   //définition du nombre de carrés selon la taille de la scene
  int pasHM=(scene_planning3->sceneRect().width()/spaceCase)+1;
  int drap2=1;
  int saut=1;
  int mois=1;
  int decalage=100; //98
  int nbJours=31;
  int jour_depart=1;
  //ajout décembre année précédente
  ajouter_vignette_mois("Decembre",3,8,Qt::lightGray);
  for(int i=24;i<31;i++)
    {
      ajouter_vignette_jour(QString::number(i+1),((i-24)*spaceCase)+4,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
    }
   for(int v=1;v<pasHM;v++)

   {
       if(v>5)
         {
           if (drap2==0)
             {
              if(mois==1)
                {

                  ajouter_vignette_mois("Janvier",(decalage),nbJours,Qt::yellow);

                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==2)
                {
                nbJours=28;
                ajouter_vignette_mois("Février",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
               decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==3)
                {
                  nbJours=31;
                ajouter_vignette_mois("Mars",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==4)
                {
                  nbJours=30;
                ajouter_vignette_mois("Avril",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==5)
                {
                  nbJours=31;
                ajouter_vignette_mois("Mai",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==6)
                {
                  nbJours=30;
                ajouter_vignette_mois("Juin",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==7)
                {
                  nbJours=31;
                ajouter_vignette_mois("Juillet",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==8)
                {
                  nbJours=31;
                ajouter_vignette_mois("Aout",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==9)
                {
                  nbJours=30;
                ajouter_vignette_mois("Septembre",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==10)
                {
                  nbJours=31;
                ajouter_vignette_mois("Octobre",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==11)
                {
                  nbJours=30;
                ajouter_vignette_mois("Novembre",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==12)
                {
                  nbJours=31;
                ajouter_vignette_mois("Décembre",(decalage),nbJours,Qt::lightGray);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::lightGray);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              if(mois==13)
                {
                  nbJours=31;
                ajouter_vignette_mois("Janvier",(decalage),nbJours,Qt::yellow);
                for(int i=jour_depart-1;i<nbJours;i++)
                  {
                    ajouter_vignette_jour(QString::number(i+1),decalage+(i*spaceCase)+1,HeightSemaine+spaceCase,spaceCase,spaceCase,Qt::yellow);
                  }
                decalage=decalage+(nbJours*spaceCase);
                }
              mois++;
              drap2=1;
              saut=1;

             }
           else
             {
               if(saut<7)
                 {
                 saut++;
                 }
               else
                 {
                   drap2=0;
                 }
             }
         }
     }
    //positionner les scrollbar sur le coin haut gauche
     ui->graphicsView_planning->horizontalScrollBar()->setSliderPosition(1);
     ui->graphicsView_Planning2->horizontalScrollBar()->setSliderPosition(1);
     ui->graphicsView_planning3->verticalScrollBar()->setSliderPosition(1);
    /*************MISE A JOUR AFFICHAGE DES TACHES SUR LE PLANNING**************/
     mise_a_jour_table_taches(0);// preparation affichage tâches

    /****************afficher repére jour courant*******************************/
     QString strDate = ui->comboBox_AnneeEnCours->currentText()+".01.01";
     QDate dateR = QDate::fromString(strDate ,"yyyy.MM.dd");
     QDate currentDate= QDate::currentDate();
     ui->lineEdit_dateDuJour->setText(currentDate.toString());
     int Nbjour=(dateR.daysTo(currentDate)*14);
     ajouter_repere_jour("",Nbjour+7,1,1,scene_planning->sceneRect().height(),Qt::red);
     QPointF date = ui->graphicsView_planning->mapFromScene(QPointF(Nbjour*14,0));
     ui->graphicsView_planning->centerOn(date);
 }



/*******************************************************************************/
/*********************SAUVEGARDE DU TABLEAU DANS UN FICHIER XML*****************/
/*******************************************************************************/

// sauver les données de la table dans un fichier XML
void MainWindow::on_actionSauver_triggered()
{
  QString cellvalue;
  QString champ;
  items_vers_tableau();
  // vérification des cellules du tableau des objets
  for(int i = 0; i < ui->tableObjets->rowCount(); i++)
  {
      for(int j = 0; j < ui->tableObjets->columnCount(); j++)
        {
          QTableWidgetItem *cellItem = ui->tableObjets->item(i,j);
          if (cellItem == 0)
            {
               ui->tableObjets->setItem(i, j,new QTableWidgetItem("xx") );
            }
         }
   }
  //suppression des objets de type case
  for(int i = ui->tableObjets->rowCount()-1; i >-1 ; i--)
    {
     if (ui->tableObjets->item(i,1)->text()=="xx" && i<ui->tableObjets->rowCount())
       {
         ui->tableObjets->removeRow(i);
         qDebug() << "remove ok"<< i;
       }
    }
  // vérification des cellules du tableau du background
  for(int i = 0; i < ui->tableBackground->rowCount(); i++)
    {
      for(int j = 0; j < ui->tableBackground->columnCount(); j++)
        {
          QTableWidgetItem *cellItem = ui->tableBackground->item(i,j);
          if (cellItem == 0)
            {
               ui->tableBackground->setItem(i, j,new QTableWidgetItem("xx") );
            }
         }
     }

  // Objet de base servant à la création de notre fichier XML
  QDomDocument document;

  // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
  QDomNode xmlNode = document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

  // On insère cet en-tête avant le premier enfant
  document.insertBefore(xmlNode, document.firstChild());

  // Création de l'élément <root>
  QDomElement root = document.createElement("root");

  // On ajoute l'élément <root> en tant que premier enfant du document
  document.appendChild(root);
  // Création de l'élément <base> pixmap du fond de l'écran
  QDomElement base = document.createElement("base");

  // On ajoute l'élément <background> en tant que premier enfant de l'élément <root>
  root.appendChild(base);

  // Création de l'élément <background> pixmap du fond de l'écran
  QDomElement background = document.createElement("background");

  // On ajoute l'élément <background> en tant que premier enfant de l'élément <root>
  root.appendChild(background);

  // Création de l'élément <objets>
  QDomElement objets = document.createElement("objets");

  // On ajoute l'élément <nom> en tant que premier enfant de l'élément <objets>
  root.appendChild(objets);

  //Ecriture du nom du fichier de la base de données sqlite
  QDomElement fichier_base = document.createElement("fichier_base");
  cellvalue = ui->lineEdit_config_nom_base->text();
  champ = "fichier";
  fichier_base.setAttribute(champ, cellvalue);
  base.appendChild(fichier_base);


  //Ecriture du background
  QDomElement fond = document.createElement("fond");
     //colonnes
      for(int j = 0; j < ui->tableBackground->columnCount(); j++)
        {
          QTableWidgetItem *myvalue = ui->tableBackground->item(0,j);
          cellvalue = myvalue->text();
          QTableWidgetItem *header = ui->tableBackground->horizontalHeaderItem(j);
          champ = header->text();
          fond.setAttribute(champ, cellvalue);
          background.appendChild(fond);
         }

  //Ecriture des objets
       // Ajouter les données du tableau dans le document DOM
       for(int i = 0; i < ui->tableObjets->rowCount(); i++)
       {
       QDomElement objet = document.createElement("Objet");
          //colonnes
           for(int j = 0; j < ui->tableObjets->columnCount(); j++)
             {
               QTableWidgetItem *myvalue = ui->tableObjets->item(i,j);
               cellvalue = myvalue->text();
               QTableWidgetItem *header = ui->tableObjets->horizontalHeaderItem(j);
               champ = header->text();
               objet.setAttribute(champ, cellvalue);
               objets.appendChild(objet);
              }
        }

   // Ecriture dans le fichier
     QString repertoire = QDir::homePath()+"/openjardin";

        QString fileName =
            QFileDialog::getSaveFileName(this, tr("Sauvegarde du tableau des objets"),
                                     repertoire,
                                     tr("XML Files (*.xml)"));
        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
          QMessageBox msgBox;
          msgBox.setText("ERREUR D'ÉCRITURE");
            return;
        }
        else
        {
            QTextStream stream(&file);
            stream << document.toString();
            file.close();
        }
}
/*****************************************************************************/
/***** lire les données du fichier XML et les importer dans la table**********/
/*****************************************************************************/


void MainWindow::on_actionOuvrir_triggered()
{
  QString champ;
  // création du document pour lire XML
  QDomDocument document;

  // ouverture du fichier pour lecture
  QString repertoire = QDir::homePath()+"/openjardin";

  QString fileName =
          QFileDialog::getOpenFileName(this, tr("Ouverture du fichier des objets"),
                                       repertoire,
                                       tr("XML Files (*.xml)"));
  if (fileName.isEmpty())
      return;

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    {
      QMessageBox msgBox;
      msgBox.setText("ERREUR DE LECTURE");
      return;
    }
  else
    {
           if(!document.setContent(&file))
              {
                  //Erreur de lecture du fichier";
              }
           else
              {
                  // Element root
                  ui->lineEdit_config_nom_Xml->setText(fileName);
                  QDomElement root = document.firstChildElement();
                  QDomNodeList base = root.elementsByTagName("fichier_base");
                  QDomNodeList fond = root.elementsByTagName("fond");
                  QDomNodeList objet = root.elementsByTagName("Objet");
                  ui->tableBackground->setRowCount(1); //fixer le nombre de lignes du tableau selon nombre d'objets
                  ui->tableBackground->setColumnCount(4); //nombre de colonnes fixe
                  ui->tableObjets->setRowCount(objet.count()); //fixer le nombre de lignes du tableau selon nombre d'objets
                  ui->tableObjets->setColumnCount(17); //nombre de colonnes fixe

                  // nom du fichier de la base sqlite qui correspond à la configuration XML
                  QDomNode basenode = base.at(0);
                  if(basenode.isElement())
                    {
                       QDomElement base = basenode.toElement();
                       ui->lineEdit_config_nom_base->setText(base.attribute("fichier"));
                       qDebug() << "boucle base";
                     }

                  // écriture dans le tableau de l'objet fond d'écran
                  QDomNode fondnode = fond.at(0);
                  if(fondnode.isElement())
                  {
                     // qDebug() << "boucle fond element";
                      QDomElement fond = fondnode.toElement();
                      ui->tableBackground->setItem(0, 0, new QTableWidgetItem(fond.attribute("Type")));
                      ui->tableBackground->setItem(0, 1, new QTableWidgetItem(fond.attribute("PositionX")));
                      ui->tableBackground->setItem(0, 2, new QTableWidgetItem(fond.attribute("PositionY")));
                      ui->tableBackground->setItem(0, 3, new QTableWidgetItem(fond.attribute("chemin")));
                  }
                  // boucle d'écriture de chaque objet dans le tableau
                  for(int i = 0; i < objet.count(); i++)
                    {
                        QDomNode objetnode = objet.at(i);
                        if(objetnode.isElement())
                        {
                            QDomElement objet = objetnode.toElement();
                            for(int j = 0; j < ui->tableObjets->columnCount(); j++)
                              {
                                QTableWidgetItem *header = ui->tableObjets->horizontalHeaderItem(j); //header des noms des colonnes
                                champ = header->text();
                                ui->tableObjets->setItem(i, j, new QTableWidgetItem(objet.attribute(champ)));
                              }
                         }
                     }
                  if(objet.count()>0)
                   {
                      tableau_vers_items();
                      qDebug() << "Lecture du fichier terminée";
                   }
                }
              file.close();
    }
  // positionnement des scrollbar en haut et à gauche
  QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
  ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
  ui->graphicsView->verticalScrollBar()->setValue(zero.y());
  if(ui->actionUtilisation->isChecked())
    {
      on_actionUtilisation_triggered();
    }
  else
    {
      on_actionModification_plan_triggered();
    }
  ui->lineEdit_ItemId->setText(QString::number(get_id()));
}

/**********************************************************/

void MainWindow::on_lineEdit_config_nom_base_textChanged(const QString &arg1)
{ // changement du nom de la base de données suite à lecture du fichier de configuration XML
  createConnection(arg1);
  init_base();
}


//VARIANTE D'OBJET CREE PixmapItem


void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    mousePressPt.setX(pos().x() - event->scenePos().x());
    mousePressPt.setY(pos().y() - event->scenePos().y());
    setSelected(true);
}
void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setX(mousePressPt.x() + event->scenePos().x());
    setY(mousePressPt.y() + event->scenePos().y());
    event->accept();
}
void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    setX(mousePressPt.x() + event->scenePos().x());
    setY(mousePressPt.y() + event->scenePos().y());
    event->accept();
}

void PixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    qDebug() << "hover event mouse";
    setToolTip(getComment());
    event->accept();
}

QString PixmapItem::getPixFile()
{
    return pixmapfileName;
}

void PixmapItem::setPixFile(QString file)
{
    pixmapfileName =file;

}

int PixmapItem::getTypeAction()
{
    return typeAction;
}

void PixmapItem::setTypeAction(int typeAct)
{
    typeAction =typeAct;

}

QString PixmapItem::getAction()
{
    return action;
}

void PixmapItem::setAction(QString act)
{
    action =act;
}

void PixmapItem::setTexte(const QString text)
  {
    itemText = text;
  }

QString PixmapItem::getTexte()
  {
    return itemText;
  }
void PixmapItem::setComment(const QString comment)
  {
    itemComment = comment;
  }

QString PixmapItem::getComment()
  {
    return itemComment;
  }

/**********************************************************/
//VARIANTE D'OBJET CREE FondItem
QString FondItem::getPixFile()
{
    return pixmapfileName;
}

void FondItem::setPixFile(QString file)
{
    pixmapfileName =file;
}

/****************************ACTIONS AJOUTS D'OBJET***************************************/

void MainWindow::on_actionAjouterRectangle_triggered()
{ //parcelle
  MyItem *item = new MyItem(50,50);
  item->setColor(QColor(Qt::gray));
  item->setPenColor(QColor(Qt::red));
  item->setOpac(0.8); //opacité de l'item
  item->setId(ui->lineEdit_ItemId->text().toInt());// valeur id en dernière position dans la table
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(sizeW/2,sizeH/2);
  item->setTexte("Parcelle");
  item->setNom("Parcelle "+ui->lineEdit_ItemId->text());
  item->setEtat(1);
  item->setMode(0);
  item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  scene->addItem(item);
  QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
  ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
  ui->graphicsView->verticalScrollBar()->setValue(zero.y());
  QList <QGraphicsItem*> itemList = scene->items();
  items_vers_tableau();

}
void MainWindow::on_actionAjouter_Rectangle_arrondi_triggered()
{
  MyItem *item = new MyItem(50,50);
  item->setColor(QColor(Qt::cyan));
  item->setPenColor(QColor(Qt::black));
  item->setId(ui->lineEdit_ItemId->text().toInt());// valeur id en dernière position dans la table
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(sizeW/2,sizeH/2);
  item->setNom(" "+ui->lineEdit_ItemId->text());
  item->setTexte(" ");
  item->setEtat(1);
  item->setMode(0);
  item->setTypeShape(2);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  scene->addItem(item);
  QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
  ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
  ui->graphicsView->verticalScrollBar()->setValue(zero.y());
  QList <QGraphicsItem*> itemList = scene->items();
  items_vers_tableau();
}

void MainWindow::on_actionAjouterCercle_triggered()
{
  MyItem *item = new MyItem(50,50);
  item->setColor(QColor(Qt::green));
  item->setPenColor(QColor(Qt::black));
   item->setOpac(0.8); //opacité de l'item
  item->setId(ui->lineEdit_ItemId->text().toInt());// valeur id en dernière position dans la table
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(sizeW/2,sizeH/2);
  item->setNom("");
  item->setTexte("");
  item->setEtat(1);
  item->setMode(0);
  item->setTypeShape(3);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  scene->addItem(item);
  QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
  ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
  ui->graphicsView->verticalScrollBar()->setValue(zero.y());
  QList <QGraphicsItem*> itemList = scene->items();
  items_vers_tableau();
}

void MainWindow::on_actionAjouterImage_triggered()
{
   //utilisation de myItem avec painter drawImage
    int ObjetPosX=10;
    int ObjetPosY=10;
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Ouverture du fichier image"),
                                         QDir::currentPath(),
                                         tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
        return;
    else
      {
        MyItem *item = new MyItem(32,32);
        item->setId(ui->lineEdit_ItemId->text().toInt());// valeur id en dernière position dans la table
        item->setPixFile(fileName);
        item->setPos(ObjetPosX,ObjetPosY);
        item->setTexte(" ");
        item->setEtat(1);
        item->setMode(0);
        item->setTypeShape(4);//4=image
        scene->addItem(item);
        qDebug() << "Fichier de l'image: " + item->getPixFile();
      }

    items_vers_tableau();

}
/**************************************item rotation*********************************/
void MainWindow::ajouter_vignette_horiz_rotation(QString titre,int ligne,int width,int height,QColor couleur)
{
  MyItem *item2 = new MyItem(width,height);
  item2->setColor(couleur);
  item2->setPenColor(QColor(Qt::black));
  item2->setOpac(1.0); //opacité de l'item
  double sizeW2 = item2->boundingRect().width();
  double sizeH2 = item2->boundingRect().height();
  item2->setPos(sizeW2/2,ligne-(sizeH2/2)-1);
  item2->setNom(titre);
  item2->setEtat(1);
  item2->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  item2->setMode(1);//mode utilisation
  scene_rotation2->addItem(item2);// bandeau parcelles

}
void MainWindow::ajouter_vignette_rotation(int culture, QString titre,int ligne,int colonne,int nbCases,QColor couleur)
{  //titre,nb lignes,colonne de depart,nb cases,couleur

 int spaceCase=14;
 int lg=(ligne)*(spaceCase*2);
 int col=(colonne+7)*spaceCase;

  MyItem *item = new MyItem(nbCases*14,28);
  item->setId(culture);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::black));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(col+(sizeW/2)-1,lg-(sizeH/2)-1);
  item->setNom(titre);
  item->setEtat(1);
  item->setMode(1); //déplacements imossibles
  item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  scene_rotation->addItem(item);//bandeau central
}
void MainWindow::ajouter_vignette_haut_rotation(QString titre,int ligne,int colonne,int nbCases,QColor couleur)
{  // années et mois

  int spaceCase=14;  //14
  int lg=(ligne)*(spaceCase*2);
  int col=(colonne+7)*spaceCase;
   MyItem *item = new MyItem(nbCases*14,28);
   item->setColor(couleur);
   item->setPenColor(QColor(Qt::black));
   double sizeW = item->boundingRect().width();
   double sizeH = item->boundingRect().height();
   item->setPos(col+(sizeW/2)+2,(lg-(sizeH/2)));
   item->setNom(titre);
   item->setEtat(1);
   item->setMode(2); //déplacements possibles
   item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle

   scene_rotation3->addItem(item); // bandeau haut horizontal
}
void MainWindow::ajouter_repere_haut_rotation(QString titre,int ligne,int colonne,int nbCases,QColor couleur)
{  // années et mois

  int spaceCase=28;  //14
  int lg=(ligne)*(spaceCase*2);

  int col=(colonne+7)*spaceCase;

   MyItem *item = new MyItem(nbCases*2,56);
   item->setColor(couleur);
   item->setPenColor(QColor(Qt::black));
   double sizeW = item->boundingRect().width();
   double sizeH = item->boundingRect().height();
   item->setPos(col+(sizeW/2)+2,(lg-(sizeH/2)));
   item->setNom(titre);
   item->setEtat(1);
   item->setMode(2); //déplacements possibles
   item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
   scene_rotation->addItem(item);; // bandeau haut horizontal
}
void MainWindow::ajouter_repere_jour_rotation(QString titre,int colonne,int ligne,int width,int height,QColor couleur)
{  // repére du jour courant
  mygrilleitem *item = new mygrilleitem(width,height);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::red));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(colonne+(sizeW/2),ligne+sizeH/2);
  item->setText(titre);
  scene_planning->addItem(item);
}

/***************************************item du planning******************************/
void MainWindow::ajouter_vignette_horizontale(QString titre,int ligne,int width,int height,QColor couleur)
{
  MyItem *item2 = new MyItem(width,height);
  item2->setColor(couleur);
  item2->setPenColor(QColor(Qt::black));
  item2->setOpac(1.0); //opacité de l'item
  double sizeW2 = item2->boundingRect().width();
  double sizeH2 = item2->boundingRect().height();
  item2->setPos(sizeW2/2,ligne-(sizeH2/2)-1);
  item2->setNom(titre);
  item2->setEtat(1);
  item2->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  item2->setMode(1);//mode utilisation
  scene_planning2->addItem(item2);

}
void MainWindow::ajouter_vignette_verticale(QString titre,int colonne,int width,int height,QColor couleur)
{  // semaines

  planning_item *item = new planning_item(width,height);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::black));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(colonne-(sizeW/2)-1,20+sizeH/2);
  item->setNom(titre);
  item->setEtat(1);
  item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  item->setMode(1);//mode utilisation
  scene_planning3->addItem(item);


}
void MainWindow::ajouter_vignette_planning(int culture,QString titre,int ligne,int colonne,int nbCases,QColor couleur)
{  //titre,nb lignes,colonne de depart,nb cases,couleur

 int spaceCase=14;
 int lg=(ligne)*(spaceCase*2);
 int col=(colonne+7)*spaceCase;

  MyItem *item = new MyItem(nbCases*14,28);
  item->setId(culture);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::black));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(col+(sizeW/2)-1,lg-(sizeH/2)-1);
  item->setNom(titre);
  item->setEtat(1);
  item->setMode(2); //déplacements possibles
  item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  scene_planning->addItem(item);
}

void MainWindow::ajouter_vignette_mois(QString titre,int colonne,int nb_jours,QColor couleur)
{
  planning_item *item = new planning_item(nb_jours*14,20);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::black));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(colonne+(sizeW/2),sizeH/2);
  item->setNom(titre);
  item->setEtat(1);
  item->setTypeShape(1);// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
  item->setMode(1);//mode utilisation
  scene_planning3->addItem(item);
}

void MainWindow::ajouter_vignette_jour(QString titre,int colonne,int ligne,int width,int height,QColor couleur)
{  // semaines

  mygrilleitem *item = new mygrilleitem(width,height);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::black));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(colonne+(sizeW/2),ligne+sizeH/2);
  item->setText(titre);
  scene_planning3->addItem(item);
}
void MainWindow::ajouter_repere_jour(QString titre,int colonne,int ligne,int width,int height,QColor couleur)
{  // repére du jour courant
  mygrilleitem *item = new mygrilleitem(width,height);
  item->setColor(couleur);
  item->setPenColor(QColor(Qt::red));
  double sizeW = item->boundingRect().width();
  double sizeH = item->boundingRect().height();
  item->setPos(colonne+(sizeW/2),ligne+sizeH/2);
  item->setText(titre);
  scene_planning->addItem(item);
}
/************************************************************************************************************/

int MainWindow::get_id()
{ //récupération du nombre de ligne du tableau des objets

  items_vers_tableau();
  // vérification des cellules du tableau des objets
  for(int i = 0; i < ui->tableObjets->rowCount(); i++)
  {
      for(int j = 0; j < ui->tableObjets->columnCount(); j++)
        {
          QTableWidgetItem *cellItem = ui->tableObjets->item(i,j);
          if (cellItem == 0)
            {
               ui->tableObjets->setItem(i, j,new QTableWidgetItem("xx") );
            }
         }
   }
  //suppression des objets de type case
  for(int i = ui->tableObjets->rowCount()-1; i >-1 ; i--)
    {
      if (ui->tableObjets->item(i,1)->text()=="xx" && i<ui->tableObjets->rowCount())
        {
          ui->tableObjets->removeRow(i);
        }
    }
  m_id=ui->tableObjets->rowCount();
  return m_id;
}

/***************************ajout des items de la scene dans le tableau**************************************/

void MainWindow::items_vers_tableau()
{

  int nbRow=0;
  QList <QGraphicsItem*> itemList = scene->items();
  ui->tableObjets->setRowCount(itemList.size());
  int row_parcelles=0;
   for(int i=0; i<itemList.size(); i++)
     {
        QPointF location = itemList[i]->pos();
        int valX = location.x();
        int valY = location.y();
        int sizeW = itemList[i]->boundingRect().width();
        int sizeH = itemList[i]->boundingRect().height();
          MyItem* item = dynamic_cast<MyItem*> (itemList[i]);


    if(itemList[i]->type() == 65536) // MyItem
    {
        ui->tableObjets->setItem(i, 0,new QTableWidgetItem(QString::number(item->getId())));//Id
        ui->tableObjets->setItem(i, 1,new QTableWidgetItem(item->getNom()));
        ui->tableObjets->setItem(i, 2, new QTableWidgetItem(QString::number(itemList[i]->type())));//Type
        ui->tableObjets->setItem(i, 3,new QTableWidgetItem(QString::number(valX)));//PositionX
        ui->tableObjets->setItem(i, 4,new QTableWidgetItem(QString::number(valY)));//PositionY
        ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(sizeW)));//width
        ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(sizeH)));//height
        QColor clr = item->getColor();
        ui->tableObjets->setItem(i,7,new QTableWidgetItem(QString::number(clr.rgb())));//brush color
        QColor clrPen = item->getPenColor();
        ui->tableObjets->setItem(i,8,new QTableWidgetItem(QString::number(clrPen.rgb())));//pen color
        int form = item->getTypeShape();
        ui->tableObjets->setItem(i,9,new QTableWidgetItem(QString::number(form)));//type shape/forme
        ui->tableObjets->setItem(i,10,new QTableWidgetItem(QString::number(item->getTypeAction())));//type action
        ui->tableObjets->setItem(i,11,new QTableWidgetItem(item->getAction()));//action
        ui->tableObjets->setItem(i,12,new QTableWidgetItem(item->getComment()));//commentaire
        ui->tableObjets->setItem(i,13,new QTableWidgetItem(QString::number(item->getEtat())));//Etat
        ui->tableObjets->setItem(i,14,new QTableWidgetItem(item->getTexte()));//texte
        ui->tableObjets->setItem(i,15,new QTableWidgetItem(QString::number(item->getMode())));//Mode
        ui->tableObjets->setItem(i, 16,new QTableWidgetItem(""));// chemin du fichier vide
        ui->tableObjets->setItem(i, 16,new QTableWidgetItem(item->getPixFile()));//chemin du fichier
        // COPIE VERS tableWidget_parcelles
        if(ui->tableObjets->item(i,9)->text().toInt()==1)
          {
          ui->tableWidget_parcelles->setRowCount(row_parcelles+1);
          ui->tableWidget_parcelles->setItem(row_parcelles, 0,new QTableWidgetItem(QString::number(item->getId())));//Id
          ui->tableWidget_parcelles->setItem(row_parcelles, 1,new QTableWidgetItem(item->getNom()));
          row_parcelles++;
          }
        nbRow++;
      }
    else
      {
       if(itemList[i]->type() == 7) //IMAGES
        {
       //ecriture dans le tableau du chemin du fichier
         PixmapItem *as_pixmapitem;
         FondItem *as_fonditem;
          if((as_pixmapitem = dynamic_cast<PixmapItem*>(itemList[i])))
           {
           ui->tableObjets->setItem(i, 0,new QTableWidgetItem(QString::number(item->getId())));//Id
           ui->tableObjets->setItem(i, 1,new QTableWidgetItem(item->getNom()));
           ui->tableObjets->setItem(i, 2, new QTableWidgetItem(QString::number(itemList[i]->type())));//Type
           ui->tableObjets->setItem(i, 3,new QTableWidgetItem(QString::number(valX)));//PositionX
           ui->tableObjets->setItem(i, 4,new QTableWidgetItem(QString::number(valY)));//PositionY
           ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(sizeW)));//width
           ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(sizeH)));//height
           PixmapItem* pItem = dynamic_cast<PixmapItem*> (itemList[i]);
           ui->tableObjets->setItem(i,9,new QTableWidgetItem(QString::number(4)));//type shape/forme
           ui->tableObjets->setItem(i,14,new QTableWidgetItem(pItem->getTexte()));//texte
           ui->tableObjets->setItem(i,10,new QTableWidgetItem(QString::number(pItem->getTypeAction())));//type action
           ui->tableObjets->setItem(i,11,new QTableWidgetItem(pItem->getAction()));//action
           ui->tableObjets->setItem(i,12,new QTableWidgetItem(pItem->getComment()));//commentaire
           ui->tableObjets->setItem(i,13,new QTableWidgetItem(QString::number(pItem->getEtat())));//Etat
            ui->tableObjets->setItem(i,14,new QTableWidgetItem(pItem->getTexte()));//texte
           ui->tableObjets->setItem(i,15,new QTableWidgetItem(QString::number(pItem->getMode())));//Mode
           ui->tableObjets->setItem(i, 16,new QTableWidgetItem(pItem->getPixFile()));//chemin du fichier
           //mise au minimum de la taille de l'image dans le tableau
           ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(32)));//width
           ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(32)));//height
           nbRow++;
         }
       else if((as_fonditem = dynamic_cast<FondItem*>(itemList[i])))
         {
           // ne rien faire car le fond est enregistré dans son tableau "tablebackground"
            ui->tableObjets->setRowCount(itemList.size()-1);
         }
       }
     }
   }
   //recherche date courante et calage du planning et rotations
   QDate currentDate= QDate::currentDate();
   QString year=QString::number(currentDate.year());
   QString strDate = "01/01/"+year;
   QDate date = QDate::fromString(strDate ,"dd/MM/yyyy");
   int jour=date.dayOfWeek();
   qDebug() << "date "<<currentDate<<" strDate "<<strDate<<" jour"<<jour;
   ui->comboBox_AnneeEnCours->setCurrentText(year);
   affiche_rotation(year.toInt());
   affiche_planning(jour); //mise à jour de la page planning

   ui->lineEdit_ItemId->setText(QString::number(nbRow));
   ui->tableObjets->setRowCount(nbRow);
}

/*******Ajout d'objet dans la liste des items à partir du tableau********************/

void MainWindow::tableau_vers_items()
{ //Ajout d'objet dans la liste des items à partir du tableau
   scene->clear();

  QString cellvalue;
  //insertion du fond d'écran
  QTableWidgetItem *myvalue = ui->tableBackground->item(0,0);
  cellvalue = myvalue->text();
  if(cellvalue == "7")
    { //image
      QString fileName;
      QTableWidgetItem *filename = ui->tableBackground->item(0,3);
       QPixmap pim(filename->text());
      FondItem *pixmap = new FondItem();
      pixmap->setPixmap(pim);
      scene->addItem(pixmap);
      pixmap->setPixFile(fileName);
      double sizeW = pixmap->boundingRect().width();
      double sizeH = pixmap->boundingRect().height();
      scene->setSceneRect(0, 0, sizeW, sizeH);
      QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
      ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
      ui->graphicsView->verticalScrollBar()->setValue(zero.y());

    }
  //dessin de la grille (taille,transparence)
  dessine_grille(30,0.1);
  //insertion des objets
  // inverser les objets dans le tableau
 for(int i = ui->tableObjets->rowCount()-1; i >-1 ; i--)
  {
      QTableWidgetItem *myvalueId= ui->tableObjets->item(i,0);
      int ObjetId = myvalueId->text().toInt();
      QTableWidgetItem *opNom = ui->tableObjets->item(i,1);
      QString ObjetNom=opNom->text();
      QTableWidgetItem *myvalue = ui->tableObjets->item(i,2);
      cellvalue = myvalue->text();
      QTableWidgetItem *opX = ui->tableObjets->item(i,3);
      int ObjetPosX=opX->text().toInt();
      QTableWidgetItem *opY = ui->tableObjets->item(i,4);
      int ObjetPosY=opY->text().toInt();
      QTableWidgetItem *opW = ui->tableObjets->item(i,5);
      int ObjetWidth=opW->text().toInt();
      QTableWidgetItem *opH = ui->tableObjets->item(i,6);
      int Objetheight=opH->text().toInt();
      QTableWidgetItem *TypeShape = ui->tableObjets->item(i,9);
      QTableWidgetItem *TypeAction = ui->tableObjets->item(i,10);
      QTableWidgetItem *Action = ui->tableObjets->item(i,11);
      QTableWidgetItem *opComment = ui->tableObjets->item(i,12);
      QString ObjetComment=opComment->text();
      QTableWidgetItem *opTexte = ui->tableObjets->item(i,14);
      QString ObjetTexte=opTexte->text();
      QTableWidgetItem *ObjetEtat = ui->tableObjets->item(i,13);
      QTableWidgetItem *ObjetMode = ui->tableObjets->item(i,15);

      if(cellvalue == "65536") //Myitem
        {
          QString fileName;
          QTableWidgetItem *filename = ui->tableObjets->item(i,16);
          QTableWidgetItem *Couleur = ui->tableObjets->item(i,7);
          QColor couleurfond=Couleur->text().toDouble();
          QTableWidgetItem *CouleurPen = ui->tableObjets->item(i,8);
          QColor couleurpen=CouleurPen->text().toDouble();
          MyItem *item = new MyItem(ObjetWidth,Objetheight);
          QPen pen(Qt::blue, 1);
          item->setColor(QColor(couleurfond));
          item->setPenColor(QColor(Qt::black));
          item->setOpac(0.8);
          item->setPos(ObjetPosX,ObjetPosY);
          item->setTexte(ObjetTexte);
          item->setTypeAction(TypeAction->text().toInt());
          item->setAction(Action->text());
          item->setTypeShape(TypeShape->text().toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
          item->setComment(ObjetComment);
          item->setPenColor(QColor(couleurpen));
          item->setPixFile(filename->text());
          item->setEtat(ObjetEtat->text().toInt());
          item->setMode(ObjetMode->text().toInt());
          item->setId(ObjetId);
          item->setNom(ObjetNom);
          scene->addItem(item);
        }

      if(cellvalue == "7")
        { //image
          QString fileName;
          QTableWidgetItem *filename = ui->tableObjets->item(i,16);
          QPixmap pim(filename->text());
          PixmapItem *pixmap = new PixmapItem();
          pixmap->setPixmap(pim);
          pixmap->setPixFile(filename->text());
          pixmap->setPos(ObjetPosX,ObjetPosY);
          pixmap->setTypeAction(TypeAction->text().toInt());
          pixmap->setAction(Action->text());
          pixmap->setTexte(ObjetTexte);
          pixmap->setComment(ObjetComment);
          pixmap->setEtat(ObjetEtat->text().toInt());
          pixmap->setMode(ObjetMode->text().toInt());
          pixmap->setId(ObjetId);
          scene->addItem(pixmap);
        }
   }
}

/*******************************FOND D'ECRAN*********************************************/

void MainWindow::on_actionFondEcran_triggered()
{ // choix fond d-ecran
  // Open a file for reading

  QString fileName =
          QFileDialog::getOpenFileName(this, tr("Ouverture du fichier fond d'écran"),
                                       QDir::currentPath(),
                                       tr("Image Files (*.png *.jpg *.bmp)"));
  if (fileName.isEmpty())
      return;
  else
    {
      QPixmap pim(fileName);
      FondItem *pixmap = new FondItem();
      pixmap->setPixmap(pim);
      scene->addItem(pixmap);
      //dimensionnement de la scene selon la taille de pixmap
      double sizeW = pixmap->boundingRect().width();
      double sizeH = pixmap->boundingRect().height();
      scene->setSceneRect(0, 0, sizeW, sizeH);
      // positionnement des scrollbar en haut et à gauche
      QPointF zero = ui->graphicsView->mapFromScene(QPointF(0,0));
      ui->graphicsView->horizontalScrollBar()->setValue(zero.x());
      ui->graphicsView->verticalScrollBar()->setValue(zero.y());
      pixmap->setPixFile(fileName);
      insererFond(0,0,fileName,7); //ecriture des valeurs dans tableBackground
      //dessin de la grille (taille,transparence)
      dessine_grille(50,0.3);
    }
}

void MainWindow::insererFond(int ObjetPosX,int ObjetPosY,QString fondfileName,int ObjetType)
{
  ui->tableBackground->setRowCount(1);
  ui->tableBackground->setItem(0, 0, new QTableWidgetItem(QString::number(ObjetType)));//Type
  ui->tableBackground->setItem(0, 1, new QTableWidgetItem(QString::number(ObjetPosX)));//PositionX
  ui->tableBackground->setItem(0, 2, new QTableWidgetItem(QString::number(ObjetPosY)));//PositionY
  ui->tableBackground->setItem(0, 3, new QTableWidgetItem(fondfileName));//chemin du fichier image
}

/*******************************CHOIX DE LA COULEUR DE L'OBJET********************************************/


void MainWindow::on_actionChoisir_la_couleur_triggered()
{
  QColorDialog *dialog = new QColorDialog(this);
  dialog->show();
  int resultCode = dialog->exec();
     if (resultCode == QDialog::Accepted)
       {
         QColor color =  dialog->currentColor();
         QVariant variant = color;
         QString rgb= variant.toString();
       }

   QList <QGraphicsItem*> itemList = scene->items();
    for(int i=0; i<itemList.size(); i++)
      {
        if(itemList[i]->isSelected() &&  itemList[i]->type() == 65536)
          {
             QColor color =  dialog->currentColor();
             QVariant variant = color;
             QString rgb= variant.toString();
             MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
             item->setColor(color);
             ui->tableObjets->setItem(i, 7,new QTableWidgetItem(QString::number(color.rgb())));//brush
           }
      }
     items_vers_tableau();//ajout des items de la scene au tableau
     delete dialog;
}

void MainWindow::on_actionChoisir_le_type_de_crayon_triggered()
{ // choisir la couleur de pen

  QColorDialog *dialog = new QColorDialog(this);
  dialog->show();
  int resultCode = dialog->exec();
     if (resultCode == QDialog::Accepted)
       {
         QColor color =  dialog->currentColor();
         QVariant variant = color;
         QString rgb= variant.toString();
       }
  QList <QGraphicsItem*> itemList = scene->items();
    for(int i=0; i<itemList.size(); i++)
      {
        if(itemList[i]->isSelected() &&  itemList[i]->type() == 65536)
          {
             QColor color =  dialog->currentColor();
             QVariant variant = color;
             QString rgb= variant.toString();
             MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
             item->setPenColor(color);
             ui->tableObjets->setItem(i, 8,new QTableWidgetItem(QString::number(color.rgb())));//brush
          }
      }
     items_vers_tableau();//ajout des items de la scene au tableau
     delete dialog;
}

/*******************************AUTRES ACTIONS*********************************************/

void MainWindow::on_actionSupprimer_triggered()
{

  QList <QGraphicsItem*> itemList = scene->items();
/*
         // l'objet est supprimé de la liste
         for(int i=0; i<itemList.size(); i++)
         {
           if(itemList[i]->hasFocus())
             {
               qDebug() << "touche ok item as focus";
               QMessageBox msgBox;
               //msgBox.setText("confirmation de suppression de parcelle");
               msgBox.setInformativeText("Voulez-vous supprimer cette parcelle ?");
               msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
               msgBox.setDefaultButton(QMessageBox::Ok);
               msgBox.setIcon(QMessageBox::Question);
               int ret = msgBox.exec();
               switch (ret)
                 {
                  case QMessageBox::Ok:
                   qDebug() << "touche ok ";
                   scene->removeItem(itemList[i]);
                   delete itemList[i];
                   ui->lineEdit->setText("objet supprimé");
                   items_vers_tableau();//ajout des items de la scene au tableau
                   break;
                 case QMessageBox::Cancel:
                     // Annuler
                  qDebug() << "touche cancel ";
                     break;
                 default:
                  qDebug() << "touche default ";
                     break;
                 }
             }
         }
*/
  //l'objet est supprimé de l'espace graphique mais non éliminé de la liste pour conserver
  // la cohérence de l'indentation des nouveaux objets
        for(int i=0; i<itemList.size(); i++)
        {
          if(itemList[i]->hasFocus())
            {
                  MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
                  item->setX(-1);
                  item->setY(-1);
                  item->setSizeShape(1,1);
                  items_vers_tableau();//ajout des items de la scene au tableau
            }
        }
}

void MainWindow::on_actionDeselectionner_tout_triggered()
{  //deselectionne tous les objets

  QList <QGraphicsItem*> itemList = scene->items();
   for(int i=0; i<itemList.size(); i++)
     {
       if(itemList[i]->isSelected())
         {
          itemList[i]->setSelected(false);
          if(itemList[i]->type() == 65536) // MyItem
          {
           MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
           item->setPenColor(QColor(Qt::red));
           }
         }
     }
}


QString MainWindow::Choisir_le_fichier()
{
  QString fileName =
          QFileDialog::getOpenFileName(this, tr("Ouverture du fichier image"),
                                       QDir::currentPath(),
                                       tr("Image Files (*.png *.jpg *.bmp)"));

  return fileName;
}


void MainWindow::switchcall(QString selection)
{// changement de la combobox type d'action non utilisé
  if(selection!="")
     qDebug() << "selection" << selection;
}


/************************BRING TO BACK AND TO FRONT************************/


void MainWindow::on_actionBringToFront_triggered()
{
  if (scene->selectedItems().isEmpty())
      return;

  QGraphicsItem *selectedItem = scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach (QGraphicsItem *item, overlapItems)
    {
      zValue = item->zValue() + 0.1;
      qDebug() << zValue;
     }
  selectedItem->setZValue(zValue);
qDebug() << "bring to front";
}

void MainWindow::on_actionBringToBack_triggered()
{
  if (scene->selectedItems().isEmpty())
      return;

  QGraphicsItem *selectedItem = scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach (QGraphicsItem *item, overlapItems)
    {
          zValue = item->zValue() - 0.1;
          qDebug() << zValue;
     }
  selectedItem->setZValue(zValue);
qDebug() << "bring to back";

}
/**********************affichage fond d'écran***************************/

void MainWindow::on_actionCacher_le_fond_triggered()
{
  QList <QGraphicsItem*> itemList = scene->items();
    for(int i=0; i<itemList.size(); i++)
     {
        if(itemList[i]->type() == 7) //IMAGES
         {
            FondItem *as_fonditem;
            if((as_fonditem = dynamic_cast<FondItem*>(itemList[i])))
              {
                itemList[i]->hide();
              }
          }
      }
}

void MainWindow::on_actionAfficher_le_fond_triggered()
{
  QList <QGraphicsItem*> itemList = scene->items();
    for(int i=0; i<itemList.size(); i++)
     {
        if(itemList[i]->type() == 7) //IMAGES
         {
            FondItem *as_fonditem;
            if((as_fonditem = dynamic_cast<FondItem*>(itemList[i])))
              {
                itemList[i]->show();
              }
          }
      }
}

/************************mis à jour tableau des objets ******************/
void MainWindow::on_pushButton_clicked()
{ //bouton mise a jour du tableau des objet
    items_vers_tableau();
}

/**************************a propos de **********************************/

void MainWindow::on_actionA_propos_de_triggered()
{
  QMessageBox::about(this, tr("A propos de Open_jardin"),
              tr("Ce programme est utilisé pour gérer graphiquement les plantations d'un potager.\n"
                 "il utilise des fichier XML pour la configuration des plans\n"
                 "et une base sqlite pour les données de culture\n"
                 "version 2017.001 license GNU GPL version 3"));
}

void MainWindow::on_actionQuitter_triggered()
{  //quitter l'application
   close();
}


/************************************************************************************************/
//******************** GESTION DES MODES CREATION-MODIFICATION DU PLAN ET UTILISATION************/
/************************************************************************************************/

void MainWindow::on_actionModification_plan_triggered()
{
    ui->actionUtilisation->setChecked(false);
    ui->actionModification_plan->setChecked(true);
    QList <QGraphicsItem*> itemList = scene->items();
     for(int i=0; i<itemList.size(); i++)
       {
          if(itemList[i]->type() == 65536) // MyItem
            {
             MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
             item->setMode(0);
            }
       }
      ui->actionAjouterRectangle->setEnabled(true);
      ui->actionAjouterCercle->setEnabled(true);
      ui->actionAjouterImage->setEnabled(true);
      ui->actionAjouter_icone->setEnabled(true);
      ui->actionAjouter_Rectangle_arrondi->setEnabled(true);
      ui->actionChoisir_la_couleur->setEnabled(true);
      ui->actionChoisir_le_type_de_crayon->setEnabled(true);
      ui->actionBringToBack->setEnabled(true);
      ui->actionBringToFront->setEnabled(true);
      ui->actionColler->setEnabled(true);
      ui->actionCopier->setEnabled(true);
      ui->actionCouper->setEnabled(true);
      ui->actionDeselectionner_tout->setEnabled(true);
      ui->actionFondEcran->setEnabled(true);
      ui->actionSelectionner_tout->setEnabled(true);
      ui->actionSupprimer->setEnabled(true);
      ui->actionModifier->setEnabled(true);


}

void MainWindow::on_actionUtilisation_triggered()
{
  ui->actionUtilisation->setChecked(true);
  ui->actionModification_plan->setChecked(false);
  QList <QGraphicsItem*> itemList = scene->items();
   for(int i=0; i<itemList.size(); i++)
     {
        if(itemList[i]->type() == 65536) // MyItem
          {
           MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
           item->setMode(1);
          }
     }
    ui->actionAjouterRectangle->setDisabled(true);
    ui->actionAjouterCercle->setDisabled(true);
    ui->actionAjouterImage->setDisabled(true);
    ui->actionAjouter_icone->setDisabled(true);
    ui->actionAjouter_Rectangle_arrondi->setDisabled(true);
    ui->actionChoisir_la_couleur->setDisabled(true);
    ui->actionChoisir_le_type_de_crayon->setDisabled(true);
    ui->actionBringToBack->setDisabled(true);
    ui->actionBringToFront->setDisabled(true);
    ui->actionColler->setDisabled(true);
    ui->actionCopier->setDisabled(true);
    ui->actionCouper->setDisabled(true);
    ui->actionDeselectionner_tout->setDisabled(true);
    ui->actionFondEcran->setDisabled(true);
    ui->actionSelectionner_tout->setDisabled(true);
    ui->actionSupprimer->setDisabled(true);
    ui->actionModifier->setDisabled(true);
  }

/********************************************************************************/
/*************************action sur click item**********************************/
/********************************************************************************/

void MainWindow::Item_clicked()
{ //si l'item est sélectionné (selection changed)
  QList <QGraphicsItem*> itemList = scene->items();
  int retourIdItem;
  for(int i=0; i<itemList.size(); i++)
     {
       if(itemList[i]->isSelected())
         {
           MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
           retourIdItem=item->getId();
           //lance_action(retourIdItem);
           ui->lineEdit_Id_Item->setText(QString::number(retourIdItem));
           ui->lineEdit_Nom_item->setText(item->getNom());
           ui->textEdit_plan_commentaires->setText(item->getTexte());
         }
     }
}

void MainWindow::ItemPlanning_clicked()
{ //si l'item est sélectionné (scene_planning selection changed)
  QList <QGraphicsItem*> itemList = scene_planning->items();
  int nbJours;
  int dateDepart;
  QString strDateAn = ui->comboBox_AnneeEnCours->currentText()+".01.01";
  QDate dateAn = QDate::fromString(strDateAn ,"yyyy.MM.dd");
  for(int i=0; i<itemList.size(); i++)
     {
       if(itemList[i]->isSelected())
         {
           MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
           nbJours=(item->getWidth()/14)+1;
           ui->lineEdit_width->setText(QString::number(nbJours));
           dateDepart=((item->pos().x())/14)-nbJours/2;
           QDate date_depart = dateAn.addDays(dateDepart);
           ui->lineEdit_dateDepart->setText(date_depart.toString("yyyy.MM.dd"));
           ui->lineEdit_id_culture->setText(QString::number(item->getId()));
           QDate date_fin = date_depart.addDays(nbJours);
           ui->lineEdit_texte_culture->setText(item->getNom());
           ui->lineEdit_date_recolte->setText(date_fin.toString("yyyy.MM.dd"));

          }
     }
}


void MainWindow::on_pushButton_Affiches_fiche_clicked()
{
  int Id= ui->lineEdit_Id_Item->text().toInt();
  if(Id >= 0)
    {
        QString fileName= ui->lineEdit_config_nom_base->text();
        Database *fenetre_database= new Database(Id,0,fileName);
        fenetre_database->show();
     }
}


void MainWindow::on_pushButton_Enregistrer_modif_item_clicked()
{
  QList <QGraphicsItem*> itemList = scene->items();
  for(int i=0; i<itemList.size(); i++)
     {
       if(itemList[i]->isSelected())
         {
           MyItem* item = dynamic_cast<MyItem*> (itemList[i]);
           item->setNom(ui->lineEdit_Nom_item->text());
           item->setTexte(ui->textEdit_plan_commentaires->document()->toPlainText());
         }
     }
}


void MainWindow::on_pushButton_ouvrir_fiche_culture_clicked()
{ // ouvre la fiche culture de la culture sélectionnée
   if(ui->lineEdit_id_culture->text()>0)
     {
        for(int row=0;row<ui->tableViewCultures->model()->rowCount();row++)
             {
               QString id_culture =  ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,0)).toString();
               QString id_parcelle = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,2)).toString();

               if(id_culture==ui->lineEdit_id_culture->text())
                 {
                   int Id= id_parcelle.toInt();
                   int cult=id_culture.toInt();
                   if(Id >= 0)
                     {
                         QString fileName= ui->lineEdit_config_nom_base->text();
                         Database *fenetre_database= new Database(Id,cult,fileName);
                         fenetre_database->show();
                     }
                 }
            }
      }
}

void MainWindow::on_pushButton_validerDates_clicked()
{ //valider les dates modifiées sur le planning

  QSqlQuery query;
  QString str1  = ui->lineEdit_dateDepart->text();
  QString str3  = ui->lineEdit_width->text();
  QString str4  = ui->lineEdit_id_culture->text();
  QString str5  = ui->lineEdit_date_recolte->text();
  QString strQuery="update cultures set date_semis= '"+str1+"',duree="+str3+",date_recolte='"+str5+"' where id="+str4;

    query.exec(strQuery);
    qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

      if (!query.isActive())
       qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
      else
       qDebug() << "enregistrement terminé";

}

/*************************************MISE A JOUR DES CULTURES DANS LE PLANNING ET ROTATION*****************************/

void MainWindow::mise_a_jour_table_taches(int choix)
{ // mise à jour des cultures  pour affichage dans les onglets planning (0)et rotations(1)
  QSqlQueryModel *model5 = new QSqlQueryModel;
  model5->setQuery("SELECT id, designation, parcelle, date_semis, duree,type_plante , commentaires , etat,date_recolte FROM cultures ");
  model5->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
  model5->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
  model5->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
  ui->tableViewCultures->setModel(model5);
  QString strDate = ui->comboBox_AnneeEnCours->currentText()+".01.01";
  QDate date = QDate::fromString(strDate ,"yyyy.MM.dd");
  QString strDateR = ui->comboBox_AnneeEnCours_2->currentText()+".01.01";
  QDate dateR = QDate::fromString(strDateR ,"yyyy.MM.dd");



  QString num_parcelle="";
  for(int h=0;h<ui->tableWidget_parcelles->rowCount();h++)
    {
      if(ui->tableWidget_parcelles->rowCount()>0)
        {
           num_parcelle=ui->tableWidget_parcelles->item(h,0)->text();
           for(int row=0;row<ui->tableViewCultures->model()->rowCount();row++)
             {
               QString str =  ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,2)).toString();
               if(str==num_parcelle)
                 {
                     ui->tableWidget_taches->setRowCount(ui->tableWidget_taches->rowCount()+1);
                     int ligne=ui->tableWidget_taches->rowCount()-1;
                     ui->tableWidget_taches->setItem(ligne, 0, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,0)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 1, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,1)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 2, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,2)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 3, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,3)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 4, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,4)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 5, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,5)).toString()));
                     ui->tableWidget_taches->setItem(ligne, 6, new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,7)).toString()));
                     int culture=ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,0)).toString().toInt();
                     QString designation=ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,1)).toString();
                     QString texte_date=ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,3)).toString();
                     int duree=ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,4)).toString().toInt();
                     QString num_plante=ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row,5)).toString();
                     QSqlQuery query;
                     QString nom_plante;
                     query.exec( QString( "select designation from plantes where id =" + num_plante));
                     if ( query.first() )
                       {
                       nom_plante = query.value(0).toString();
                       }
                     else
                       qDebug() <<"erreur query num plante:" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

                     QString num_espece;
                     query.exec( QString( "select espece from plantes where id =" + num_plante));
                     if ( query.first() )
                       {
                       num_espece = query.value(0).toString();
                       }
                     else
                       qDebug() <<"erreur query num plante:" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

                     QString num_famille;
                     query.exec( QString( "select famille from especes where id =" + num_espece));
                     if ( query.first() )
                       {
                       num_famille = query.value(0).toString();
                       }
                     else
                       qDebug() <<"erreur query num espece :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

                     QString nom_famille;
                     query.exec( QString( "select designation from familles where id =" + num_famille));
                     if ( query.first() )
                       {
                       nom_famille = query.value(0).toString();
                        }
                     else
                       qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

                    QDate date_tache = QDate::fromString(texte_date ,"yyyy.MM.dd");
                     int jour=date.daysTo(date_tache);
                     int semaine=((dateR.daysTo(date_tache))/6);
                     int famille=num_famille.toInt();

                     QColor couleur_famille;
                     int red;
                     int green;
                     int blue;
                     red=ui->tableWidget_rotation_3->horizontalHeaderItem(10)->backgroundColor().red();
                     green=ui->tableWidget_rotation_3->horizontalHeaderItem(10)->backgroundColor().green();
                     blue=ui->tableWidget_rotation_3->horizontalHeaderItem(10)->backgroundColor().blue();
                     couleur_famille.setBlue(blue);
                     couleur_famille.setGreen(green);
                     couleur_famille.setRed(red);

                     if(famille==3)//apiacées
                       {

                        red=ui->tableWidget_rotation_3->horizontalHeaderItem(0)->backgroundColor().red();
                        green=ui->tableWidget_rotation_3->horizontalHeaderItem(0)->backgroundColor().green();
                        blue=ui->tableWidget_rotation_3->horizontalHeaderItem(0)->backgroundColor().blue();
                        couleur_famille.setBlue(blue);
                        couleur_famille.setGreen(green);
                        couleur_famille.setRed(red);

                       }
                     if(famille==4)//astéracées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(1)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(1)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(1)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);

                       }
                     if(famille==7)//brassicacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(2)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(2)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(2)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==8)//chenopodacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(3)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(3)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(3)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==10)//cucurbitacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(4)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(4)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(4)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==12)//fabacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(5)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(5)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(5)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==15)//alliacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(6)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(6)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(6)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==17)//poacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(7)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(7)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(7)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==21)//solanacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(8)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(8)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(8)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                     if(famille==22)//valerianacées
                       {
                         red=ui->tableWidget_rotation_3->horizontalHeaderItem(9)->backgroundColor().red();
                         green=ui->tableWidget_rotation_3->horizontalHeaderItem(9)->backgroundColor().green();
                         blue=ui->tableWidget_rotation_3->horizontalHeaderItem(9)->backgroundColor().blue();
                         couleur_famille.setBlue(blue);
                         couleur_famille.setGreen(green);
                         couleur_famille.setRed(red);
                       }
                   if(choix==0)
                    {
                         ajouter_vignette_planning(culture, designation,h+1,jour-7,duree,couleur_famille);
                    }
                    if(choix==1)
                     ajouter_vignette_rotation(culture, nom_plante+"\n "+nom_famille,h+1,semaine-6,(duree/6)+1,couleur_famille); //titre,nb ligne,colonne depart,nb cases,couleur
                 }
             }
          }
    }
}

/*****************************EVENEMENTS DE LA BASE DE DONNEES***************************/


void MainWindow::on_tableView_plantes_clicked(const QModelIndex &index)
{
  int row=index.row();
  QString id_plante=ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,0)).toString();
  QString str =  ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,1)).toString();
  QString str1 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,3)).toString();
  QString str2 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,4)).toString();
  QString str3 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,5)).toString();
  QString str4 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,6)).toString();
  QString str5 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,7)).toString();
  QString str6 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,8)).toString();
  int str7 = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row,2)).toString().toInt();
  ui->lineEdit_valeur_printemps->setText(str4);
  ui->lineEdit_valeur_ete->setText(str5);
  ui->lineEdit_valeur_automne->setText(str6);
  ui->lineEdit_nom_latin->setText(str1);
  ui->lineEdit_designation_plantes->setText(str);
  ui->comboBox_lunaire_de_plantes->setCurrentIndex(str7-1);
  //selection espece dans combobox
  QSqlQuery query;
  QString resultat;
  query.exec( QString( "select designation from especes where id =" + str2));
  if ( query.first() )
    {
    resultat = query.value(0).toString();
    qDebug() << resultat;
    ui->comboBox_especes_de_plantes->setCurrentIndex(ui->comboBox_especes_de_plantes->findText(resultat));
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

  ui->textEdit_commentaires_plantes->setText(str3);
  ui->lineEdit_Id_plantes->setText(id_plante);

  mise_a_jour_tables_semis(str4,str5,str6);

}



void MainWindow::on_comboBox_especes_de_plantes_currentIndexChanged(const QString &arg1)
{
  QSqlQuery query;
  QString resultat;
  query.exec( QString( "select id from especes where designation ='" + arg1+"'"));
  if ( query.first() )
    {
    resultat = query.value(0).toString();
    qDebug() << resultat <<" nom "<<arg1;

    for(int index=0;index<ui->tableView_especes->model()->rowCount();index++)
      {
        QString id= ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index,0)).toString();
        if(id==resultat)
          {
              QString str3 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index,3)).toString();
              QString str4 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index,4)).toString();
              QString str8 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index,8)).toString();
              ui->textEdit_plante_positif->setText(str3);
              ui->textEdit_plante_negatif->setText(str4);

              //mise à jour du tableau des rotations
              if(str8=="")
                ui->lineEdit_valeur_rotation_familles_2->setText("0000000000000000000000");
              else
                ui->lineEdit_valeur_rotation_familles_2->setText(str8);//rotation familles précédentes
          }
       }
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

  query.exec( QString( "select famille from especes where designation ='" + arg1+"'"));
  if ( query.first() )
    {
    resultat = query.value(0).toString();


    for(int index=0;index<ui->tableView_familles->model()->rowCount();index++)
      {
        QString id= ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index,0)).toString();
        QString designation_famille = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index,1)).toString();
        if(id==resultat)
          {
             ui->comboBox_familles_de_plantes->setCurrentIndex(ui->comboBox_familles_de_plantes->findText(designation_famille));
          }
       }
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

}


void MainWindow::on_tableView_familles_clicked(const QModelIndex &index)
{
  int row=index.row();
  QString id_famille =  ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(row,0)).toString();
  QString str =  ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(row,1)).toString();
  ui->lineEdit_Designation_famille->setText(str);
  ui->lineEdit_Id_familles->setText(id_famille);
}

void MainWindow::on_pushButton_enregistrer_plantes_clicked()
{ //enregistrer la nouvelle fiche plante
  QSqlQuery query;
  QString nom_espece=ui->comboBox_especes_de_plantes->currentText();
  QString id_espece;
  query.exec( QString( "select id from especes where designation ='" + nom_espece+"'"));
  if ( query.first() )
    {
    id_espece = query.value(0).toString();
    qDebug() << nom_espece <<" id "<<id_espece;
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();


  QString str1=apos(ui->lineEdit_designation_plantes->text());
  QString str2=id_espece;
  QString str3=apos(ui->textEdit_commentaires_plantes->document()->toPlainText());
  QString str4=ui->lineEdit_valeur_printemps->text();
  QString str5=ui->lineEdit_valeur_ete->text();
  QString str6=ui->lineEdit_valeur_automne->text();
  QString str7=ui->lineEdit_nom_latin->text();
  QString str8=QString::number(ui->comboBox_lunaire_de_plantes->currentIndex()+1);

  QString str="insert into plantes (designation,type_lune,nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne) values('"+str1+"',"+str8+",'"+str7+"',"+str2+",'"+str3+"','"+str4+"','"+str5+"','"+str6+"')";
  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation,type_lune,nom_latin, espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    ui->tableView_plantes->setModel(model3);
    on_pushButton_Nouveau_plantes_clicked();
    init_base();
}

void MainWindow::on_pushButton_Modifier_plantes_clicked()
{// enregister la fiche modifiée   "SELECT id, designation,type_lune,nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes");
  QSqlQuery query;
  QString nom_espece=ui->comboBox_especes_de_plantes->currentText();
  QString id_espece;
  query.exec( QString( "select id from especes where designation ='" + nom_espece+"'"));
  if ( query.first() )
    {
    id_espece = query.value(0).toString();
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();


  QString str1=apos(ui->lineEdit_designation_plantes->text());
  QString str2=id_espece;
  QString str3=apos(ui->textEdit_commentaires_plantes->document()->toPlainText());
  QString str4=ui->lineEdit_valeur_printemps->text();
  QString str5=ui->lineEdit_valeur_ete->text();
  QString str6=ui->lineEdit_valeur_automne->text();
  QString str7=apos(ui->lineEdit_nom_latin->text());
  QString str8=QString::number(ui->comboBox_lunaire_de_plantes->currentIndex()+1);

  QString str="update plantes set designation = '"+ str1+"',type_lune="+str8+",nom_latin='"+str7+"',espece="+str2+",commentaires= '"+str3+"',semis_printemps= '"+str4+"',semis_ete= '"+str5+"',semis_automne= '"+str6+"' where id="+ui->lineEdit_Id_plantes->text();

  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation,type_lune, nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    ui->tableView_plantes->setModel(model3);
    on_pushButton_Nouveau_plantes_clicked();
init_base();

}

void MainWindow::on_pushButton_Nouveau_plantes_clicked()
{ //nouvelle fiche plantes vide
  ui->lineEdit_designation_plantes->setText("");
  ui->comboBox_especes_de_plantes->setCurrentIndex(0);
  ui->textEdit_commentaires_plantes->setText("");
  ui->lineEdit_Id_plantes->setText("");
}


void MainWindow::on_pushButton_supprimer_plantes_clicked()
{
   QString strId=ui->lineEdit_Id_plantes->text();
   QString str="delete from plantes where id="+ strId;
    QSqlQuery query;
   query.exec(str);
   qDebug() <<"delete: " << query.lastQuery().toUtf8();

     if (!query.isActive())
      qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
     else
      qDebug() << "suppression terminée";
     QSqlQueryModel *model3 = new QSqlQueryModel;
     model3->setQuery("SELECT id, designation,type_lune, nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes");
     model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
     model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
     model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
     ui->tableView_plantes->setModel(model3);
     on_pushButton_Nouveau_plantes_clicked();
     init_base();
}

/**********************************fiche especes*****************************/
void MainWindow::on_tableView_especes_clicked(const QModelIndex &index)
{
  int row=index.row();
  QString id_espece =  ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,0)).toString();
  QString str =  ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,1)).toString();
  QString str2 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,2)).toString();
  QString str3 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,3)).toString();
  QString str4 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,4)).toString();
  QString str5 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,5)).toString();
  QString str6 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,6)).toString();
  QString str7 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,7)).toString();
  QString str8 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row,8)).toString();
  ui->lineEdit_designation_espece->setText(str);
  //selection famille dans combobox
  QSqlQuery query;
  QString resultat;
  query.exec( QString( "select famille from especes where designation ='" + str+"'"));
  if ( query.first() )
    {
    resultat = query.value(0).toString();
    qDebug() << resultat;
    for(int index=0;index<ui->tableView_familles->model()->rowCount();index++)
      {
        QString id= ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index,0)).toString();
        QString designation_famille = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index,1)).toString();
        if(id==resultat)
          {
             ui->comboBox_familles_de_especes->setCurrentIndex(ui->comboBox_familles_de_especes->findText(designation_famille));
          }
       }
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();

  ui->lineEdit_Id_especes->setText(id_espece);
  ui->textEdit_positif->setText(str3);
  ui->textEdit_negatif->setText(str4);
  ui->textEdit_especes_commentaires->setText(str5); //commentaires
  ui->comboBox_rotation_compost->setCurrentIndex(str6.toInt()-1); //compost
  ui->comboBox_rotation_annees->setCurrentIndex(str7.toInt()-1); //rotation années
  if(str8=="")
    ui->lineEdit_valeur_rotation_familles->setText("0000000000000000000000");
  else
    ui->lineEdit_valeur_rotation_familles->setText(str8);//rotation familles précédentes

}

void MainWindow::on_pushButton_enregistrer_especes_clicked()
{

  QString str1=apos(ui->lineEdit_designation_espece->text());
  QSqlQuery query;
  QString nom_famille=ui->comboBox_familles_de_especes->currentText();
  QString id_famille;
  query.exec( QString( "select id from familles where designation ='" + nom_famille+"'"));
  if ( query.first() )
    {
    id_famille = query.value(0).toString();
    qDebug() << nom_famille <<" id "<<id_famille;
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
  QString str2=id_famille;
  QString str3=apos(ui->textEdit_positif->document()->toPlainText());
  QString str4=apos(ui->textEdit_negatif->document()->toPlainText());
  QString str5=apos(ui->textEdit_especes_commentaires->document()->toPlainText()); //commentaires
  QString str6=QString::number(ui->comboBox_rotation_compost->currentIndex()+1); //compost
  QString str7=QString::number(ui->comboBox_rotation_annees->currentIndex()+1); //rotation années
  QString str8=ui->lineEdit_valeur_rotation_familles->text();//rotation familles précédentes


  QString str="insert into especes (designation,famille,positif,negatif,commentaires, compost, rotation_ans,rotation_familles)"
      "values('"+str1+"',"+str2+",'"+str3+"','"+str4+"','"+str5+"',"+str6+","+str7+",'"+str8+"')";
  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model1 = new QSqlQueryModel;
    model1->setQuery("SELECT id, designation, famille ,positif,negatif,commentaires, compost, rotation_ans,rotation_familles FROM especes");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);

init_base();
}

void MainWindow::on_pushButton_Modifier_especes_clicked()
{
  QString str1=ui->lineEdit_designation_espece->text();
  QSqlQuery query;
  QString nom_famille=ui->comboBox_familles_de_especes->currentText();
  QString id_famille;
  query.exec( QString( "select id from familles where designation ='" + nom_famille+"'"));
  if ( query.first() )
    {
    id_famille = query.value(0).toString();
    qDebug() << nom_famille <<" id "<<id_famille;
    }
  else
    qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();



  QString str2=id_famille;
  QString str3=apos(ui->textEdit_positif->document()->toPlainText());
  QString str4=apos(ui->textEdit_negatif->document()->toPlainText());
  QString str5=apos(ui->textEdit_especes_commentaires->document()->toPlainText()); //commentaires
  QString str6=QString::number(ui->comboBox_rotation_compost->currentIndex()+1); //compost
  QString str7=QString::number(ui->comboBox_rotation_annees->currentIndex()+1); //rotation années
  QString str8=ui->lineEdit_valeur_rotation_familles->text();//rotation familles précédentes

  QString str="update especes set designation = '"+ str1+"',famille="+str2+",positif='"+str3+"',negatif='"+str4+"',commentaires='"+str5.toHtmlEscaped()  +"',compost="+str6+",rotation_ans="+str7+",rotation_familles='"+str8+"' where id="+ui->lineEdit_Id_especes->text();

  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model1 = new QSqlQueryModel;
    model1->setQuery("SELECT id, designation, famille,positif,negatif, commentaires, compost, rotation_ans,rotation_familles FROM especes");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);

    init_base();
}

void MainWindow::on_pushButton_Nouveau_especes_clicked()
{
    ui->lineEdit_designation_espece->setText("");
    ui->comboBox_familles_de_especes->setCurrentIndex(0);
    ui->lineEdit_Id_especes->setText("");
    ui->textEdit_positif->setText("");
    ui->textEdit_negatif->setText("");
    ui->textEdit_especes_commentaires->setText("");
    ui->comboBox_rotation_compost->setCurrentIndex(0);
    ui->comboBox_rotation_annees->setCurrentIndex(0);
    ui->lineEdit_valeur_rotation_familles->setText("0000000000000000000000");
}

/***********************************fiche familles***************************/



void MainWindow::on_pushButton_enregistrer_familles_clicked()
{
  QSqlQuery query;
  QString famille=apos(ui->lineEdit_Designation_famille->text());
  QString str="insert into familles (designation)"
      "values('"+famille+"')";
  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation FROM familles");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_familles->setModel(model3);
    init_base();
}

void MainWindow::on_pushButton_Modifier_familles_clicked()
{
  QSqlQuery query;
  QString famille=apos(ui->lineEdit_Designation_famille->text());
  QString str="update familles set designation = '"+ famille+"' where id="+ui->lineEdit_Id_familles->text();

  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation FROM familles");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_familles->setModel(model3);
    init_base();
}

void MainWindow::on_pushButton_Nouveau_familles_clicked()
{
  ui->lineEdit_Designation_famille->setText("");
  ui->lineEdit_Id_familles->setText("");
}

/************************************fiche tâches***************************/


void MainWindow::on_pushButton_enregistrer_operations_clicked()
{
  QSqlQuery query;
  QString tache=apos(ui->lineEdit_designation_taches->text());
  QString str="insert into taches (designation)"
      "values('"+tache+"')";
  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    init_base();
}

void MainWindow::on_pushButton_Modifier_operations_clicked()
{
  QSqlQuery query;
  QString tache=apos(ui->lineEdit_designation_taches->text());
  QString str="update taches set designation = '"+ tache+"' where id="+ui->lineEdit_Id_taches->text();

  query.exec(str);
  qDebug() <<"PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
     qDebug() <<"erreur query :" <<  query.lastError().text() <<"  " <<query.lastError().databaseText()<<query.driver();
    else
     qDebug() << "enregistrement terminé";

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    init_base();
}

void MainWindow::on_pushButton_Nouveau_operations_clicked()
{
  ui->lineEdit_designation_taches->setText("");
  ui->lineEdit_Id_taches->setText("");
}

void MainWindow::on_tableView_taches_clicked(const QModelIndex &index)
{
  int row=index.row();
  QString id_tache =  ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row,0)).toString();
  QString str =  ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row,1)).toString();
  ui->lineEdit_designation_taches->setText(str);
  ui->lineEdit_Id_taches->setText(id_tache);
}

/***************************TABLES DES SEMIS ET RECOLTE ********************/


void MainWindow::on_tableWidget_culture_printemps_cellClicked(int row, int column)
{

    QColor case_color=ui->tableWidget_culture_printemps->item(row,column)->backgroundColor();
    if(row==0)
      {
        if(case_color==Qt::green)
          ui->tableWidget_culture_printemps->item(row,column)->setBackgroundColor(Qt::white);
        else
          ui->tableWidget_culture_printemps->item(row,column)->setBackgroundColor(Qt::green);
      }
    else
      {
        if(case_color==Qt::gray)
          ui->tableWidget_culture_printemps->item(row,column)->setBackgroundColor(Qt::white);
        else
          ui->tableWidget_culture_printemps->item(row,column)->setBackgroundColor(Qt::gray);
      }
   test_tables_semis();
}

void MainWindow::on_tableWidget_culture_ete_cellClicked(int row, int column)
{
  QColor case_color=ui->tableWidget_culture_ete->item(row,column)->backgroundColor();
  if(row==0)
    {
      if(case_color==Qt::green)
        ui->tableWidget_culture_ete->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_ete->item(row,column)->setBackgroundColor(Qt::green);
    }
  else
    {
      if(case_color==Qt::gray)
        ui->tableWidget_culture_ete->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_ete->item(row,column)->setBackgroundColor(Qt::gray);
    }
  test_tables_semis();
}

void MainWindow::on_tableWidget_culture_automne_cellClicked(int row, int column)
{
  QColor case_color=ui->tableWidget_culture_automne->item(row,column)->backgroundColor();
  if(row==0)
    {
      if(case_color==Qt::green)
        ui->tableWidget_culture_automne->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_automne->item(row,column)->setBackgroundColor(Qt::green);
    }
  else
    {
      if(case_color==Qt::gray)
        ui->tableWidget_culture_automne->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_automne->item(row,column)->setBackgroundColor(Qt::gray);
    }
  test_tables_semis();
}

void MainWindow::test_tables_semis()
{

  QString printemps_semis="";
  QString printemps_recolte="";
  for(int y=0;y<ui->tableWidget_culture_printemps->columnCount();y++)
        {
           if(ui->tableWidget_culture_printemps->item(0,y)->backgroundColor()==Qt::green)
                printemps_semis=printemps_semis+"1";
           else
               printemps_semis=printemps_semis+"0";
        }

  for(int y=0;y<ui->tableWidget_culture_printemps->columnCount();y++)
      {
         if(ui->tableWidget_culture_printemps->item(1,y)->backgroundColor()==Qt::gray)
              printemps_semis=printemps_semis+"1";
         else
             printemps_semis=printemps_semis+"0";
      }
  ui->lineEdit_valeur_printemps->setText(printemps_semis);


  QString ete_semis="";
  QString ete_recolte="";
  for(int y=0;y<ui->tableWidget_culture_ete->columnCount();y++)
        {
           if(ui->tableWidget_culture_ete->item(0,y)->backgroundColor()==Qt::green)
                ete_semis=ete_semis+"1";
           else
               ete_semis=ete_semis+"0";
        }

  for(int y=0;y<ui->tableWidget_culture_ete->columnCount();y++)
      {
         if(ui->tableWidget_culture_ete->item(1,y)->backgroundColor()==Qt::gray)
              ete_semis=ete_semis+"1";
         else
            ete_semis=ete_semis+"0";
      }
  ui->lineEdit_valeur_ete->setText(ete_semis);


  QString automne_semis="";
  QString automne_recolte="";
  for(int y=0;y<ui->tableWidget_culture_automne->columnCount();y++)
        {
           if(ui->tableWidget_culture_automne->item(0,y)->backgroundColor()==Qt::green)
                automne_semis=automne_semis+"1";
           else
               automne_semis=automne_semis+"0";
        }

  for(int y=0;y<ui->tableWidget_culture_automne->columnCount();y++)
      {
         if(ui->tableWidget_culture_automne->item(1,y)->backgroundColor()==Qt::gray)
              automne_semis=automne_semis+"1";
         else
             automne_semis=automne_semis+"0";
      }
  ui->lineEdit_valeur_automne->setText(automne_semis);
}


void MainWindow::mise_a_jour_tables_semis(const QString &printemps,const QString &ete,const QString &automne)
{
  int pos=0;

  for(int y=0;y<ui->tableWidget_culture_printemps->columnCount();y++)
        {
           if(printemps.mid(y,1)=="0")
             ui->tableWidget_culture_printemps->item(0,y)->setBackgroundColor(Qt::white);
           else
             ui->tableWidget_culture_printemps->item(0,y)->setBackgroundColor(Qt::green);
           pos=y;
        }


  for(int z=0;z<ui->tableWidget_culture_printemps->columnCount();z++)
      {
      if(printemps.mid(pos+z+1,1)=="0")
        ui->tableWidget_culture_printemps->item(1,z)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_printemps->item(1,z)->setBackgroundColor(Qt::gray);
      }

  //table été
  pos=0;

  for(int y=0;y<ui->tableWidget_culture_ete->columnCount();y++)
        {
           if(ete.mid(y,1)=="0")
             ui->tableWidget_culture_ete->item(0,y)->setBackgroundColor(Qt::white);
           else
             ui->tableWidget_culture_ete->item(0,y)->setBackgroundColor(Qt::green);
           pos=y;
        }


  for(int z=0;z<ui->tableWidget_culture_ete->columnCount();z++)
      {
      if(ete.mid(pos+z+1,1)=="0")
        ui->tableWidget_culture_ete->item(1,z)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_ete->item(1,z)->setBackgroundColor(Qt::gray);
      }

  //table automne
 pos=0;
  for(int y=0;y<ui->tableWidget_culture_automne->columnCount();y++)
        {
           if(automne.mid(y,1)=="0")
             ui->tableWidget_culture_automne->item(0,y)->setBackgroundColor(Qt::white);
           else
             ui->tableWidget_culture_automne->item(0,y)->setBackgroundColor(Qt::green);
           pos=y;
        }


  for(int z=0;z<ui->tableWidget_culture_automne->columnCount();z++)
      {
      if(automne.mid(pos+z+1,1)=="0")
        ui->tableWidget_culture_automne->item(1,z)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_culture_automne->item(1,z)->setBackgroundColor(Qt::gray);
      }
}



/*********************************IMPRESSIONS*****************************************/

void MainWindow::on_pushButton_print_clicked()
{
    on_actionImprimer_le_plan_triggered();

}

void MainWindow::on_actionImprimer_le_plan_triggered()
{
  QPrinter * printer = new QPrinter(QPrinter::HighResolution);
  printer->setPaperSize (QPrinter::A4);
  printer->setOutputFormat(QPrinter::NativeFormat);
 // printer->setOutputFormat (QPrinter::PdfFormat);
  printer->setOrientation(QPrinter::Landscape);
  printer->setResolution(96);
  printer->setFullPage(true);
  QPrintDialog printDialog(printer, this);
  if ( printDialog.exec() == 1)
    {
      QPainter painter(printer);
      painter.setRenderHint(QPainter::Antialiasing);
      scene->render(&painter);
    }
}


/***********************************ROTATIONS******************************************/

void MainWindow::on_tableWidget_rotation_cellClicked(int row, int column)
{
  // modification du tableau des rotations de cultures et mise à jour de la valeur à enregistre dans la base

  QColor case_color=ui->tableWidget_rotation->item(row,column)->backgroundColor();
  if(row==0)
    {
      if(case_color==Qt::green)
        ui->tableWidget_rotation->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_rotation->item(row,column)->setBackgroundColor(Qt::green);
    }
  else
    {
      if(case_color==Qt::red)
        ui->tableWidget_rotation->item(row,column)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_rotation->item(row,column)->setBackgroundColor(Qt::red);
    }
  //mise à jour de la valeur rotation
  QString rotation="";
  for(int y=0;y<ui->tableWidget_rotation->columnCount();y++)
        {
           if(ui->tableWidget_rotation->item(0,y)->backgroundColor()==Qt::green)
                rotation=rotation+"1";
           else
               rotation=rotation+"0";
        }

  for(int y=0;y<ui->tableWidget_rotation->columnCount();y++)
      {
         if(ui->tableWidget_rotation->item(1,y)->backgroundColor()==Qt::red)
              rotation=rotation+"1";
         else
             rotation=rotation+"0";
      }
  ui->lineEdit_valeur_rotation_familles->setText(rotation);
}

void MainWindow::on_lineEdit_valeur_rotation_familles_textChanged(const QString &arg1)
{
  //mise à jour du tableau des rotations de culture

 int pos=0;
  for(int y=0;y<ui->tableWidget_rotation->columnCount();y++)
        {
           if(arg1.mid(y,1)=="0")
             ui->tableWidget_rotation->item(0,y)->setBackgroundColor(Qt::white);
           else
             ui->tableWidget_rotation->item(0,y)->setBackgroundColor(Qt::green);
           pos=y;
        }


  for(int z=0;z<ui->tableWidget_rotation->columnCount();z++)
      {
      if(arg1.mid(pos+z+1,1)=="0")
        ui->tableWidget_rotation->item(1,z)->setBackgroundColor(Qt::white);
      else
        ui->tableWidget_rotation->item(1,z)->setBackgroundColor(Qt::red);
      }
}


void MainWindow::on_lineEdit_valeur_rotation_familles_2_textChanged(const QString &arg1)
{//mise à jour du tableau des rotations de culture fiche plante

  int pos=0;
   for(int y=0;y<ui->tableWidget_rotation_2->columnCount();y++)
         {
            if(arg1.mid(y,1)=="0")
              {
                ui->tableWidget_rotation_2->setItem(0, y, new QTableWidgetItem);
                ui->tableWidget_rotation_2->item(0,y)->setBackgroundColor(Qt::white);
              }
            else
              {
                ui->tableWidget_rotation_2->setItem(0, y, new QTableWidgetItem);
                ui->tableWidget_rotation_2->item(0,y)->setBackgroundColor(Qt::green);
              }
            pos=y;
         }

   for(int z=0;z<ui->tableWidget_rotation_2->columnCount();z++)
       {
           if(arg1.mid(pos+z+1,1)=="0")
             {
               ui->tableWidget_rotation_2->setItem(1, z, new QTableWidgetItem);
               ui->tableWidget_rotation_2->item(1,z)->setBackgroundColor(Qt::white);
             }
           else
             {
               ui->tableWidget_rotation_2->setItem(1, z, new QTableWidgetItem);
               ui->tableWidget_rotation_2->item(1,z)->setBackgroundColor(Qt::red);
             }
       }
}

/***************************************************************************************/
//AFFICHAGE DE LA GRILLE


void MainWindow::on_actionAfficher_grille_triggered()
{ //AFFICHER LA GRILLE
   setGrille(0);
   ui->actionAfficher_grille->setChecked(true);
   ui->actionCacher_la_grille->setChecked(false);
   tableau_vers_items();
}

void MainWindow::on_actionCacher_la_grille_triggered()
{ //CACHER LA GRILLE
   setGrille(1);
   ui->actionAfficher_grille->setChecked(false);
   ui->actionCacher_la_grille->setChecked(true);
   tableau_vers_items();
}
