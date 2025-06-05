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

#include "cultures.h"
#include "ui_cultures.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphic/myitem.h"
#include "dialogs/fiche_plantes.h"
#include "dialogs/dialog_taches.h"
#include "utilitaires/util.h"
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QtSql>
#include <QtWidgets>
#include <QDebug>
#include <QMessageBox>

Cultures::Cultures(const int&IdItem, const int&IdCulture, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cultures)
{
    // translator
    QTranslator translator;
    QString     fileContent = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fileContent);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    setIdItem(IdItem);     // id of the plot
    setIdCulture(IdCulture);
    ui->lineEditIdParcelle->setText(QString::number(m_idItem));
    ui->lineEdit_id_cultures->setText(QString::number(m_idCulture));

    // set the dateEdit of the sheet to today's date
    QDate currentDate;
    currentDate = currentDate.currentDate();
    ui->dateEdit->setDate(currentDate);
    ui->dateEdit_tache->setDate(currentDate);
    // update harvest date according to estimated duration
    QString str1  = ui->dateEdit->date().toString("yyyy.MM.dd");
    QDate   dateS = QDate::fromString(str1, "yyyy.MM.dd");
    QDate   dateR = dateS.addDays(ui->lineEdit_duree->text().toInt());
    ui->dateEdit_fin_culture->setDate(dateR);

    //fill combobox species plant type
    QSqlQueryModel *modelM = new QSqlQueryModel;
    modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
    ui->comboBox_plante->setModel(modelM);

    //fill combobox task type
    QSqlQueryModel *modelTache = new QSqlQueryModel;
    modelTache->setQuery("SELECT designation FROM taches ORDER BY designation ASC");
    ui->comboBox_type_tache->setModel(modelTache);
    init_models(IdCulture);
}

void Cultures::init_models(int idculture)
{
    if (idculture == 0)
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires , etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                            m_idItem));
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("plot"));
        ui->tableViewCultures->setModel(model);

        QSqlQueryModel *modelObs = new QSqlQueryModel;
        modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + QString::number(
                               idculture));
        modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
        ui->tableView_taches->setModel(modelObs);
    }
    else
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires , etat,duree,date_recolte FROM cultures WHERE id =" + QString::number(
                            idculture));
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("plot"));
        ui->tableViewCultures->setModel(model);
        QSqlQueryModel *modelObs = new QSqlQueryModel;
        modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + QString::number(
                               idculture));
        modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
        ui->tableView_taches->setModel(modelObs);
        int     row   = 0;
        QString strId = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString();
        QString str   = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 1)).toString();
        QString str1  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 3)).toString();
        QString str2  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 4)).toString();
        QString str3  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 5)).toString();
        QString str4  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 6)).toString();
        QString str5  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 7)).toString();
        QString str6  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 8)).toString();
        ui->lineEditDesignation->setText(str);
        QDate date = QDate::fromString(str1, "yyyy.MM.dd");
        ui->dateEdit->setDate(date);
        QDate date2 = QDate::fromString(str6, "yyyy.MM.dd");
        ui->dateEdit_fin_culture->setDate(date2);
        ui->lineEdit_duree->setText(str5);
        //select plant in combobox
        QSqlQuery query;
        QString   result;
        query.exec(QString("select designation from plantes where id =" + str2));
        if (query.first())
        {
            result = query.value(0).toString();

            ui->comboBox_plante->setCurrentIndex(ui->comboBox_plante->findText(result));
        }
        ui->comboBox_etat_culture->setCurrentIndex(str4.toInt() - 1);
        ui->plainTextEdit_commentaires->setPlainText(str3);
    }
}

Cultures::~Cultures()
{
    delete ui;
}

void Cultures::on_pushButton_nouvelleOperation_clicked()
{   //create a blank crop record
    ui->lineEditDesignation->setText("");
    ui->plainTextEdit_commentaires->setPlainText("");
    ui->comboBox_plante->setCurrentIndex(0);
    ui->lineEdit_id_cultures->setText("");
}

