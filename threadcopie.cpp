#include "threadcopie.h"

ThreadCopie::ThreadCopie(): QThread()
{

}

ThreadCopie::ThreadCopie(QString chemSource, QString chemDest, QString type): QThread(), cheminSource(chemSource), cheminDest(chemDest), typeElement(type)
{
    QObject::connect(this, SIGNAL(finished()), this, SLOT(deleteLater()) );
    QObject::connect(this, SIGNAL(copyProgress(qint64, qint64)), this, SLOT(afficherProgression(qint64, qint64)));

    //progress = new QProgressDialog("Copie en cours", "----", 0, 100);
    progress = new QProgressDialog;
    progress->setLabel(new QLabel("Copie en cours"));
    progress->setCancelButton(nullptr); progress->setWindowTitle("Copie en cours");
    progress->show();
}

void ThreadCopie::run()
{
std::cout<<"***Entrée dans le thread de Copie***"<<std::endl;
    bool resultat = false;

    const char* chemSource = cheminSource.toStdString().c_str();
    const char* chemDest = cheminDest.toStdString().c_str();

    std::cout<<"chemSource: "<<cheminSource.toStdString()<<"\nchemDest: "<<cheminDest.toStdString()<<std::endl;

    if(QFileInfo(cheminDest).exists())
    {
        if(cheminDest == cheminSource) //Si source et destination c'est la même chose, on arrête tout.
        {
            return;
        }

        qDebug()<<"Un élément de même nom existe déjà à cet emplacement!!";
        progress->close();
        return;

        int reponse = QMessageBox::warning(nullptr, "Copie...",
                    "Un "+ typeElement +" nommé " + QFileInfo(cheminSource).fileName() + " existe déjà à cet emplacement" +
                    "\n\nVoulez-vous le remplacer?",  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if(reponse == QMessageBox::Yes)//là on remplace le fichier
        {
            //resultat = CopyFileA(chemSource, chemDest, false); //false représente failIfExists

            QFile(cheminDest).remove(); //Supression du fichier existant

            //resultat = QFile::copy(cheminSource, cheminDest);

            resultat = copierColler();
            if(resultat) std::cout<<"Fichier collé avec remplacement: "<<cheminSource.toStdString()<<std::endl;
            else //Si la copie a échoué, on rééssaye d'une autre façon
            {
                if((resultat = CopyFileA(chemSource, chemDest, false))){}
                else std::cout << "Echec de la copie: "<< GetLastError()<<std::endl; //On affiche l'erreur si ça n'a pas marché
            }
        }

        else {//progress->close();
        }
    }
    else {
        //resultat = CopyFileA(chemSource, chemDest, true);
        //resultat = QFile::copy(cheminSource, cheminDest);

        resultat = copierColler();
        if(resultat) std::cout<<"Fichier effectivement collé: "<<cheminSource.toStdString()<<std::endl;
        else //Si la copie a échoué, on rééssaye d'une autre façon
        {
            if((resultat = CopyFileA(chemSource, chemDest, false))){}
            else std::cout << "Echec de la copie: "<< GetLastError()<<std::endl; //On affiche l'erreur si ça n'a pas marché
        }
    }

    emit copieFinie(resultat, typeElement, cheminDest);

}

bool ThreadCopie::copierColler()
{
    QFile srcFile(cheminSource), dstFile(cheminDest);

    if (!srcFile.open(QFileDevice::ReadOnly)) { //Ouverture du fichier source en mode lecture
               qDebug()<< QStringLiteral("failed to open %1 (error:%1)").arg(srcFile.errorString());;
               return false;
           }

           if (!dstFile.open(QFileDevice::WriteOnly)) { //ouverture du fichier destination en mode écriture
               qDebug()<< QStringLiteral("failed to open %1 (error:%1)").arg(dstFile.errorString());;
               return false;
           }


qint64 total = srcFile.size(), written = 0;
    /* copy the content in portion of chunk size */
            qint64 fSize = total;
            while (fSize) {
                const auto data = srcFile.read(CHUNK_SIZE); //Lecture des données
                const auto _written = dstFile.write(data); //écriture des données
                if (data.size() == _written) {
                    written += _written;
                    fSize -= data.size();
                   //progress->setValue(100*((double)written/total));
                    emit copyProgress(written, total);
                } else {
                    qWarning() << QStringLiteral("failed to write to %1 (error:%2)").arg(dstFile.fileName()).arg(dstFile.errorString());
                    fSize = 0;
                    break; // skip this operation
                }
            }
            return true;
}


void ThreadCopie::afficherProgression(qint64 written, qint64 total)
{
 double valeur = 100*((double)written/total);
 progress->setValue(valeur);

 QString chaine = "Copie de: " + cheminSource + "\n Vers: " + QFileInfo(cheminDest).absolutePath() + "\n\n";
 chaine += QString::number((total - written)/CHUNK_SIZE) + " Mo restants sur " + QString::number(total/CHUNK_SIZE);

 progress->setLabelText(chaine);
 //qDebug()<<"Progression ("<<valeur<<"): "<<written<<" sur "<<total;
}
