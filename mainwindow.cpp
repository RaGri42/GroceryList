#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_fooddialog.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringListModel>
#include <QTableWidget>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include <QPdfWriter>
#include <QRect>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // Anwendung nicht bei Dialogende schliessen
   // dialog->setAttribute(Qt::WA_QuitOnClose);

    connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(pushItem()));
    connect(ui->pushButton_2, SIGNAL(clicked()),this, SLOT(deleteItem()));
    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(pushItem()));
    connect(ui->tableWidget_2, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(deleteItem()));
    connect(ui->pushButton_4, SIGNAL(clicked()),this, SLOT(printList()));
    connect(ui->pushButton_5, SIGNAL(clicked()),this, SLOT(clearList()));
    connect(ui->pushButton_6, SIGNAL(clicked()),this, SLOT(initialize()));

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(showDialog()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(showEditDialog()));

    // Neues Lebensmittel Dialog initialisieren
    newform = new QDialog;
    newform->setModal(true);
    Form.setupUi(newform);
    connect (Form.pushButton, SIGNAL(clicked()), this, SLOT(writeFood()));
    connect (Form.pushButton_2, SIGNAL(clicked()), newform, SLOT(close()));

    // Editieren Lebensmittel Dialog initialisieren
    editform = new QDialog;
    editform->setModal(true);
    Formedit.setupUi(editform);
    connect (Formedit.pushButton, SIGNAL(clicked()), this, SLOT(writeFood()));
    connect (Formedit.pushButton_2, SIGNAL(clicked()), editform, SLOT(close()));




    _anzElements = -1;
    _rowsTable2 = 1;
   // model = new QStringListModel(this);

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialize(){

        _listVec.clear();
        int i = 0;
        int rows = 0;
        // csv - Datei welche die Lebensmittel enthaelt
        QString foodFile = "Einkaufszettel.csv";
        QString path = "/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/";
        QString fileName = path + foodFile;

        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly)) {
         std::cout << "ja ist offen";
        }
        else {
            std::cout << "Ne ist zu!" << std::endl;
        }

        QTextStream in(&file);
        QString line;




        // Datei einlesen und zeilenweise in Vector schreiben
        // Anzahl der Reihen bestimmen

        while(!in.atEnd()) {
            line = in.readLine();
            _listVec.push_back(line);
            rows++;
        }



        ui->tableWidget->setRowCount(rows);

        for (auto& line1 : _listVec) {
        QTableWidgetItem *item = new QTableWidgetItem(line1);
        item->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i,0, item);
        i++;
        }
        ui->tableWidget->sortItems(0);


       // QHeaderView* header = ui->tableWidget->horizontalHeader();
       // header->setSectionResizeMode(QHeaderView::Stretch);

        file.close();
}

void MainWindow::showDialog() {

  newform->show();
}

void MainWindow::showEditDialog() {

  editform->show();
}

void MainWindow::clearList() {

    ui->tableWidget_2->clearContents();
    _anzElements = 0;

}


void MainWindow::printList() {
        QString itemName;
        int zeile = 0;
        int zeilePdf = 0;
        const QString filename("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkauf.pdf");


        QPdfWriter pdfwriter(filename);
        pdfwriter.setPageSize(QPageSize(QPageSize::A4));
        QPainter painter(&pdfwriter);

        painter.drawText(0,zeilePdf, "Einkaufsliste");
        zeilePdf += 400;



        // Zeilenweise in pdf schreiben

        while(ui->tableWidget_2->item(zeile, 0)) {
        itemName = ui->tableWidget_2->item(zeile,0)->text();
        painter.drawText(0,zeilePdf, itemName);

        zeilePdf += 200;
        zeile++;

        }


}

// __________________________________________________________________________________________
// Lebensmittel in csv einfuegen

void MainWindow::writeFood() {
    QString str;
    if(Form.lineEdit->text().isEmpty()) {
      QMessageBox::information(0, tr("Fehler!"), "Bitte Felder vollständig ausfüllen.");
    }
    // QString in Standardstring umwandeln
    str = Form.lineEdit->text();

    // Pruefen ob Eintrag schon vorhanden ist
    auto ret = std::find(_listVec.begin(), _listVec.end(), str);

    // Wenn Eintrag schon vorhanden wird er nicht in Liste aufgenommen
        if (ret != _listVec.end()) {
            QMessageBox::information(0, tr("Fehler!"), "Eintrag schon vorhanden.");
        }
    // Wenn Eintrag nicht vorhanden ist wird er in die csv Datei eingetragen
        else {
             QMessageBox::information(0, tr("Aktion erfolgreich!!"), "Neues Lebensmittel wurde eingetragen.");
            _listVec.push_back(str);
            saveList();
            initialize();

    }
}

// ___________________________________________________________________________________________
// neue csv Liste speichern

void MainWindow::saveList() {
     QFile file("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkaufszettel.csv");

     if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        for(auto& linie : _listVec) {
        out << linie << endl ;
        }


}
    //file.write("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkaufszettel.csv");
    file.close();
}

void MainWindow::pushItem(){


    if((ui->tableWidget->currentItem())) {
    QTableWidgetItem *item2 = ui->tableWidget->currentItem()->clone();
    item2->setFlags(Qt::ItemIsEnabled);
    ui->tableWidget_2->setRowCount(_rowsTable2);
    ui->tableWidget_2->setItem(_anzElements, 0, item2);
    ui->tableWidget_2->sortItems(0);
    _anzElements++;
    _rowsTable2++;



}
}



void MainWindow::deleteItem(){

    int reihe =  ui->tableWidget_2->currentRow();

    // Loeschen wenn Item vorhanden
    if((ui->tableWidget_2->currentItem()) && (_anzElements >= 0)) {
        //std::cout << "rowCount"  << reihe << std::endl;
        ui->tableWidget_2->removeRow(reihe);
        ui->tableWidget_2->setRowCount(10);
        _anzElements--;
}

    else {
        std::cout << "Erst waehlen" << std::endl;
    }

}

void MainWindow::editItem(){



    // Editieren wenn Item vorhanden
    if(ui->tableWidget_2->currentItem()) {


    ui->tableWidget_2->setRowCount(10);
    _anzElements--;
}

}




