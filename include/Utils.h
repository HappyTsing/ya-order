#ifndef YA_ORDER_UTILS_H
#define YA_ORDER_UTILS_H

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

void printStudentClientHelpMessage() {
    cout
            << "***************************************************************************************************************************"
            << endl << endl
            << "                                           Welcome to Student Client" << endl
            << "                                           Author: happytsing" << endl << endl
            << "***************************************************************************************************************************"
            << endl
            << "  help                                                                                  print help message"
            << endl
            << "  exit                                                                                  exit program"
            << endl
            << "  show                                                                                  show content"
            << endl
            << "  buy <ItemID>                                                                          buy" << endl
            << "***************************************************************************************************************************"
            << endl;
}

void printStoreClientHelpMessage() {
    cout
            << "***************************************************************************************************************************"
            << endl << endl
            << "                                           Welcome to Store Client" << endl
            << "                                           Author: happytsing" << endl << endl
            << "***************************************************************************************************************************"
            << endl
            << "  help                                                                                  print help message"
            << endl
            << "  exit                                                                                  exit program"
            << endl
            << "  show                                                                                  show content"
            << endl
            << "  add <ItemID> <ItemPrice> <ItemAmount>                                                 add" << endl
            << "  update <ItemID> <ItemID> <ItemPrice> <ItemAmount>                                     buy" << endl
            << "  remove <ItemID>                                                                       remove" << endl
            << "***************************************************************************************************************************"
            << endl;
}


void printMainHelpMessage() {
    cout
            << "***************************************************************************************************************************"
            << endl << endl
            << "                                           Welcome to ya-order" << endl
            << "                                           Author: happytsing" << endl << endl
            << "***************************************************************************************************************************"
            << endl
            << "  start server 1" << endl
            << "  start server 2" << endl
            << "  start student client 1" << endl
            << "  start student client 2" << endl
            << "  start store client 1" << endl
            << "  start store client 2" << endl
            << "***************************************************************************************************************************"
            << endl;
}


void printExitMessage() {
    cout << "bye!" << endl;
}

void printLineHeader() {
    cout << "wangleqing > ";
}

void printErrorCommandMessage(const char *command) {
    string errorCommand = command;
    cout << "「" + errorCommand + "」is not a correct command. See 「help」." << endl;
}

vector<string> lexer(char *command) {
    vector<string> items;
    char splitPattern = ' ';
    char *item = strtok(command, &splitPattern);
    while (item != nullptr) {
        items.push_back(item);
        item = strtok(NULL, &splitPattern);
    }
    return items;
}


void printSplit() {
    cout << "+-------------+-------------+-------------+" << endl;
}

void printTableHeader() {
    cout << "|     id      |    price    |    amount   |" << endl;
}


void printItem(Item item) {
    std::cout << "| ";
    vector<int> record;
    record.push_back(item.id);
    record.push_back(item.price);
    record.push_back(item.amount);
    for (int64_t columnValue: record) {
        cout << columnValue;
        int spaceNum = to_string(((int64_t) 100000000000 / columnValue)).length();

        bool flag = true;
        for (int i = 0; i < to_string(columnValue).length(); i++) {
            int j = stoi(to_string(columnValue).substr(i, i + 1));
            if (i == 0 && j == 1) {
                continue;
            }
            if (i != 0 && j == 0) {
                continue;
            }
            flag = false;
        }

        if (flag) {
            spaceNum -= 1;
        }

        for (int j = 0; j < spaceNum; j++) {
            cout << " ";
        }
        cout << "| ";
    }
    std::cout << std::endl;
}

#endif //YA_ORDER_UTILS_H
