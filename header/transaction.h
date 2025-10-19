#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <ctime>
#include "item.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

enum class OrderStatus {
    PAID,
    COMPLETED,
    CANCELLED
};

class Transaction {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & transactionId;
        ar & buyerId;
        ar & sellerId;
        ar & items;
        ar & totalPrice;
        ar & transactionDate;
        ar & status;
    }

    string transactionId;
    string buyerId;
    string sellerId;
    vector<Item> items;
    double totalPrice;
    time_t transactionDate;
    OrderStatus status;

public:
    Transaction() {}
    Transaction(const string& buyerId, const string& sellerId, const vector<Item>& purchasedItems);
    
    string getTransactionId() const;
    string getBuyerId() const;
    string getSellerId() const;
    const vector<Item>& getItems() const;
    double getTotalPrice() const;
    time_t getDate() const;
    OrderStatus getStatus() const;

    void setStatus(OrderStatus newStatus);
};

#endif