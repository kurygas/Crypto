#include "bacon.h"
#include "ui_bacon.h"
#include <map>
#include <random>

using namespace std;

struct row {
    char letter;
    string code;
    int k1, k2;

    row(char l_, string& c_, int k1_, int k2_)
        : letter(l_), code(c_), k1(k1_), k2(k2_) {}
};

pair<vector<string>, vector<row>> theBaconCipher(string s1, string s2) {
    for (auto& i : s1) i = toupper(i);
    for (auto& i : s2) i = toupper(i);

    auto bin = [](int num) -> string {
        string res;
        while (num > 0) {
            res.push_back('0' + (num % 2));
            num /= 2;
        }
        while (res.size() < 5) res.push_back('0');
        reverse(res.begin(), res.end());
        return res;
    };

    vector<string> codes(26);
    for (int i = 1; i <= 26; i++) {
        codes[i - 1] = bin(i);
    }

    vector<int> k1(26, 0);
    for (int i = 0; i < 13; i++) k1[i] = 1;

    random_device rd;
    mt19937 g(rd());
    shuffle(codes.begin(), codes.end(), g);
    shuffle(k1.begin(), k1.end(), g);

    vector<row> gpsh;
    for (int i = 0; i < 26; i++) {
        gpsh.emplace_back('A' + i, codes[i], k1[i], -1);
    }
    vector<char> k1_equal1, k1_equal0;
    for (int i = 0; i < 26; i++) {
        if (gpsh[i].k1 == 0) {
            k1_equal0.push_back(gpsh[i].letter);
        } else {
            k1_equal1.push_back(gpsh[i].letter);
        }
    }

    string translate_s1 = gpsh[s1[0] - 'A'].code, translate_s2 = gpsh[s2[0] - 'A'].code;
    for (int i = 1; i < s1.size(); i++) {
        translate_s1 += gpsh[s1[i] - 'A'].code;
    }
    for (int i = 1; i < s2.size(); i++) {
        translate_s2 += gpsh[s2[i] - 'A'].code;
    }

    map<char, pair<int, int>> used;
    char confl_00 = k1_equal0[0], confl_01 = k1_equal0[1], confl_10 = k1_equal1[0], confl_11 = k1_equal1[1];
    used[confl_00] = {0, 0};
    used[confl_01] = {0, 1};
    used[confl_10] = {1, 0};
    used[confl_11] = {1, 1};

    int ind0 = 0, ind1 = 0;
    string merge;
    for (int i = 0; i < translate_s1.size(); i++) {
        if (ind0 == 13) ind0 = 0;
        if (ind1 == 13) ind1 = 0;

        if (translate_s1[i] == '0') {
            char try_letter = k1_equal0[ind0];
            if (used.find(try_letter) != used.end()) {
                if (used[try_letter] == make_pair(0, (translate_s2[i] - '0'))) {
                    merge.push_back(try_letter);
                } else {
                    if (translate_s2[i] == '0') {
                        merge.push_back(confl_00);
                    } else {
                        merge.push_back(confl_01);
                    }
                }
            } else {
                used[try_letter] = {0, translate_s2[i] - '0'};
                merge.push_back(try_letter);
            }
            ind0++;
        } else {
            char try_letter = k1_equal1[ind1];
            if (used.find(try_letter) != used.end()) {
                if (used[try_letter] == make_pair(1, (translate_s2[i] - '0'))) {
                    merge.push_back(try_letter);
                } else {
                    if (translate_s2[i] == '0') {
                        merge.push_back(confl_10);
                    } else {
                        merge.push_back(confl_11);
                    }
                }
            } else {
                used[try_letter] = {1, translate_s2[i] - '0'};
                merge.push_back(try_letter);
            }
            ind1++;
        }
    }

    for (auto& [letter, pair] : used) {
        auto& [_, k2_] = pair;
        gpsh[letter - 'A'].k2 = k2_;
    }

    vector<string> ans;
    ans.push_back(s1);
    ans.push_back(translate_s1);
    ans.push_back(merge);
    ans.push_back(translate_s2);
    ans.push_back(s2);

    return make_pair(ans, gpsh);
}

Bacon::Bacon(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Bacon)
{
    ui->setupUi(this);
}

Bacon::~Bacon()
{
    delete ui;
}

void Bacon::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget_2->clear();

    auto [str, r] = theBaconCipher(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString());

    ui->tableWidget_2->setRowCount(5);
    ui->tableWidget_2->setColumnCount(ui->lineEdit->text().size());

    for (int i = 0; i < ui->lineEdit->text().size(); ++i) {
        auto* itm1 = new QTableWidgetItem(QString(str[0][i]));
        itm1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem(0, i, itm1);

        auto* itm2 = new QTableWidgetItem(QString::fromStdString(str[1].substr(i * 5, 5)));
        itm2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem(1, i, itm2);

        auto* itm3 = new QTableWidgetItem(QString::fromStdString(str[2].substr(i * 5, 5)));
        itm3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem(2, i, itm3);

        auto* itm4 = new QTableWidgetItem(QString::fromStdString(str[3].substr(i * 5, 5)));
        itm4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem(3, i, itm4);

        auto* itm5 = new QTableWidgetItem(QString(str[4][i]));
        itm5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setItem(4, i, itm5);
    }

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(26);

    for (int i = 0; i < 26; ++i) {
        auto itm1 = new QTableWidgetItem(QString(r[i].letter));
        itm1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, itm1);

        auto itm2 = new QTableWidgetItem(QString::fromStdString(r[i].code));
        itm2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 1, itm2);

        auto itm3 = new QTableWidgetItem(QString::number(r[i].k1));
        itm3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, itm3);

        auto itm4 = new QTableWidgetItem(QString::number(r[i].k2));
        itm4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, itm4);
    }
}
