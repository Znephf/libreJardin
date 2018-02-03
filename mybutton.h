#ifndef MYBUTTON_H
#define MYBUTTON_H


#include <QToolButton>
#include <QEvent>

class Button : public QToolButton
{
    Q_OBJECT

public:
    explicit Button(const QString &text, QWidget *parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;


    void setNom(const QString nom)
    {
      m_BoutonNom = nom;
    }

    QString getNom()
    {
      return m_BoutonNom;
    }
    void setFamille(const int &famille)
    {
      m_itemFamille = famille;
    }

    int getFamille()
    {
      return m_itemFamille;
    }

    void setCodeArticle(const int &codeArticle)
    {
      m_codeArticle = codeArticle;
    }

    int getCodeArticle()
    {
      return m_codeArticle;
    }

    void setPV(const float &PrixVente)
    {
      m_prixVente = PrixVente;
    }

    float getPV()
    {
      return m_prixVente;
    }

    void setUnite(const int &unite)
    {
      m_unite = unite;
    }

    int getunite()
    {
      return m_unite;
    }

    void setTaux(const int &taux)
    {
      m_taux = taux;
    }

    int getTaux()
    {
      return m_taux;
    }

    void setId(const int &Id)
    {
      m_Id = Id;
    }

    int getId()
    {
      return m_Id;
    }
private slots:


protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);


private:
    QString m_BoutonNom;
    int m_itemFamille;
    int m_codeArticle;
    float m_prixVente;
    int m_unite;
    int m_taux;
    int m_Id;
};



#endif // MYBUTTON_H
