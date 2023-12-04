#ifndef MONTHREAD_H
#define MONTHREAD_H
#include <QThread>
#include <QFileInfoList>
#include <QDir>
#include <QDateTime>

class MonThread : public QThread
{
    Q_OBJECT
public:
    MonThread();
    MonThread(QString dossier, QString pattern);
    void setDossier(QString dossier);
    void setPattern(QString pattern);

    QFileInfoList resultatRecherche;
    qint64 tempsDebut, tempsActuel;

    void rechercherDansDossier(QString dossier, QString pattern); //Recherche pattern dans dossier

private:
    QString dossier, pattern;

protected:
    void run() override;
};

#endif // MONTHREAD_H
