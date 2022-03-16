#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_fooddialog.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPalette>
#include <QString>
#include <QTextStream>
#include <QStringListModel>
#include <QTableWidget>
#include <QMap>
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
// _________________________________________________________________________________________________
    // SIGNAL / SLOTS Mainwindow
    connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(pushItem()));
    connect(ui->pushButton_2, SIGNAL(clicked()),this, SLOT(deleteItemList()));
    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(pushItem()));
    connect(ui->tableWidget_2, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(deleteItemList()));
    connect(ui->pushButton_4, SIGNAL(clicked()),this, SLOT(printList2()));
    connect(ui->pushButton_5, SIGNAL(clicked()),this, SLOT(clearList()));
    connect(ui->pushButton_6, SIGNAL(clicked()),this, SLOT(initialize()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(showDialog()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(showEditDialog()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(deleteItem()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(sortList()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(saveActiveList()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(loadActiveList()));

// _________________________________________________________________________________________________
// Neues Item Dialog initialisieren
// _________________________________________________________________________________________________
    newform = new QDialog;
    newform->setModal(true);
    Form.setupUi(newform);
    connect (Form.pushButton, SIGNAL(clicked()), this, SLOT(writeFood()));
    connect (Form.pushButton_2, SIGNAL(clicked()), newform, SLOT(close()));

    // Editieren Item Dialog initialisieren
    editform = new QDialog;
    editform->setModal(true);
    Formedit.setupUi(editform);
    connect (Formedit.pushButton, SIGNAL(clicked()), this, SLOT(editItem()));
    connect (Formedit.pushButton_2, SIGNAL(clicked()), editform, SLOT(close()));

    _anzElements = -1;
    _rowsTable2 = 1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize(){

    _listVec.clear();
    _listVec2.clear();

    int rows = 0;
    // csv - Datei welche die Items enthaelt
    QString itemFile = "Einkaufszettel2.csv";
    QString path = "/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/";
    QString fileName = path + itemFile;
    QFile file(fileName);

    if(!(file.open(QIODevice::ReadOnly))) {
        std::cerr << "Datei konnte nicht geoeffnet werden!" << std::endl;
    }

    QTextStream in(&file);
    QString line;
    QStringList list;

    // Datei einlesen und zeilenweise in QMap schreiben [key] = { used? (y/n), Shop, Preis, kcal, Protein ... }
    // Anzahl der Reihen bestimmen

    // Nur ausführen wenn Einkaufsliste leer ist


    while(!in.atEnd()) {
        line = in.readLine();

        // Auspalten der Linie
        list = line.split(";");


        // key extrahieren
        QString key = list.at(0).toLocal8Bit().constData();
        // erste Position der QStringlist entfernen
        list.removeFirst();
        list.prepend("n");

        // key und QStringlist in QMap füllen
        _itemMap.insert(key, list);

        rows++;
    }


    _columns = list.size();

    // Spalten und Reihen für Tabelle1 setzen
    ui->tableWidget->setColumnCount(_columns);
    ui->tableWidget->setRowCount(rows);

    // Spalten für Einkaufsliste setzen
    ui->tableWidget_2->setColumnCount(_columns);
    ui->tableWidget_2->setRowCount(100);
    file.close();
    initializeMap();
}


void MainWindow::initializeMap() {

    int j = 0;
    // Map Iterator zum durchlaufen der QMap
    QMapIterator<QString, QStringList> it(_itemMap);
    QStringList values;

        while(it.hasNext()) {
            it.next();
            //std::cout << it.key().toStdString() << "##" ;

            // Key in Spalte 0 eintragen und nicht editierbar machen
            QTableWidgetItem *keyItem = new QTableWidgetItem(it.key());
            ui->tableWidget->setItem(j, 0, keyItem);
            keyItem->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget->resizeColumnToContents(0);

            // Values = QStringList zu angegebenem key
            values = it.value();

            // QStringList durchlaufen und valueItems setzen
            for (int i = 1; i < values.size(); i++){
                QTableWidgetItem *valueItem = new QTableWidgetItem(values.at(i));
                valueItem->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget->setItem(j,i , valueItem);

            }
            // nächste Zeile
            j++;
        }
        // Tabelle an Inhalt anpassen und sortieren
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->sortItems(0);


}



// _________________________________________________________________________________________________
// Bei Änderung eines Items in der csv Einkaufsliste neu initialisieren, damit Wert hier übernommen wird
// _________________________________________________________________________________________________

void MainWindow::initializeEinkauf() {


    ui->tableWidget_2->clearContents();

    QMapIterator<QString, QStringList> it(_itemMap);
    QStringList list;
    int j = 0;
        while(it.hasNext()) {
            it.next();


            if(it.value().at(0) == "y") {
            QTableWidgetItem *keyItem = new QTableWidgetItem(it.key());
            ui->tableWidget_2->setItem(j, 0, keyItem);
            // keyItem->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_2->resizeColumnToContents(0);

            // Values = QStringList zu angegebenem key
            list = it.value();

            // QStringList durchlaufen und valueItems setzen
            for (int i = 1; i < list.size(); i++){

                QTableWidgetItem *valueItem = new QTableWidgetItem(list.at(i));
                valueItem->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget_2->setItem(j,i , valueItem);

            }
            // nächste Zeile
            j++;
        }
        // Tabelle an Inhalt anpassen und sortieren
        ui->tableWidget_2->resizeColumnsToContents();
        ui->tableWidget_2->sortItems(0);




}
}



// ______________________________________________________________________
// Pruefen ob String bereits vorkommt (call by reference) Wenn String vorhanden -> 1
// ______________________________________________________________________

bool MainWindow::checkItem(QString itemStr, QVector<QString>& vec) {

    // Pruefen ob der Eintrag(itemStr) schon im gegebenen Vector vorkommt
    auto ret = std::find(vec.begin(), vec.end(), itemStr);

    // Eintrag schon vorhanden
    if (ret != vec.end()) {
        return 1;
    }

    // Eintrag nicht vorhanden
    else {
        return 0;
     }
}

// _________________________________________________________________________________________________
// Neues Element anlegen Dialog
// _________________________________________________________________________________________________

void MainWindow::showDialog() {
    newform->show();
}

// _________________________________________________________________________________________________
// Element editieren Dialog
// _________________________________________________________________________________________________

void MainWindow::showEditDialog() {

    if(ui->tableWidget->currentItem()) {
    editform->show();

    // Setzt aktuellen Wert in lineEdit
    QString text = ui->tableWidget->currentItem()->text();
    Formedit.lineEdit->setText(text);
    Formedit.lineEdit_2->setText(_itemMap[text].at(1));
    }
}

// _________________________________________________________________________________________________
// Einkaufsliste komplett loeschen
// _________________________________________________________________________________________________

void MainWindow::clearList() {

    ui->tableWidget_2->clearContents();

    QMapIterator<QString, QStringList> it(_itemMap);
    QStringList values;

        // Alle Statuseintraege in QMap auf "n" setzen
        while(it.hasNext()) {
            it.next();
            _itemMap[it.key()][0] = "n";
        }

        ui->tableWidget_2->setRowCount(1);
        //_anzElements = 0;

}


// _________________________________________________________________________________________________
// Liste sortieren
// _________________________________________________________________________________________________

void MainWindow::sortList() {

    ui->tableWidget_2->sortItems(1);

}

// _________________________________________________________________________________________________
// Liste drucken
// _________________________________________________________________________________________________

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

// _________________________________________________________________________________________________
// Liste drucken mit FiledDialog
// _________________________________________________________________________________________________

void MainWindow::printList2() {
        QString itemName;
        QString itemName2;
        QString filename = QFileDialog::getSaveFileName(this, "bitte eine Datei auswaehlen", QDir::homePath(),".*pdf");


        if( !filename.isNull() ) {
            QString info("Folgende Datei wurde ausgewählt:\n");
            info.append(filename);
            QMessageBox::information(this, "Ihre Auswahl", info, QMessageBox::Ok);



        int zeile = 0;
        int zeilePdf = 0;
       // const QString filename("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkauf.pdf");


        QPdfWriter pdfwriter(filename);
        pdfwriter.setPageSize(QPageSize(QPageSize::A4));
        QPainter painter(&pdfwriter);

        painter.drawText(0,zeilePdf, "Einkaufsliste");
        zeilePdf += 400;


        // Zeilenweise in pdf schreiben

        while(ui->tableWidget_2->item(zeile, 0)) {
        itemName = ui->tableWidget_2->item(zeile,0)->text();
        itemName2 = ui->tableWidget_2->item(zeile,1)->text();


        painter.drawText(0,zeilePdf, itemName);
        painter.drawText(2400,zeilePdf, itemName2);

        zeilePdf += 200;
        zeile++;

        }
    }
}

// __________________________________________________________________________________________
// Neues Lebensmittel in csv einfuegen
// _________________________________________________________________________________________________

void MainWindow::writeFood() {
    QString strItem, strShop;
    QStringList newList;

    if(Form.lineEdit->text().isEmpty()) {
      QMessageBox::information(0, tr("Fehler!"), "Bitte Namen vergeben / vollständig ausfüllen.");
     return;
    }
    strItem = Form.lineEdit->text();
    strShop = Form.lineEdit_2->text();

    // Prueft ob Eintrag schon vorhanden
    if(_itemMap.count(strItem) > 0) {
            QMessageBox::information(0, tr("Fehler!"), "Eintrag schon vorhanden.");
    } else {

        QMessageBox::information(0, tr("Aktion erfolgreich!!"), "Neues Lebensmittel wurde eingetragen.");

        newList.append("n");
        newList.append(strShop);
        _itemMap[strItem] = newList;

        //_listVec.push_back(strItem);
        Form.lineEdit->setText("");
        saveList();
        initializeMap();

         }
}


// _________________________________________________________________________________________________
// neue csv Liste speichern
// _________________________________________________________________________________________________

void MainWindow::saveList() {
     QFile file("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkaufszettel2.csv");

     if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);


        QMapIterator<QString, QStringList> it(_itemMap);
        QStringList list;

            // Alle Eintraege aus QMap in csv speichern
            while(it.hasNext()) {
                it.next();

                list = it.value();
                //list.removeFirst();

                out << it.key() ;
                out <<";" ;

                for (int i = 1; i < list.size() ; ++i) {
                    out << list.at(i) << (i < list.size()- 1 ? ";":" ");
                }
                out << endl;
            }

}
    //file.write("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkaufszettel.csv");
    file.close();
}



// _________________________________________________________________________________________________
// aktuelle csv Einkaufsliste zwischenspeichern
// _________________________________________________________________________________________________

void MainWindow::saveActiveList() {
     QFile file("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/EinkaufszettelAktuell.csv");

     if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);


        QMapIterator<QString, QStringList> it(_itemMap2);
        QStringList list;

            // Alle Eintraege aus QMap in csv speichern
            while(it.hasNext()) {
                it.next();

                list = it.value();
                //list.removeFirst();

                out << it.key() ;
                out <<";" ;

                for (int i = 1; i < list.size() ; ++i) {
                    out << list.at(i) << (i < list.size()- 1 ? ";":" ");
                }
                out << endl;
            }

}
    //file.write("/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/Einkaufszettel.csv");
    file.close();
}



//___________________________________________________________________________________________________
// aktuelle Einkaufsliste laden
//___________________________________________________________________________________________________

void MainWindow::loadActiveList() {

    int rows = 0;
    // csv - Datei welche die Items enthaelt
    QString itemFile = "EinkaufszettelAktuell.csv";
    QString path = "/home/botage/QTWorkspace/Einkaufsliste/Einkaufsliste/";
    QString fileName = path + itemFile;
    QFile file(fileName);

    if(!(file.open(QIODevice::ReadOnly))) {
        std::cerr << "Datei konnte nicht geoeffnet werden!" << std::endl;
    }

    QTextStream in(&file);
    QString line;
    QStringList list;

    // Datei einlesen und zeilenweise in QMap schreiben [key] = { used? (y/n), Shop, Preis, kcal, Protein ... }
    // Anzahl der Reihen bestimmen

    // Nur ausführen wenn Einkaufsliste leer ist


    while(!in.atEnd()) {
        line = in.readLine();

        // Auspalten der Linie
        list = line.split(";");


        // key extrahieren
        QString key = list.at(0).toLocal8Bit().constData();
        // erste Position der QStringlist entfernen


        // key und QStringlist in QMap füllen
        _itemMap2.insert(key, list);

        rows++;
    }

    //+++++++++++++++++++++++++++++++


    int j = 0;
    // Map Iterator zum durchlaufen der QMap
    QMapIterator<QString, QStringList> it(_itemMap2);
    QStringList values;

        while(it.hasNext()) {
            it.next();
            //std::cout << it.key().toStdString() << "##" ;

            // Key in Spalte 0 eintragen und nicht editierbar machen
            QTableWidgetItem *keyItem = new QTableWidgetItem(it.key());
            ui->tableWidget_2->setItem(j, 0, keyItem);
            //keyItem->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget->resizeColumnToContents(0);

            // Values = QStringList zu angegebenem key
            values = it.value();

            // QStringList durchlaufen und valueItems setzen
            for (int i = 1; i < values.size(); i++){
                QTableWidgetItem *valueItem = new QTableWidgetItem(values.at(i));
                //valueItem->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget_2->setItem(j,i , valueItem);

            }
            // nächste Zeile
            j++;
            // Tabelle an Inhalt anpassen und sortieren
            ui->tableWidget_2->resizeColumnsToContents();
            ui->tableWidget_2->sortItems(0);
        }


    //+++++++++++++++++++++++++++++++




}





// _________________________________________________________________________________________________
// Item in Einkaufsliste schieben
// _________________________________________________________________________________________________

void MainWindow::pushItem(){

    // nur ausführen wenn Item ausgewählt und Spalte = 0
    if((ui->tableWidget->currentItem())&& ui->tableWidget->currentColumn()==0) {

        QString text = ui->tableWidget->currentItem()->text();

        // Pruefen ob Eintrag schon vorhanden ist wenn nicht dann steht in der Map in QStringList(0) = n

        if(_itemMap[text].at(0) == "n") {
            // Leere Reihe einfügen am Ende


                QTableWidgetItem *item = new QTableWidgetItem(text);
                // Status in QMap ändern


                //ui->twTable->setItem( 0 , 0 , new QTableWidgetItem( str[i] ) );

                 _anzElements = ui->tableWidget_2->rowCount();

                 // item in Tabelle2 schreiben

                 ui->tableWidget_2->setItem(_anzElements-1, 0, item);


                 // QStringList durchlaufen

                 for (int i = 1; i < _itemMap[text].size(); ++i){

                    ui->tableWidget_2->setItem(_anzElements - 1 , i , new QTableWidgetItem(_itemMap[text].at(i)) );

                    // item in QMap schieben für aktuelle Einkaufsliste

                    _itemMap2.insert(text, _itemMap[text]);

                    std::cout <<_itemMap[text].at(1).toStdString() << std::endl;
                   }


                 if(_itemMap[text].at(1) == "Penny") {
                     std::cout <<_itemMap[text].at(1).toStdString()<< std::endl;
                         ui->tableWidget_2->item(_anzElements - 1 , 1)->setBackground(QBrush(QColor(204,255,255)));
                 }


                else if(_itemMap[text].at(1) == "Edeka") {
                ui->tableWidget_2->item(_anzElements - 1 , 1)->setBackground(QBrush(QColor(0,255,33)));
                std::cout <<_itemMap[text].at(1).toStdString()<< std::endl;
                 }

                 else {
                      ui->tableWidget_2->item(_anzElements - 1 , 1)->setBackground(QBrush(QColor(255,255,153)));
                 }

                    ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
                   _itemMap[text][0] = "y";


                   ui->tableWidget_2->resizeColumnsToContents();
                   ui->tableWidget_2->sortItems(0);

}
        else {

            std::cout << "Eintrag schon vorhanden!" << std::endl;
        }


    }
}






// _________________________________________________________________________________________________
// item aus Einkaufsliste loeschen
// _________________________________________________________________________________________________

void MainWindow::deleteItemList(){

    int reihe;

    // Loeschen wenn Item vorhanden
    if((ui->tableWidget_2->currentItem()) && (ui->tableWidget_2->rowCount())) {

        QString text = ui->tableWidget_2->currentItem()->text();
        reihe =  ui->tableWidget_2->currentRow();
        //std::cout << "rowCount"  << reihe << std::endl;
        ui->tableWidget_2->removeRow(reihe);
        // Setze Index wieder auf n
        _itemMap[text][0] = "n";
        //ui->tableWidget_2->setRowCount(10);
        //_anzElements--;

    } else {
        std::cout << "Erst waehlen" << std::endl;
    }
}


// _________________________________________________________________________________________________
// Item aus csv loeschen
// _________________________________________________________________________________________________

void MainWindow::deleteItem() {

    if(ui->tableWidget->currentItem()) {
      //  int index = _listVec.indexOf(ui->tableWidget->currentItem()->text());


        QString key = ui->tableWidget->currentItem()->text();

        _itemMap.remove(key);

        std::cout << "Raus damit" << std::endl;

       // _listVec.remove(_listVec.indexOf(ui->tableWidget->currentItem()->text()));

        saveList();
        initializeMap();
        initializeEinkauf();
    }
}

// _________________________________________________________________________________________________
// Item in csv editieren und unter neuem Namen speichern
// _________________________________________________________________________________________________

void MainWindow::editItem(){

    // Editieren nur wenn Item ausgewählt
    if(ui->tableWidget->currentItem() && ui->tableWidget->currentColumn()==0) {


          QString textAlt = ui->tableWidget->currentItem()->text();
          QString text = Formedit.lineEdit->text();
          QString str1 = Formedit.lineEdit_2->text();
          QStringList newList;

        if(_itemMap.count(text) > 0 &&_itemMap[text].at(1) == str1) {
            QMessageBox::information(0, tr("Fehler!"), "Eintrag schon vorhanden.");
        } else {
            int ret = QMessageBox::question(this,"Aendern?", "Soll der Eintrag wirklich geändert werden?", QMessageBox::Yes | QMessageBox::No);

            if(ret == QMessageBox::Yes) {
                // alten Zustand in neue Liste übernehmen
                newList.append(_itemMap[textAlt].at(0));
                newList.append(str1);

                  _itemMap[text] = newList;
                   if(text != textAlt) {
                       _itemMap.remove(textAlt);
                   }


            }
         editform->close();
            saveList();
            initializeMap();
            initializeEinkauf();

        }
    }
}




