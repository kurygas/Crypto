#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QDialog>
#include <set>

using namespace std;

struct TResult {
    vector<int> polynom;
    vector<vector<string>> cycles;
};

namespace Ui {
class RandomGenerator;
}

class RandomGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit RandomGenerator(QWidget* parent = nullptr);
    ~RandomGenerator();

    static TResult generate(vector<int> polynom, const int MOD);

private slots:
    void on_calcButton_clicked();

private:
    Ui::RandomGenerator* ui;
};

#endif // RANDOMGENERATOR_H
