#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsondata.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle("KriptYashka - JSON");
    QIcon winIcon("favicon.ico");
    this->setWindowIcon(winIcon);
}

MainWindow::~MainWindow(){
    delete ui;
    delete viewModel;
}

void MainWindow::on_btnShowJson_clicked(){
    ui->colView->setModel(viewModel);
}

QStandardItem* convert(GroupItem item){
    QStandardItem* standardItem = new QStandardItem(QString::fromStdString(item.getName()));
    for (int childIndex = 0; childIndex < item.getChildrenCount(); ++childIndex){
        QStandardItem* standardChild = convert(item.getChild(childIndex));
        standardItem->appendRow(standardChild);
    }
    return standardItem;
}

void MainWindow::on_btnReadJson_clicked(){
    //QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    string path = "test.json";
    //JsonData jsondata = JsonData(filePath.toStdString());
    JsonData jsondata = JsonData(path);
    string resultText = jsondata.getCodText();
    ui->resultLabel->setText(QString::fromStdString(resultText));
    QFont font = ui->resultLabel->font();
    viewModel->clear();
    ui->jsontext->setText(QString::fromStdString(jsondata.getJsonText()));

    if (jsondata.isError()){
        int line = jsondata.getLineError(), col = jsondata.getColError();
        if (line != -1 && col != -1){
            QString errorstr = "Error line: " + QString::number(line) + ", col: " + QString::number(col);
            ui->resultLabel->setText(ui->resultLabel->text() + '\n' + errorstr);
        }
        font.setBold(true);
        ui->resultLabel->setStyleSheet("QLabel { color : red; } QLabel:hover { color : black; }");
        ui->btnShowJson->setDisabled(true);
    } else {
        ui->btnShowJson->setDisabled(false);
        font.setBold(false);
        ui->resultLabel->setStyleSheet("QLabel { color : green;  } QLabel:hover { color : black; }");
        ui->resultLabel->setFont(font);
        for (int groupnum = 0; groupnum < jsondata.rootSize() ; ++groupnum){
            GroupItem rootItem = jsondata.at(groupnum);
            QStandardItem *rootGroup = convert(rootItem);
            viewModel->appendRow(rootGroup);
        }
    }
}
