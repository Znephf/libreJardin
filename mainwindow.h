#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLabel>
#include <QDomDocument>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);


  ~MainWindow();
  void setGrille(const int &grille)
      {
        m_grille = grille;
      }

  int getGrille()
      {
        return m_grille;
      }

private slots:

  void on_actionSauver_triggered();

  void on_actionOuvrir_triggered();

  void on_actionAjouterRectangle_triggered();

  void items_vers_tableau();

  void on_actionAjouterCercle_triggered();

  void tableau_vers_items();

  void on_actionFondEcran_triggered();

  void on_actionAjouterImage_triggered();

  void on_actionChoisir_la_couleur_triggered();

  void on_actionSupprimer_triggered();

  void on_actionDeselectionner_tout_triggered();



  void on_actionAjouter_Rectangle_arrondi_triggered();

  void on_actionBringToFront_triggered();

  void on_actionBringToBack_triggered();

  void on_pushButton_clicked();

  void switchcall(QString selection);

  void on_actionA_propos_de_triggered();

  void on_actionQuitter_triggered();

  void on_actionChoisir_le_type_de_crayon_triggered();

  void on_actionModification_plan_triggered();

  void on_actionUtilisation_triggered();

  void Item_clicked();

  int get_id();

  void ItemPlanning_clicked();

  void on_actionCacher_le_fond_triggered();

  void on_actionAfficher_le_fond_triggered();

  void on_tableView_plantes_clicked(const QModelIndex &index);

  void on_tableView_especes_clicked(const QModelIndex &index);

  void on_tableView_familles_clicked(const QModelIndex &index);

  void on_pushButton_enregistrer_plantes_clicked();

  void on_pushButton_Modifier_plantes_clicked();

  void on_pushButton_Nouveau_plantes_clicked();
  void ajouter_vignette_verticale(const QString titre,const int colonne,const int width,const int height,const QColor couleur);
  void ajouter_vignette_horizontale(const QString titre,const int ligne,const int width,const int height,const QColor couleur);
  void ajouter_vignette_planning(const int culture,const QString titre,const int ligne,const int colonne,const int nbCases,const QColor couleur);
  void ajouter_vignette_mois(const QString titre,const int colonne,const int nb_jours,const QColor couleur);
  void ajouter_vignette_jour(const QString titre,const int colonne,const int ligne,const int width,const int height,const QColor couleur);
  void ajouter_repere_jour(const QString titre,const int colonne,const int ligne,const int width,const int height,const QColor couleur);
  void ajouter_repere_jour_rotation(const QString titre,const int colonne,const int ligne,const int width,const int height,const QColor couleur);

  void affiche_planning(const int day);
  void dessine_grille(const int espace,const qreal &Opacite);

  void ajouter_vignette_haut_rotation(const QString titre,const int ligne,const int colonne,const int nbCases,const QColor couleur);
  void ajouter_vignette_rotation(const int culture,const QString titre,const int ligne,const int colonne,const int nbCases,const QColor couleur);
  void ajouter_vignette_horiz_rotation(const QString titre,const int ligne,const int width,const int height,const QColor couleur);
  void affiche_rotation(const int year);
  void ajouter_repere_haut_rotation(const QString titre,const int ligne,const int colonne,const int nbCases,const QColor couleur);
  void on_comboBox_AnneeEnCours_currentIndexChanged(const QString &arg1);

  void on_pushButton_Affiches_fiche_clicked();

  void on_pushButton_Enregistrer_modif_item_clicked();

  void on_pushButton_enregistrer_especes_clicked();

  void on_pushButton_Modifier_especes_clicked();

  void on_pushButton_Nouveau_especes_clicked();

  void on_pushButton_enregistrer_familles_clicked();

  void on_pushButton_Modifier_familles_clicked();

  void on_pushButton_Nouveau_familles_clicked();

  void on_pushButton_enregistrer_operations_clicked();

  void on_pushButton_Modifier_operations_clicked();

  void on_pushButton_Nouveau_operations_clicked();

  void on_tableView_taches_clicked(const QModelIndex &index);

  void on_tableWidget_culture_printemps_cellClicked(int row, int column);

  void on_tableWidget_culture_ete_cellClicked(int row, int column);

  void on_tableWidget_culture_automne_cellClicked(int row, int column);

  void test_tables_semis();
  void mise_a_jour_tables_semis(const QString &printemps,const QString &ete,const QString &automne);
  void mise_a_jour_table_taches(int choix);

  void createConnection(QString fileName);

  void init_base();

  void on_pushButton_print_clicked();

  void on_actionImprimer_le_plan_triggered();

  void on_tableWidget_rotation_cellClicked(int row, int column);

  void on_lineEdit_valeur_rotation_familles_textChanged(const QString &arg1);

  QString apos(const QString texte); //remplacement des apostrophes pour requetes sql

  void on_lineEdit_valeur_rotation_familles_2_textChanged(const QString &arg1);

  void on_comboBox_AnneeEnCours_2_currentIndexChanged(const QString &arg1);

  void on_pushButton_color_clicked();

  void on_comboBox_especes_de_plantes_currentIndexChanged(const QString &arg1);

  void on_pushButton_ouvrir_fiche_culture_clicked();

  void on_pushButton_validerDates_clicked();

  void on_pushButton_supprimer_plantes_clicked();



  void on_actionAfficher_grille_triggered();

  void on_actionCacher_la_grille_triggered();

  void on_lineEdit_Nom_item_textChanged(const QString &arg1);



  void on_pushButton_Supprimer_especes_clicked();

  void on_pushButton_Supprimer_familles_clicked();

  void on_pushButton_Supprimer_operations_clicked();

