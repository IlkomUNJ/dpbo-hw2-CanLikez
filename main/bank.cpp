#include "../header/bank.h"
#include "../header/bankCustomer.h"
#include "../header/utils.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <map>

using namespace std;
// tampilkan semua akun
void Bank::listAllCustomers(const vector<BankCustomer>& allCustomers) {
    cout << "\n=== Daftar semua akun di bank ===\n";
    if (allCustomers.empty()) {
        cout << "belum ada akun yang terdaftar :( \n";
        return;
    }

    cout << "OwnerID\t\tAkunID\t\tSaldo\n";
    cout << "----------------------------------------\n";
    for (auto &cust : allCustomers) {
        cout << cust.getOwnerId() << "\t\t"
             << cust.getAccountId() << "\t"
             << cust.getBalance() << endl;
    }
}

// cek akun yang udah ga aktif sebulan
void Bank::listDormantAccounts(const vector<BankCustomer>& allCustomers) {
    cout << "\n=== Akun yang udah lama ga transaksi ===\n";
    time_t now = time(nullptr);
    double sebulan = 30 * 24 * 60 * 60;
    bool adaYangTidur = false;

    for (auto &cust : allCustomers) {
        auto history = cust.getTransactionHistory();
        if (history.empty()) continue;

        time_t last = history.back().timestamp;
        if (difftime(now, last) > sebulan) {
            cout << "- " << cust.getAccountId()
                 << " (" << cust.getOwnerId() << ")"
                 << " terakhir transaksi: " << formatTanggal(last) << endl;
            adaYangTidur = true;
        }
    }

    if (!adaYangTidur)
        cout << "semua akun masih aktif\n";
}

// cari user paling aktif hari ini
void Bank::listTopUsersToday(const vector<BankCustomer>& allCustomers, int n) {
    cout << "\n=== " << n << " pengguna paling aktif hari ini ===\n";

    time_t now = time(nullptr);
    tm* t = localtime(&now);
    t->tm_hour = 0; t->tm_min = 0; t->tm_sec = 0;
    time_t startHari = mktime(t);

    map<string, int> transaksiUser;

    for (auto &cust : allCustomers) {
        int count = 0;
        for (auto &trx : cust.getTransactionHistory()) {
            if (trx.timestamp >= startHari) count++;
        }
        if (count > 0)
            transaksiUser[cust.getOwnerId()] = count;
    }

    if (transaksiUser.empty()) {
        cout << "belum ada transaksi hari ini 😅\n";
        return;
    }

    vector<pair<string, int>> urutan(transaksiUser.begin(), transaksiUser.end());
    sort(urutan.begin(), urutan.end(), [](auto &a, auto &b) {
        return a.second > b.second;
    });

    int rank = 1;
    for (auto &user : urutan) {
        if (rank > n) break;
        cout << rank++ << ". " << user.first << " (" << user.second << "x transaksi)\n";
    }
}

// tampil transaksi minggu ini
void Bank::listWeeklyTransactions(const BankCustomer& customer) {
    cout << "\n=== Transaksi seminggu terakhir ===\n";
    cout << "Akun: " << customer.getAccountId() << " (" << customer.getOwnerId() << ")\n";

    time_t now = time(nullptr);
    double seminggu = 7 * 24 * 60 * 60;
    bool ada = false;

    for (auto &trx : customer.getTransactionHistory()) {
        if (difftime(now, trx.timestamp) <= seminggu) {
            cout << "- " << formatTanggal(trx.timestamp)
                 << " | " << trx.type
                 << " | " << trx.amount << endl;
            ada = true;
        }
    }

    if (!ada)
        cout << "ga ada transaksi minggu ini.\n";
}
