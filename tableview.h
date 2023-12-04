#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT

public:
    TableView(QWidget *parent = nullptr);
    void setnomFichierASelectionner(QString nom);
    virtual QModelIndexList getSelectedIndexes() const;

private:
   QString nomFichierASelectionner;

protected:
  void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

  virtual void contextMenuEvent(QContextMenuEvent *event) override;

public slots:
    void selectRowByName(); //Sélectionne l'élément du tableView qui a pour nom nomFichierASelectionner

signals:
void changementDeLigne();
void clicDroit(QContextMenuEvent *event);

};

#endif // TABLEVIEW_H
