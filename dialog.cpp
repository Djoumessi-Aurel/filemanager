#include "dialog.h"

/* Classe représentant la boîte de dialogue <<Propriétés>> */

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    this->resize(563, 305);
    this->setWindowIcon(QIcon(":/D:/images/icones/icones filemanager/proprietes.png"));
            this->setMinimumSize(QSize(0, 0));
            QFont font;
            font.setPointSize(10);
            this->setFont(font);
            buttonBox = new QDialogButtonBox(this);
            buttonBox->setGeometry(QRect(30, 280, 341, 32));
            QFont font1;
            font1.setFamily(QString::fromUtf8("MV Boli"));
            font1.setPointSize(11);
            buttonBox->setFont(font1);
            buttonBox->setOrientation(Qt::Horizontal);
            buttonBox->setStandardButtons(QDialogButtonBox::Ok);
            formLayoutWidget = new QWidget(this);
            formLayoutWidget->setGeometry(QRect(10, 10, 541, 231));
            formLayout = new QFormLayout(formLayoutWidget);
            formLayout->setSizeConstraint(QLayout::SetFixedSize);
            formLayout->setContentsMargins(0, 0, 0, 0);

            this->setWindowTitle("Propriétés");

            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

            QMetaObject::connectSlotsByName(this);

    formLayout->addRow("Nom: ",nom);
    formLayout->addRow("Type: ",type);
    formLayout->addRow("Emplacement: ",emplacement);
    formLayout->addRow("Chemin complet: ",cheminComplet);
    formLayout->addRow("Taille: ",taille);
    formLayout->addRow("Contenu: ",contenu);
    formLayout->addRow("Créé le: ",created);
    formLayout->addRow("Dernière modification le: ",lastModified);
    formLayout->addRow("Dernière lecture le: ",lastRead);

    this->setFixedSize(600,350);

    /* MISE EN FORME */
    QList<QLabel*> listeLabel = this->findChildren<QLabel*>();
    QList<QLineEdit*> listeLine = this->findChildren<QLineEdit*>();

    for(int i=0; i<listeLabel.size();i++)
    {
        listeLabel.at(i)->setFont(QFont("Verdana",11,QFont::Normal));
    }
    for(int i=0; i<listeLine.size();i++)
    {
        listeLine.at(i)->setFont(QFont("Verdana",11,QFont::Normal));
        listeLine.at(i)->setMinimumWidth(350);
        listeLine.at(i)->home(false); //remettre le curseur au début de la zone de texte
        listeLine.at(i)->setReadOnly(true);
    }
    /* FIN MISE EN FORME*/

}

Dialog::~Dialog()
{
}

void Dialog::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->destroy(); //On libère la mémoire occupée par la fenêtre
}

void Dialog::setFileInfo(const QFileInfo &fileInfo)
{
    this->setWindowTitle("Propriétés de " + fileInfo.fileName());

    nom->setText(fileInfo.fileName());
    emplacement->setText(fileInfo.canonicalPath());
    cheminComplet->setText(fileInfo.canonicalFilePath());
    qint64 maTaille = fileInfo.size();
    taille->setText(Fenetre::tailleFichierReduite(maTaille) + " (" + QString::number(maTaille) + " octets)");

    created->setText(fileInfo.birthTime().toString("dddd 'le' d MMMM yyyy 'à' hh:mm"));
    lastModified->setText(fileInfo.lastModified().toString("dddd 'le' d MMMM yyyy 'à' hh:mm"));
    lastRead->setText(fileInfo.lastRead().toString("dddd 'le' d MMMM yyyy 'à' hh:mm"));

    QString ch = "";

    if(fileInfo.isShortcut() || fileInfo.isSymLink())
    {
        ch = "Raccourci";
    }
    else if (fileInfo.isRoot())
    {
        ch = "Disque"; this->setWindowTitle("Propriétés de " + fileInfo.canonicalFilePath());
    }
    else if (fileInfo.isDir())
    {
        ch = "Dossier";
    }
    else if (fileInfo.isFile())
    {
        ch = "Fichier " + fileInfo.suffix().toUpper();
    }

    type->setText(ch); //Le type de l'élément sélectionné (fichier, dossier,etc.)

            /*Gestion des cas particuliers (disques, dossiers, raccourcis)*/
    if (fileInfo.isDir() || fileInfo.isShortcut() || fileInfo.isSymLink())
        {
            formLayout->removeRow(emplacement);
            formLayout->removeRow(taille);
        }
    if (fileInfo.isRoot())
        {
            formLayout->removeRow(nom);
        }
    if(fileInfo.isShortcut() || fileInfo.isSymLink())
    {
        ( (QLabel*)(formLayout->labelForField(cheminComplet)) )->setText("Cible: ");
    }

    if(!fileInfo.isDir() || fileInfo.isShortcut() || fileInfo.isSymLink()) //Si ce n'est pas un dossier
    {
        formLayout->removeRow(contenu);
    }
    /* Calcul du contenu (nbre de dossiers et de fichiers) dans le cas d'un dossier */
    else
    {
        int nbDossier = 0, nbFichier = 0;
        QFileInfoList liste = QDir(fileInfo.absoluteFilePath()).entryInfoList
          (QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); //Contenu du dossier. Ces paramètres sont très importants

        for(QFileInfo info : liste)
        {
            if(info.isShortcut() || info.isSymLink()) nbFichier++;
            else if(info.isDir()) {nbDossier++;
                /*std::cout<<"Dossier compté: "<<info.absoluteFilePath().toStdString()<<std::endl;*/}
            else if(info.isFile()) nbFichier++;
        }

        contenu->setText(QString::number(nbFichier) + " fichiers, " + QString::number(nbDossier) + " dossiers.");
    }


}
