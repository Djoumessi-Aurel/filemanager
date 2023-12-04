#include "fenrecherche.h"
#include "tchar.h"

FenRecherche::FenRecherche(QString chemin, QWidget *parent) : QMainWindow(parent)
{
    this->cheminDossier = chemin;
    infoDossier.setFile(cheminDossier);

    QString nom = infoDossier.isRoot()? infoDossier.absoluteFilePath() : infoDossier.fileName();
    QString type = infoDossier.isRoot()? "disque" : "dossier";

    this->resize(800,500);
    this->setWindowIcon(QIcon(":/D:/images/icones/icones filemanager/rechercher.png"));
    labelNom->setText("Recherche dans le " + type + " \"" + nom + "\"");
    this->setWindowTitle(labelNom->text());

    labelChemin->setText(cheminDossier);
    labelDetails->setFont(QFont("Book Antiqua", 12));

    setCentralWidget(centralWidget);
    centralWidget->setLayout(layoutV);
    layoutV->addWidget(labelNom);
    layoutV->addWidget(labelChemin);
    layoutV->addLayout(layoutRech);
    layoutV->addWidget(labelDetails);
    layoutV->addWidget(tableView);

    layoutRech->addWidget(editRech);
    layoutRech->addWidget(btRech);

    tableView->setSortingEnabled(true); //Pour permettre de ranger selon les colonnes
    tableView->setCornerButtonEnabled(false);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //empêcher la modification des cellules du tableau
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //On ne peut sélectionner qu'une ligne à la fois

    tableView->setModel(modele);

    QObject::connect( btRech, SIGNAL(clicked()), this, SLOT(avantRecherche()) );
    QObject::connect( editRech, SIGNAL(returnPressed()), this, SLOT(avantRecherche()) );
    QObject::connect( thread, SIGNAL(finished()), this, SLOT(afficherResultats()) );

    menu->addAction(actionOuvrir);
    menu->addAction(actionEmplacement);
    menu->addAction(actionProprietes);

    QObject::connect( actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrir()) );
    QObject::connect( actionEmplacement, SIGNAL(triggered()), this, SLOT(ouvrirEmplacement()) );
    QObject::connect( actionProprietes, SIGNAL(triggered()), this, SLOT(ouvrirProprietes()) );
    QObject::connect(tableView, SIGNAL(activated(QModelIndex)), this, SLOT(ouvrir()));

    editRech->setFocus();

}

void FenRecherche::closeEvent(QCloseEvent *event)
{
    modele->clear();
    event->accept();
    this->destroy(); //On libère la mémoire occupée par la fenêtre
}

void FenRecherche::setCheminDossier(QString chemin)
{
    this->cheminDossier = chemin;
}

void FenRecherche::avantRecherche()
{
    if(editRech->text().isEmpty()) return;
    labelDetails->setText("PATIENTEZ SVP......");
    modele->clear();

    /* On paramètre le thread qui va faire la recherche et on le lance */
    thread->setDossier(cheminDossier);
    thread->setPattern(editRech->text());
    thread->start();

}

void FenRecherche::afficherResultats()
{
    double duree = ( (double)(thread->tempsActuel - thread->tempsDebut) )/1000;
    labelDetails->setText("Nombre de résultats: " + QString::number(thread->resultatRecherche.size())
                          +"       Temps écoulé: " + QString::number(duree) + " s");
    //for(QFileInfo res : thread->resultatRecherche) std::cout<<"**"<<res.absoluteFilePath().toStdString()<<"**"<<std::endl;

    /* Maintenant, on remplit le taleView */
    QList<QStandardItem *> listeItem;
    QString nom, chemin, type;
    QIcon icone;

    for(QFileInfo fileInfo : thread->resultatRecherche)
    {
        nom = fileInfo.fileName();
        type = fileInfo.isDir() ? "Dossier" : "Fichier "+fileInfo.suffix();
        chemin = fileInfo.absoluteFilePath();

        icone = fileInfo.isDir() ? QIcon(":/D:/images/icones/Nouveau dossier/dossier2.png")
                                  : QIcon(":/D:/images/icones/Nouveau dossier/fichier.png");

        listeItem.clear();
        listeItem<<new QStandardItem(icone,nom)<<new QStandardItem(type)<<new QStandardItem(chemin);
        modele->appendRow(listeItem);
    }

    QHeaderView *header = tableView->horizontalHeader();
    header->resizeSection(0,320); header->resizeSection(2,380);
    header->setSortIndicator(1,Qt::AscendingOrder);//Ranger selon la colonne N°2 (Type) par ordre croissant

    modele->setHeaderData(0, Qt::Horizontal, QVariant("Nom"));
    modele->setHeaderData(1, Qt::Horizontal, QVariant("Type"));
    modele->setHeaderData(2, Qt::Horizontal, QVariant("Chemin"));

}

void FenRecherche::ouvrir()
{
    if(tableView->selectionModel()->selectedIndexes().size()>0) //Si un élément est sélectionné
    {
        QFileInfo info = this->selectionInfo();
        QString path = info.absoluteFilePath();
        std::cout<<"OUVRIR: "<<path.toStdString()<<std::endl;
        if(info.isDir())
        {
            Fenetre *fen = new Fenetre(path);
            fen->show();
        }
        else if(info.isFile())
        {
            TCHAR commandeW[_MAX_PATH] = _T("");
            path.toWCharArray(commandeW);

                //On lance le fichier
                ShellExecuteW(NULL, _T("open"), commandeW, NULL, NULL, SW_SHOW);
        }

    }
}

void FenRecherche::ouvrirEmplacement()
{
    if(tableView->selectionModel()->selectedIndexes().size()>0) //Si un élément est sélectionné
    {
        QDir parent = this->selectionInfo().dir(); //Le dossier parent
        QString nom = this->selectionInfo().fileName();
        if(parent.exists())
            {
            std::cout<<"EMPLACEMENT: "<<parent.absolutePath().toStdString()<<std::endl;
            Fenetre *fen = new Fenetre(parent.absolutePath());
                fen->show();
            fen->getTableView()->setnomFichierASelectionner(nom);
            QTimer::singleShot( 200, fen->getTableView(), SLOT(selectRowByName()) );
            }
    }
}

void FenRecherche::ouvrirProprietes()
{
    if(tableView->selectionModel()->selectedIndexes().size()>0) //Si un élément est sélectionné
    {
        Dialog * dialog = new Dialog();
        dialog->setFileInfo(this->selectionInfo());
        dialog->show();
    }
}

QFileInfo FenRecherche::selectionInfo() //Renvoie le QFileInfo correspondant au résultat sélectionné
{
    QItemSelectionModel *selection = tableView->selectionModel();
    QModelIndex indexElementSelectionne = selection->currentIndex();
    int ligne = indexElementSelectionne.row();
    QVariant cheminAbs = modele->data(modele->index(ligne, 2), Qt::DisplayRole); //colonne N°3 (chemin absolu)

    return QFileInfo(cheminAbs.toString());
}

void FenRecherche::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
}
