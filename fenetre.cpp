#include <QApplication>
#include "fenetre.h"
#include "tchar.h"
#include "shlobj.h"

QList<Fenetre*> Fenetre::listeFenetres;

QString Fenetre::cheminSource;
QFileInfo Fenetre::sourceCopie;
bool Fenetre::couperSource = false;
bool Fenetre::premiereFenetre = true; //indique si la fenêtre crée est la première fenêtre
QMap<QString, QString> Fenetre::iconesFenetres;
QMap<QString, QString> Fenetre::cheminsSpeciaux = Fenetre::getCheminsSpeciaux();


Fenetre::Fenetre(QString cheminDepart, QWidget *parent)
    : QMainWindow(parent)
{
    Fenetre::listeFenetres.append(this); //On ajoute la fenêtre dans le tableau
    std::cout<<"Nombre de fenêtre ouvertes: "<<Fenetre::listeFenetres.size()<<std::endl;

    this->resize(800,540);
    this->setWindowTitle("Ordinateur");

    if(!premiereFenetre)
    {
        this->setAttribute(Qt::WA_DeleteOnClose, true); //Si ce n'est pas le première fenêtre
    }
    else premiereFenetre = false;

        /* INITIALISATION DU CENTRALWIDGET, DU TABLEVIEW, DES BOUTONS, ETC */
    centralwidget = new QWidget(this);
            verticalLayout_2 = new QVBoxLayout(centralwidget);
            verticalLayout = new QVBoxLayout();
            horizontalLayout = new QHBoxLayout();
            btPrecedent = new QPushButton(centralwidget);
            QIcon icon11;
            icon11.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/precedent.png"), QSize(), QIcon::Normal, QIcon::Off);
            btPrecedent->setIcon(icon11);
            btPrecedent->setIconSize(QSize(24, 24));
            btPrecedent->setFlat(true);

            horizontalLayout->addWidget(btPrecedent);

            btSuivant = new QPushButton(centralwidget);
            QIcon icon12;
            icon12.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/suivant.png"), QSize(), QIcon::Normal, QIcon::Off);
            btSuivant->setIcon(icon12);
            btSuivant->setIconSize(QSize(24, 24));
            btSuivant->setFlat(true);

            horizontalLayout->addWidget(btSuivant);

            btParent = new QPushButton(centralwidget);
            QIcon icon13;
            icon13.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/parent.png"), QSize(), QIcon::Normal, QIcon::Off);
            btParent->setIcon(icon13);
            btParent->setIconSize(QSize(24, 24));
            btParent->setFlat(true);
            horizontalLayout->addWidget(btParent);

            lineEdit = new QLineEdit(centralwidget);
            horizontalLayout->addWidget(lineEdit);

            btRech = new QPushButton(centralwidget);
            QIcon icon14;
            icon14.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/rechercher.png"), QSize(), QIcon::Normal, QIcon::Off);
            btRech->setIcon(icon14);
            btRech->setIconSize(QSize(24, 24));
            btRech->setFlat(true);
            horizontalLayout->addWidget(btRech);

            verticalLayout->addLayout(horizontalLayout);
            verticalLayout_2->addLayout(verticalLayout);

            splitter = new QSplitter(centralwidget);
            splitter->setOrientation(Qt::Horizontal);
            treeView = new QTreeView(splitter);
            splitter->addWidget(treeView);
            tableView = new TableView(splitter);
            splitter->addWidget(tableView);

            verticalLayout_2->addWidget(splitter);

            this->setCentralWidget(centralwidget);

        /**/

    btActualiser = new QPushButton("Actual.");
    QObject::connect(btActualiser, SIGNAL(clicked()), this, SLOT(actualiser()));

    QList<int> liste;
    liste.append(100); liste.append(400); //Les tailles relatives des deux éléments du QSplitter
    splitter->setSizes(liste);

    btParent->setEnabled(false);
    btPrecedent->setEnabled(false);
    btSuivant->setEnabled(false);
    btRech->setEnabled(false);

    modele = new QFileSystemModel();
    modele->setReadOnly(false); //Pour permettre les modifications (copier, coller, supprimer, etc.)
    modele->setRootPath(QDir::currentPath());

    /* On actualise la barre d'état lorsque qu'un dossier a fini d'être chargé */
    QObject::connect(modele,SIGNAL(directoryLoaded(QString)), this, SLOT(actualiserBarreDEtat(QString)));


        tableView->setSortingEnabled(true); //Pour permettre de ranger selon les colonnes
        tableView->setCornerButtonEnabled(false);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //empêcher la modification des cellules du tableau
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection); //On ne peut sélectionner qu'une ligne à la fois
        tableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);//Ranger selon la colonne "Name" par ordre croissant

     modeleArbre = new QFileSystemModel();
     modeleArbre->setFilter(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot); //Le treeview n'affichera que les dossiers
     modeleArbre->setRootPath(QDir::currentPath());
     treeView->setModel(modeleArbre);
     treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
     treeView->header()->resizeSection(0,300);

     /*#########################MENUS , TOOLBAR ET QActions#######################*/
     //D'abord les QAction
     actionProprietes = new QAction(this);
     actionProprietes->setText("Propriétés");
     QIcon icon;
     icon.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/proprietes.png"), QSize(), QIcon::Normal, QIcon::Off);
     actionProprietes->setIcon(icon);

             action_newFen = new QAction(this);
             QIcon icon0;
             icon0.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/new_window.png"), QSize(), QIcon::Normal, QIcon::Off);
             action_newFen->setIcon(icon0);
             action_Quitter = new QAction(this);
             QIcon icon1;
             icon1.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/quitter.png"), QSize(), QIcon::Normal, QIcon::Off);
             action_Quitter->setIcon(icon1);
             actionNouvelle_fen_tre_Ordinateur = new QAction(this);
             QIcon icon2;
             icon2.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/new_window_home.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionNouvelle_fen_tre_Ordinateur->setIcon(icon2);
             actionCreerDossier = new QAction(this);
             QIcon icon3;
             icon3.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/new_folder.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionCreerDossier->setIcon(icon3);
             actionRenommer = new QAction(this);
             QIcon icon4;
             icon4.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/renommer.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionRenommer->setIcon(icon4);
             actionSupprimer = new QAction(this);
             QIcon icon5;
             icon5.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/supprimer.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionSupprimer->setIcon(icon5);
             actionNouveau_fichier = new QAction(this);
             QIcon icon6;
             icon6.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/new_file.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionNouveau_fichier->setIcon(icon6);
             actionCopier = new QAction(this);
             QIcon icon7;
             icon7.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/copier.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionCopier->setIcon(icon7);
             actionCouper = new QAction(this);
             QIcon icon8;
             icon8.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/couper.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionCouper->setIcon(icon8);
             actionColler = new QAction(this);
             QIcon icon9;
             icon9.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/coller.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionColler->setIcon(icon9);
             actionAPropos = new QAction(this);
             QIcon icon10;
             icon10.addFile(QString::fromUtf8(":/D:/images/icones/icones filemanager/a_propos.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionAPropos->setIcon(icon10);

             QAction *actionOrdinateur = new QAction(this);
             actionOrdinateur->setText("Ordinateur");
             actionOrdinateur->setData(Fenetre::cheminsSpeciaux["ordinateur"]);
             QIcon icon20;
             icon20.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/ordinateur.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionOrdinateur->setIcon(icon20);

             QAction *actionBureau = new QAction(this);
             actionBureau->setText("Bureau");
             actionBureau->setData(Fenetre::cheminsSpeciaux["bureau"]);
             QIcon icon21;
             icon21.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/bureau.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionBureau->setIcon(icon21);

             QAction *actionDocuments = new QAction(this);
             actionDocuments->setText("Documents");
             actionDocuments->setData(Fenetre::cheminsSpeciaux["documents"]);
             QIcon icon22;
             icon22.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/documents.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionDocuments->setIcon(icon22);

             QAction *actionTelechargements = new QAction(this);
             actionTelechargements->setText("Téléchargements");
             actionTelechargements->setData(Fenetre::cheminsSpeciaux["téléchargements"]);
             QIcon icon23;
             icon23.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/download.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionTelechargements->setIcon(icon23);

             QAction *actionImages = new QAction(this);             
             actionImages->setText("Images");
             actionImages->setData(Fenetre::cheminsSpeciaux["images"]);
             QIcon icon24;
             icon24.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/images.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionImages->setIcon(icon24);

             QAction *actionMusique = new QAction(this);
             actionMusique->setText("Musique");
             actionMusique->setData(Fenetre::cheminsSpeciaux["musique"]);
             QIcon icon25;
             icon25.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/musique.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionMusique->setIcon(icon25);

             QAction *actionVideos = new QAction(this);
             actionVideos->setText("Vidéos");
             actionVideos->setData(Fenetre::cheminsSpeciaux["vidéos"]);
             QIcon icon26;
             icon26.addFile(QString::fromUtf8(":/D:/images/icones/Nouveau dossier/vidéos.png"), QSize(), QIcon::Normal, QIcon::Off);
             actionVideos->setIcon(icon26);

             checkHidden = new QCheckBox("Afficher les éléments cachés");

      //Ensuite les menus et la barre d'outils
             menubar = new QMenuBar(this);
             menubar->setGeometry(QRect(0, 0, 596, 21));
             menuFichier = new QMenu(menubar);
             menuEdition = new QMenu(menubar);
             menu = new QMenu(menubar);
             this->setMenuBar(menubar);
             statusbar = new QStatusBar(this);
             this->setStatusBar(statusbar);
             toolBar = new QToolBar(this);
             this->addToolBar(Qt::TopToolBarArea, toolBar);

             menubar->addAction(menuFichier->menuAction());
             menubar->addAction(menuEdition->menuAction());
             menubar->addAction(menu->menuAction());
             menuFichier->addAction(action_newFen);
             menuFichier->addAction(actionNouvelle_fen_tre_Ordinateur);
             menuFichier->addSeparator();
             menuFichier->addAction(action_Quitter);
             menuEdition->addAction(actionCreerDossier);
             menuEdition->addAction(actionNouveau_fichier);
             menuEdition->addAction(actionRenommer);
             menuEdition->addSeparator();
             menuEdition->addAction(actionCopier);
             menuEdition->addAction(actionCouper);
             menuEdition->addAction(actionColler);
             menuEdition->addSeparator();
             menuEdition->addAction(actionSupprimer);
             menu->addAction(actionAPropos);

             // REMPLISSAGE DE LA BARRE D'OUTILS
             toolBar->addAction(actionCreerDossier);
             toolBar->addAction(actionNouveau_fichier);
             toolBar->addAction(actionRenommer);
             toolBar->addAction(actionSupprimer);
             toolBar->addSeparator();
             toolBar->addAction(actionCopier);
             toolBar->addAction(actionCouper);
             toolBar->addAction(actionColler);
             toolBar->addSeparator();
             toolBar->addAction(actionProprietes);
             toolBar->addWidget(new QLabel("          Aller à:     "));
             toolBar->addAction(actionOrdinateur);
             toolBar->addSeparator();
             toolBar->addAction(actionBureau);
             toolBar->addSeparator();
             toolBar->addAction(actionDocuments);
             toolBar->addSeparator();
             toolBar->addAction(actionTelechargements);
             toolBar->addSeparator();
             toolBar->addAction(actionImages);
             toolBar->addSeparator();
             toolBar->addAction(actionMusique);
             toolBar->addSeparator();
             toolBar->addAction(actionVideos);

             toolBar->addWidget(new QLabel("             "));
             toolBar->addWidget(checkHidden);

     //Maintenant, les raccourcis et les tooltip
                     action_newFen->setText(QCoreApplication::translate("Fenetre", "&Nouvelle fenêtre", nullptr));
                     action_newFen->setToolTip(QCoreApplication::translate("Fenetre", "Ouvir une fenêtre à l'emplacement actuel", nullptr));
                     action_newFen->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+N", nullptr));
                     action_Quitter->setText(QCoreApplication::translate("Fenetre", "&Fermer", nullptr));
                     action_Quitter->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+Q", nullptr));
                     actionNouvelle_fen_tre_Ordinateur->setText(QCoreApplication::translate("Fenetre", "Nouvelle fenêtre \"&Ordinateur\"", nullptr));
                     actionNouvelle_fen_tre_Ordinateur->setToolTip(QCoreApplication::translate("Fenetre", "Ouvir une fenêtre à l'emplacement \"Ordinateur\"", nullptr));
                     actionNouvelle_fen_tre_Ordinateur->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+Alt+N", nullptr));
                     actionCreerDossier->setText(QCoreApplication::translate("Fenetre", "Créer un dossier", nullptr));
                     actionCreerDossier->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+D", nullptr));
                     actionRenommer->setText(QCoreApplication::translate("Fenetre", "Renommer", nullptr));
                     actionSupprimer->setText(QCoreApplication::translate("Fenetre", "Supprimer", nullptr));
                     actionSupprimer->setShortcut(QCoreApplication::translate("Fenetre", "Del", nullptr));
                     actionNouveau_fichier->setText(QCoreApplication::translate("Fenetre", "Nouveau fichier texte", nullptr));
                     actionNouveau_fichier->setToolTip(QCoreApplication::translate("Fenetre", "Créer un nouveau fichier texte", nullptr));
                     actionNouveau_fichier->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+Alt+D", nullptr));
                     actionCopier->setText(QCoreApplication::translate("Fenetre", "Copier", nullptr));
                     actionCopier->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+C", nullptr));
                     actionCouper->setText(QCoreApplication::translate("Fenetre", "Couper", nullptr));
                     actionCouper->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+X", nullptr));
                     actionColler->setText(QCoreApplication::translate("Fenetre", "Coller", nullptr));
                     actionColler->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+V", nullptr));
                     actionAPropos->setText(QCoreApplication::translate("Fenetre", "A propos de FileManager", nullptr));

                     btPrecedent->setToolTip(QCoreApplication::translate("Fenetre", "Précédent", nullptr));
                     btPrecedent->setText(QString());
                     btPrecedent->setShortcut(QCoreApplication::translate("Fenetre", "Backspace", nullptr));

                     btSuivant->setToolTip(QCoreApplication::translate("Fenetre", "Suivant", nullptr));
                     btSuivant->setText(QString());
                     btSuivant->setShortcut(QCoreApplication::translate("Fenetre", "Alt+Right", nullptr));

                     btParent->setToolTip(QCoreApplication::translate("Fenetre", "Dossier Parent", nullptr));
                     btParent->setText(QString());
                     btParent->setShortcut(QCoreApplication::translate("Fenetre", "Alt+Up", nullptr));

                     btRech->setToolTip(QCoreApplication::translate("Fenetre", "Faire une recherche", nullptr));
                     btParent->setShortcut(QCoreApplication::translate("Fenetre", "Ctrl+R", nullptr));

                     menuFichier->setTitle(QCoreApplication::translate("Fenetre", "&Fichier", nullptr));
                     menuEdition->setTitle(QCoreApplication::translate("Fenetre", "&Edition", nullptr));
                     menu->setTitle(QCoreApplication::translate("Fenetre", "?", nullptr));

                     toolBar->setWindowTitle(QCoreApplication::translate("Fenetre", "toolBar", nullptr));

     /*########################FIN MENUS et QActions#####################*/

                     this->setStyleSheet("QTableView::item:!selected:hover {"
                         "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #FAFBFE, stop: 1 rgba(110,180,255,30%));}"
                                         "QTableView{gridline-color:rgba(210,230,255,1%);}");

            /*################## LA BARRE DE STATUT ##################*/
                     labelStatut1 = new QLabel();
                     this->statusBar()->addWidget(labelStatut1);
                     labelStatutTaille = new QLabel("Barbie");
                     this->statusBar()->addWidget(labelStatutTaille);

  /*CONNEXION DES signaux des QAction AUX différents SLOTS*/
  QObject::connect(action_newFen, SIGNAL(triggered()), this, SLOT(nouvelleFenetre()));
  QObject::connect(actionNouvelle_fen_tre_Ordinateur, SIGNAL(triggered()), this, SLOT(nouvelleFenetreOrdinateur()));
  QObject::connect(actionCreerDossier, SIGNAL(triggered()), this, SLOT(creerDossier()));
  QObject::connect(actionNouveau_fichier, SIGNAL(triggered()), this, SLOT(creerFichier()));
  QObject::connect(actionRenommer, SIGNAL(triggered()), this, SLOT(renommerElement()));
  QObject::connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimerElement()));
  QObject::connect(actionCopier, SIGNAL(triggered()), this, SLOT(copier()));
  QObject::connect(actionCouper, SIGNAL(triggered()), this, SLOT(couper()));
  QObject::connect(actionColler, SIGNAL(triggered()), this, SLOT(coller()));
  QObject::connect(actionAPropos, SIGNAL(triggered()), this, SLOT(afficherAPropos()));
  QObject::connect(actionProprietes, SIGNAL(triggered()), this, SLOT(afficherProprietes()));
  QObject::connect(action_Quitter, SIGNAL(triggered()), this, SLOT(close()));

  QObject::connect(actionOrdinateur, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionBureau, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionDocuments, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionTelechargements, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionImages, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionMusique, SIGNAL(triggered()), this, SLOT(allerA()));
  QObject::connect(actionVideos, SIGNAL(triggered()), this, SLOT(allerA()));

  QObject::connect(checkHidden, SIGNAL(stateChanged(int)), this, SLOT(showHiddenElements(int)));

  QObject::connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_tableView_clicked(QModelIndex)));
  QObject::connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_tableView_doubleClicked(QModelIndex)));
  QObject::connect(tableView, SIGNAL(activated(QModelIndex)), this, SLOT(on_tableView_activated(QModelIndex)));
  QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_treeView_clicked(QModelIndex)));
  QObject::connect(btParent, SIGNAL(clicked()), this, SLOT(on_btParent_clicked()));
  QObject::connect(btPrecedent, SIGNAL(clicked()), this, SLOT(on_btPrecedent_clicked()));
  QObject::connect(btSuivant, SIGNAL(clicked()), this, SLOT(on_btSuivant_clicked()));
  QObject::connect(btRech, SIGNAL(clicked()), this, SLOT(ouvrirRecherche()));
  QObject::connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));

  QObject::connect(tableView, SIGNAL(changementDeLigne()), this, SLOT(actualiserStatutTailleFichier()));
  QObject::connect(tableView, SIGNAL(clicDroit(QContextMenuEvent*)), this, SLOT(afficherMenuContextuel(QContextMenuEvent*)));

  /* REMPLISSAGE DU MENU CONTEXTUEL */
  menuContextuel = new QMenu(tableView);

  menuContextuel->addAction(actionProprietes);
  menuContextuel->addSeparator();
  menuContextuel->addAction(actionCopier);
  menuContextuel->addAction(actionCouper);
  menuContextuel->addAction(actionColler);
  menuContextuel->addSeparator();
  menuContextuel->addAction(actionRenommer);
  menuContextuel->addAction(actionSupprimer);
  menuContextuel->addSeparator();
  menuContextuel->addAction(actionCreerDossier);
  menuContextuel->addAction(actionNouveau_fichier);
  menuContextuel->addAction(action_newFen);
  menuContextuel->addAction(actionNouvelle_fen_tre_Ordinateur);

  /* POUR TERMINER ON OUVRE L'EMPLACEMENT VOULU (cheminDepart) */
  tableView->setModel(modele);
  tableView->horizontalHeader()->resizeSection(0,280); //On agrandit un peu la colonne "Name"
  this->ouvrirDossier(cheminDepart);

}

