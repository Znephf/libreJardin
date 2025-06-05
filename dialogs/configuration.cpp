#include "configuration.h"
#include "ui_configuration.h"
#include "mainwindow.h"
#include "utilitaires/util.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtWidgets>
#include <QDialog>
#include <QDir>
//xml
#include <QDomDocument>


Configuration::Configuration(const QString&fileNameXML, const QString&fileNameSQL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    // translator
    QTranslator translator;
    QString     fileContent = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fileContent);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    init_base();
    ui->lineEdit_config_nom_Xml->setText(fileNameXML);
    ui->lineEdit_config_nom_base->setText(fileNameSQL);
    // store filenames for later use
    setXmlFileName(fileNameXML);
    setSqlFileName(fileNameSQL);
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    accept();
    close();
}

void Configuration::on_pushButton_fermer_clicked()
{
    close();
}

/****************replacement of apostrophes in QStrings for sql queries*************/


void Configuration::createConnection(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->plainTextMessages->insertPlainText("Error reading file: " + fileName + "\n");
        ui->plainTextMessages->insertPlainText("Check the file path!!\n");
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        qDebug() << "database connection error " << fileName;
    }
    else
    {
        qDebug() << "database open " << fileName;
        ui->lineEdit_config_nom_base->setText(fileName);
        ui->plainTextMessages->insertPlainText("Database opened: " + fileName + "\n");
    }
    setSqlFileName(fileName);
}

void Configuration::init_base()
{   /***********************initialization of sqlite databases******************/
    //TASKS
    QSqlQueryModel *model4 = new QSqlQueryModel;

    model4->setQuery("SELECT id,designation FROM taches ORDER BY designation ASC");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    ui->tableView_taches->setModel(model4);
}

void Configuration::on_pushButton_changeDataBase_clicked()
{
    // change the active database
    QSqlQuery query;
    QString   directory = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open a database"),
                                     directory,
                                     tr("SQLI files (*.sqli *.db);; All files (*.*) "));

    // tr("SQLI Files (*.sqli *.db)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->plainTextMessages->insertPlainText("Error reading file: " + fileName + "\n");
        return;
    }
    else
    {
        ui->plainTextMessages->insertPlainText("Database opened:" + fileName + "\n");
        createConnection(fileName);
    }
}

// save database filename to a new XML file
void Configuration::saveDataBase()
{
    // save the name of the database in the XML file
    QString      cellvalue;
    QString      field;
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    // Create the <base> element
    QDomElement base = document.createElement("base");

    // Add the <background> element as the first child of the <root> element
    root.appendChild(base);
    //Write the name of the sqlite database file
    QDomElement file_base = document.createElement("file_base");
    cellvalue = ui->lineEdit_config_nom_base->text();
    field     = "file";
    file_base.setAttribute(field, cellvalue);
    base.appendChild(file_base);
    // Write to file
    QString directory = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Save object table"),
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
    }
}

