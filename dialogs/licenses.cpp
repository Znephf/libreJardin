#include "licenses.h"
#include "ui_licenses.h"
#include "mainwindow.h"
#include "utilitaires/util.h"

Licenses::Licenses(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Licenses)
{
    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fichier);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
}

Licenses::~Licenses()
{
    delete ui;
}
