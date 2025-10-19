#ifndef SELLER_H
#define SELLER_H

#include "buyer.h"
#include "item.h"
#include "transaction.h"
#include <string>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

using namespace std;

class Seller : public Buyer {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Buyer>(*this);
        ar & itemsForSale;
    }
    vector<Item> itemsForSale;

public:
    Seller() {}
    Seller(const string& username, const string& password);
    Seller(const string& userId, const string& username, const string& password, double balance);
    void manageItems();

    void discoverTopKItems(int k, const vector<Transaction>& allTransactions) const;
    void discoverLoyalCustomer(const vector<Transaction>& allTransactions) const;

    vector<Item>& getItemsForSale();
};

#endif