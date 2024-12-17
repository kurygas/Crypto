#include "rsa.h"
#include "ui_rsa.h"

using namespace std;

namespace NRSA {
struct TMessage {
    vector<int> data;
    const long long sign;
    const long long pubKey;
    const long long secKey;
    const long long p, q;
};

auto hash = [](const auto& m) {
    const long long MOD = 8;
    return accumulate(m.begin(), m.end(), 0ll) % MOD;
};

auto toPowerModular = [](const long long x, long long power, const long long MOD) {
    long long cur = 1;
    while (power--) {
        cur *= x;
        cur %= MOD;
    }

    return x;
};

TMessage RSASend(const vector<int>& message, const long long p, const long long q) {
    const long long HASH = hash(message);
    const long long N = p * q;
    const long long euler = (p - 1) * (q - 1);
    const long long e = (p == 13 || q == 13) ? (p == 7 || q == 7) ? 17 : 7 : 13;

    auto getInverseModulo = [MOD = euler](const long long x) {
        long long rev = 1;
        while ((x * rev) % MOD != 1) {
            ++rev;
        }
        return rev;
    };

    const long long secKey = getInverseModulo(e);

    const long long sign = toPowerModular(HASH, secKey, N);

    return {message, sign, euler, secKey, p, q};
}

bool RSARecieve(const TMessage& recievedMessage) {
    const long long HASH = hash(recievedMessage.data);
    const long long N = recievedMessage.p * recievedMessage.q;
    const long long CHECK_HASH = toPowerModular(recievedMessage.sign, recievedMessage.pubKey, N);

    return HASH == CHECK_HASH;
}

struct TResult {
    TMessage message;
    bool result;
};

TResult RSA(const vector<int>& message, const long long p, const long long q) {
    auto m = RSASend(message, p, q);
    return {m, RSARecieve(m)};
}
}

Rsa::Rsa(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Rsa)
{
    ui->setupUi(this);
}

Rsa::~Rsa()
{
    delete ui;
}

void Rsa::on_pushButton_clicked()
{
    std::vector<int> v;
    for (const auto& c : ui->lineEdit->text()) {
        v.push_back(c.digitValue());
    }

    auto res = NRSA::RSA(v, ui->lineEdit_2->text().toInt(), ui->lineEdit_3->text().toInt());

    ui->label_4->setText("Public key: " + QString::number(res.message.pubKey));
    ui->label_5->setText("Secure key: " + QString::number(res.message.secKey));
    ui->label_6->setText("Электронная подпись: " + QString::number(res.message.sign));
    ui->label_7->setText(QString("ЭП ") + (res.result ? "" : "не ") + "принимается");
}
