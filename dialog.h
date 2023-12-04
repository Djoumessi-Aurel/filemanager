#ifndef DIALOG_H
#define DIALOG_H

#include <QtCore/QVariant>
#include <QDialog>
#include <QLineEdit>
#include <QFileInfo>
#include <QDateTime>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QWidget>

#ifndef FENETRE_H
#include "fenetre.h"
#endif

/* Classe représentant la boîte de dialogue <<Propriétés>> */

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setFileInfo(QFileInfo const& fileInfo);

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;

    QLineEdit *nom = new QLineEdit(this);
    QLineEdit *type = new QLineEdit(this); //Fichier, dossier, racine
    QLineEdit *emplacement = new QLineEdit(this);
    QLineEdit *cheminComplet = new QLineEdit(this);
    QLineEdit *taille = new QLineEdit(this);
    QLineEdit *lastModified = new QLineEdit(this);
    QLineEdit *lastRead = new QLineEdit(this);
    QLineEdit *created = new QLineEdit(this);

    QLabel *contenu = new QLabel(this);
};

#endif // DIALOG_H
