#include "../header/bankCustomer.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

// constructor: auto generate ID random
BankCustomer::BankCustomer(const string& ownerId, double awal)
    : ownerId(ownerId), balance(awal) {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(100000, 999999);
    accountId = "ACC" + to_string(dist(gen));
}

string BankCustomer::getAccountId() const { return accountId; }
string BankCustomer::getOwnerId() const { return ownerId; }
double BankCustomer::getBalance() const { return balance; }
const vector<BankTransaction>& BankCustomer::getTransactionHistory() const { return transactionHistory; }

// nambah saldo
void BankCustomer::topUp(double jumlah) {
    if (jumlah <= 0) {
        cout << "jumlah top up tidak valid\n";
        return;
    }

    balance += jumlah;
    transactionHistory.push_back({jumlah, "topup", time(nullptr)});
    cout << "top up berhasil! saldo sekarang: " << balance << endl;
}

// tarik saldo
bool BankCustomer::withdraw(double jumlah) {
    if (jumlah <= 0) {
        cout << "jumlah penarikan tidak valid\n";
        return false;
    }

    if (jumlah > balance) {
        cout << "saldo kurang, saldo: " << balance << endl;
        return false;
    }

    balance -= jumlah;
    transactionHistory.push_back({jumlah, "withdraw", time(nullptr)});
    cout << "penarikan berhasil! sisa saldo: " << balance << endl;
    return true;
}

// liat transaksi hari ini
void BankCustomer::listCashFlowToday() const {
    cout << "\nCashflow hari ini (" << accountId << ") \n";
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    t->tm_hour = 0; t->tm_min = 0; t->tm_sec = 0;
    time_t startHari = mktime(t);

    bool ada = false;
    for (auto &trx : transactionHistory) {
        if (trx.timestamp >= startHari) {
            cout << "- " << trx.type << " " << trx.amount << endl;
            ada = true;
        }
    }

    if (!ada)
        cout << "belum ada transaksi hari ini\n";
}

// liat transaksi 30 hari terakhir
void BankCustomer::listCashFlowMonth() const {
    cout << "Cashflow 30 hari terakhir (" << accountId << ") " << endl;
    time_t now = time(nullptr);
    double sebulan = 30 * 24 * 60 * 60;
    bool ada = false;

    for (auto &trx : transactionHistory) {
        if (difftime(now, trx.timestamp) <= sebulan) {
            cout << "- " << trx.type << " " << trx.amount << endl;
            ada = true;
        }
    }

    if (!ada)
        cout << "belum ada transaksi bulan ini\n";
}

// void BankCustomer::backdateLastTransaction(int days) {
//     if (transactionHistory.empty()) return;
//     // kurangi timestamp terakhir sebanyak days hari (days * 24*60*60)
//     transactionHistory.back().timestamp -= static_cast<time_t>(days) * 24 * 60 * 60;
// }