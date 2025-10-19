#include "../header/buyer.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

// constructor
Buyer::Buyer(const string& username, const string& password)
    : username(username), password(password), loggedIn(false)
{
    random_device rd;
    int num = rd() % 9000 + 1000;
    userId = "USR" + to_string(num);
    bankAccount = BankCustomer(userId, 0.0);

    cout << "[info] Akun bank " << bankAccount.getAccountId() 
         << " telah dibuat untuk pengguna '" << this->username << "'.\n";
}

// getter
string Buyer::getUserId() const { return userId; }
string Buyer::getUsername() const { return username; }
string Buyer::getPassword() const { return password; }

bool Buyer::checkPassword(const string& pass) const {
    return password == pass;
}

bool Buyer::login(const string& pass) {
    if (checkPassword(pass)) {
        loggedIn = true;
        return true;
    }
    return false;
}

void Buyer::logout() {
    loggedIn = false;
}

bool Buyer::isLoggedIn() const {
    return loggedIn;
}

BankCustomer& Buyer::getBankAccount() {
    return bankAccount;
}

// fitur bank
void Buyer::topUp(double amount) {
    cout << "\n[" << username << "] Top Up: " << amount << endl;
    bankAccount.topUp(amount);
}

void Buyer::withdraw(double amount) {
    cout << "\n[" << username << "] Withdraw: " << amount << endl;
    bankAccount.withdraw(amount);
}

void Buyer::showCashFlowToday() const {
    cout << "\n[" << username << "] Cash Flow Hari Ini:\n";
    bankAccount.listCashFlowToday();
}

void Buyer::showCashFlowMonth() const {
    cout << "\n[" << username << "] Cash Flow Bulan Ini:\n";
    bankAccount.listCashFlowMonth();
}

// fitur store sementara
void Buyer::purchaseItem(double price) {
    cout << "\n[" << username << "] Mau beli item seharga " << price << endl;
    if (bankAccount.getBalance() >= price) {
        bankAccount.withdraw(price);
        cout << "Pembelian berhasil! saldo tersisa: " 
             << bankAccount.getBalance() << endl;
    } else {
        cout << "Saldo tidak cukup. Transaksi dibatalkan.\n";
    }
}

// void Buyer::listOrders() const {
//     cout << "\n[" << username << "] Lihat daftar order\n";
// }

void Buyer::checkSpending(int k_days) const {
    cout << "\n[" << username << "] Pengeluaran " << k_days << " hari terakhir:\n";
    const auto& history = bankAccount.getTransactionHistory();
    time_t now = time(nullptr);
    double total = 0;

    for (const auto& trx : history) {
        if (trx.type == "Debit" && difftime(now, trx.timestamp) <= k_days * 24 * 60 * 60) {
            total += trx.amount;
        }
    }

    cout << "Total pengeluaran dalam " << k_days << " hari terakhir: " << total << endl;
}
