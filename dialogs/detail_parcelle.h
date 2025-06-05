#ifndef DETAIL_PARCELLE_H
#define DETAIL_PARCELLE_H

#include <QDialog>
#include "modeles/parcelle.h"
#include "graphic/mygraphicsscene.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

namespace Ui {
class detail_parcelle;
}

class detail_parcelle : public QDialog
{
    Q_OBJECT

public:
    explicit detail_parcelle(const int&IdParcelle, const QString&fileName, QWidget *parent = nullptr);
    ~detail_parcelle();
    void open_XMLFile(QString fileName);


    void setIdParcelle(const int&idParcelle)
    {
        m_idParcelle = idParcelle;
    }

    int getIdParcelle()
    {
        return m_idParcelle;
    }

    void setFileNameXML(const QString fileName)
    {
        m_fileName_XML = fileName;
    }

    QString getFileNameXML()
    {
        return m_fileName_XML;
    }

    void setFileNameBackGround(const QString fileNameBG)
    {
        m_fileNameBackGround = fileNameBG;
    }

    QString getFileNameBackGround()
    {
        return m_fileNameBackGround;
    }

    QString getfileNameSQL()
    {
        return m_fileName_SQL;
    }

    QPolygon convertStrToPoly(const QString MyString);

private slots:
    void on_pushButton_open_clicked();
    void Item_clicked();
    int get_MaxId();

    void on_toolButton_Save_clicked();

    void on_toolButton_AddRectangle_clicked();

    void on_toolButton_AddCircle_clicked();

    void on_comboBox_lineWidth_P_currentIndexChanged(const QString&arg1);

    void on_comboBox_lineType_P_currentIndexChanged(int index);

    void on_toolButton_PenColor_clicked();

    void on_toolButton_BackgroundColor_clicked();

    void on_pushButton_zoomIn_clicked();

    void on_pushButton_ZoomOut_clicked();
    void zoomGraphicsView(int ratio);

    void on_toolButton_modification_clicked();

    void on_toolButton_Usage_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_delete_clicked();

    void on_toolButton_front_clicked();

    void on_toolButton_save_as_clicked();

    void on_pushButton_DisplaySheet_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::detail_parcelle *ui;
    QGraphicsScene *scene;
    int m_idParcelle;
    int m_MaxId;
    QString m_fileName_XML;
    QString m_fileName_SQL;
    QList <Parcelle *> plotList; // Changed variable name
    QString m_fileNameBackGround;
    qreal m_ZoomRatio;
    int m_mode;
};

#endif // DETAIL_PARCELLE_H
