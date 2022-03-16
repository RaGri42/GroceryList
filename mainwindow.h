#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_form.h"
#include "ui_formedit.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QTableWidget>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    bool checkItem(QString itemStr, QVector<QString>& vec);


    // Memberfunktionen



public slots:
    void pushItem();

    void deleteItem();
    void deleteItemList();
    void editItem();
    void showDialog();
    void showEditDialog();
    void sortList();
    void writeFood();
    void printList();
    void printList2();
    void clearList();
    void saveList();
    void saveActiveList();
    void loadActiveList();
    void initialize();
    void initializeEinkauf();
    void initializeMap();



private:
    Ui::MainWindow *ui;
    QDialog *newform;
    QDialog *editform;
    Ui::Form Form;
    Ui::Formedit Formedit;




public:
     QTableWidget *tableWidget;
     QStringListModel *model;
     //QTableWidget *tableWidget;
     QStringList list;
     QStringList listi;

     // öffentliche Membervariablen
     // QMap mit Item als Schlüssel und Qvector als Wert, "Shop", "Preis"
     QMap <QString, QStringList> _itemMap;

     // QMap mit Item als Schlüssel und Qvector als Wert, "Shop", "Preis"
     QMap <QString, QStringList> _itemMap2;

     // Listenvektor für eingelesene csv
     QVector <QString> _listVec;
     // Listenvektor für Einkaufsliste
     QVector <QString> _listVec2;

     int _columns;
     int _anzElements;
     int _rowsTable2;
};
#endif // MAINWINDOW_H
