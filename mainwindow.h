#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_form.h"
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
    void initialize();


public slots:
    void pushItem();
    void deleteItem();
    void showDialog();
    void writeFood();
    void printList();
    void clearList();
    void saveList();



private:
    Ui::MainWindow *ui;
    QDialog *newform;
    Ui::Form Form;


public:
     QTableWidget *tableWidget;
     QStringListModel *model;
     //QTableWidget *tableWidget;
     QStringList list;
     QStringList listi;

     // öffentliche Membervariablen
     QVector <QString> _listVec;
     int _anzElements;
};
#endif // MAINWINDOW_H
