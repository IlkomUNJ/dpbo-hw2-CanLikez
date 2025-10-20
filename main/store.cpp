#include "../header/store.h"
#include "../header/utils.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

Store::Store() {
    loadData();
}

// STORE

void Store::run() {
    string input;
    int choice = 0;

    while (choice != 3) {
        cout << "\n==================================" << endl;
        cout << "   Selamat Datang di Toko" << endl;
        cout << "==================================" << endl;
        cout << "1. Register Akun Baru" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit Aplikasi" << endl;
        cout << "Pilihan Anda: ";
        cout.flush();

        getline(cin, input);
        if (input.empty()) continue;
        try {
            choice = stoi(input);
        } catch (...) {
            choice = 0;
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2: login(); break;
            case 3:
                cout << "Menyimpan data sebelum keluar..." << endl;
                saveData();
                cout << "Terima kasih telah menggunakan aplikasi. Sampai jumpa!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }
    }
}

// manajemen pengguna
void Store::registerUser() {
    cout << "\n--- Registrasi Pengguna Baru ---" << endl;

    string input;
    cout << "Daftar sebagai (1) Buyer atau (2) Seller? ";
    cout.flush();

    getline(cin, input);
    while (input != "1" && input != "2") {
        cout << "Input tidak valid! Masukkan 1 (Buyer) atau 2 (Seller): ";
        getline(cin, input);
    }

    int role = stoi(input);

    string username, password;
    cout << "Masukkan Username: ";
    getline(cin, username);
    cout << "Masukkan Password: ";
    getline(cin, password);

    for (const auto& b : buyers)
        if (b.getUsername() == username) {
            cout << "Username sudah ada!" << endl;
            return;
        }

    for (const auto& s : sellers)
        if (s.getUsername() == username) {
            cout << "Username sudah ada!" << endl;
            return;
        }

    if (role == 1) {
        buyers.emplace_back(username, password);
        cout << "Registrasi Buyer berhasil!" << endl;
    } else {
        sellers.emplace_back(username, password);
        cout << "Registrasi Seller berhasil!" << endl;
    }
}

void Store::login() {
    cout << "\n--- Login Pengguna ---" << endl;
    string username, password;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    for (auto& seller : sellers) {
        if (seller.getUsername() == username) {
            if (seller.login(password)) {
                cout << "Login sebagai Seller berhasil!" << endl;
                showSellerMenu(seller);
                return;
            } else {
                cout << "Password salah." << endl;
                return;
            }
        }
    }

    for (auto& buyer : buyers) {
        if (buyer.getUsername() == username) {
            if (buyer.login(password)) {
                cout << "Login sebagai Buyer berhasil!" << endl;
                showBuyerMenu(buyer);
                return;
            } else {
                cout << "Password salah." << endl;
                return;
            }
        }
    }
    cout << "Username tidak ditemukan." << endl;
}

// menu pengguna

void Store::showBuyerMenu(Buyer& buyer) {
    int choice = 0;
    while (buyer.isLoggedIn()) {
        cout << "\n--- Menu Buyer: " << buyer.getUsername() << " ---" << endl;
        cout << "1. Beli Barang" << endl;
        cout << "2. Lihat Riwayat Pesanan Saya" << endl;
        cout << "3. Fitur Perbankan" << endl;
        cout << "4. Logout" << endl;
        cout << "Pilihan: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: purchaseItem(buyer); break;
            case 2: {
                cout << "\n--- Filter Riwayat Pesanan ---" << endl;
                cout << "Tampilkan pesanan dengan status: " << endl;
                cout << "1. Semua" << endl;
                cout << "2. PAID" << endl;
                cout << "3. COMPLETED" << endl;
                cout << "4. CANCELLED" << endl;
                cout << "Pilihan: ";
                int filterChoice;
                cin >> filterChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                OrderStatus filterStatus;
                bool filter = true;
                switch(filterChoice) {
                    case 1: filter = false; break;
                    case 2: filterStatus = OrderStatus::PAID; break;
                    case 3: filterStatus = OrderStatus::COMPLETED; break;
                    case 4: filterStatus = OrderStatus::CANCELLED; break;
                    default: cout << "Pilihan tidak valid." << endl; continue;
                }

                cout << "\n--- Riwayat Pesanan " << buyer.getUsername() << " ---" << endl;
                bool found = false;
                for(const auto& trx : transactions) {
                    if (trx.getBuyerId() == buyer.getUserId()) {
                        if (!filter || trx.getStatus() == filterStatus) {
                            cout << "ID: " << trx.getTransactionId() << " | Total: " << trx.getTotalPrice() << " | Waktu: " << formatTanggal(trx.getDate()) << " | Status: " 
                                << (trx.getStatus() == OrderStatus::PAID ? "PAID" : 
                                    trx.getStatus() == OrderStatus::COMPLETED ? "COMPLETED" : "CANCELLED") << endl;
                            found = true;
                        }
                    }
                }
                if (!found) cout << "Tidak ada pesanan yang sesuai dengan filter." << endl;
                break;
            }
            case 3: {
                // Sub-menu Perbankan
                int bankChoice = 0;
                cout << "\n--- Menu Perbankan ---" << endl;
                cout << "1. Top Up Saldo" << endl;
                cout << "2. Tarik Saldo" << endl;
                cout << "3. Cek Arus Kas Hari Ini" << endl;
                cout << "4. Cek Arus Kas Bulan Ini" << endl;
                cout << "Pilihan: ";
                cin >> bankChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(bankChoice == 1) { double amount; cout << "Jumlah: "; cin >> amount; buyer.topUp(amount); }
                else if(bankChoice == 2) { double amount; cout << "Jumlah: "; cin >> amount; buyer.withdraw(amount); }
                else if(bankChoice == 3) buyer.showCashFlowToday();
                else if(bankChoice == 4) buyer.showCashFlowMonth();
                else cout << "Pilihan tidak valid." << endl;
                break;
            }
            case 4: buyer.logout(); break;
            default: cout << "Pilihan tidak valid." << endl;
        }
    }
}

