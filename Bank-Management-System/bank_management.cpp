/*
 * ============================================================
 *        BANK MANAGEMENT APPLICATION
 * ============================================================
 * Language : C++
 * Concepts : OOP, File Handling, Input Validation
 * Storage  : Text file (accounts.txt)
 * Author   : Thiranex
 * ============================================================
 */

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ──────────────────── UTILITY FUNCTIONS ────────────────────

// Clear the console screen
void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Pause and wait for user
void pauseScreen() {
  cout << "\n  Press Enter to continue...";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();
}

// Print a decorative header
void printHeader(const string &title) {
  cout << "\n";
  cout << "  +================================================+\n";
  cout << "  |";
  int pad = (48 - (int)title.length()) / 2;
  for (int i = 0; i < pad; i++)
    cout << " ";
  cout << title;
  for (int i = 0; i < 48 - pad - (int)title.length(); i++)
    cout << " ";
  cout << "|\n";
  cout << "  +================================================+\n\n";
}

// Print a separator line
void printLine() {
  cout << "  --------------------------------------------------\n";
}

// Validate that a string contains only digits
bool isNumeric(const string &s) {
  if (s.empty())
    return false;
  for (char c : s) {
    if (!isdigit(c))
      return false;
  }
  return true;
}

// Validate that a string contains only letters and spaces
bool isAlphaString(const string &s) {
  if (s.empty())
    return false;
  for (char c : s) {
    if (!isalpha(c) && c != ' ')
      return false;
  }
  return true;
}

// Validate phone number (10 digits and starts with 6,7,8,9)
bool isValidPhone(const string &phone) {
  if (phone.length() != 10 || !isNumeric(phone))
    return false;
  char first = phone[0];
  return (first == '6' || first == '7' || first == '8' || first == '9');
}

// Get current date-time as string
string getCurrentDateTime() {
  time_t now = time(0);
  struct tm *tstruct = localtime(&now);
  char buf[80];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tstruct);
  return string(buf);
}

// Safe integer input
int getIntInput(const string &prompt) {
  int value;
  while (true) {
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "  [!] Invalid input. Please enter a number.\n";
    } else {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return value;
    }
  }
}

// Safe double input
double getDoubleInput(const string &prompt) {
  double value;
  while (true) {
    cout << prompt;
    cin >> value;
    if (cin.fail() || value < 0) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "  [!] Invalid input. Please enter a valid amount.\n";
    } else {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return value;
    }
  }
}

// ──────────────────── TRANSACTION CLASS ────────────────────

class Transaction {
private:
  string dateTime;
  string type; // DEPOSIT, WITHDRAWAL, ACCOUNT_CREATED
  double amount;
  double balanceAfter;

public:
  Transaction() : amount(0), balanceAfter(0) {}

  Transaction(const string &type, double amount, double balAfter)
      : type(type), amount(amount), balanceAfter(balAfter) {
    dateTime = getCurrentDateTime();
  }

  // Getters
  string getDateTime() const { return dateTime; }
  string getType() const { return type; }
  double getAmount() const { return amount; }
  double getBalanceAfter() const { return balanceAfter; }

  // Convert to string for file storage
  string toString() const {
    return dateTime + "|" + type + "|" + to_string(amount) + "|" +
           to_string(balanceAfter);
  }

  // Parse from string
  void fromString(const string &line) {
    stringstream ss(line);
    getline(ss, dateTime, '|');
    getline(ss, type, '|');
    string amtStr, balStr;
    getline(ss, amtStr, '|');
    getline(ss, balStr, '|');
    amount = stod(amtStr);
    balanceAfter = stod(balStr);
  }
};

// ──────────────────── BANK ACCOUNT CLASS ────────────────────

class BankAccount {
private:
  int accountNumber;
  string customerName;
  int age;
  string phoneNumber;
  string address;
  string accountType; // Savings or Current
  double balance;
  string pin; // 4-digit PIN for security
  string createdDate;
  vector<Transaction> history;

public:
  // Default constructor
  BankAccount() : accountNumber(0), age(0), balance(0.0) {}

  // Parameterized constructor
  BankAccount(int accNo, const string &name, int age, const string &phone,
              const string &addr, const string &type, double bal,
              const string &pin)
      : accountNumber(accNo), customerName(name), age(age), phoneNumber(phone),
        address(addr), accountType(type), balance(bal), pin(pin) {
    createdDate = getCurrentDateTime();
    history.push_back(Transaction("ACCOUNT_CREATED", bal, bal));
  }