Fenetre::~Fenetre()
{
    std::cout<<"Destruction d'une fenêtre"<<Fenetre::listeFenetres.size()<<std::endl;
}

void Fenetre::closeEvent(QCloseEvent *event)
{
    Fenetre::listeFenetres.removeAll(this); //On retire la fenêtre du tableau
    std::cout<<"Nombre de fenêtre restantes: "<<Fenetre::listeFenetres.size()<<std::endl;

    event->accept();
    //this->destroy(); //On libère la mémoire occupée par la fenêtre (ça marche vraiment???? NON - peut faire PLANTER!)
}

void Fenetre::on_btParent_clicked()
{
    //QFileInfo info = modele->fileInfo(tableView->rootIndex()); //ceci chie lorsqu'on est dans une nouvelle fenêtre
    QFileInfo info(historique.at(positionDansHistorique));
        QDir parent = info.dir(); //On récupère le dossier parent du dossier dans lequel on se trouve actuellement

        if(info.isRoot()) //Si on est à la racine d'un disque
        {
            this->ouvrirDossier("Ordinateur");
        }

        else if(parent.exists())
        {
            this->ouvrirDossier(parent.absolutePath());
        }
        else{
            std::cout<<"Echec btParent"<<std::endl;
        }

}

void Fenetre::on_tableView_clicked(const QModelIndex &index)
{
   activerBoutonsModeA(true); /* Active les boutons qui doivent être activés lorsqu'une ligne du
        tableview est sélectionnée (Propriétés, copier, couper, supprimer, etc.)*/
}

