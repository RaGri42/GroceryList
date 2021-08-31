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

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(showDialog()));

    // Dialog initialisieren
    newform = new QDialog;
    newform->setModal(true);
    Form.setupUi(newform);
    connect (Form.pushButton, SIGNAL(clicked()), this, SLOT(writeFood()));
    connect (Form.pushButton_2, SIGNAL(clicked()), newform, SLOT(close()));




    _anzElements = -1;
   // model = new QStringListModel(this);

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialize(){

    listi << "Lebensmittel" << "willst";
    //tableWidget = new QTableWidget(4, 2, this);
    ui->tableWidget->setHorizontalHeaderLabels(listi);
        int i = 0;
        QFile file("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/test.csv");
        if(file.open(QIODevice::ReadOnly)) {
         std::cout << "ja ist offen";
        }
        else {
            std::cout << "Ne ist zu!" << std::endl;
        }

        QTextStream in(&file);

        QString line;


        while(!in.atEnd()) {
            line = in.readLine();
            _listVec.push_back(line);
          // QString strline = qPrintable(line);


    QTableWidgetItem *item = new QTableWidgetItem(line);
    item->setFlags(Qt::ItemIsEnabled);
    ui->tableWidget->setItem(i,0, item);
          //  tableWidget->setItem(i, 0, new QTableWidgetItem(tr("0")));
            i++;

}
        file.close();
}

void MainWindow::showDialog() {

  newform->show();
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

void MainWindow::writeFood() {
    QString str;
    if(Form.lineEdit->text().isEmpty()) {
      QMessageBox::information(0, tr("Fehler!"), "Bitte Felder vollständig ausfüllen.");
    }
    // QString in Standardstring umwandeln
    str = Form.lineEdit->text();
    _listVec.push_back(str);
    saveList();

    std::cout <<  str.toStdString() << std::endl;


}


void MainWindow::saveList() {
     QFile file("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/test.csv");






    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);

        for(auto& linie : _listVec) {
             out << linie << endl ;

    }


}
    //file.write("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/test.csv");
    file.close();
}

void MainWindow::pushItem(){


    if((ui->tableWidget->currentItem())) {
    QTableWidgetItem *item2 = ui->tableWidget->currentItem()->clone();
    item2->setFlags(Qt::ItemIsEnabled);
    ui->tableWidget_2->setItem(_anzElements, 0, item2);
    _anzElements++;

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