  // ── Getters ──
  int getAccountNumber() const { return accountNumber; }
  string getCustomerName() const { return customerName; }
  int getAge() const { return age; }
  string getPhoneNumber() const { return phoneNumber; }
  string getAddress() const { return address; }
  string getAccountType() const { return accountType; }
  double getBalance() const { return balance; }
  string getPin() const { return pin; }
  string getCreatedDate() const { return createdDate; }
  vector<Transaction> &getHistory() { return history; }

  // ── Setters ──
  void setPhoneNumber(const string &phone) { phoneNumber = phone; }
  void setAddress(const string &addr) { address = addr; }
  void setAccountType(const string &type) { accountType = type; }
  void setPin(const string &p) { pin = p; }

  // ── Banking Operations ──

  // Deposit money
  bool deposit(double amount) {
    if (amount <= 0) {
      cout << "  [!] Deposit amount must be positive.\n";
      return false;
    }
    balance += amount;
    history.push_back(Transaction("DEPOSIT", amount, balance));
    return true;
  }

  // Withdraw money
  bool withdraw(double amount) {
    if (amount <= 0) {
      cout << "  [!] Withdrawal amount must be positive.\n";
      return false;
    }
    double minBalance = (accountType == "Savings") ? 500.0 : 1000.0;
    if (balance - amount < minBalance) {
      cout << "  [!] Insufficient balance! Minimum balance: Rs." << fixed
           << setprecision(2) << minBalance << "\n";
      return false;
    }
    balance -= amount;
    history.push_back(Transaction("WITHDRAWAL", amount, balance));
    return true;
  }

  // Display account details
  void display() const {
    printLine();
    cout << "  Account Number : " << accountNumber << "\n";
    cout << "  Customer Name  : " << customerName << "\n";
    cout << "  Age            : " << age << "\n";
    cout << "  Phone Number   : " << phoneNumber << "\n";
    cout << "  Address        : " << address << "\n";
    cout << "  Account Type   : " << accountType << "\n";
    cout << "  Balance        : Rs." << fixed << setprecision(2) << balance
         << "\n";
    cout << "  Created On     : " << createdDate << "\n";
    printLine();
  }

  // Display mini statement (last 5 transactions)
  void miniStatement() const {
    cout << "\n  ── Mini Statement (Last 5 Transactions) ──\n\n";
    cout << "  " << left << setw(22) << "Date/Time" << setw(18) << "Type"
         << setw(14) << "Amount"
         << "Balance\n";
    printLine();

    int start = (int)history.size() > 5 ? (int)history.size() - 5 : 0;
    for (int i = start; i < (int)history.size(); i++) {
      cout << "  " << left << setw(22) << history[i].getDateTime() << setw(18)
           << history[i].getType() << "Rs." << right << setw(10) << fixed
           << setprecision(2) << history[i].getAmount() << "  Rs." << setw(10)
           << history[i].getBalanceAfter() << "\n";
    }
    if (history.empty()) {
      cout << "  No transactions found.\n";
    }
    printLine();
  }

  // ── File I/O ──

  // Write account to file stream
  void writeToFile(ofstream &fout) const {
    fout << "---ACCOUNT_START---\n";
    fout << accountNumber << "\n";
    fout << customerName << "\n";
    fout << age << "\n";
    fout << phoneNumber << "\n";
    fout << address << "\n";
    fout << accountType << "\n";
    fout << fixed << setprecision(2) << balance << "\n";
    fout << pin << "\n";
    fout << createdDate << "\n";
    fout << history.size() << "\n";
    for (const auto &t : history) {
      fout << t.toString() << "\n";
    }
    fout << "---ACCOUNT_END---\n";
  }

  // Read account from file stream
  bool readFromFile(ifstream &fin) {
    string line;
    // Read start marker
    if (!getline(fin, line) || line != "---ACCOUNT_START---")
      return false;

    getline(fin, line);
    accountNumber = stoi(line);
    getline(fin, customerName);
    getline(fin, line);
    age = stoi(line);
    getline(fin, phoneNumber);
    getline(fin, address);
    getline(fin, accountType);
    getline(fin, line);
    balance = stod(line);
    getline(fin, pin);
    getline(fin, createdDate);

    getline(fin, line);
    int txnCount = stoi(line);
    history.clear();
    for (int i = 0; i < txnCount; i++) {
      getline(fin, line);
      Transaction t;
      t.fromString(line);
      history.push_back(t);
    }

    // Read end marker
    getline(fin, line);
    return true;
  }
};

