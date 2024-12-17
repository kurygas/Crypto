#ifndef RSA_H
#define RSA_H

#include <QDialog>
#include <vector>

namespace Ui {
class Rsa;
}

class Rsa : public QDialog
{
    Q_OBJECT

public:
    explicit Rsa(QWidget* parent);
    ~Rsa();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Rsa* ui;
};

#endif // RSA_H