void Store::showSellerMenu(Seller& seller) {
    int choice = 0;
    while (seller.isLoggedIn()) {
        cout << "\n--- Menu Seller: " << seller.getUsername() << " ---" << endl;
        cout << "1. Manajemen Item" << endl;
        cout << "2. Lihat Statistik Penjualan Toko" << endl;
        cout << "3. Kelola Pesanan Masuk" << endl;
        cout << "4. Beli Barang (sebagai Buyer)" << endl;
        cout << "5. Fitur Perbankan" << endl;
        cout << "6. Logout" << endl;
        cout << "Pilihan: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: seller.manageItems(); break;
            case 2:
                seller.discoverTopKItems(3, transactions);
                seller.discoverLoyalCustomer(transactions);
                break;
            case 3: manageSellerOrders(seller); break;
            case 4: purchaseItem(seller); break;
            case 5: {
                int bankChoice = 0;
                cout << "\n--- Menu Perbankan ---" << endl;
                cout << "1. Top Up Saldo" << endl;
                cout << "2. Tarik Saldo" << endl;
                cout << "3. Cek Arus Kas Hari Ini" << endl;
                cout << "4. Cek Arus Kas Bulan Ini" << endl;
                cout << "Pilihan: ";
                cin >> bankChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(bankChoice == 1) { double amount; cout << "Jumlah: "; cin >> amount; seller.topUp(amount); }
                else if(bankChoice == 2) { double amount; cout << "Jumlah: "; cin >> amount; seller.withdraw(amount); }
                else if(bankChoice == 3) seller.showCashFlowToday();
                else if(bankChoice == 4) seller.showCashFlowMonth();
                else cout << "Pilihan tidak valid." << endl;
                break;
            }
            case 6: seller.logout(); break;
            default: cout << "Pilihan tidak valid." << endl;
        }
    }
}

// fungsi utama toko

void Store::displayAllItems() {
    cout << "\n--- Daftar Semua Item yang Tersedia ---" << endl;
    bool anyItem = false;
    for (auto& seller : sellers) {
        for (const auto& item : seller.getItemsForSale()) {
            if (item.getStock() > 0) {
                cout << "ID: " << item.getItemId() << " | Nama: " << item.getName()
                     << " | Harga: " << item.getPrice() << " | Stok: " << item.getStock()
                     << " | Penjual: " << seller.getUsername() << endl;
                anyItem = true;
            }
        }
    }
    if (!anyItem) {
        cout << "Sayang sekali, belum ada item yang dijual." << endl;
    }
}

