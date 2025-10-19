#include "../header/transaction.h"
#include <numeric>
#include <random>
#include <string>

using namespace std;

Transaction::Transaction(const string& buyerId, const string& sellerId, const vector<Item>& purchasedItems)
    : buyerId(buyerId),
      sellerId(sellerId),
      items(purchasedItems),
      status(OrderStatus::PAID)
{
    // total harga
    this->totalPrice = accumulate(items.begin(), items.end(), 0.0,
        [](double sum, const Item& currentItem) {
            return sum + currentItem.getPrice();
        });

    // waktu transaksi
    this->transactionDate = time(nullptr);

    // id transaksi unik
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100000, 999999);
    this->transactionId = "TRX" + to_string(distrib(gen));
}

string Transaction::getTransactionId() const {
    return transactionId;
}

string Transaction::getBuyerId() const {
    return buyerId;
}

string Transaction::getSellerId() const {
    return sellerId;
}

const vector<Item>& Transaction::getItems() const {
    return items;
}

double Transaction::getTotalPrice() const {
    return totalPrice;
}

time_t Transaction::getDate() const {
    return transactionDate;
}

OrderStatus Transaction::getStatus() const {
    return status;
}

void Transaction::setStatus(OrderStatus newStatus) {
    this->status = newStatus;
}