#include "knapsack.h"
#include "ui_knapsack.h"
#include <random>

using namespace std;

struct task4 {
    vector<long long> ai, bi;
    long long R, S, T, encoded_message, decoded_message, check;

    task4(vector<long long>& ai_, vector<long long>& bi_, long long R_, long long S_, long long T_,
          long long encoded_message_, long long decoded_message_, long long check_) :
        ai(ai_), bi(bi_), R(R_), S(S_), T(T_), encoded_message(encoded_message_),
        decoded_message(decoded_message_), check(check_) {}
};

task4 knapsack(const string& ai, const string& message) {
    vector<string> step2 = {"01", "02", "04", "08", "16"};
    long long ind = -1, position = -1;
    for (long long i = 0; i < 5; i++) {
        if (ai.find(step2[i]) != string::npos) {
            ind = i; position = ai.find(step2[i]);
            break;
        }
    }

    long long rev_position = ai.size() - position;

    vector<char> mass;
    for (long long i = 1; i < 6; i++) mass.push_back('0' + i);

    auto generate_ai = [&](long long step) -> string {
        string now = string(ai.size(), '0');
        random_device rd;
        mt19937 g(rd());

        now[position] = step2[step][0];
        now[position + 1] = step2[step][1];
        for (long long i = 0; i < position; i++) {
            shuffle(mass.begin(), mass.end(), g);
            now[i] = mass[0];
        }
        for (long long i = position + 3; i < 5; i++) {
            shuffle(mass.begin(), mass.end(), g);
            reverse(mass.begin(), mass.end());
            now[i] = mass[0];
        }
        return now;
    };

    vector<string> a;
    for (long long i = 0; i < 5; i++) {
        if (i == ind) {
            a.push_back(ai);
        } else {
            a.push_back(generate_ai(i));
        }
    }

    vector<long long> a_int;
    for (auto& i : a) {
        a_int.push_back(stoi(i));
    }

    auto isPrime = [](long long x) -> bool {
        if (x == 1) return false;
        for (long long i = 2; i * i <= x; i++) {
            if (x % i == 0) return false;
        }
        return true;
    };

    auto binPow = [](long long x, long long n, long long mod, auto&& binPow) -> long long {
        if (n == 0) return 0;
        if (n == 1) return x;
        if (n % 2 == 0) {
            long long res = binPow(x, n / 2, mod, binPow) % mod;
            return res * res % mod;
        }
        return (x * binPow(x, n - 1, mod, binPow)) % mod;
    };

    auto inv = [&](long long a, long long mod) -> long long {
        return binPow(a, mod - 2, mod, binPow);
    };

    long long T = accumulate(a_int.begin(), a_int.end(), 0);
    while (!isPrime(T)) T++;

    srand(time(0));
    long long R = rand() % 150 + 152;
    while (!isPrime(R)) R++;

    long long S = inv(R, T);

    string reverse_message = message;
    reverse(reverse_message.begin(), reverse_message.end());

    vector<long long> b_int;
    for (long long i = 0; i < 5; i++) {
        b_int.push_back(a_int[i] * R % T);
    }

    long long encoded_message = 0;
    for (long long i = 0; i < message.size(); i++) {
        encoded_message += (reverse_message[i] - '0') * b_int[i];
    }

    long long decoded_message = encoded_message * S % T;
    string str_decod = to_string(decoded_message);
    string check;
    check.push_back(str_decod[str_decod.size() - rev_position]); check.push_back(str_decod[str_decod.size() - rev_position + 1]);

    return task4(a_int, b_int, R, S, T, encoded_message, decoded_message, stoi(check));
}


Knapsack::Knapsack(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Knapsack)
{
    ui->setupUi(this);
}

Knapsack::~Knapsack()
{
    delete ui;
}

void Knapsack::on_pushButton_clicked()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("a_i"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("b_i"));

    auto res = knapsack(ui->lineEdit_2->text().toStdString(), ui->lineEdit->text().toStdString());

    for (int i = 0; i < 5; ++i) {
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));

        auto itm1 = new QTableWidgetItem(QString::number(res.ai[i]));
        ui->tableWidget->setItem(i, 0, itm1);
        itm1->setTextAlignment(Qt::AlignHCenter);

        auto itm2 = new QTableWidgetItem(QString::number(res.bi[i]));
        ui->tableWidget->setItem(i, 1, itm2);
        itm2->setTextAlignment(Qt::AlignHCenter);
    }

    ui->label_3->setText("R = " + QString::number(res.R));
    ui->label_4->setText("S = " + QString::number(res.S));
    ui->label_5->setText("T = " + QString::number(res.T));
    ui->label_6->setText("Check = " + QString::number(res.check));
    ui->label_8->setText("M = " + QString::number(res.decoded_message));
    ui->label_7->setText("M' = " + QString::number(res.encoded_message));
}
