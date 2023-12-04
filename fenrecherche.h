#ifndef FENRECHERCHE_H
#define FENRECHERCHE_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <QTimer>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include "monthread.h"
#include "dialog.h"

class FenRecherche : public QMainWindow
{
    Q_OBJECT
public:
    explicit FenRecherche(QString chemin = "", QWidget *parent = nullptr);
    void setCheminDossier(QString chemin);
    QFileInfo selectionInfo(); //Renvoie le QFileInfo correspondant au résultat sélectionné

public slots:
    void afficherResultats();
    void avantRecherche(); //paramètre le thread de recherche et le lance
    void ouvrir(); //Ouvre le résultat de recherche (fichier ou dossier)
    void ouvrirEmplacement(); //Ouvre l'emplacement du fichier ou dossier
    void ouvrirProprietes();
    void contextMenuEvent(QContextMenuEvent *event) override;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    QString cheminDossier; //Le chemin du dossier dans lequel on fait la recherche
    QFileInfo infoDossier;

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layoutV = new QVBoxLayout(this);
    QHBoxLayout *layoutRech = new QHBoxLayout(this);
    QLabel *labelNom = new QLabel(this);
    QLabel *labelChemin = new QLabel(this);
    QLabel *labelDetails = new QLabel(this);
    QLineEdit *editRech = new QLineEdit(this);
    QPushButton *btRech = new QPushButton("Rechercher",this);
    QTableView *tableView = new QTableView(this);

    MonThread *thread = new MonThread;
    QStandardItemModel *modele = new QStandardItemModel;

    QMenu *menu = new QMenu("Actions", tableView);
    QAction *actionOuvrir = new QAction("Ouvrir", this);
    QAction *actionEmplacement = new QAction("Ouvrir son emplacement", this);
    QAction *actionProprietes = new QAction("Propriétés",this);

signals:

};

#endif // FENRECHERCHE_H