void Fenetre::on_tableView_doubleClicked(const QModelIndex &index)
{
/* Ceci est déjà pris en compte par le slot on_tableView_activated(const QModelIndex &index)
 Insérer du code ici créerait un doublon */
}

void Fenetre::on_tableView_activated(const QModelIndex &index)//Lorsqu'on appuie "entrée" quand une ligne est sélectionnée ou qu'on double-clique sur la ligne
{
    QFileInfo info = modele->fileInfo(index);
     std::cout<<"Clic sur le TableView: "<<info.absoluteFilePath().toStdString()<<"**"<<info.isDir()<<"**"<<info.isRoot()<<std::endl;
    this->ouvrirDossier(info.absoluteFilePath());
}

void Fenetre::on_treeView_clicked(const QModelIndex &index)
{
    this->on_tableView_activated(index);
}

void Fenetre::ouvrirDossier(QString chemin, bool ouvertureViaHistorique)
{
    bool fichierOuvert = false; //Va devenir true si c'est un fichier qu'on ouvre
std::cout<<"chemin à ouvrir est: "<<chemin.toStdString()<<std::endl;

 if(chemin=="Ordinateur")
 {
     tableView->setRootIndex(modele->setRootPath("")); situeAOrdinateur = true;

     std::cout<<"Essayons de monter à ORDINATEUR"<<std::endl;
     lineEdit->setText(chemin);
     btParent->setEnabled(false);
     btRech->setEnabled(false);

     std::cout<<situeAOrdinateur<<std::endl;

     //Ajout du chemin du dossier ouvert dans l'historique, si ce n'est pas une navigation dans l'historique
     if(!ouvertureViaHistorique) ajouterDansHistorique(chemin);

 }

 else if(QFileInfo::exists(chemin))
 {
     QFileInfo info(chemin);

     //Si c'est un lien, on récupère le chemin absolu de ce vers quoi il pointe
     QString cheminAOuvrir = info.isSymLink() ? info.symLinkTarget() : info.absoluteFilePath();

     if(info.isDir()) //Si c'est un dossier (ou un lien vers un dossier)
         {
         tableView->setRootIndex(modele->setRootPath(cheminAOuvrir)); //On ouvre le dossier
         situeAOrdinateur = false;
         lineEdit->setText(modele->rootPath());//On indique le chemin absolu dans la zone de texte
         btParent->setEnabled(true);
         btRech->setEnabled(true);
         //Ajout du chemin du dossier ouvert dans l'historique, si ce n'est pas une navigation dans l'historique
         if(!ouvertureViaHistorique) ajouterDansHistorique(cheminAOuvrir);
         }
         else if(info.isFile())  //Si c'est un fichier (ou un lien vers un fichier)
         {
             this->ouvrirFichier(cheminAOuvrir);
             fichierOuvert = true;
         }
 }

 if(!fichierOuvert) this->labelStatutTaille->setText("");
 //this->setWindowTitle(this->historique.at(positionDansHistorique));

 //On écrit le nom du dossier dans lequel on se trouve en titre de la fenêtre
 if(this->situeAOrdinateur) {this->setWindowTitle("Ordinateur");}
 else if(QFileInfo(chemin).isRoot()) { this->setWindowTitle(chemin); }
 else { this->setWindowTitle(QDir(modele->rootPath()).dirName()); }

 if(!fichierOuvert) this->activerBoutonsModeA(false);

 /* Modification de l'icône de la fenêtre*/
 QMap<QString, QString>::const_iterator i = iconesFenetres.constBegin(); //Pour parcourir le tableau des icônes

 if(QFileInfo(chemin).isRoot()) {
     this->setWindowIcon(QIcon(iconesFenetres.value("disque"))); //L'icône des disques (C:, etc.)
     return;
 }

  while (i != iconesFenetres.constEnd()) {
      if(i.key().toLower() == historique.at(positionDansHistorique).toLower())
      {
          QIcon ic;
          ic.addFile(i.value());
          this->setWindowIcon(ic);
          return; //On sort de la fonction
      }
      ++i;
  }

  this->setWindowIcon(QIcon(iconesFenetres.value("dossier"))); //L'icône par défaut
}