void Configuration::on_pushButton_NewdatabaseFull_clicked()
{
    //  create an empty database and import data into this database
    ui->plainTextMessages->insertPlainText("Starting data import\n");
    QString directory = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Create a new pre-filled database"),
                                     directory,
                                     tr("SQLI Files (*.sqli *.db)"));
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
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("");
        db.setDatabaseName(fileName);
        if (!db.open())
        {
            qDebug() << "database connection error " << fileName;
        }
        else
        {
            qDebug() << "database open " << fileName;
            ui->plainTextMessages->insertPlainText("Database opened:" + fileName + "\n");
        }
        QString queryStr;
        QFile   file(":/sql/restore.sql");
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("WRITE ERROR");
            return;
        }
        else
        {
            QTextStream stream(&file);

            queryStr = stream.readAll();

            file.close();
        }

        QSqlQuery query;



        ui->plainTextMessages->insertPlainText("Data imported into the new database \n");
        //Check if the SQL driver supports Transactions
        if (db.driver()->hasFeature(QSqlDriver::Transactions))
        {
            //Replace comments, tabs and newlines with spaces
            queryStr =
                queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                    QRegularExpression::CaseInsensitiveOption |
                                                    QRegularExpression::MultilineOption), " ");
            //Remove unnecessary spaces
            queryStr = queryStr.trimmed();

            //Extract queries
            QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
            //Initialize expressions to detect special queries (`begin transaction` and `commit`).
            QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
            QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

            bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
            if (!isStartedWithTransaction)
            {
                db.transaction();
            }
            //Execute each query individually

            foreach(const QString &s, qList)
            {
                if (re_transaction.match(s).hasMatch())        //<== detect special queries
                {
                    db.transaction();
                }
                else if (re_commit.match(s).hasMatch())        //<== detect special queries
                {
                    db.commit();
                }
                else
                {
                    query.exec(s);                             //<== execute normal queries
                    if (query.lastError().type() != QSqlError::NoError)
                    {
                        // qDebug() << "error 262 " << query.lastError().text() << query.lastQuery();

                        db.rollback();                         //<== rollback the transaction if there is a problem
                        ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                    }
                    else
                    {
                        //  qDebug() << "269 " << query.lastQuery();
                    }
                }
            }
            ui->plainTextMessages->insertPlainText("Data import finished \n");

            if (!isStartedWithTransaction)
            {
                db.commit();
            }
        }
        else
        {         //if the sql driver does not support transactions
                  //...it is necessary to delete special queries (`begin transaction` and `commit`)
            queryStr =
                queryStr.replace(QRegularExpression(
                                     "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                     QRegularExpression::CaseInsensitiveOption |
                                     QRegularExpression::MultilineOption), " ");
            queryStr = queryStr.trimmed();

            //Execute each query individually
            QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
            foreach(const QString &s, qList)
            {
                query.exec(s);
                if (query.lastError().type() != QSqlError::NoError)
                {
                    ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                }
                else
                {
                }
                ui->plainTextMessages->insertPlainText("Data import finished \n");
            }
        }
    }
}

void Configuration::on_pushButton_import_clicked()
{
    //  create an empty database and import data into this database
    ui->plainTextMessages->insertPlainText("Starting data import\n");
    QString directory = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("New database name"),
                                     directory,
                                     tr("SQLI Files (*.sqli *.db)"));
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
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("");
        db.setDatabaseName(fileName);
        if (!db.open())
        {
            qDebug() << "database connection error " << fileName;
        }
        else
        {
            qDebug() << "database open " << fileName;
            ui->plainTextMessages->insertPlainText("Database opened:" + fileName + "\n");
        }

        QSqlQuery query;
        QString   directory = QDir::homePath() + "/openjardin/";

        QString fileNameImport =
            QFileDialog::getOpenFileName(this, tr("Choose the .sql file to import into the database"),
                                         directory,
                                         tr("SQL file (*.sql)"));
        if (fileNameImport.isEmpty())
        {
            return;
        }

        QFile fileImport(fileNameImport);
        if (!fileImport.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("READ ERROR");
            return;
        }
        else
        {
            //read the .sql file
            QTextStream Str(&fileImport);
            QString     queryStr;
            queryStr = Str.readAll();
            fileImport.close();
            ui->plainTextMessages->insertPlainText("File imported:" + fileNameImport + "\n");
            //Check if the SQL driver supports Transactions
            if (db.driver()->hasFeature(QSqlDriver::Transactions))
            {
                //Replace comments, tabs and newlines with spaces
                queryStr =
                    queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                        QRegularExpression::CaseInsensitiveOption |
                                                        QRegularExpression::MultilineOption), " ");
                //Remove unnecessary spaces
                queryStr = queryStr.trimmed();

                //Extract queries
                QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
                //Initialize expressions to detect special queries (`begin transaction` and `commit`).
                QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
                QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

                bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
                if (!isStartedWithTransaction)
                {
                    db.transaction();
                }
                //Execute each query individually
                foreach(const QString &s, qList)
                {
                    if (re_transaction.match(s).hasMatch())    //<== detect special queries
                    {
                        db.transaction();
                    }
                    else if (re_commit.match(s).hasMatch())    //<== detect special queries
                    {
                        db.commit();
                    }
                    else
                    {
                        query.exec(s);                        //<== execute normal queries
                        if (query.lastError().type() != QSqlError::NoError)
                        {
                            qDebug() << query.lastError().text();
                            qDebug() << s;
                            db.rollback();                    //<== rollback the transaction if there is a problem
                            ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                        }
                    }
                }
                ui->plainTextMessages->insertPlainText("Data import finished \n");

                if (!isStartedWithTransaction)
                {
                    db.commit();
                }
            }
            else  //if the sql driver does not support transactions

            {     //...it is necessary to delete special queries (`begin transaction` and `commit`)
                queryStr =
                    queryStr.replace(QRegularExpression(
                                         "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                         QRegularExpression::CaseInsensitiveOption |
                                         QRegularExpression::MultilineOption), " ");
                queryStr = queryStr.trimmed();

                //Execute each query individually
                QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
                foreach(const QString &s, qList)
                {
                    query.exec(s);
                    if (query.lastError().type() != QSqlError::NoError)
                    {
                        qDebug() << query.lastError().text();
                        ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                    }
                    else
                    {
                    }
                    ui->plainTextMessages->insertPlainText("Data import finished \n");
                }
            }
        }
    }
}

