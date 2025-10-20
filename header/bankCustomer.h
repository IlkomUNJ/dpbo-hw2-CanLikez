#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <vector>
#include <ctime>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

struct BankTransaction {
    double amount;
    string type;
    time_t timestamp;
};

class BankCustomer {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & accountId;
        ar & ownerId;
        ar & balance;
    }
    string accountId;
    string ownerId;
    double balance;
    vector<BankTransaction> transactionHistory;

public:
    BankCustomer() : accountId(""), ownerId(""), balance(0.0) {}
    BankCustomer(const string& ownerId, double initialBalance = 0.0);
    BankCustomer(const string& accountId, const string& ownerId, double balance);

    string getAccountId() const;
    string getOwnerId() const;
    double getBalance() const;
    const vector<BankTransaction>& getTransactionHistory() const;

    void topUp(double amount);
    bool withdraw(double amount);
    void listCashFlowToday() const;
    void listCashFlowMonth() const;
    // void backdateLastTransaction(int days);
};

#endif