#include "monthread.h"
#include <iostream>

MonThread::MonThread() : QThread()
{

}

MonThread::MonThread(QString dossier, QString pattern) : QThread()
{
    this->dossier = dossier; this->pattern = pattern;
}

void MonThread::run() //recherche
{
    resultatRecherche.clear();
    tempsDebut = QDateTime::currentDateTime().toMSecsSinceEpoch();
    rechercherDansDossier(dossier, pattern);
    std::cout<<"\n^^^^^^^^FIN DU RUN ^^^^^^"<<std::endl;
    tempsActuel = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void MonThread::rechercherDansDossier(QString dossier, QString pattern) //Recherche pattern dans dossier
{
    QStringList filtres;
    filtres<<"*" + pattern + "*";

    QFileInfoList liste = QDir(dossier).entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot); //liste des sous-dossiers directs

    //Les dossiers correspondant à la recherche
    QFileInfoList listeMatch = QDir(dossier).entryInfoList(filtres, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    resultatRecherche.append(listeMatch); //on ajoute les éléments qui correspondent à la recherche

    tempsActuel = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(tempsActuel - tempsDebut > 20000) //Si la recherche a déjà fait plus de 20 secondes, on arrête.
    {
        std::cout<<"LIGNE 62, TEMPS ECOULE"<<std::endl;
        return;
    }

    /* On fait pareil pour tous les sous-dossiers directs (récursivité) */
    //std::cout<<"NOMBRE DE SOUS-DOSSIERS: "<<liste.size()<<std::endl;
    for(QFileInfo sousDossier : liste)
    {
        rechercherDansDossier(sousDossier.absoluteFilePath(), pattern);
    }

}

void MonThread::setDossier(QString dossier)
{
    this->dossier = dossier;
}

void MonThread::setPattern(QString pattern)
{
    this->pattern = pattern;
}