void Fenetre::ouvrirFichier(QString chemin)
{
    TCHAR commandeW[_MAX_PATH] = _T("");
    chemin.toWCharArray(commandeW);

        //On lance le fichier
        ShellExecuteW(NULL, _T("open"), commandeW, NULL, NULL, SW_SHOW);
}

void Fenetre::on_lineEdit_returnPressed()
{
    this->ouvrirDossier(lineEdit->text());
}

void Fenetre::actualiserBarreDEtat()
{
    this->actualiserBarreDEtat(historique.at(positionDansHistorique));
}

void Fenetre::actualiserBarreDEtat(QString chemin) //Indique le nombre d'éléments du dossier ouvert dans la barre d'état
{
    if(this->situeAOrdinateur) {labelStatut1->setText("");} //Si on est revenu à "Ordinateur"
    else
    {
        labelStatut1->setText(QString("Nombre d'éléments: ")+QString::number(modele->rowCount(modele->index(chemin))));
    }

    QModelIndexList listeIndex = tableView->getSelectedIndexes();
    if(listeIndex.size()>0) //Si un élément du tableview est sélectionné...
    {
        //activerBoutonsModeA();
        tableView->setFocus();
        tableView->scrollTo(listeIndex.at(0));
        this->actualiserStatutTailleFichier();
    }
    /*
std::cout<<"Modèle: "<<modele->rowCount(modele->index(modele->rootPath()))<<std::endl;
std::cout<<"Modèle 2: "<<modele->rowCount(modele->index(chemin))<<std::endl;
std::cout<<"Tableau: "<<modele->rowCount(tableView->rootIndex())<<std::endl;
    */
//std::cout<<"Tableau 2: "<<modele->rowCount(tableView->currentIndex())<<std::endl; //Celui-ci chie flop
}