/**********************************EXPORT TO A TEXT FILE OF SQL QUERIES************************/
void Configuration::on_pushButton_export_clicked()
{
    // export data to a .sql query file
    QString directory = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Create a database export"),
                                     directory,
                                     tr("SQL File (*.sql)"));

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
        stream << "BEGIN TRANSACTION;\n";
        QSqlQuery   query;
        QStringList tables;
        query.exec("SELECT * FROM sqlite_master ");
        int     i2 = 0;
        QString table_values;
        //CREATE table queries
        while (query.next())
        {
            if (i2 > 0)
            {
                if (query.value("type").toString() == "table")
                {
                    tables << query.value("name").toString();
                }
                table_values = query.value("sql").toString();
                table_values.replace(QString("\""), QString("`"));
                table_values.replace(QString("\n"), QString(""));
                table_values.replace(QString("\t"), QString(" "));
                qDebug().noquote() << table_values.toUtf8() << ";\n";;
                stream << table_values.toUtf8().constData() << ";\n";
            }
            i2++;
        }
        //INSERT data queries
        QString    values;
        QString    columns;
        QString    insert;
        QSqlRecord record;

        foreach(const QString &table, tables)
        {
            query.exec(QString("SELECT * FROM [%1]").arg(table));
            while (query.next())
            {
                record  = query.record();
                columns = "";
                values  = "";
                for (int i = 0; i < record.count(); i++)
                {
                    if (columns == "")
                    {
                        columns = record.fieldName(i);
                    }
                    else
                    {
                        columns = columns + "," + record.fieldName(i);
                    }

                    if (record.field(i).type() == QVariant::String)
                    {
                        if (values == "")
                        {
                            values = "'" + util::apos(record.value(i).toString()) + "'";
                        }
                        else
                        {
                            values = values + " , " + "'" + util::apos(record.value(i).toString()) + "'";
                        }
                    }
                    else
                    {
                        if (values == "")
                        {
                            values = record.value(i).toString();
                        }
                        else
                        {
                            if (record.value(i).toString() == "")
                            {
                                values = values + " , " + "NULL";
                            }
                            else
                            {
                                values = values + " , " + record.value(i).toString();
                            }
                        }
                    }
                }
                insert = "INSERT INTO `" + table + "` (" + columns + ") VALUES (" + values + ");\n";
                stream << insert;
            }
        }
        stream << "COMMIT;";
        file.close();
    }
}

void Configuration::on_pushButton_Modifier_operations_clicked()
{
    QSqlQuery query;
    QString   tache = util::apos(ui->lineEdit_designation_taches->text());
    QString   str   = "update taches set designation = '" + tache + "' where id=" + ui->lineEdit_Id_taches->text();

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "record finished";
    }

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    //  init_base();
}

