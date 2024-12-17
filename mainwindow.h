#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "randomgenerator.h"
#include "rsa.h"
#include "bacon.h"
#include "knapsack.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent);
    ~MainWindow();

private slots:
    void on_generatorButton_clicked();
    void on_rsaButton_clicked();
    void on_baconButton_clicked();
    void on_knapknapsackButton_clicked();

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