void Fenetre::on_btPrecedent_clicked()
{
 if(positionDansHistorique>0)
 {
     positionDansHistorique --;
     ouvrirDossier(historique.at(positionDansHistorique), true);
     if(!btSuivant->isEnabled()) btSuivant->setEnabled(true);

     if(positionDansHistorique==0) btPrecedent->setEnabled(false);
 }
}

void Fenetre::on_btSuivant_clicked()
{
    if(positionDansHistorique<historique.size()-1)
    {
        positionDansHistorique ++;
        ouvrirDossier(historique.at(positionDansHistorique), true);
        if(!btPrecedent->isEnabled()) btPrecedent->setEnabled(true);

        if(positionDansHistorique==historique.size()-1) btSuivant->setEnabled(false);
    }
}

void Fenetre::ajouterDansHistorique(QString chemin) //Ajoute un chemin dans l'historique
{
    while(positionDansHistorique < historique.size()-1)
    {
        historique.pop_back(); //On supprime tout ce qui est après notre position actuelle
    }

    //Ensuite, on ajoute le chemin dans l'historique
    if(historique.isEmpty()) {historique.append(chemin);}
    else if(historique.at(historique.size()-1)!=chemin) historique.append(chemin);

    positionDansHistorique = historique.size()-1;//On met à jour notre position actuelle dans l'historique
    if(positionDansHistorique>0) //S'il y a au moins un élément Précédent, on active le bouton Précédent
    {
        if(!btPrecedent->isEnabled()) btPrecedent->setEnabled(true);
    }
    if(btSuivant->isEnabled()) btSuivant->setEnabled(false);

    if(historique.size()>MAX_HISTORIQUE) historique.pop_front(); //On supprime le premier élément de l'historique
}

void Fenetre::nouvelleFenetre() //Ouvre une nouvelle fenêtre, à l'emplacement actuel
{
 Fenetre *f = new Fenetre(historique.at(positionDansHistorique));
 //f->ouvrirDossier(historique.at(positionDansHistorique));
 f->show();
}

void Fenetre::nouvelleFenetreOrdinateur()//Ouvre une nouvelle à l'emplacement "Ordinateur"
{
    Fenetre *f = new Fenetre();
    //f->ouvrirDossier("Ordinateur");
    f->show();
}

void Fenetre::actualiserStatutTailleFichier()
{
    QModelIndexList listeIndex = tableView->getSelectedIndexes();

    if(listeIndex.size()>0){//Si un élément est sélectionné

        this->activerBoutonsModeA(true);

        QFileInfo info = modele->fileInfo(listeIndex.at(0));

        if(info.isFile() && !info.isSymLink())//Si c'est un fichier, mais pas un raccourci
        {
            labelStatutTaille->setText("         1 élément sélectionné   "+ Fenetre::tailleFichierReduite(info.size(), 3));
        }
        else
        {
            labelStatutTaille->setText("         1 élément sélectionné   ");
        }

    }
    else {//si on n'a rien sélectionné, alors on efface la taille dans la barre de statut

        std::cout<<"Aucun élément sélecionné"<<std::endl;
        this->activerBoutonsModeA(false);
        labelStatutTaille->setText("");
    }
}

/*void Fenetre::actualiserStatutTailleFichier(QString texte)
{
    labelStatutTaille->setText(texte);
}*/

void Fenetre::afficherMenuContextuel(QContextMenuEvent *event)
{
    menuContextuel->exec(event->globalPos());
}

