#include "../header/item.h"
#include <random>
#include <string>
#include <iostream>
using namespace std;

// Constructor
Item::Item(const string& sellerId, const string& name, double price, int stock)
    : sellerId(sellerId),
      name(name),
      price(price >= 0 ? price : 0),
      stock(stock >= 0 ? stock : 0)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10000, 99999);
    this->itemId = "ITM" + to_string(distrib(gen));
}

// Getter
string Item::getItemId() const { return itemId; }
string Item::getSellerId() const { return sellerId; }
string Item::getName() const { return name; }
double Item::getPrice() const { return price; }
int Item::getStock() const { return stock; }

// Setter
void Item::setPrice(double newPrice) {
    if (newPrice >= 0) {
        price = newPrice;
        cout << "Harga item '" << name << "' diperbarui menjadi " << price << endl;
    } else {
        cout << "Harga tidak boleh bernilai negatif.\n";
    }
}

void Item::setStock(int newStock) {
    if (newStock >= 0) {
        stock = newStock;
        cout << "Stok item '" << name << "' diperbarui menjadi " << stock << endl;
    } else {
        cout << "Stok tidak boleh bernilai negatif.\n";
    }
}