void Cultures::on_pushButton_validerData_clicked()
{   //add a record to the "cultures" table, validate the data and save to the database
    QSqlQuery query;
    QString   plant_name = util::apos(ui->comboBox_plante->currentText());
    QString   id_plant;

    query.exec(QString("select id from plantes where designation ='" + plant_name + "'"));
    if (query.first())
    {
        id_plant = query.value(0).toString();
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    QString str1 = ui->dateEdit->date().toString("yyyy.MM.dd");
    QString str2 = id_plant;
    QString str3 = util::apos(ui->plainTextEdit_commentaires->document()->toPlainText());
    QString str4 = QString::number(ui->comboBox_etat_culture->currentIndex() + 1);
    QString str5 = ui->lineEdit_duree->text();
    QString str6 = ui->dateEdit_fin_culture->date().toString("yyyy.MM.dd");
    QString str  = "insert into cultures (designation,parcelle,date_semis,type_plante,commentaires,etat,duree,date_recolte)"
                   "values('" + util::apos(ui->lineEditDesignation->text()) + "'," + ui->lineEditIdParcelle->text() + ",'" +
                   str1 +
                   "'," + str2 + ",'" + str3 + "'," + str4 + "," + str5 + ",'" + str6 + "')";
    query.exec(str);
    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    else
    {
        qDebug() << "record finished";
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                        m_idItem));
    model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model->setHeaderData(3, Qt::Vertical, QObject::tr("plot"));
    ui->tableViewCultures->setModel(model);
}

void Cultures::on_tableViewCultures_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString();
    QString str   = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 1)).toString();
    QString str1  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 3)).toString();
    QString str2  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 4)).toString();
    QString str3  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 5)).toString();
    QString str4  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 6)).toString();
    QString str5  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 7)).toString();
    QString str6  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 8)).toString();

    ui->lineEditDesignation->setText(str);
    QDate date = QDate::fromString(str1, "yyyy.MM.dd");
    ui->dateEdit->setDate(date);
    QDate date2 = QDate::fromString(str6, "yyyy.MM.dd");
    ui->dateEdit_fin_culture->setDate(date2);
    ui->lineEdit_duree->setText(str5);
    //select plant in combobox
    QSqlQuery query;
    QString   result;
    query.exec(QString("select designation from plantes where id =" + str2));
    if (query.first())
    {
        result = query.value(0).toString();
        ui->comboBox_plante->setCurrentIndex(ui->comboBox_plante->findText(result));
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    ui->comboBox_etat_culture->setCurrentIndex(str4.toInt() - 1);
    ui->plainTextEdit_commentaires->setPlainText(str3);
    ui->lineEdit_id_cultures->setText(strId);
}