QString Fenetre::tailleFichierReduite(qint64 tailleOctets, int precision) //long long int équivant à qint64
                                      //precision c'est le nombre de chiffres après la virgule que le résultat aura
{
    double taille = (double)tailleOctets; double tailleReduite;

    int i = 0, ko = 1024, mo =  ko *1024, go = 1024 * mo;
    QList<QString> unites ={"octets", "ko", "Mo", "Go"};

    if(taille>=go) {tailleReduite = taille/go; i=3;}
    else if(taille>=mo) {tailleReduite = taille/mo; i=2;}
    else if(taille>=ko) {tailleReduite = taille/ko; i=1;}
    else {tailleReduite = taille; i=0;}

    if(precision==0){ precision = i<3 ? 1 : i-1;} //On crée une précision dépendamment de l'unité
    int puissance = pow(10, precision);

    tailleReduite =  ceil(puissance * tailleReduite) /puissance;

    return QString::number(tailleReduite) + " " + unites.at(i);
}

void Fenetre::afficherProprietes()
{
    Dialog * dialog = new Dialog();

    if(tableView->getSelectedIndexes().size()>0)
    {
        dialog->setFileInfo(modele->fileInfo(tableView->getSelectedIndexes()[0]));
        dialog->show();
    }

}

void Fenetre::activerBoutonsModeA(bool activer) /*Désactive les éléments qui doivent être désactivés
            lorsqu'aucun item n'est sélectionné dans le tableview (Proriétés, copier, supprimer, etc.)*/
{
    /* d'abord les éléments qu'on active si et seulement si une ligne est sélectionnée */
    actionProprietes->setEnabled(activer);

    this->actionRenommer->setEnabled(activer);
    this->actionSupprimer->setEnabled(activer);
    this->actionCopier->setEnabled(activer);
    this->actionCouper->setEnabled(activer);

    /* Ensuite, ceux qu'on désactive lorsqu'on est à "Ordinateur" */
    this->activerBoutonsModeB();
}

void Fenetre::activerBoutonsModeB()
{
    if(this->situeAOrdinateur)
    {
        this->actionCreerDossier->setEnabled(false);
        this->actionNouveau_fichier->setEnabled(false);
        this->actionRenommer->setEnabled(false);
        this->actionSupprimer->setEnabled(false);
        this->actionCopier->setEnabled(false);
        this->actionCouper->setEnabled(false);
        this->actionColler->setEnabled(false);
    }
    else {
        this->actionCreerDossier->setEnabled(true);
        this->actionNouveau_fichier->setEnabled(true);

        if(!Fenetre::cheminSource.isEmpty()) //Si on a un élément copié
            this->actionColler->setEnabled(true);
        else //Si RIEN n'est copié, on désactive le bouton coller
            this->actionColler->setEnabled(false);
    }
}


void Fenetre::creerDossier(QString type, QString extension)
{
    if(type!="fichier") extension = "";//Il n'y a que les fichiers qui utilisent l'extension

    if(situeAOrdinateur == false /* && positionDansHistorique>0 */)
    {
        bool ok = false;
        QString nomDossier = QInputDialog::getText(this, "Créer un " + type,
                                                   "Entrez le nom du " + type + ":              ",
                                               QLineEdit::Normal, QString(), &ok);

        if (ok && !nomDossier.isEmpty()) //Si on a entré un nom de dossier et cliqué sur OK
        {
            QRegExp rx("[^/:*<>|?\\\"]+");
            QRegExpValidator validator(rx, this);
            int pos = 0;

            if(validator.validate(nomDossier,pos) == QValidator::Acceptable &&
                    !nomDossier.contains("\\"))//Le nom saisi est valide
            {
                QString cheminNewDossier(historique.at(positionDansHistorique)+"/"+nomDossier+extension);
                QFileInfo newDossier(cheminNewDossier);

                if(newDossier.exists())
                {
                    QMessageBox::critical(this,"Echec de l'opération",
                                          "Un "+ type + " avec ce nom existe déjà!");
                    return;
                }

                /*//On crée un nouveau dossier(et donc une nouvelle ligne, et on la sélectionne)
                tableView->setCurrentIndex(modele->mkdir(tableView->rootIndex(),nomDossier));
                std::cout<<"Dossier créé à l'emplacement: "<<modele->rootPath().toStdString()<<std::endl;
                */

                //Convertissons la chaîne en un const char* dans l'encodage utilisé par le SE
                const char *commandeAnsi = cheminNewDossier.toLocal8Bit().constData();
                std::cout<<type.toStdString()<<" à creer : "<<commandeAnsi<<std::endl;

                if(type=="fichier")
                {
                    std::ofstream monFlux(commandeAnsi); //CREATION DU FICHIER
                    monFlux.close();
                }
                else{
                    bool check = mkdir(commandeAnsi); //CREATION DU DOSSIER

                        // vérifions si le dossier a été créé ou non
                        if (!check)
                            std::cout<<"Dossier créé\n";
                        else {
                            std::cout<<"Incapable de créer le dossier\n";
                        }
                }

              //ENFIN on sélectionne l'élément créé  // tableView->setCurrentIndex(modele->index(cheminNewDossier));
                tableView->setnomFichierASelectionner(nomDossier+extension);
                QTimer::singleShot( 200, tableView, SLOT(selectRowByName()) );

            }
            else //le nom saisi est invalide
            {
                QMessageBox::critical(this,"Echec de l'opération",
                                      "Le nom saisi est invalide!");
            }

        }

    }
    else{
        std::cout<<"On ne crée pas de dossier ici. SitueAOrdinateur="<<situeAOrdinateur
                <<" positionDansHistorique="<<positionDansHistorique<<std::endl;
    }
}

void Fenetre::creerFichier()
{
    creerDossier("fichier", ".txt");
}


