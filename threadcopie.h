#ifndef THREADCOPIE_H
#define THREADCOPIE_H
#include <QThread>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>
#include <QLabel>
#include <QDebug>
#include <iostream>
#include <windows.h>
#include <winbase.h>

class ThreadCopie : public QThread
{
    Q_OBJECT
public:
    ThreadCopie();
    ThreadCopie(QString chemSource, QString chemDest, QString type);
    bool copierColler();
    static const int CHUNK_SIZE = 1024 * 1024 * 1; //taille maximale des portions de données à lire, 1Mo

private:
    QString cheminSource, cheminDest, typeElement;
    QProgressDialog *progress;

protected:
    void run() override;

public slots:
    void afficherProgression(qint64 written, qint64 total);

signals:
    void copieFinie(bool resultat, QString type, QString cheminDest); //émis lorsque l'opération de copie est terminée
    void copyProgress(qint64 written, qint64 total); //Signal sur l'évolution de la copie

};

#endif // THREADCOPIE_H
