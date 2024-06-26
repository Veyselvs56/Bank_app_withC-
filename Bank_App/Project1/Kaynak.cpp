#include <iostream>
#include <string>
#include <vector>
#include <ctime>   // For getting the current date and time

using namespace std;

// Configurations
const int MAX_ATTEMPTS = 3; // Maximum number of login attempts
const int PASSWORD_LENGTH = 6; // Password length
const int ID_LENGTH = 11; // User ID length

// Struct to store transaction details
struct Transaction {
    string recipientID; // Recipient's ID
    double amount; // Amount sent
    string date; // Date of transaction
};

// Bank account class
class BankAccount {
private:
    string userID; // User ID
    string password; // User password
    double balance; // Account balance
    vector<Transaction> transactions; // List of transactions

public:
    // Constructor
    BankAccount(string id, string pw) : userID(id), password(pw), balance(0.0) {}

    // Password validation
    bool validatePassword(string pw) {
        return password == pw;
    }

    // Get current balance
    double getBalance() {
        return balance;
    }

    // Deposit money into the account
    void deposit(double amount) {
        balance += amount;
        cout << "Successfully deposited " << amount << ". New balance: " << balance << endl;
    }

    // Withdraw money from the account
    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Successfully withdrew " << amount << ". New balance: " << balance << endl;
        }
        else {
            cout << "Insufficient balance." << endl;
        }
    }

    // Send money to another account
    void sendMoney(string recipientID, double amount, const string& date) {
        if (amount <= balance) {
            balance -= amount;
            Transaction t = { recipientID, amount, date };
            transactions.push_back(t);
            cout << "Successfully sent " << amount << " to " << recipientID << ". New balance: " << balance << endl;
        }
        else {
            cout << "Insufficient balance." << endl;
        }
    }

    // Show transaction history
    void showTransactions() {
        cout << "Transactions: " << endl;
        for (const auto& t : transactions) {
            cout << "Recipient ID: " << t.recipientID << ", Amount: " << t.amount << ", Date: " << t.date << endl;
        }
    }

    // Get user ID
    string getUserID() {
        return userID;
    }
};

// Function to clear the screen
void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

// Function to get the current date and time using localtime_s
string getCurrentDateTime() {
    time_t now;
    struct tm ltm;
    char buffer[20];

    time(&now);
    localtime_s(&ltm, &now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &ltm);

    return string(buffer);
}

// Function to validate user ID
bool isValidID(const string& id) {
    if (id.length() != ID_LENGTH) return false;
    for (char c : id) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to validate password
bool isValidPassword(const string& pw) {
    if (pw.length() != PASSWORD_LENGTH) return false;
    for (char c : pw) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main() {
    string id, pw;
    int attempts = 0;
    bool authenticated = false;
    BankAccount* account = nullptr;

    // User authentication
    while (attempts < MAX_ATTEMPTS && !authenticated) {
        cout << "Enter User ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> pw;

        if (!isValidID(id)) {
            cout << "Invalid ID. ID must be 11 digits long." << endl;
        }
        else if (!isValidPassword(pw)) {
            cout << "Invalid Password. Password must be 6 digits long." << endl;
        }
        else {
            account = new BankAccount(id, pw);
            authenticated = true;
        }
        attempts++;
    }

    if (!authenticated) {
        cout << "Too many failed attempts. Exiting." << endl;
        return 1;
    }

    int choice;
    while (true) {
        clearScreen();
        cout << "1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Send Money" << endl;
        cout << "4. Show Transactions" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            double amount;
            cout << "Enter amount to deposit: ";
            cin >> amount;
            account->deposit(amount);
            break;
        }
        case 2: {
            double amount;
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            account->withdraw(amount);
            break;
        }
        case 3: {
            string recipientID;
            double amount;
            cout << "Enter recipient ID: ";
            cin >> recipientID;
            cout << "Enter amount to send: ";
            cin >> amount;
            string date = getCurrentDateTime();
            account->sendMoney(recipientID, amount, date);
            break;
        }
        case 4:
            account->showTransactions();
            break;
        case 5:
            cout << "Exiting." << endl;
            delete account;
            return 0;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
    return 0;
}
