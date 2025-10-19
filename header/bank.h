#ifndef BANK_H
#define BANK_H

#include <vector>
#include "bankCustomer.h"

class Bank {
public:
    static void listAllCustomers(const std::vector<BankCustomer>& allCustomers);
    static void listDormantAccounts(const std::vector<BankCustomer>& allCustomers);
    static void listTopUsersToday(const std::vector<BankCustomer>& allCustomers, int n);
    static void listWeeklyTransactions(const BankCustomer& customer);
};

#endif