#ifndef BACON_H
#define BACON_H

#include <QDialog>
#include <string>

namespace Ui {
class Bacon;
}

class Bacon : public QDialog
{
    Q_OBJECT

public:
    explicit Bacon(QWidget* parent);
    ~Bacon();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Bacon* ui;
};

#endif // BACON_H
