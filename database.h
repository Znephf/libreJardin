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
