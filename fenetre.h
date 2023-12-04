#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <fstream>
#include <cmath>
#include "dialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeView>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QCheckBox>
#include "tableview.h"
#include "fenrecherche.h"
#include "threadcopie.h"
#include <direct.h>
#include <windows.h>
#include <winbase.h>


class Fenetre : public QMainWindow
{
    Q_OBJECT

public:
    //Fenetre(QWidget *parent = nullptr);
    Fenetre(QString cheminDepart = "Ordinateur", QWidget *parent = nullptr);
    ~Fenetre();


    void ouvrirDossier(QString chemin, bool ouvertureViaHistorique = false);
    void ouvrirFichier(QString chemin);
    static QString tailleFichierReduite(qint64 tailleOctets, int precision = 0);

    void activerBoutonsModeA(bool activer=true); /* Active/désactive certains éléments lorsqu'on sélectionne un item
                ou qu'aucun item n'est sélectionné dans le tableview (Proriétés, copier, supprimer, etc.)*/

    void activerBoutonsModeB(); // Active/désactive les boutons "Nouveau dossier", "Renommer" et
                //"Supprimer", "copier", "couper", selon qu'on soit à l'emplacement "Ordinateur" ou non

    TableView* getTableView();

    static QString getTypeElement(QFileInfo info); //Renvoie le type (fichier, dossier, raccourci) d'un élément

    static QMap<QString, QString> getCheminsSpeciaux(); //Calcule les chemins spéciaux (bureau, documents, etc.) et les ajoute dans la QMap cheminsSpeciaux

    static QMap<QString, QString> iconesFenetres;
                    //La clé est le chemin absolu du dossier, la valeur est le chemin de l'icône associée
    QMenu *menuFichier;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

public slots:
    void creerDossier(QString type = "dossier", QString extension = "");
    void creerFichier();
    void actualiser();
    void renommerElement();
    void supprimerElement();
    void supprimerFichierOuDossier(QFileInfo info);
    void copier();
    void couper();
    void coller();
    void apresColler(bool resultat, QString type, QString cheminDest); //Ce qui s'exécute une fois que la copie est terminée. resultat = true si la copie s'est bien passée
                                    //type représente le type de l'élément collé (fichier, dossier, etc.)
                                    //chemDest est le chemin de l'élément collé
    void afficherAPropos();
    void actualiserStatutTailleFichier();
    void afficherMenuContextuel(QContextMenuEvent *event);

    void showHiddenElements(int state); //Affiche ou Masque les éléments cachés
    //rowsInserted(const QModelIndex &parent, int first, int last)

    void ouvrirRecherche(); //Ouvre la fenêtre de recherche

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_btParent_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_treeView_clicked(const QModelIndex &index);

    void on_lineEdit_returnPressed();

    void actualiserBarreDEtat(QString chemin);

    void actualiserBarreDEtat();

    void on_btPrecedent_clicked();

    void ajouterDansHistorique(QString chemin);

    void on_btSuivant_clicked();

    void nouvelleFenetre();

    void nouvelleFenetreOrdinateur();

    void afficherProprietes();

    void allerA(); //accède aux dossiers "Ordinateur", "Bureau", "Documents", "Téléchargements", "Images"

private:
    static QList<Fenetre*> listeFenetres; //Liste qui contiendra toutes les fenêtres ouvertes
    QMenu  *menuContextuel; //Le menu contextuel du tableView

    QFileSystemModel *modele; //Le modèle du QTableView
    QFileSystemModel *modeleArbre; //Le modèle du QTreeView
    QLabel *labelStatut1;
    QLabel *labelStatutTaille;
    QList<QString> historique;
    int positionDansHistorique = 0;
    int const MAX_HISTORIQUE = 30; //le nombre maximal d'éléments présents dans l'historique

    bool situeAOrdinateur = true; //true si on est situé à l'emplacement "Ordinateur"

    static QFileInfo sourceCopie; //Le QFileInfo représentant l'élément copié.
    static QString cheminSource;//Le chemin absolu de l'élément copié
    static bool couperSource; //sera false si on veut copier, et true si on veut couper.
    static bool premiereFenetre; //indique si la fenêtre crée est la première fenêtre

    //static QString cheminOrdinateur, cheminBureau, cheminDocuments, cheminTelechargements, cheminImages;
    static QMap<QString, QString> cheminsSpeciaux; //La clé est une chaîne de caractères (ex: "bureau"),
                            // la valeur est le chemin du dossier spécial correspondant (ex: C:/Users/DJOUMESSI/Desktop)

    QPushButton *btActualiser;

        QAction *actionProprietes;
        QAction *action_newFen;
        QAction *action_Quitter;
        QAction *actionNouvelle_fen_tre_Ordinateur;
        QAction *actionCreerDossier;
        QAction *actionRenommer;
        QAction *actionSupprimer;
        QAction *actionNouveau_fichier;
        QAction *actionCopier;
        QAction *actionCouper;
        QAction *actionColler;
        QAction *actionAPropos;
        QAction *actionOrdinateur;
        QAction *actionBureau;
        QAction *actionDocuments;
        QAction *actionTelechargements;
        QAction *actionImages;
        QAction *actionMusique;
        QAction *actionVideos;
        QWidget *centralwidget;
        QVBoxLayout *verticalLayout_2;
        QVBoxLayout *verticalLayout;
        QHBoxLayout *horizontalLayout;
        QPushButton *btProprietes;
        QPushButton *btPrecedent;
        QPushButton *btSuivant;
        QPushButton *btParent;
        QLineEdit *lineEdit;
        QSplitter *splitter;
        QTreeView *treeView;
        TableView *tableView;
        QMenuBar *menubar;
        //QMenu *menuFichier; //Je l'ai mis en PUBLIC plus haut pour y accéder facilement dans main.cpp
        QMenu *menuEdition;
        QMenu *menu;
        QStatusBar *statusbar;
        QToolBar *toolBar;
        QCheckBox *checkHidden;

        QPushButton *btRech;

};
#endif // FENETRE_H
