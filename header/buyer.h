#ifndef BUYER_H
#define BUYER_H

#include <string>
#include "BankCustomer.h"
#include <boost/serialization/string.hpp>

using namespace std;

class Buyer {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & userId;
        ar & username;
        ar & password;
        ar & loggedIn;
        ar & bankAccount;
    }

protected:
    string userId;
    string username;
    string password;
    bool loggedIn;
    BankCustomer bankAccount;

public:
    Buyer() {}
    Buyer(const string& username, const string& password);
    Buyer(const string& userId, const string& username, const string& password, double balance);

    string getUserId() const;
    string getUsername() const;
    string getPassword() const;
    
    bool checkPassword(const string& pass) const;
    bool login(const string& pass);
    void logout();
    bool isLoggedIn() const;

    BankCustomer& getBankAccount();

    void topUp(double amount);
    void withdraw(double amount);
    void showCashFlowToday() const;
    void showCashFlowMonth() const;
    
    void purchaseItem(double price);
    void checkSpending(int k_days) const;
};

#endif