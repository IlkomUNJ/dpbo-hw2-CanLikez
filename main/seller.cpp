#include "../header/seller.h"
#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

Seller::Seller(const string& username, const string& password)
    : Buyer(username, password) {}
    
void Seller::manageItems() {
    int choice = 0;
    while (choice != 4) {
        cout << "\n--- Manajemen Item untuk " << username << " ---" << endl;
        cout << "1. Registrasi Item Baru" << endl;
        cout << "2. Ubah Stok / Harga Item" << endl;
        cout << "3. Lihat Semua Item Saya" << endl;
        cout << "4. Kembali ke Menu Utama" << endl;
        cout << "Pilihan Anda: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            cout << "Input tidak valid. Harap masukkan angka." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name, itemId;
        double price;
        int stock;

        switch (choice) {
            case 1: {
                cout << "Masukkan Nama Item: ";
                getline(cin, name);
                cout << "Masukkan Harga Item: ";
                cin >> price;
                cout << "Masukkan Stok Awal: ";
                cin >> stock;
                itemsForSale.emplace_back(this->getUserId(), name, price, stock);
                cout << "Item '" << name << "' berhasil diregistrasi!" << endl;
                break;
            }
            case 2: {
                if (itemsForSale.empty()) {
                    cout << "Anda belum memiliki item untuk diubah." << endl;
                    break;
                }
                for (const auto& item : itemsForSale) {
                    cout << "ID: " << item.getItemId() << " | Nama: " << item.getName()
                         << " | Harga: " << item.getPrice() << " | Stok: " << item.getStock() << endl;
                }
                cout << "Masukkan ID Item yang akan diubah: ";
                getline(cin, itemId);
                
                auto it = find_if(itemsForSale.begin(), itemsForSale.end(),
                                  [&](const Item& item) { return item.getItemId() == itemId; });

                if (it != itemsForSale.end()) {
                    cout << "Item ditemukan: " << it->getName() << endl;
                    cout << "Masukkan Stok Baru (masukkan -1 jika tidak ingin mengubah): ";
                    cin >> stock;
                    if (stock != -1) it->setStock(stock);

                    cout << "Masukkan Harga Baru (masukkan -1 jika tidak ingin mengubah): ";
                    cin >> price;
                    if (price != -1) it->setPrice(price);
                    
                    cout << "Item berhasil diperbarui." << endl;
                } else {
                    cout << "Error: Item dengan ID '" << itemId << "' tidak ditemukan." << endl;
                }
                break;
            }
            case 3: {
                cout << "\n--- Daftar Item Milik " << username << " ---" << endl;
                if (itemsForSale.empty()) {
                    cout << "Anda belum memiliki item untuk dijual." << endl;
                } else {
                    for (const auto& item : itemsForSale) {
                        cout << "ID: " << item.getItemId() << " | Nama: " << item.getName()
                             << " | Harga: " << item.getPrice() << " | Stok: " << item.getStock() << endl;
                    }
                }
                break;
            }
        }
    }
}

void Seller::discoverTopKItems(int k, const vector<Transaction>& allTransactions) const {
    cout << "\n--- Top " << k << " Item Terpopuler Anda (Bulan Ini) ---" << endl;
    map<string, int> itemCounts;

    time_t now = time(nullptr);
    tm* localtm = localtime(&now);
    localtm->tm_mday = 1; localtm->tm_hour = 0; localtm->tm_min = 0; localtm->tm_sec = 0;
    time_t startOfMonth = mktime(localtm);

    for (const auto& trx : allTransactions) {
        // PERBAIKAN: Tambahkan pengecekan status di sini
        if (trx.getSellerId() == this->getUserId() && trx.getDate() >= startOfMonth && trx.getStatus() != OrderStatus::CANCELLED) {
            for (const auto& item : trx.getItems()) {
                 itemCounts[item.getName()]++;
            }
        }
    }

    if (itemCounts.empty()) {
        cout << "Belum ada item yang terjual bulan ini." << endl;
        return;
    }

    vector<pair<string, int>> sortedItems(itemCounts.begin(), itemCounts.end());
    sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int rank = 1;
    for (const auto& item : sortedItems) {
        if (rank > k) break;
        cout << rank++ << ". " << item.first << " (" << item.second << " terjual)" << endl;
    }
}

void Seller::discoverLoyalCustomer(const vector<Transaction>& allTransactions) const {
cout << "\n--- Pelanggan Paling Setia Anda (Bulan Ini) ---" << endl;
    map<string, int> customerCounts;

    time_t now = time(nullptr);
    tm* localtm = localtime(&now);
    localtm->tm_mday = 1; localtm->tm_hour = 0; localtm->tm_min = 0; localtm->tm_sec = 0;
    time_t startOfMonth = mktime(localtm);

    for (const auto& trx : allTransactions) {
        if (trx.getSellerId() == this->getUserId() && trx.getDate() >= startOfMonth && trx.getStatus() != OrderStatus::CANCELLED) {
            customerCounts[trx.getBuyerId()]++;
        }
    }

    if (customerCounts.empty()) {
        cout << "Anda belum memiliki transaksi bulan ini." << endl;
        return;
    }

    auto loyalCustomer = max_element(customerCounts.begin(), customerCounts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    cout << "Pelanggan paling setia adalah ID : " << loyalCustomer->first 
         << " dengan " << loyalCustomer->second << " total pembelian." << endl;
}

vector<Item>& Seller::getItemsForSale() {
    return itemsForSale;
}