void Configuration::on_pushButton_Supprimer_operations_clicked()
{
    QMessageBox msgBox;

    msgBox.setInformativeText("Do you want to delete this operation?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        QString   strId = ui->lineEdit_Id_taches->text();
        QString   str   = "delete from taches where id=" + strId;
        QSqlQuery query;
        query.exec(str);
        qDebug() << "delete: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
        }
        else
        {
            qDebug() << "delete finished";
        }
        QSqlQueryModel *model4 = new QSqlQueryModel;
        model4->setQuery("SELECT id, designation FROM taches");
        model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        ui->tableView_taches->setModel(model4);
        on_pushButton_Nouveau_operations_clicked();
        //    init_base();

        break;
    }
}

void Configuration::on_pushButton_Nouveau_operations_clicked()
{
    ui->lineEdit_designation_taches->setText("");
    ui->lineEdit_Id_taches->setText("");
}

void Configuration::on_pushButton_enregistrer_operations_clicked()
{
    QSqlQuery query;
    QString   tache = util::apos(ui->lineEdit_designation_taches->text());
    QString   str   = "insert into taches (designation)"
                      "values('" + tache + "')";

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "query error :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "record finished";
    }

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    // init_base();
}

void Configuration::on_tableView_taches_clicked(const QModelIndex&index)
{
    int     row      = index.row();
    QString id_tache = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 0)).toString();
    QString str      = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 1)).toString();

    ui->lineEdit_designation_taches->setText(str);
    ui->lineEdit_Id_taches->setText(id_tache);
}

void Configuration::on_pushButton_mise_a_jour_clicked()
{   // Update an old database to version 1.06
    QString      fileName(getSqlFileName());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        qDebug() << "database connection error " << fileName;
    }
    else
    {
        qDebug() << "database open " << fileName;
        ui->plainTextMessages->insertPlainText("Database opened:" + fileName + "\n");
    }

    QString queryStr;
    QFile   file(":/sql/planner.sql");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("WRITE ERROR");
        return;
    }
    else
    {
        QTextStream stream(&file);

        queryStr = stream.readAll();

        file.close();
    }

    QSqlQuery query;



    ui->plainTextMessages->insertPlainText("Data imported into the new database \n");
    //Check if the SQL driver supports Transactions
    if (db.driver()->hasFeature(QSqlDriver::Transactions))
    {
        //Replace comments, tabs and newlines with spaces
        queryStr =
            queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                QRegularExpression::CaseInsensitiveOption |
                                                QRegularExpression::MultilineOption), " ");
        //Remove unnecessary spaces
        queryStr = queryStr.trimmed();

        //Extract queries
        QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
        //Initialize expressions to detect special queries (`begin transaction` and `commit`).
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
        if (!isStartedWithTransaction)
        {
            db.transaction();
        }
        //Execute each query individually

        foreach(const QString &s, qList)
        {
            if (re_transaction.match(s).hasMatch())        //<== detect special queries
            {
                db.transaction();
            }
            else if (re_commit.match(s).hasMatch())        //<== detect special queries
            {
                db.commit();
            }
            else
            {
                query.exec(s);                             //<== execute normal queries
                if (query.lastError().type() != QSqlError::NoError)
                {
                    qDebug() << "error 292 " << query.lastError().text() << query.lastQuery();

                    db.rollback();                         //<== rollback the transaction if there is a problem
                    ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                }
                else
                {
                    //  qDebug() << "298 " << query.lastQuery();
                }
            }
        }
        ui->plainTextMessages->insertPlainText("Database update finished \n");

        if (!isStartedWithTransaction)
        {
            db.commit();
        }
    }
    else
    {         //if the sql driver does not support transactions
              //...it is necessary to delete special queries (`begin transaction` and `commit`)
        queryStr =
            queryStr.replace(QRegularExpression(
                                 "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                 QRegularExpression::CaseInsensitiveOption |
                                 QRegularExpression::MultilineOption), " ");
        queryStr = queryStr.trimmed();

        //Execute each query individually
        QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
        foreach(const QString &s, qList)
        {
            query.exec(s);
            if (query.lastError().type() != QSqlError::NoError)
            {
                ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
            }
            else
            {
            }
            ui->plainTextMessages->insertPlainText("Database update finished \n");
        }
    }
}