void Cultures::on_comboBox_plante_currentIndexChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_plant;
    QString   result;
    QString   id_species;
    QString   design_species;
    QString   id_family;
    QString   design_family;

    query.exec(QString("select id from plantes where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        id_plant = query.value(0).toString();
        ui->lineEditTypeLune->clear(); //clear
        query.exec(QString("select type_lune from plantes where id=" + id_plant));
        if (query.first())
        {
            result = query.value(0).toString();
        }
        else
        {
            qWarning("cannot retrieve moon type value");
        }

        ui->lineEditTypeLune->setText(result);

        query.exec(QString("select espece from plantes where id=" + id_plant));
        if (query.first())
        {
            id_species = query.value(0).toString();
        }
        else
        {
            qWarning("cannot retrieve species value");
        }

        query.exec(QString("select designation from especes where id=" + id_species));
        if (query.first())
        {
            design_species = query.value(0).toString();
        }
        else
        {
            qWarning("cannot retrieve species designation value");
        }

        query.exec(QString("select famille from especes where id=" + id_species));
        if (query.first())
        {
            id_family = query.value(0).toString();
        }
        else
        {
            qWarning("cannot retrieve species family value");
        }

        query.exec(QString("select designation from familles where id=" + id_family));
        if (query.first())
        {
            design_family = query.value(0).toString();
        }
        else
        {
            qWarning("cannot retrieve family designation value");
        }

        ui->lineEdit_Fiche_culture_espece->setText(design_species);
        ui->lineEdit_Fiche_culture_famille->setText(design_family);
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_lineEditTypeLune_textChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   result;
    int       row = arg1.toInt();

    ui->lineEditDesign_Lune->clear(); //clear
    query.exec(QString("select designation from lune where id=" + QString::number(row)));
    if (query.first())
    {
        result = query.value(0).toString();
    }


    ui->lineEditDesign_Lune->setText(result);

    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_pushButton_modifier_clicked()
{
    //save the modified sheet
    QSqlQuery query;
    QString   plant_name = util::apos(ui->comboBox_plante->currentText());
    QString   id_plant;

    query.exec(QString("select id from plantes where designation ='" + plant_name + "'"));
    if (query.first())
    {
        id_plant = query.value(0).toString();
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    QString str1 = ui->dateEdit->date().toString("yyyy.MM.dd");
    QString str  = util::apos(ui->lineEditDesignation->text());
    QString str2 = id_plant;
    QString str3 = util::apos(ui->plainTextEdit_commentaires->document()->toPlainText());
    QString str4 = ui->lineEdit_id_cultures->text();
    QString str5 = ui->lineEditIdParcelle->text();
    QString str6 = QString::number(ui->comboBox_etat_culture->currentIndex() + 1);
    QString str7 = ui->lineEdit_duree->text();
    QString str8 = ui->dateEdit_fin_culture->date().toString("yyyy.MM.dd");

    QString strQuery = "update cultures set designation = '" + str + "',parcelle=" + str5 + ",date_semis= '" + str1 +
                       "',type_plante= " + str2 + ",commentaires='" + str3 + "',etat= " + str6 + ",duree=" + str7 +
                       ",date_recolte='" + str8 +
                       "' where id=" + str4;

    query.exec(strQuery);

    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    else
    {
        qDebug() << "record finished";
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                        m_idItem));
    model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model->setHeaderData(3, Qt::Vertical, QObject::tr("plot"));
    ui->tableViewCultures->setModel(model);
}

/*************** database of tasks and observations*************************/

void Cultures::on_pushButton_modifier_tache_clicked()
{
    //save the modified sheet
    QSqlQuery query;
    QString   task_type_name = ui->comboBox_type_tache->currentText();
    QString   id_task_type;

    query.exec(QString("select id from taches where designation ='" + task_type_name + "'"));
    if (query.first())
    {
        id_task_type = query.value(0).toString();
        qDebug() << task_type_name << " id " << id_task_type;
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }

    QString str1     = ui->dateEdit_tache->date().toString("yyyy.MM.dd");                            //task date
    QString str      = util::apos(ui->lineEdit_designation_tache->text());                           //task designation
    QString str2     = id_task_type;                                                                // id of task type
    QString str3     = util::apos(ui->plainTextEdit_observations_taches->document()->toPlainText()); //observations
    QString str4     = ui->lineEdit_id_tache->text();                                                //id
    QString str5     = ui->lineEdit_id_cultures->text();                                             // id culture
    QString strQuery = "update observations set designation = '" + str + "',date='" + str1 + "',type= " + str2 +
                       ",commentaires='" + str3 + "',id_culture=" + str5 + " where id=" + str4;

    query.exec(strQuery);
    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    else
    {
        qDebug() << "record finished";
    }
    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5);
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
}

void Cultures::on_pushButton_creer_tache_clicked()
{
    //add a record to the "OBSERVATIONS" table, validate the data and save to the database

    QSqlQuery query;
    QString   task_type_name = ui->comboBox_type_tache->currentText();
    QString   id_task_type;

    query.exec(QString("select id from taches where designation ='" + task_type_name + "'"));
    if (query.first())
    {
        id_task_type = query.value(0).toString();
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    QString str1 = ui->dateEdit_tache->date().toString("yyyy.MM.dd");                            //task date
    QString str  = util::apos(ui->lineEdit_designation_tache->text());                           //task designation
    QString str2 = id_task_type;                                                                // id of task type
    QString str3 = util::apos(ui->plainTextEdit_observations_taches->document()->toPlainText()); //observations
    QString str4 = ui->lineEdit_id_tache->text();                                                //id
    QString str5 = ui->lineEdit_id_cultures->text();                                             // id culture
    QString strQ = "insert into observations (designation, date,type , commentaires,id_culture)"
                   "values('" + str + "','" + str1 + "'," + str2 + ",'" + str3 + "'," + str5 + ")";
    query.exec(strQ);


    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Recording error"),
                                 tr("Please check that all fields are filled in correctly"));
    }
    else
    {
        qDebug() << "record finished";
    }
    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5);
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
}