// ──────────────────── BANK MANAGER CLASS ────────────────────

class BankManager {
private:
  vector<BankAccount> accounts;
  string dataFile;
  string adminPassword;

  // Load all accounts from file
  void loadFromFile() {
    accounts.clear();
    ifstream fin(dataFile);
    if (!fin.is_open())
      return;

    BankAccount acc;
    while (acc.readFromFile(fin)) {
      accounts.push_back(acc);
      acc = BankAccount(); // reset for next read
    }
    fin.close();
  }

  // Save all accounts to file
  void saveToFile() {
    ofstream fout(dataFile);
    if (!fout.is_open()) {
      cout << "  [!] Error: Could not save data to file.\n";
      return;
    }
    for (const auto &acc : accounts) {
      acc.writeToFile(fout);
    }
    fout.close();
  }

  // Find account index by number (-1 if not found)
  int findAccount(int accNo) {
    for (int i = 0; i < (int)accounts.size(); i++) {
      if (accounts[i].getAccountNumber() == accNo)
        return i;
    }
    return -1;
  }

  // Verify PIN for an account
  bool verifyPin(int idx) {
    string enteredPin;
    cout << "  Enter 4-digit PIN: ";
    getline(cin, enteredPin);
    if (enteredPin != accounts[idx].getPin()) {
      cout << "  [!] Incorrect PIN. Access denied.\n";
      return false;
    }
    return true;
  }

  // Generate next account number
  int generateAccountNumber() {
    int maxNo = 1000;
    for (const auto &acc : accounts) {
      if (acc.getAccountNumber() >= maxNo)
        maxNo = acc.getAccountNumber() + 1;
    }
    return maxNo;
  }

public:
  BankManager(const string &file = "accounts.txt")
      : dataFile(file), adminPassword("admin123") {
    loadFromFile();
  }

  ~BankManager() { saveToFile(); }

  // ── 1. Create Account ──
  void createAccount() {
    printHeader("CREATE NEW ACCOUNT");

    int accNo = generateAccountNumber();
    cout << "  Auto-assigned Account Number: " << accNo << "\n\n";

    // Customer Name
    string name;
    while (true) {
      cout << "  Enter Customer Name : ";
      getline(cin, name);
      if (isAlphaString(name) && name.length() >= 2)
        break;
      cout << "  [!] Name must contain only letters (min 2 chars).\n";
    }

    // Age
    int age;
    while (true) {
      age = getIntInput("  Enter Age           : ");
      if (age >= 18 && age <= 120)
        break;
      cout << "  [!] Age must be between 18 and 120.\n";
    }

    // Phone
    string phone;
    while (true) {
      cout << "  Enter Phone (10 digits): ";
      getline(cin, phone);
      if (!isNumeric(phone)) {
        cout << "  [!] Phone must contain only digits.\n";
      } else if (phone.empty() || phone[0] < '6' || phone[0] > '9') {
        cout << "  [!] Phone number must start with 6, 7, 8, or 9.\n";
      } else if (phone.length() != 10) {
        cout << "  [!] Phone number must be exactly 10 digits.\n";
      } else {
        break;
      }
    }

    // Address
    string address;
    while (true) {
      cout << "  Enter Address       : ";
      getline(cin, address);
      if (!address.empty())
        break;
      cout << "  [!] Address cannot be empty.\n";
    }

    // Account Type
    string accType;
    while (true) {
      cout << "  Account Type (1-Savings / 2-Current): ";
      string choice;
      getline(cin, choice);
      if (choice == "1") {
        accType = "Savings";
        break;
      }
      if (choice == "2") {
        accType = "Current";
        break;
      }
      cout << "  [!] Please enter 1 or 2.\n";
    }

    // Initial Balance
    double minBal = (accType == "Savings") ? 500.0 : 1000.0;
    double balance;
    while (true) {
      balance = getDoubleInput("  Initial Deposit (Rs.): ");
      if (balance >= minBal)
        break;
      cout << "  [!] Minimum initial deposit for " << accType
           << " account is Rs." << fixed << setprecision(2) << minBal << "\n";
    }

    // PIN
    string pin;
    while (true) {
      cout << "  Set 4-digit PIN     : ";
      getline(cin, pin);
      if (pin.length() == 4 && isNumeric(pin))
        break;
      cout << "  [!] PIN must be exactly 4 digits.\n";
    }

    // Create and save
    BankAccount newAcc(accNo, name, age, phone, address, accType, balance, pin);
    accounts.push_back(newAcc);
    saveToFile();

    cout << "\n  Account created successfully!\n";
    cout << "  Account Number: " << accNo << "\n";
    cout << "  Remember your PIN for future transactions.\n";
  }