void Store::purchaseItem(Buyer& activeBuyer) {
    displayAllItems();
    cout << "Masukkan ID Item yang ingin dibeli: ";
    string itemId;
    getline(cin, itemId);

    for (auto& seller : sellers) {
        for (auto& item : seller.getItemsForSale()) {
            if (item.getItemId() == itemId) {
                if (item.getStock() <= 0) { cout << "Stok item habis." << endl; return; }
                if (activeBuyer.getUserId() == seller.getUserId()) { cout << "Anda tidak bisa membeli item Anda sendiri." << endl; return; }
                if (activeBuyer.getBankAccount().getBalance() < item.getPrice()) { cout << "Saldo tidak mencukupi." << endl; return; }

                double price = item.getPrice();
                if (activeBuyer.getBankAccount().withdraw(price)) {
                    seller.getBankAccount().topUp(price);
                    item.setStock(item.getStock() - 1);
                    
                    vector<Item> purchasedItems = {item};
                    transactions.emplace_back(activeBuyer.getUserId(), seller.getUserId(), purchasedItems);

                    cout << "Pembelian '" << item.getName() << "' berhasil!" << endl;
                }
                return;
            }
        }
    }
    cout << "Item dengan ID tersebut tidak ditemukan." << endl;
}

void Store::manageSellerOrders(Seller& seller) {
    cout << "\n--- Kelola Pesanan Masuk untuk " << seller.getUsername() << " ---" << endl;
    vector<Transaction*> relevantTransactions;

    cout << "Pesanan yang perlu diproses (Status: PAID):" << endl;
    for (Transaction& trx : transactions) {
        if (trx.getSellerId() == seller.getUserId() && trx.getStatus() == OrderStatus::PAID) {
            cout << "  -> ID: " << trx.getTransactionId() << " | Pembeli: " << trx.getBuyerId() 
                 << " | Total: " << trx.getTotalPrice() << " | Waktu: " << formatTanggal(trx.getDate()) << endl;
            relevantTransactions.push_back(&trx);
        }
    }

    if (relevantTransactions.empty()) {
        cout << "Tidak ada pesanan yang perlu diproses." << endl;
        return;
    }

    cout << "Masukkan ID Transaksi yang ingin diubah statusnya: ";
    string trxId;
    getline(cin, trxId);

    for (Transaction* trxPtr : relevantTransactions) {
        if (trxPtr->getTransactionId() == trxId) {
            cout << "Ubah status menjadi: (1) COMPLETED, (2) CANCELLED: ";
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) {
                trxPtr->setStatus(OrderStatus::COMPLETED);
                cout << "Status berhasil diubah menjadi COMPLETED." << endl;
            } else if (choice == 2) {
                double refundAmount = trxPtr->getTotalPrice();

                // 1. Cari Buyer yang sesuai
                Buyer* buyerToRefund = nullptr;
                for (auto& buyer : buyers) {
                    if (buyer.getUserId() == trxPtr->getBuyerId()) {
                        buyerToRefund = &buyer;
                        break;
                    }
                }

                // Jika buyer tidak ditemukan, batalkan
                if (!buyerToRefund) {
                    cout << "Error: Data buyer tidak ditemukan. Refund gagal." << endl;
                    return;
                }
                
                // 2. Proses transfer uang kembali
                cout << "Memproses refund sebesar " << refundAmount << "..." << endl;
                seller.getBankAccount().withdraw(refundAmount);
                buyerToRefund->getBankAccount().topUp(refundAmount);

                // 3. Ubah status transaksi
                trxPtr->setStatus(OrderStatus::CANCELLED);
                cout << "Pesanan berhasil dibatalkan dan dana telah dikembalikan." << endl;
            } else {
                cout << "Pilihan tidak valid." << endl;
            }
            return;
        }
    }
    cout << "Transaksi dengan ID tersebut tidak ditemukan." << endl;
}
// store analysis