protected:
    QPoint mousePressPt;
    void mousePressEvent(QMouseEvent *event);
    void insererFond(int ObjetPosX,int ObjetPosY,QString fondfileName,int ObjetType);
    void Choisir_la_couleur(int nCell);
    QString Choisir_le_fichier();

private:
  Ui::MainWindow *ui;
  QGraphicsScene *scene;
  QGraphicsScene *scene_planning;
  QGraphicsScene *scene_planning2;
  QGraphicsScene *scene_planning3;
  QGraphicsScene *scene_rotation;
  QGraphicsScene *scene_rotation2;
  QGraphicsScene *scene_rotation3;
  QString fondfileName;
  bool mode_modifier;
  int m_id;
  int m_grille;//0 affiche - 1 cache

  QSqlTableModel model1; //especes
  QSqlTableModel model2; //familles
  QSqlTableModel model3; //plantes
  QSqlTableModel model4; //taches
  QSqlTableModel modelF; //combobox familles
  QSqlTableModel modelE; //combobox especes
  QSqlTableModel model5; //toutes les taches

};

/*********************CLASS PIXMAPITEM************************/
// QGraphicsPixmapItem subclass
class PixmapItem : public QGraphicsPixmapItem
{

public:

  PixmapItem() : QGraphicsPixmapItem()
    {
        setCursor(Qt::OpenHandCursor);
        setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
         setAcceptHoverEvents(true);
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void setPixFile(QString pixmapfileName);
    QString getPixFile();
    void setTypeAction(int typeAction);
    int getTypeAction();
    void setAction(QString action);
    QString getAction();
    void setTexte(const QString text);
    QString getTexte();
    void setComment(const QString comment);
    QString getComment();
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

    void setId(const int &id)
      {
        m_pixmapId = id;
      }

    int getId()
      {
        return m_pixmapId;
      }
protected:
    QPoint mousePressPt;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString pixmapfileName;
    int typeAction;
    QString action;
    QString itemText;
    QString itemComment;
    int m_etat;
    int m_mode;
    int m_pixmapId;

};
/********************CLASS FONDITEM************************/

// QGraphicsPixmapItem subclass
class FondItem : public QGraphicsPixmapItem
{

public:

  FondItem() : QGraphicsPixmapItem()
    {
        //setCursor(Qt::OpenHandCursor);
        //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    }

    void setPixFile(QString pixmapfileName);
    QString getPixFile();


protected:
    QPoint mousePressPt;


private:
    QString pixmapfileName;

};
/*******************************************************/

#endif // MAINWINDOW_H
