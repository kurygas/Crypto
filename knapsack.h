#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <QDialog>

namespace Ui {
class Knapsack;
}

class Knapsack : public QDialog
{
    Q_OBJECT

public:
    explicit Knapsack(QWidget* parent);
    ~Knapsack();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Knapsack* ui;
};

#endif // KNAPSACK_H