void Store::listLatestTransactions(int k_days) const {
    cout << "\n--- Transaksi dalam " << k_days << " Hari Terakhir ---" << endl;
    time_t now = time(nullptr);
    bool found = false;
    for (const auto& trx : transactions) {
        if (difftime(now, trx.getDate()) <= k_days * 24 * 60 * 60) {
            cout << "ID: " << trx.getTransactionId() << " | Buyer: " << trx.getBuyerId() << " | Total: " << trx.getTotalPrice() << endl;
            found = true;
        }
    }
    if (!found) cout << "Tidak ada transaksi dalam periode tersebut." << endl;
}

void Store::listPaidButUncompleted() const {
    cout << "\n--- Transaksi Dibayar Belum Selesai ---" << endl;
    bool found = false;
    for (const auto& trx : transactions) {
        if (trx.getStatus() == OrderStatus::PAID) {
            cout << "ID: " << trx.getTransactionId() << " | Buyer: " << trx.getBuyerId() << endl;
            found = true;
        }
    }
    if (!found) cout << "Semua transaksi sudah selesai atau dibatalkan." << endl;
}

void Store::listMostFrequentItems(int m) const {
    cout << "\n--- Top " << m << " Item Paling Sering Dibeli ---" << endl;
    map<string, int> itemCounts;
    for (const auto& trx : transactions) {
        for (const auto& item : trx.getItems()) {
            itemCounts[item.getName()]++;
        }
    }

    if (itemCounts.empty()) { cout << "Belum ada transaksi." << endl; return; }

    vector<pair<string, int>> sortedItems(itemCounts.begin(), itemCounts.end());
    sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

    int rank = 1;
    for (const auto& item : sortedItems) {
        if (rank > m) break;
        cout << rank++ << ". " << item.first << " (" << item.second << "x dibeli)" << endl;
    }
}

void Store::listMostActiveBuyers() const {
    cout << "\n--- Pembeli Paling Aktif (berdasarkan jumlah transaksi) ---" << endl;
    map<string, int> buyerCounts;
    for (const auto& trx : transactions) {
        buyerCounts[trx.getBuyerId()]++;
    }

    if (buyerCounts.empty()) { cout << "Belum ada transaksi." << endl; return; }

    auto activeBuyer = max_element(buyerCounts.begin(), buyerCounts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    cout << "Pembeli paling aktif adalah ID: " << activeBuyer->first 
         << " dengan " << activeBuyer->second << " total transaksi." << endl;
}

void Store::listMostActiveSellers() const {
    cout << "\n--- Penjual Paling Aktif (berdasarkan jumlah transaksi) ---" << endl;
    map<string, int> sellerCounts;
    for (const auto& trx : transactions) {
        sellerCounts[trx.getSellerId()]++;
    }

    if (sellerCounts.empty()) { cout << "Belum ada transaksi." << endl; return; }

    auto activeSeller = max_element(sellerCounts.begin(), sellerCounts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    cout << "Penjual paling aktif adalah ID: " << activeSeller->first 
         << " dengan " << activeSeller->second << " total transaksi." << endl;
}

// serializaiton
void Store::saveData() {
    try {
        ofstream ofs("data/store_data.boost");
        if (ofs) {
            boost::archive::text_oarchive oa(ofs);
            oa << buyers;
            oa << sellers;
            oa << transactions;
            cout << "Data berhasil disimpan dengan Boost." << endl;
        }
    } catch (const exception& e) {
        cerr << "Error saat menyimpan data: " << e.what() << endl;
    }
}

void Store::loadData() {
    try {
        ifstream ifs("data/store_data.boost");
        if (ifs) {
            boost::archive::text_iarchive ia(ifs);
            ia >> buyers;
            ia >> sellers;
            ia >> transactions;
            cout << "Data berhasil dimuat dengan Boost." << endl;
        } else {
            cout << "Info: File data tidak ditemukan. Memulai sesi baru." << endl;
        }
    } catch (const exception& e) {
        cerr << "Error saat memuat data: " << e.what() << ". Memulai sesi baru." << endl;
        buyers.clear();
        sellers.clear();
        transactions.clear();
    }
}