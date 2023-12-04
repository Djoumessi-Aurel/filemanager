#include "tableview.h"
#include <iostream>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileInfo>
#include <QFileSystemModel>

TableView::TableView(QWidget *parent) : QTableView(parent), nomFichierASelectionner("")
{
    setDragEnabled(true); //Activation du drag and drop sur le tableview
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::DragDrop); //On autorise le drag et le drop
        setDragDropOverwriteMode(false); //Pour êmpecher l'écrasement d'un élément existant
}

/* Lorsqu'on sélectionne un autre élément dans le tableView, affiche sa taille dans la barre de statuts*/
void TableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
}

/* Lorsqu'on sélectionne un autre élément dans le tableView, affiche sa taille dans la barre de statuts*/
void TableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);
    std::cout<<"TableView::selectionChanged"<<std::endl;

    emit changementDeLigne();
}

void TableView::setnomFichierASelectionner(QString nom)
{
    this->nomFichierASelectionner = nom;
}

QModelIndexList TableView::getSelectedIndexes() const
{
    return this->selectedIndexes();
}

void TableView::selectRowByName()
{
    int i=0;
    QFileInfo fileInfo;
    do{
        fileInfo = ((QFileSystemModel)this->model()).fileInfo( this->model()->index(i,0,this->rootIndex()) );
        if(fileInfo.fileName()==nomFichierASelectionner)
        {
            this->setFocus();
            this->selectRow(i);
            std::cout<<"Ligne sélectionnée: "<<i<<std::endl;
        }

       // std::cout<<fileInfo.fileName().toStdString()<<std::endl;
        i++;
    }
    while(fileInfo.exists() && i<500); //On inspecte maxi 500 lignes car ça peut ralentir le programme

}

void TableView::contextMenuEvent(QContextMenuEvent *event)
{
    emit clicDroit(event);
}
