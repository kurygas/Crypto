#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generatorButton_clicked()
{
    RandomGenerator(this).exec();
}


void MainWindow::on_rsaButton_clicked()
{
    Rsa(this).exec();
}


void MainWindow::on_baconButton_clicked()
{
    Bacon(this).exec();
}


void MainWindow::on_knapknapsackButton_clicked()
{
    Knapsack(this).exec();
}