void Cultures::on_tableView_taches_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 0)).toString();
    QString str   = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 1)).toString();
    QString str1  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 2)).toString();
    QString str2  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 3)).toString();
    QString str3  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 4)).toString();

    ui->lineEdit_id_tache->setText(strId);
    ui->lineEdit_designation_tache->setText(str);
    ui->plainTextEdit_observations_taches->document()->setPlainText(str3);
    QDate date = QDate::fromString(str1, "yyyy.MM.dd");
    ui->dateEdit_tache->setDate(date);

    //set combo_type_tache to id value
    QSqlQuery query;
    QString   result;
    query.exec(QString("select designation from taches where id =" + str2));
    if (query.first())
    {
        result = query.value(0).toString();
        ui->comboBox_type_tache->setCurrentIndex(ui->comboBox_type_tache->findText(result));
    }
    else
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_lineEdit_id_cultures_textChanged(const QString&arg1)
{
    QSqlQueryModel *modelObs = new QSqlQueryModel;

    modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + arg1);
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
}

/*******************************PRINT SHEET***********************/

void Cultures::on_pushButton_print_fiche_clicked()
{
    QString   title   = " CROP SHEET ";
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    printer->setPaperSize(QPrinter::A4);
    // printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(QPrinter::Portrait);
    printer->setFullPage(true);

    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == 1)
    {
        QTextBrowser *editor = new QTextBrowser;
        //create writing formats
        QTextCharFormat NormalFormat;

        QTextCharFormat ItalicFormat;
        ItalicFormat.setFontItalic(true);

        //Insert the current date and time at the beginning of the first page
        QDate date;
        QTime time;
        date = date.currentDate();
        time = time.currentTime();
        QString modif = "Done on : " + date.toString("dddd dd MMMM yyyy") + " at " + time.toString();

        //change writing format
        editor->setCurrentCharFormat(ItalicFormat);
        editor->setAlignment(Qt::AlignLeft);

        //add the QString at the cursor position
        editor->append(modif);
        editor->setCurrentCharFormat(NormalFormat);

        //insert the table title
        QTextCharFormat format_large_title;
        format_large_title.setFontPointSize(20);
        format_large_title.setFontWeight(QFont::Bold);
        format_large_title.setVerticalAlignment(QTextCharFormat::AlignMiddle);
        //  format_large_title.setUnderlineStyle(QTextCharFormat::SingleUnderline);

        QString titleStr = QString::fromUtf8(title.toStdString().c_str());
        editor->setCurrentCharFormat(format_large_title);
        editor->setAlignment(Qt::AlignCenter);

        editor->append(titleStr);

        QString plot = "Plot N° : " + ui->lineEditIdParcelle->text() + " \n";
        editor->append(plot);

        editor->setCurrentCharFormat(NormalFormat);
        //create a cursor at the current cursor position
        QTextCursor cursor = editor->textCursor();
        cursor.beginEditBlock();

        //Creation of the format of the table to be printed
        QTextTableFormat tableFormat;
        tableFormat.setAlignment(Qt::AlignHCenter);
        tableFormat.setAlignment(Qt::AlignLeft);
        tableFormat.setBackground(QColor("#ffffff"));
        tableFormat.setCellPadding(1);
        tableFormat.setCellSpacing(1);

        //Creation of the table to be printed with the number of columns
        //and rows contained in the table passed as parameter
        QTextTable *table = cursor.insertTable(ui->tableViewCultures->model()->rowCount() + 3,
                                                 ui->tableViewCultures->model()->columnCount(), tableFormat);

        QTextFrame *     frame       = cursor.currentFrame();
        QTextFrameFormat frameFormat = frame->frameFormat();
        frameFormat.setBorder(0);
        frame->setFrameFormat(frameFormat);

        //Format of the table HEADERS
        QTextCharFormat format_table_header;
        format_table_header.setFontPointSize(10);
        format_table_header.setFontWeight(QFont::Bold);

        //Format of the text in the table cells
        QTextCharFormat format_cell;
        format_cell.setFontPointSize(10);


        QTextTableCell cell;
        QTextCursor    cellCursor;
        //write the table HEADERS in the table to be printed
        for (int col = 0; col < table->columns(); col++)
        {
            QString text = ui->tableViewCultures->model()->headerData(col, Qt::Horizontal).toString();
            cell       = table->cellAt(0, col);
            cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(text, format_cell);
        }

        for (int row = 1; row < table->rows() - 2; row++)
        {
            for (int col = 0; col < table->columns(); col++)
            {
                QString text1 = "";
                if (col == 1 || col == 2)
                {
                    text1 = ui->tableViewCultures->model()->index(row - 1, col).data().toString();
                }
                else
                {
                    text1 = QString::number(ui->tableViewCultures->model()->index(row - 1, col).data().toDouble());
                }
                if (col == 3)
                {
                    text1 = ui->tableViewCultures->model()->index(row - 1, col).data().toDate().toString("dd/MM/yyyy");
                }
                if (col == 5)
                {
                    text1 = ui->tableViewCultures->model()->index(row - 1, col).data().toString();
                }
                cell       = table->cellAt(row, col);
                cellCursor = cell.firstCursorPosition();
                cellCursor.insertText(text1, format_cell);
            }
        }

        //end of editing
        cursor.endEditBlock();
        //print editor to QPrinter initialized at the beginning of the function
        editor->print(printer);
    }
}