void Fenetre::renommerElement()
{
    if(situeAOrdinateur == false && positionDansHistorique>0)
    {
        bool ok = false;
        QString newName = QInputDialog::getText(this, "Renommer", "Entrez le nouveau nom: \n"
                                                                  "(ne pas mettre l'extension)\n"
                                                                  "NB: pas de caractères spéciaux",
                                               QLineEdit::Normal, QString(), &ok);

        newName = newName.trimmed();//On retire les espaces au début et à la fin

        if (ok && !newName.isEmpty()) //Si on a entré un nom de dossier et cliqué sur OK
        {
            QString type="", extension="";

            //QRegExp rx("[^/:*<>|?\"\\]+");
            QRegExp rx("[^/:*<>|?\\\"]+");
            QRegExpValidator validator(rx, this);
            int pos = 0;

            if(validator.validate(newName,pos) == QValidator::Acceptable &&
                    !newName.contains("\\"))//Le nom saisi est valide
            {

                //On récupère les informations sur le fichier sélectionné
                QFileInfo info = modele->fileInfo(tableView->getSelectedIndexes()[0]);

                QString chemin = info.canonicalFilePath();

                if(info.isDir()){type = "Dossier";}

                else if(info.isFile())
                {
                    type = "Fichier"; extension = "." + info.suffix();
                }

                /*GERONS LE CAS DES RACCOURCIS*/
                if(info.isSymLink() || info.isShortcut())
                {
                    chemin = historique.at(positionDansHistorique) + "/" + info.fileName();
                    type = "Raccourci";
                    extension = "." + info.suffix();
                }
                /* FIN CAS DES RACCOURCIS*/

                std::cout<<"Chemin réel du fichier à renommer: "<<chemin.toStdString()<<std::endl;

                //On regarde si un dossier ou fichier avec le nouveau nom existe déjà
                QFileInfo newDossier(historique.at(positionDansHistorique)+"/"+newName+extension);

                if(newDossier.exists())
                {
                    QMessageBox::critical(this,"Echec de l'opération",
                                          "Un dossier ou fichier avec ce nom existe déjà!");
                    return;
                }
                //FIN

                QFile fichier(chemin);

                if(fichier.exists())
                {
                    QDir parent = info.dir();

                    if(fichier.rename(parent.absolutePath() + "/" + newName  + extension ))
                    {
                        std::cout<<type.toStdString()<<" renommé! :"<<fichier.fileName().toStdString()<<std::endl;
                    }
                    else{
                        QMessageBox::critical(this,"Echec de l'opération",
                                              "Le fichier \"" + fichier.fileName() + "\" n'a pas pu être renommé");
                        return;
                    }
                }

                //ENFIN on sélectionne l'élément créé  // tableView->setCurrentIndex(modele->index(cheminNewDossier));
                  tableView->setnomFichierASelectionner(newName+extension);
                  QTimer::singleShot( 200, tableView, SLOT(selectRowByName()) );

            }
            else //le nom saisi est invalide
            {
                QMessageBox::critical(this,"Echec de l'opération",
                                      "Le nom saisi est invalide!");
            }


        }

    }
    else{
        std::cout<<"On ne Renomme pas de dossier ici"<<std::endl;
    }
}


void Fenetre::supprimerElement()
{
    QString type = "";
    //On récupère les informations sur le fichier sélectionné
    QFileInfo info = modele->fileInfo(tableView->getSelectedIndexes()[0]);

    QString chemin = info.canonicalFilePath();

    if(info.isDir()){type = "Dossier";}

    else if(info.isFile())
    {
        type = "Fichier";
    }

    /*GERONS LE CAS DES RACCOURCIS*/
    if(info.isSymLink() || info.isShortcut())
    {
        chemin = historique.at(positionDansHistorique) + "/" + info.fileName();
        type = "Raccourci";
    }
    /* FIN CAS DES RACCOURCIS*/

    std::cout<<type.toStdString()<<" sur le point d'être supprimé: "<<chemin.toStdString()<<std::endl;

    QFile fichier(chemin);

    int reponse = QMessageBox::warning(this, "Supprimer...",
    "Voulez-vous vraiment supprimer ce " + type + "?\n\"" + info.fileName() + "\"\n\nCETTE ACTION EST IRREVERSIBLE",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        if(fichier.remove())  //Cette fonction supprime les Fichiers (y compris les raccourcis), mais pas les dossiers
        {
            std::cout<<"SUPPRESSION EFFECTUEE"<<std::endl;
        }
        else if(modele->remove(tableView->getSelectedIndexes()[0])) //Ceci peut supprimer les dossiers
        {
            std::cout<<"SUPPRESSION EFFECTUEE"<<std::endl;
        }
        else
        {
            QMessageBox::warning(this,"Echec de la suppression",
                                 "La suppression du " + type + " \"" + info.fileName() + "\" a échoué!");
        }
    }

}

void Fenetre::actualiser()
{
    modele->setRootPath("");
    this->ouvrirDossier(historique.at(positionDansHistorique), true);
}

void Fenetre::copier()
{
    if(this->situeAOrdinateur) return;

    //On récupère les informations sur le fichier sélectionné
    Fenetre::sourceCopie = modele->fileInfo(tableView->getSelectedIndexes()[0]);

    Fenetre::cheminSource = Fenetre::sourceCopie.canonicalFilePath(); //Le chemin de l'élément à copier

    this->couperSource = false;

    /*GERONS LE CAS DES RACCOURCIS*/
    if(Fenetre::sourceCopie.isSymLink() || Fenetre::sourceCopie.isShortcut())
    {
       Fenetre::cheminSource = historique.at(positionDansHistorique) + "/" + Fenetre::sourceCopie.fileName();
    }
    /* FIN CAS DES RACCOURCIS*/

    std::cout<<"Fichier à copier: "<<Fenetre::cheminSource.toStdString()<<std::endl;

    //if(!actionColler->isEnabled()) actionColler->setEnabled(true); //on active le bouton "coller";

        for(int i=0; i<Fenetre::listeFenetres.size(); i++)
        {
         if(!Fenetre::listeFenetres.at(i)->situeAOrdinateur)
            Fenetre::listeFenetres.at(i)->actionColler->setEnabled(true);//on active le bouton "coller" dans toutes les fenêtres;
        }

}

void Fenetre::couper()
{
    if(this->situeAOrdinateur) return;

    this->copier();
    Fenetre::couperSource = true;
}

void Fenetre::coller()
{
    if(Fenetre::cheminSource.isEmpty())//Si rien n'est copié
    {
        actionColler->setEnabled(false); //On désactive le bouton "Coller"
        return; //on sort de la fonction.
    }
    if(this->situeAOrdinateur) return;

    QString cheminDest = historique.at(positionDansHistorique) + "/" + Fenetre::sourceCopie.fileName();
    QString type = getTypeElement(Fenetre::sourceCopie); //Le type de l'élément qu'on veut coller

   //On crée un thread pour la copie et on le lance
   ThreadCopie * threadCopie = new ThreadCopie(Fenetre::cheminSource,cheminDest,type);
   QObject::connect(threadCopie, SIGNAL(copieFinie(bool, QString, QString)), this, SLOT(apresColler(bool, QString, QString)));
   threadCopie->start();
}

