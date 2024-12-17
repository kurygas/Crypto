#include "randomgenerator.h"
#include "ui_randomgenerator.h"

RandomGenerator::RandomGenerator(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RandomGenerator)
{
    ui->setupUi(this);
    show();
}

RandomGenerator::~RandomGenerator()
{
    delete ui;
}

void RandomGenerator::on_calcButton_clicked()
{
    ui->treeWidget->clear();
    auto mod = ui->pEdit->text().toInt();
    vector<int> polynom = {ui->aEdit->text().toInt(), ui->bEdit->text().toInt(), ui->cEdit->text().toInt()};
    auto result = generate(polynom, mod);
    auto bText = QString::number(-result.polynom[1]) == "1" ? "" : QString::number(-result.polynom[1]);
    auto cText = QString::number(-result.polynom[2]) == "1" ? "" : QString::number(-result.polynom[2]);
    ui->polynom->setText("φ(x) = x^2 - " + bText + "x - " + QString::number(-result.polynom[2]));
    ui->qLabel1->setText("Q1* = " + cText + "Q2 (mod" + QString::number(mod) + ")");
    ui->qLabel2->setText("Q2* = " + bText + "Q2 + Q1 (mod" + QString::number(mod) + ")");

    for (int i = 1; i <= result.cycles.size(); ++i) {
        auto* itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0, "Цикл " + QString::number(i));

        for (const auto& j : result.cycles[i - 1]) {
            auto* num = new QTreeWidgetItem(itm);
            num->setText(0, QString::fromStdString(j));
        }
    }
}

TResult RandomGenerator::generate(vector<int> polynom, const int MOD) {
    int a = *polynom.begin();
    int k = 1;
    while ((a * k) % MOD != 1) {
        ++k;
    }
    a = (a * k) % MOD;

    auto create = [MOD](int x) {
        return (x < 0) ? x : MOD - x;
    };

    int b = (*next(polynom.begin()) * k) % MOD;
    b = create(b);
    int c = (*prev(polynom.end()) * k) % MOD;
    c = create(c);
    TResult result;
    result.polynom = {a, -b, -c};

    set<pair<int, int>> numbers;
    for (int i = 0; i < MOD; ++i) {
        for (int j = 0; j < MOD; ++j) {
            numbers.emplace(i, j);
        }
    }

    numbers.erase(make_pair(0, 0));

    auto go = [&](const int q1, const int q2, auto&& go) -> void {
        numbers.erase(make_pair(q1, q2));
        result.cycles.back().emplace_back((q1 == 0) ? "0" + to_string(q2) : to_string(q1) + to_string(q2));
        const int Q1 = (q1 * b + q2) % MOD;
        const int Q2 = (q1 * c) % MOD;
        if (numbers.find(make_pair(Q1, Q2)) != numbers.end()) {
            go(Q1, Q2, go);
        }
    };

    result.cycles.emplace_back();
    result.cycles.back().emplace_back("00");
    while (!numbers.empty()) {
        const auto [q1, q2] = *numbers.begin();
        result.cycles.emplace_back();
        go(q1, q2, go);
    }

    return result;

}