void Cultures::on_pushButton_supprimer_culture_clicked()
{   // delete a crop record
    int ret = QMessageBox::warning(this, tr("Delete a crop"),
                                   tr("This crop can be deleted.\n"
                                      "Confirm crop deletion"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Ok:
    {
        QString   strId = ui->lineEdit_id_cultures->text();
        QString   str   = "delete from cultures where id=" + strId;
        QSqlQuery query;

        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
        }
        else
        {
            qDebug() << "delete finished";
        }
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                            m_idItem));
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("plot"));
        ui->tableViewCultures->setModel(model);
    }

    case QMessageBox::Cancel:
    {
        break;
    }

    default:
        break;
    }
}

void Cultures::on_pushButton_supprimer_tache_clicked()
{   //delete a task and observation sheet
    QString   strId = ui->lineEdit_id_tache->text();
    QString   str   = "delete from observations where id=" + strId;
    QString   str5  = ui->lineEdit_id_cultures->text();  // id culture
    QSqlQuery query;

    query.exec(str);

    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "delete finished";
    }

    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5);
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
}

void Cultures::on_lineEdit_duree_textChanged(const QString&arg1)
{
    QString str1  = ui->dateEdit->date().toString("yyyy.MM.dd");
    QDate   dateS = QDate::fromString(str1, "yyyy.MM.dd");
    QDate   dateR = dateS.addDays(arg1.toInt());

    ui->dateEdit_fin_culture->setDate(dateR);
}

void Cultures::on_toolButton_FichePlantes_clicked()
{   // display the plant varieties sheet
    int Id          = ui->comboBox_plante->currentIndex();
    int Num_culture = ui->lineEdit_id_cultures->text().toInt();

    if (Id >= 0)
    {
        Fiche_plantes *plant_sheet = new Fiche_plantes(Id);
        int            result       = plant_sheet->exec();

        if (result == QDialog::Accepted)
        {
            QSqlQueryModel *modelM = new QSqlQueryModel;
            modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
            ui->comboBox_plante->setModel(modelM);
            init_models(Num_culture);
        }
    }
}

void Cultures::on_toolButton_NouvellePlante_clicked()
{   //add a new variety
    // display the plant varieties sheet
    int Id          = 0;
    int Num_culture = ui->lineEdit_id_cultures->text().toInt();

    Fiche_plantes *plant_sheet = new Fiche_plantes(Id);
    int            result       = plant_sheet->exec();

    if (result == QDialog::Accepted)
    {
        Id = plant_sheet->getIdPlante();

        QSqlQueryModel *modelM = new QSqlQueryModel;
        modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
        ui->comboBox_plante->setModel(modelM);
        init_models(Num_culture);

        QSqlQuery query;
        QString   plant_name;
        QString   id_plant = QString::number(Id);

        query.exec(QString("select designation from plantes where id =" + id_plant));
        if (query.first())
        {
            plant_name = query.value(0).toString();

            ui->comboBox_plante->setCurrentText(plant_name);
        }
        else
        {
            qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
            QMessageBox::information(this, tr("Recording error"),
                                     tr("Please check that all fields are filled in correctly"));
        }
    }
}

