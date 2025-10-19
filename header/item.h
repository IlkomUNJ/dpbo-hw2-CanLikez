#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>

using namespace std;

class Item {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & itemId;
        ar & sellerId;
        ar & name;
        ar & price;
        ar & stock;
    }
    string itemId;
    string sellerId;
    string name;
    double price;
    int stock;

public:
    Item() {}
    Item(const string& sellerId, const string& name, double price, int stock);
    Item(const string& itemId, const string& sellerId, const string& name, double price, int stock);
    
    string getItemId() const;
    string getSellerId() const;
    string getName() const;
    double getPrice() const;
    int getStock() const;

    void setPrice(double newPrice);
    void setStock(int newStock);
};

#endif