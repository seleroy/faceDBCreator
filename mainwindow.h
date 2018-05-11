#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datapreparator.h"
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_dataPrepButton_pressed();

private:
    Ui::MainWindow *ui;
    DataPreparator preparator;
};

#endif // MAINWINDOW_H
