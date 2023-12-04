#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDebug>
#include <QStyleFactory>
#include <QSystemTrayIcon>

#include "fenetre.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTranslator;
       if (qtTranslator.load(QLocale::system(),
                   "qt", "_",
                   QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
       {
           qDebug() << "qtTranslator ok";
           a.installTranslator(&qtTranslator);
       }

    QTranslator qtBaseTranslator;
        if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            qDebug() << "qtBaseTranslator ok";
            a.installTranslator(&qtBaseTranslator);
        }

        //qDebug() << QStyleFactory::keys();
        a.setStyle(QStyleFactory::create("Fusion"));

    Fenetre w;
    w.show();

    QSystemTrayIcon tray (QIcon(Fenetre::iconesFenetres.value("app")));
    tray.setToolTip("File Manager. by Djoumessi Aurel");
    tray.setContextMenu(w.menuFichier);
    tray.show();
    tray.showMessage("File Manager. by Djoumessi Aurel",
                     "Vous venez d'ouvir mon gestionnaire de fichiers.\nBonne utilisation!");

    return a.exec();
}