void Fenetre::apresColler(bool resultat, QString type, QString cheminDest)
{std::cout<<"###ENTREE DANS LA FONCTION apresColler()###"<<std::endl;
    if(resultat) //Si la copie a marché
    {
        //On sélectionne l'élément créé, si et seulement si on est à l'endroit où on l'a collé
        if(historique.at(positionDansHistorique)==QFileInfo(cheminDest).absolutePath())
        {
            this->actualiser();
            tableView->setnomFichierASelectionner(QFileInfo(cheminDest).fileName());
            QTimer::singleShot( 200, tableView, SLOT(selectRowByName()) );
        }

    }

    if(resultat && this->couperSource) //Si la copie a marché et on veut couper alors...
    {
        if(type=="dossier")
        {
            //rmdir(chemSource); //On supprime le dossier copié
            QDir(Fenetre::cheminSource).removeRecursively();
        }
        else
        {
            //remove(chemSource); //On supprime le fichier copié
            QFile(Fenetre::cheminSource).remove();
        }

        Fenetre::cheminSource = ""; //On vide le texte représentant l'élément copié
        //actionColler->setEnabled(false); //On désactive le bouton "Coller"

        for(int i=0; i<Fenetre::listeFenetres.size(); i++)
        {
            Fenetre::listeFenetres.at(i)->actionColler->setEnabled(false);//on désactive le bouton "coller" dans toutes les fenêtres;
        }

    }
}

void Fenetre::supprimerFichierOuDossier(QFileInfo info)
{
    if(getTypeElement(info)=="dossier")
    {
        //rmdir(chemSource); //On supprime le dossier copié
        QDir(info.canonicalFilePath()).removeRecursively();
    }
    else
    {
        //remove(chemSource); //On supprime le fichier copié
        QFile(info.canonicalFilePath()).remove();
    }
}

QString Fenetre::getTypeElement(QFileInfo info) //Renvoie le type (fichier, dossier, raccourci) d'un élément
{
    QString type="";
    if(info.isDir()){type = "Dossier";}

    else if(info.isFile())
    {
        type = "Fichier";
    }

    if(info.isSymLink() || info.isShortcut())
    {
        type = "Raccourci";
    }

    return type.toLower();
}

void Fenetre::afficherAPropos()
{
    QMessageBox::information(this,"A propos...",
                          "Gestionnaire de fichiers.\nRéalisé en C++.\n\n\tPar DJOUMESSI Aurel,\n\tDécembre 2021");
}

void Fenetre::allerA()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QString cheminAcces = action->data().toString(); //la donnée qui est associée au QAction

    if(!cheminAcces.isEmpty()) this->ouvrirDossier(cheminAcces);

}

QMap<QString, QString> Fenetre::getCheminsSpeciaux() //Calcule les chemins spéciaux (bureau, documents, etc.) et les ajoute dans la QMap cheminsSpeciaux
{
    QList<QString> liste;
    QString cheminAcces = "";
    TCHAR path[_MAX_PATH] = _T(""); // must be _MAX_PATH in size//Ce qui va contenir le chemin du dossier

    liste<<"ordinateur"<<"bureau"<<"documents"<<"téléchargements"<<"vidéos"<<"images"<<"musique";
    int csidl;
    QMap<QString, QString> map;

    for(QString texte : liste)
    {   csidl = -1;

        QString cheminIcone;

        if(texte=="ordinateur")
        {
            cheminAcces="Ordinateur";
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/ordinateur.png");
        }
        else if(texte=="bureau")
        {
            csidl = CSIDL_DESKTOP;
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/bureau.png");
        }
        else if(texte=="documents")
        {
            csidl = CSIDL_MYDOCUMENTS;
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/documents.png");
        }
        else if(texte=="téléchargements")
        {
            csidl = CSIDL_PROFILE; //Ceci correspond au dossier Users/username
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/download.png");
        }
        else if(texte=="images")
        {
            csidl = CSIDL_MYPICTURES;
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/images.png");
        }
        else if(texte=="musique")
        {
            csidl = CSIDL_MYMUSIC;
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/musique.png");
        }
        else if(texte=="vidéos")
        {
            csidl = CSIDL_MYVIDEO;
            cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/vidéos.png");
        }

        //HRESULT hr =
             if(csidl!=-1) {SHGetFolderPath( NULL, // no parent window
                                      csidl,
                                      0,    // no flags
                                      NULL, // no tokens
                                      path );
        cheminAcces = QString::fromWCharArray(path);
        if(texte=="téléchargements") cheminAcces += "/Downloads";

        cheminAcces = QFileInfo(cheminAcces).absoluteFilePath(); //Pour ne plus avoir d'antislash dans le chemin
             }

       map.insert(texte, cheminAcces);
       iconesFenetres.insert(cheminAcces, cheminIcone);
    }

    QString cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/dossier.png");
    iconesFenetres.insert("dossier", cheminIcone); //L'icone de la fenêtre pour les dossiers classiques

    cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/root.png");
    iconesFenetres.insert("disque", cheminIcone); //L'icone de la fenêtre pour les disques (C:, etc.)

    cheminIcone = QString::fromUtf8(":/D:/images/icones/Nouveau dossier/iconeapp.png");
    iconesFenetres.insert("app", cheminIcone); //L'icone de l'application
    std::cout<<"Nombre d'icones: "<<iconesFenetres.size()<<std::endl;

    return map;
}


void Fenetre::showHiddenElements(int state)
{
    if(state == Qt::Checked) //case cochée, on affiche les éléments cachés <<QDir::Hidden>>
        modele->setFilter(QDir::AllDirs | QDir::Hidden | QDir::Files | QDir::NoDotAndDotDot);
    else
        modele->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    QTimer::singleShot( 200, this, SLOT(actualiserBarreDEtat()) );
    QTimer::singleShot( 200, this, SLOT(actualiserStatutTailleFichier()) );
}

void Fenetre::ouvrirRecherche()
{
    FenRecherche *fen = new FenRecherche(historique.at(positionDansHistorique));
    fen->show();
}

TableView* Fenetre::getTableView()
{
    return tableView;
}
