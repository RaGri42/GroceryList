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


    // Memberfunktionen



public slots:
    void pushItem();
    void deleteItem();
    void editItem();
    void showDialog();
    void showEditDialog();
    void writeFood();
    void printList();
    void clearList();
    void saveList();
    void initialize();



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
     QVector <QString> _listVec;
     int _anzElements;
     int _rowsTable2;
};
#endif // MAINWINDOW_H
