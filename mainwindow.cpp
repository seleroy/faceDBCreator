#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dataPrepButton_pressed()
{
    try {
        QString status = preparator.processDBImages();
        ui->statusLabel->setText(status);
    } catch(const std::runtime_error& error){
        ui->statusLabel->setText(error.what());
    }

}
