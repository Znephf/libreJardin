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
#ifndef DATABASE_H
#define DATABASE_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QWidget>
QT_BEGIN_NAMESPACE
class QComboBox;
class QDataWidgetMapper;
class QItemSelectionModel;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlRelationalTableModel;
class QStandardItemModel;
class QStringListModel;
class QTextEdit;
QT_END_NAMESPACE

namespace Ui {
  class Database;
}

class Database : public QDialog
{
  Q_OBJECT

public:
  explicit Database(const int &IdItem,const int&IdCulture, const QString &fileName,QWidget *parent = 0);
  ~Database();

  void setIdItem(const int &idItem)
  {
    m_idItem=idItem;
  }
  void setBaseFile(const QString &filename)
   {
    baseFileName =filename;

   }
  void setIdCulture(const int &idCulture)
  {
    m_idCulture=idCulture;
  }
private slots:
  void on_pushButton_validerData_clicked();

  void on_tableViewCultures_clicked(const QModelIndex &index);

  void on_lineEditTypeLune_textChanged(const QString &arg1);

  void on_pushButton_modifier_clicked();

  void on_pushButton_nouvelleOperation_clicked();

  void on_pushButton_print_fiche_clicked();

  void on_comboBox_plante_currentIndexChanged(const QString &arg1);

  void on_pushButton_creer_tache_clicked();

  void on_tableView_taches_clicked(const QModelIndex &index);

  void on_pushButton_modifier_tache_clicked();

  void on_lineEdit_id_cultures_textChanged(const QString &arg1);

  void on_pushButton_supprimer_culture_clicked();

  void on_pushButton_supprimer_tache_clicked();

private:

  int m_idItem;
  int m_idCulture;
//  int especeIndex;
//  int luneIndex;
  QString baseFileName;

  Ui::Database *ui;
  QSqlTableModel model;
  QSqlTableModel modelAll;
  QSqlTableModel *modelM;
  QSqlTableModel *modelLune;
  QSqlQueryModel *modelL;
  QSqlQueryModel *modelObs;



  //  QSqlRelationalTableModel *modelM;
  QItemSelectionModel *selectionModel;
  QDataWidgetMapper *mapper;

bool createConnection()
  {
      QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
      db.setHostName("localhost");
      db.setUserName("root");
      db.setPassword("");
      db.setDatabaseName(baseFileName);
      qDebug() << "fichier "<<baseFileName;

      if (!db.open())
        {
            qDebug() << "connection database erreur ";
            return false;
        }
      else
        {

          return true;
        }
  }


};




#endif // DATABASE_H
