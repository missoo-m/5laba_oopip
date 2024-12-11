#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <locale.h>

class RealEstateTransaction
{
public:
    int id;
    std::string type;
    std::string propertyAddress;
    double price;
    std::string date;

    RealEstateTransaction(int i, const std::string& t, const std::string& addr, double p, const std::string& d)
        : id(i), type(t), propertyAddress(addr), price(p), date(d) {}

    void display() const
    {
        std::cout << "ID: " << id << ", Тип: " << type
            << ", Адрес: " << propertyAddress
            << ", Цена: " << price
            << ", Дата: " << date << "\n";
    }

    bool operator<(const RealEstateTransaction& other) const// для проперти очереди
    {
        return price < other.price;
    }
};


void addTransaction(std::queue<RealEstateTransaction>& transactions,
    std::priority_queue<RealEstateTransaction>& priorityTransactions,
    std::stack<RealEstateTransaction>& history,
    int id, const std::string& type, const std::string& address, double price, const std::string& date)
{
    RealEstateTransaction newTransaction(id, type, address, price, date);
    transactions.push(newTransaction);
    priorityTransactions.push(newTransaction);
    history.push(newTransaction); 
    std::cout << "Сделка добавлена.\n";
}


void removeTransactionById(std::queue<RealEstateTransaction>& transactions,
    std::stack<RealEstateTransaction>& history, int id)
{
    std::queue<RealEstateTransaction> tempQueue;
    bool found = false;

    while (!transactions.empty())
    {
        RealEstateTransaction transaction = transactions.front();
        transactions.pop();

        if (transaction.id == id)
        {
            found = true;
            history.push(transaction); 
            std::cout << "Сделка с ID " << id << " удалена.\n";
            continue;
        }

        tempQueue.push(transaction);
    }

    if (!found)
    {
        std::cout << "Сделка с ID " << id << " не найдена.\n";
    }

    transactions = tempQueue;
}


void editTransaction(std::queue<RealEstateTransaction>& transactions,
    std::stack<RealEstateTransaction>& history, int id,
    const std::string& newType, const std::string& newAddress, double newPrice, const std::string& newDate)
{
    std::queue<RealEstateTransaction> tempQueue;
    bool found = false;

    while (!transactions.empty())
    {
        RealEstateTransaction transaction = transactions.front();
        transactions.pop();

        if (transaction.id == id)
        {
            found = true;
            history.push(transaction); 
            transaction.type = newType;
            transaction.propertyAddress = newAddress;
            transaction.price = newPrice;
            transaction.date = newDate;
            std::cout << "Сделка с ID " << id << " обновлена.\n";
        }

        tempQueue.push(transaction);
    }

    if (!found)
    {
        std::cout << "Сделка с ID " << id << " не найдена.\n";
    }

    transactions = tempQueue;
}


void undoLastOperation(std::queue<RealEstateTransaction>& transactions, std::stack<RealEstateTransaction>& history)
{
    if (history.empty())
    {
        std::cout << "Нет операций для отмены.\n";
        return;
    }

    RealEstateTransaction lastTransaction = history.top();
    history.pop();

    
    std::queue<RealEstateTransaction> tempQueue;
    bool restored = false;

    while (!transactions.empty())
    {
        RealEstateTransaction transaction = transactions.front();
        transactions.pop();

        if (transaction.id == lastTransaction.id && !restored)
        {
            tempQueue.push(lastTransaction); 
            restored = true;
            std::cout << "Отмена операции с ID: " << lastTransaction.id << "\n";
        }
        else
        {
            tempQueue.push(transaction);
        }
    }

    if (!restored)
    {
        tempQueue.push(lastTransaction); 
    }

    transactions = tempQueue;
}


void displayMostExpensiveTransaction(const std::priority_queue<RealEstateTransaction>& priorityTransactions)
{
    if (priorityTransactions.empty())
    {
        std::cout << "Нет доступных сделок.\n";
        return;
    }
    std::cout << "Самая дорогая сделка:\n";
    priorityTransactions.top().display();
}


void displayTransactions(const std::queue<RealEstateTransaction>& transactions)
{
    std::queue<RealEstateTransaction> tempQueue = transactions;
    while (!tempQueue.empty()) {
        tempQueue.front().display();
        tempQueue.pop();
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    std::queue<RealEstateTransaction> transactions;
    std::stack<RealEstateTransaction> history;
    std::priority_queue<RealEstateTransaction> priorityTransactions;

    int choice;

    while (true)
    {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить сделку\n";
        std::cout << "2. Удалить сделку по ID\n";
        std::cout << "3. Редактировать сделку\n";
        std::cout << "4. Отменить последнюю операцию\n";
        std::cout << "5. Показать самую дорогую сделку\n";
        std::cout << "6. Вывести все сделки\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        if (choice == 0) break;

        int id;
        std::string type, address, date;
        double price;

        switch (choice)
        {
        case 1:
            std::cout << "Введите ID: "; std::cin >> id;
            std::cout << "Тип сделки (buy/sell): "; std::cin >> type;
            std::cout << "Адрес недвижимости: "; std::cin >> address;
            std::cout << "Цена: "; std::cin >> price;
            std::cout << "Дата: "; std::cin >> date;
            addTransaction(transactions, priorityTransactions, history, id, type, address, price, date);
            break;

        case 2:
            std::cout << "Введите ID сделки для удаления: ";
            std::cin >> id;
            removeTransactionById(transactions, history, id);
            break;

        case 3:
            std::cout << "Введите ID сделки для редактирования: "; std::cin >> id;
            std::cout << "Новый тип (buy/sell): "; std::cin >> type;
            std::cout << "Новый адрес: "; std::cin >> address;
            std::cout << "Новая цена: "; std::cin >> price;
            std::cout << "Новая дата: "; std::cin >> date;
            editTransaction(transactions, history, id, type, address, price, date);
            break;

        case 4:
            undoLastOperation(transactions, history);
            break;

        case 5:
            displayMostExpensiveTransaction(priorityTransactions);
            break;

        case 6:
            displayTransactions(transactions);
            break;

        default:
            std::cout << "Неверный ввод. Попробуйте снова.\n";
            break;
        }
    }

    return 0;
}
