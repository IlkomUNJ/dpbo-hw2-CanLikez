#ifndef STORE_H
#define STORE_H

#include "buyer.h"
#include "seller.h"
#include "transaction.h"
#include <vector>
#include <string>

using namespace std;

class Store {
private:
    vector<Buyer> buyers;
    vector<Seller> sellers;
    vector<Transaction> transactions;

    void showBuyerMenu(Buyer& buyer);
    void showSellerMenu(Seller& seller);

    void saveData();
    void loadData();
public:
    Store();

    void run();

    void registerUser();
    void login();

    void displayAllItems();
    void purchaseItem(Buyer& activeBuyer);
    void manageSellerOrders(Seller& seller);

    void listLatestTransactions(int k_days) const;
    void listPaidButUncompleted() const;
    void listMostFrequentItems(int m) const;
    void listMostActiveBuyers() const;
    void listMostActiveSellers() const;
};

#endif