  // ── 2. Deposit Money ──
  void depositMoney() {
    printHeader("DEPOSIT MONEY");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;

    double amount = getDoubleInput("  Enter Deposit Amount (Rs.): ");
    if (accounts[idx].deposit(amount)) {
      saveToFile();
      cout << "\n  Rs." << fixed << setprecision(2) << amount
           << " deposited successfully!\n";
      cout << "  New Balance: Rs." << accounts[idx].getBalance() << "\n";
    }
  }

  // ── 3. Withdraw Money ──
  void withdrawMoney() {
    printHeader("WITHDRAW MONEY");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;

    cout << "  Current Balance: Rs." << fixed << setprecision(2)
         << accounts[idx].getBalance() << "\n";
    double amount = getDoubleInput("  Enter Withdrawal Amount (Rs.): ");
    if (accounts[idx].withdraw(amount)) {
      saveToFile();
      cout << "\n  Rs." << fixed << setprecision(2) << amount
           << " withdrawn successfully!\n";
      cout << "  Remaining Balance: Rs." << accounts[idx].getBalance() << "\n";
    }
  }

  // ── 4. Balance Inquiry ──
  void balanceInquiry() {
    printHeader("BALANCE INQUIRY");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;
    accounts[idx].display();
  }

  // ── 5. Search Account ──
  void searchAccount() {
    printHeader("SEARCH ACCOUNT");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    // Public search - shows basic info only
    cout << "\n  Account Found!\n";
    printLine();
    cout << "  Account Number : " << accounts[idx].getAccountNumber() << "\n";
    cout << "  Customer Name  : " << accounts[idx].getCustomerName() << "\n";
    cout << "  Account Type   : " << accounts[idx].getAccountType() << "\n";
    printLine();
  }

  // ── 6. Update Account ──
  void updateAccount() {
    printHeader("UPDATE ACCOUNT");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;

    cout << "\n  Current Details:\n";
    accounts[idx].display();

    cout << "\n  What would you like to update?\n";
    cout << "  [1] Phone Number\n";
    cout << "  [2] Address\n";
    cout << "  [3] Account Type\n";
    cout << "  [4] Change PIN\n";
    cout << "  [0] Cancel\n";

    int choice = getIntInput("\n  Your choice: ");

    switch (choice) {
    case 1: {
      string phone;
      while (true) {
        cout << "  New Phone (10 digits): ";
        getline(cin, phone);
        if (!isNumeric(phone)) {
          cout << "  [!] Phone must contain only digits.\n";
        } else if (phone.empty() || phone[0] < '6' || phone[0] > '9') {
          cout << "  [!] Phone number must start with 6, 7, 8, or 9.\n";
        } else if (phone.length() != 10) {
          cout << "  [!] Phone number must be exactly 10 digits.\n";
        } else {
          break;
        }
      }
      accounts[idx].setPhoneNumber(phone);
      cout << "  Phone number updated.\n";
      break;
    }
    case 2: {
      string addr;
      while (true) {
        cout << "  New Address: ";
        getline(cin, addr);
        if (!addr.empty())
          break;
        cout << "  [!] Address cannot be empty.\n";
      }
      accounts[idx].setAddress(addr);
      cout << "  Address updated.\n";
      break;
    }
    case 3: {
      string type;
      while (true) {
        cout << "  New Account Type (1-Savings / 2-Current): ";
        string ch;
        getline(cin, ch);
        if (ch == "1") {
          type = "Savings";
          break;
        }
        if (ch == "2") {
          type = "Current";
          break;
        }
        cout << "  [!] Please enter 1 or 2.\n";
      }
      accounts[idx].setAccountType(type);
      cout << "  Account type updated.\n";
      break;
    }
    case 4: {
      string newPin;
      while (true) {
        cout << "  New 4-digit PIN: ";
        getline(cin, newPin);
        if (newPin.length() == 4 && isNumeric(newPin))
          break;
        cout << "  [!] PIN must be exactly 4 digits.\n";
      }
      accounts[idx].setPin(newPin);
      cout << "  PIN updated.\n";
      break;
    }
    case 0:
      cout << "  Update cancelled.\n";
      return;
    default:
      cout << "  [!] Invalid choice.\n";
      return;
    }
    saveToFile();
  }