void Cultures::on_pushButton_Gantt_clicked()
{   // open the Gantt chart tasks sheet
    Dialog_taches *TaskSheet = new Dialog_taches(0, this);

    connect(this, SIGNAL(sendText(const QString&)),
            TaskSheet, SLOT(getIdCulture(const QString&)));
    if (ui->lineEdit_id_cultures->text() != "0")
    {
        //  emit sendText(ui->lineEdit_id_cultures->text());
        //  TaskSheet->exec();
        create_phase();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Please select a crop"));
    }
}

void Cultures::create_phase()
{
    QSqlQuery query;
    QString   designation     = util::apos(ui->lineEditDesignation->text());
    QString   comments    = "";
    QString   start          = ui->dateEdit->date().toString("dd-MM-yyyy");
    QString   end             = ui->dateEdit->date().toString("dd-MM-yyyy");
    QString   duration           = "1";
    QString   id_crop      = ui->lineEdit_id_cultures->text();
    QString   date_constraint = "0";
    QString   progress      = "0";
    QString   type            = "1";
    QString   id_previous    = "0";
    QString   id_ParentPhase;

    query.exec(QString("SELECT COUNT(*) FROM tasks"));
    if (query.first())
    {
        int NbRowTable = query.value(0).toInt();
        if (NbRowTable > 0)
        {
            query.exec(QString("SELECT id FROM tasks WHERE NOT EXISTS  (SELECT id FROM tasks WHERE id_culture = " + id_crop +
                               ")"));
            if (query.first())
            {
                int id = query.value(0).toInt();

                if (id > 0)
                {
                    query.exec(QString("SELECT MAX(id) FROM tasks "));
                    if (query.first())
                    {
                        int id_MAX = query.value(0).toInt();
                        id_ParentPhase = QString::number(id_MAX + 1);
                        qDebug() << "query 1:" << query.lastQuery();
                    }
                    else
                    {
                        qDebug() << "query error 1:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                            query.driver() << query.lastQuery();
                    }

                    QString str =
                        "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
                        "values('" + designation + "','" + comments + "','" + start + "','" + end +
                        "'," + duration + "," + id_previous + "," + progress + "," + type + "," + date_constraint + "," +
                        id_ParentPhase +
                        "," +
                        id_crop + ")";
                    query.exec(str);
                    if (!query.isActive())
                    {
                        qDebug() << "validate query error:" << query.lastError().text() << "  " <<
                            query.lastError().databaseText() <<
                            query.lastQuery().toUtf8();
                        QMessageBox::information(this, tr("Recording error"),
                                                 tr("Please check that all fields are filled in correctly"));
                    }
                    else
                    {
                        QMessageBox::information(this, tr("Crop phase recording"),
                                                 tr("The crop phase has been successfully created!"));
                    }
                }
                else
                {
                    qDebug() << "query 2:" << query.lastQuery();
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Error"),
                                     tr("This crop phase already exists!"));
                qDebug() << "query error 3:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.driver() << query.lastQuery();
            }
        }
        else
        {
            id_ParentPhase = QString::number(1);
            QString str =
                "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
                "values('" + designation + "','" + comments + "','" + start + "','" + end +
                "'," + duration + "," + id_previous + "," + progress + "," + type + "," + date_constraint + "," + id_ParentPhase +
                "," +
                id_crop + ")";
            query.exec(str);
            if (!query.isActive())
            {
                qDebug() << "validate query error:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.lastQuery().toUtf8();
                QMessageBox::information(this, tr("Recording error"),
                                         tr("Please check that all fields are filled in correctly"));
            }
            else
            {
                QMessageBox::information(this, tr("Crop phase recording"),
                                         tr("The crop phase has been successfully created!"));
            }
        }
    }
}

void Cultures::on_toolButton_clicked()
{
    close();
}
