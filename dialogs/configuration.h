#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(const QString&fileNameXML, const QString&fileNameSQL, QWidget *parent = nullptr);
    ~Configuration() override;


    void createConnection(QString fileName);
    void init_base();

    void setXmlFileName(const QString&xmlFileName)
    {
        m_xmlFileName = xmlFileName;
    }

    QString getXmlFileName()
    {
        return m_xmlFileName;
    }

    void setSqlFileName(const QString&sqlFileName)
    {
        m_sqlFileName = sqlFileName;
    }

    QString getSqlFileName()
    {
        return m_sqlFileName;
    }

    void saveDataBase();

private slots:
    void on_pushButton_changeDataBase_clicked();

    void on_pushButton_NewdatabaseFull_clicked();

    void on_pushButton_import_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_Modifier_operations_clicked();

    void on_pushButton_Supprimer_operations_clicked();

    void on_pushButton_Nouveau_operations_clicked();

    void on_pushButton_enregistrer_operations_clicked();

    void on_tableView_taches_clicked(const QModelIndex&index);

    void on_pushButton_fermer_clicked();


    void on_pushButton_mise_a_jour_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Configuration *ui;
    QString m_xmlFileName;
    QString m_sqlFileName;
};

#endif // CONFIGURATION_H