  // ── 7. Delete Account ──
  void deleteAccount() {
    printHeader("DELETE ACCOUNT");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;

    cout << "\n  Account to be deleted:\n";
    accounts[idx].display();

    cout << "\n  Are you sure? (yes/no): ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "yes" || confirm == "YES" || confirm == "y") {
      accounts.erase(accounts.begin() + idx);
      saveToFile();
      cout << "  Account #" << accNo << " deleted successfully.\n";
    } else {
      cout << "  Deletion cancelled.\n";
    }
  }

  // ── 9. Mini Statement ──
  void showMiniStatement() {
    printHeader("MINI STATEMENT");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;
    accounts[idx].miniStatement();
  }

  // ── 10. Interest Calculation ──
  void calculateInterest() {
    printHeader("INTEREST CALCULATOR");

    int accNo = getIntInput("  Enter Account Number: ");
    int idx = findAccount(accNo);
    if (idx == -1) {
      cout << "  [!] Account not found.\n";
      return;
    }

    if (!verifyPin(idx))
      return;

    double rate = (accounts[idx].getAccountType() == "Savings") ? 4.0 : 2.0;
    double bal = accounts[idx].getBalance();
    double interest = bal * rate / 100.0;

    cout << "\n  Account Type    : " << accounts[idx].getAccountType() << "\n";
    cout << "  Current Balance : Rs." << fixed << setprecision(2) << bal
         << "\n";
    cout << "  Interest Rate   : " << rate << "% per annum\n";
    cout << "  Annual Interest : Rs." << interest << "\n";
    cout << "  Balance + Int.  : Rs." << bal + interest << "\n";
  }
};

// ──────────────────── MAIN MENU ────────────────────

void showMainMenu() {
  cout << "\n";
  cout << "  +================================================+\n";
  cout << "  |         BANK MANAGEMENT SYSTEM                 |\n";
  cout << "  +================================================+\n";
  cout << "  |                                                |\n";
  cout << "  |   [1]  Create New Account                      |\n";
  cout << "  |   [2]  Deposit Money                           |\n";
  cout << "  |   [3]  Withdraw Money                          |\n";
  cout << "  |   [4]  Balance Inquiry                         |\n";
  cout << "  |   [5]  Search Account                          |\n";
  cout << "  |   [6]  Update Account Details                  |\n";
  cout << "  |   [7]  Delete Account                          |\n";
  cout << "  |   [8]  Mini Statement                          |\n";
  cout << "  |   [9]  Interest Calculator                     |\n";
  cout << "  |   [0]  Exit                                    |\n";
  cout << "  |                                                |\n";
  cout << "  +================================================+\n";
}

int main() {
  BankManager bank("accounts.txt");

  int choice;
  do {
    clearScreen();
    showMainMenu();
    choice = getIntInput("\n  Enter your choice: ");

    switch (choice) {
    case 1:
      bank.createAccount();
      pauseScreen();
      break;
    case 2:
      bank.depositMoney();
      pauseScreen();
      break;
    case 3:
      bank.withdrawMoney();
      pauseScreen();
      break;
    case 4:
      bank.balanceInquiry();
      pauseScreen();
      break;
    case 5:
      bank.searchAccount();
      pauseScreen();
      break;
    case 6:
      bank.updateAccount();
      pauseScreen();
      break;
    case 7:
      bank.deleteAccount();
      pauseScreen();
      break;
    case 8:
      bank.showMiniStatement();
      pauseScreen();
      break;
    case 9:
      bank.calculateInterest();
      pauseScreen();
      break;
    case 0:
      cout << "\n  Thank you for using Bank Management System!\n";
      cout << "  Goodbye!\n\n";
      break;
    default:
      cout << "  [!] Invalid choice. Please try again.\n";
      pauseScreen();
    }
  } while (choice != 0);

  return 0;
}
