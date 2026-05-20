/*
 * ============================================================
 *        LIBRARY MANAGEMENT SYSTEM
 * ============================================================
 * Language : C++
 * Concepts : OOP, File Handling, Input Validation
 * Storage  : Text files (books.txt, members.txt, issues.txt)
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <ctime>
#include <algorithm>
using namespace std;

// ───────────────────── UTILITY ─────────────────────

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printLine(char c = '-', int w = 52) {
    cout << "  ";
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

void printHeader(const string &title) {
    cout << "\n";
    printLine('=');
    int pad = (52 - (int)title.size()) / 2;
    cout << "  " << string(pad, ' ') << title << "\n";
    printLine('=');
    cout << "\n";
}

string currentDateTime() {
    time_t now = time(0);
    tm *t = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", t);
    return string(buf);
}

bool isNumeric(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

bool isAlpha(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!isalpha(c) && c != ' ') return false;
    return true;
}

int getInt(const string &prompt) {
    int v;
    while (true) {
        cout << prompt;
        cin >> v;
        if (!cin.fail()) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Enter a number.\n";
    }
}

string getLine(const string &prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// toLower for case-insensitive search
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// ───────────────────── BOOK CLASS ─────────────────────

class Book {
    int id;
    string title, author;
    int quantity, available;
public:
    Book() : id(0), quantity(0), available(0) {}
    Book(int id, const string &t, const string &a, int qty)
        : id(id), title(t), author(a), quantity(qty), available(qty) {}

    int    getId()        const { return id; }
    string getTitle()     const { return title; }
    string getAuthor()    const { return author; }
    int    getQuantity()  const { return quantity; }
    int    getAvailable() const { return available; }

    void setTitle(const string &t)  { title = t; }
    void setAuthor(const string &a) { author = a; }
    void setQuantity(int q)         { quantity = q; }

    bool issueOne()  { if (available <= 0) return false; available--; return true; }
    bool returnOne() { if (available >= quantity) return false; available++; return true; }

    void display() const {
        printLine();
        cout << "  Book ID    : " << id << "\n";
        cout << "  Title      : " << title << "\n";
        cout << "  Author     : " << author << "\n";
        cout << "  Quantity   : " << quantity << "\n";
        cout << "  Available  : " << available
             << (available == 0 ? "  [UNAVAILABLE]" : "  [AVAILABLE]") << "\n";
        printLine();
    }

    string serialize() const {
        ostringstream ss;
        ss << id << "|" << title << "|" << author << "|"
           << quantity << "|" << available;
        return ss.str();
    }

    bool deserialize(const string &line) {
        istringstream ss(line);
        string tok;
        try {
            getline(ss, tok, '|'); id = stoi(tok);
            getline(ss, title,  '|');
            getline(ss, author, '|');
            getline(ss, tok, '|'); quantity  = stoi(tok);
            getline(ss, tok, '|'); available = stoi(tok);
            return true;
        } catch (...) { return false; }
    }
};

// ───────────────────── MEMBER CLASS ─────────────────────

class Member {
    int id;
    string name, contact;
public:
    Member() : id(0) {}
    Member(int id, const string &n, const string &c)
        : id(id), name(n), contact(c) {}

    int    getId()      const { return id; }
    string getName()    const { return name; }
    string getContact() const { return contact; }

    void setName(const string &n)    { name = n; }
    void setContact(const string &c) { contact = c; }

    void display() const {
        printLine();
        cout << "  Member ID  : " << id << "\n";
        cout << "  Name       : " << name << "\n";
        cout << "  Contact    : " << contact << "\n";
        printLine();
    }

    string serialize() const {
        return to_string(id) + "|" + name + "|" + contact;
    }

    bool deserialize(const string &line) {
        istringstream ss(line);
        string tok;
        try {
            getline(ss, tok, '|');  id = stoi(tok);
            getline(ss, name,    '|');
            getline(ss, contact, '|');
            return true;
        } catch (...) { return false; }
    }
};

// ───────────────────── ISSUE RECORD CLASS ─────────────────────

class IssueRecord {
    int recId, bookId, memberId;
    string issueDate, returnDate;
    bool returned;
public:
    IssueRecord() : recId(0), bookId(0), memberId(0), returned(false) {}
    IssueRecord(int rid, int bid, int mid)
        : recId(rid), bookId(bid), memberId(mid),
          issueDate(currentDateTime()), returned(false) {}

    int    getRecId()     const { return recId; }
    int    getBookId()    const { return bookId; }
    int    getMemberId()  const { return memberId; }
    string getIssueDate() const { return issueDate; }
    string getReturnDate()const { return returnDate; }
    bool   isReturned()   const { return returned; }

    void markReturned() { returned = true; returnDate = currentDateTime(); }

    void display() const {
        printLine();
        cout << "  Record ID  : " << recId << "\n";
        cout << "  Book ID    : " << bookId << "\n";
        cout << "  Member ID  : " << memberId << "\n";
        cout << "  Issue Date : " << issueDate << "\n";
        if (returned)
            cout << "  Return Date: " << returnDate << "\n";
        cout << "  Status     : " << (returned ? "Returned" : "Issued") << "\n";
        printLine();
    }

    string serialize() const {
        ostringstream ss;
        ss << recId << "|" << bookId << "|" << memberId << "|"
           << issueDate << "|" << returnDate << "|" << (returned ? 1 : 0);
        return ss.str();
    }

    bool deserialize(const string &line) {
        istringstream ss(line);
        string tok;
        try {
            getline(ss, tok, '|');  recId    = stoi(tok);
            getline(ss, tok, '|');  bookId   = stoi(tok);
            getline(ss, tok, '|');  memberId = stoi(tok);
            getline(ss, issueDate,  '|');
            getline(ss, returnDate, '|');
            getline(ss, tok, '|');  returned = (tok == "1");
            return true;
        } catch (...) { return false; }
    }
};

// ───────────────────── LIBRARY MANAGER CLASS ─────────────────────

class LibraryManager {
    vector<Book>        books;
    vector<Member>      members;
    vector<IssueRecord> issues;

    const string BOOKS_FILE   = "books.txt";
    const string MEMBERS_FILE = "members.txt";
    const string ISSUES_FILE  = "issues.txt";

    // ── File I/O ──

    void loadBooks() {
        books.clear();
        ifstream f(BOOKS_FILE);
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Book b;
            if (b.deserialize(line)) books.push_back(b);
        }
    }

    void saveBooks() {
        ofstream f(BOOKS_FILE);
        for (auto &b : books) f << b.serialize() << "\n";
    }

    void loadMembers() {
        members.clear();
        ifstream f(MEMBERS_FILE);
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Member m;
            if (m.deserialize(line)) members.push_back(m);
        }
    }

    void saveMembers() {
        ofstream f(MEMBERS_FILE);
        for (auto &m : members) f << m.serialize() << "\n";
    }

    void loadIssues() {
        issues.clear();
        ifstream f(ISSUES_FILE);
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            IssueRecord r;
            if (r.deserialize(line)) issues.push_back(r);
        }
    }

    void saveIssues() {
        ofstream f(ISSUES_FILE);
        for (auto &r : issues) f << r.serialize() << "\n";
    }

    // ── ID Helpers ──

    int nextBookId() {
        int mx = 100;
        for (auto &b : books) if (b.getId() >= mx) mx = b.getId() + 1;
        return mx;
    }

    int nextMemberId() {
        int mx = 200;
        for (auto &m : members) if (m.getId() >= mx) mx = m.getId() + 1;
        return mx;
    }

    int nextIssueId() {
        int mx = 1;
        for (auto &r : issues) if (r.getRecId() >= mx) mx = r.getRecId() + 1;
        return mx;
    }

    int findBook(int id) {
        for (int i = 0; i < (int)books.size(); i++)
            if (books[i].getId() == id) return i;
        return -1;
    }

    int findMember(int id) {
        for (int i = 0; i < (int)members.size(); i++)
            if (members[i].getId() == id) return i;
        return -1;
    }

    // ── Sample Data ──

    void seedData() {
        if (!books.empty() || !members.empty()) return;

        cout << "  Generating sample data...\n";

        books.push_back(Book(100, "The C++ Programming Language", "Bjarne Stroustrup", 3));
        books.push_back(Book(101, "Clean Code", "Robert C. Martin", 2));
        books.push_back(Book(102, "Introduction to Algorithms", "Cormen et al.", 4));
        books.push_back(Book(103, "Design Patterns", "Gang of Four", 2));
        books.push_back(Book(104, "The Pragmatic Programmer", "Hunt and Thomas", 3));
        saveBooks();

        members.push_back(Member(200, "Arun Kumar",  "9876543210"));
        members.push_back(Member(201, "Priya Singh", "9123456789"));
        members.push_back(Member(202, "Ravi Sharma", "9000012345"));
        saveMembers();

        cout << "  Sample books and members added.\n";
    }

public:
    LibraryManager() {
        loadBooks();
        loadMembers();
        loadIssues();
        seedData();
    }

    // ════════════════ BOOK MANAGEMENT ════════════════

    void addBook() {
        printHeader("ADD NEW BOOK");

        int id = nextBookId();
        cout << "  Auto Book ID : " << id << "\n\n";

        string title;
        while (true) {
            title = getLine("  Title        : ");
            if (!title.empty()) break;
            cout << "  [!] Title cannot be empty.\n";
        }

        string author;
        while (true) {
            author = getLine("  Author       : ");
            if (!author.empty()) break;
            cout << "  [!] Author cannot be empty.\n";
        }

        int qty;
        while (true) {
            qty = getInt("  Quantity     : ");
            if (qty > 0) break;
            cout << "  [!] Quantity must be positive.\n";
        }

        books.push_back(Book(id, title, author, qty));
        saveBooks();
        cout << "\n  [✓] Book added successfully! ID = " << id << "\n";
    }

    void displayAllBooks() {
        printHeader("ALL BOOKS");
        if (books.empty()) { cout << "  No books found.\n"; return; }
        for (auto &b : books) b.display();
        cout << "\n  Total: " << books.size() << " book(s).\n";
    }

    void searchBooks() {
        printHeader("SEARCH BOOKS");
        cout << "  [1] Search by Title\n";
        cout << "  [2] Search by Author\n";
        int ch = getInt("\n  Choice: ");

        string kw = getLine("  Keyword: ");
        string kwL = toLower(kw);
        bool found = false;

        for (auto &b : books) {
            bool match = (ch == 1 && toLower(b.getTitle()).find(kwL) != string::npos)
                      || (ch == 2 && toLower(b.getAuthor()).find(kwL) != string::npos);
            if (match) { b.display(); found = true; }
        }
        if (!found) cout << "  [!] No books matched \"" << kw << "\".\n";
    }

    void updateBook() {
        printHeader("UPDATE BOOK");
        int id = getInt("  Enter Book ID: ");
        int idx = findBook(id);
        if (idx == -1) { cout << "  [!] Book not found.\n"; return; }

        cout << "\n  Current Details:\n";
        books[idx].display();

        cout << "  [1] Title\n  [2] Author\n  [3] Quantity\n  [0] Cancel\n";
        int ch = getInt("\n  Choice: ");

        switch (ch) {
        case 1: {
            string t = getLine("  New Title : ");
            if (!t.empty()) { books[idx].setTitle(t); cout << "  [✓] Title updated.\n"; }
            break;
        }
        case 2: {
            string a = getLine("  New Author: ");
            if (!a.empty()) { books[idx].setAuthor(a); cout << "  [✓] Author updated.\n"; }
            break;
        }
        case 3: {
            int q = getInt("  New Quantity: ");
            if (q > 0) { books[idx].setQuantity(q); cout << "  [✓] Quantity updated.\n"; }
            else cout << "  [!] Quantity must be positive.\n";
            break;
        }
        case 0: cout << "  Update cancelled.\n"; return;
        default: cout << "  [!] Invalid choice.\n"; return;
        }
        saveBooks();
    }

    void deleteBook() {
        printHeader("DELETE BOOK");
        int id = getInt("  Enter Book ID: ");
        int idx = findBook(id);
        if (idx == -1) { cout << "  [!] Book not found.\n"; return; }

        // Check active issues
        for (auto &r : issues)
            if (r.getBookId() == id && !r.isReturned()) {
                cout << "  [!] Cannot delete. Book is currently issued.\n";
                return;
            }

        books[idx].display();
        string confirm = getLine("  Confirm delete? (yes/no): ");
        if (confirm == "yes" || confirm == "y") {
            books.erase(books.begin() + idx);
            saveBooks();
            cout << "  [✓] Book deleted.\n";
        } else {
            cout << "  Deletion cancelled.\n";
        }
    }

    // ════════════════ MEMBER MANAGEMENT ════════════════

    void addMember() {
        printHeader("ADD NEW MEMBER");
        int id = nextMemberId();
        cout << "  Auto Member ID : " << id << "\n\n";

        string name;
        while (true) {
            name = getLine("  Name    : ");
            if (!name.empty()) break;
            cout << "  [!] Name cannot be empty.\n";
        }

        string contact;
        while (true) {
            contact = getLine("  Contact : ");
            if (isNumeric(contact) && contact.size() == 10) break;
            cout << "  [!] Contact must be 10 digits.\n";
        }

        members.push_back(Member(id, name, contact));
        saveMembers();
        cout << "\n  [✓] Member added! ID = " << id << "\n";
    }

    void displayAllMembers() {
        printHeader("ALL MEMBERS");
        if (members.empty()) { cout << "  No members found.\n"; return; }
        for (auto &m : members) m.display();
        cout << "\n  Total: " << members.size() << " member(s).\n";
    }

    void searchMember() {
        printHeader("SEARCH MEMBER");
        cout << "  [1] Search by ID\n  [2] Search by Name\n";
        int ch = getInt("\n  Choice: ");
        bool found = false;

        if (ch == 1) {
            int id = getInt("  Member ID: ");
            int idx = findMember(id);
            if (idx != -1) { members[idx].display(); found = true; }
        } else if (ch == 2) {
            string kw = getLine("  Name keyword: ");
            string kwL = toLower(kw);
            for (auto &m : members)
                if (toLower(m.getName()).find(kwL) != string::npos)
                    { m.display(); found = true; }
        }
        if (!found) cout << "  [!] No member found.\n";
    }

    void updateMember() {
        printHeader("UPDATE MEMBER");
        int id = getInt("  Enter Member ID: ");
        int idx = findMember(id);
        if (idx == -1) { cout << "  [!] Member not found.\n"; return; }

        members[idx].display();
        cout << "  [1] Name\n  [2] Contact\n  [0] Cancel\n";
        int ch = getInt("\n  Choice: ");

        switch (ch) {
        case 1: {
            string n = getLine("  New Name   : ");
            if (!n.empty()) { members[idx].setName(n); saveMembers(); cout << "  [✓] Name updated.\n"; }
            break;
        }
        case 2: {
            string c = getLine("  New Contact: ");
            if (isNumeric(c) && c.size() == 10) {
                members[idx].setContact(c); saveMembers();
                cout << "  [✓] Contact updated.\n";
            } else cout << "  [!] Must be 10 digits.\n";
            break;
        }
        case 0: cout << "  Update cancelled.\n"; break;
        default: cout << "  [!] Invalid choice.\n";
        }
    }

    void deleteMember() {
        printHeader("DELETE MEMBER");
        int id = getInt("  Enter Member ID: ");
        int idx = findMember(id);
        if (idx == -1) { cout << "  [!] Member not found.\n"; return; }

        for (auto &r : issues)
            if (r.getMemberId() == id && !r.isReturned()) {
                cout << "  [!] Cannot delete. Member has unreturned books.\n";
                return;
            }

        members[idx].display();
        string confirm = getLine("  Confirm delete? (yes/no): ");
        if (confirm == "yes" || confirm == "y") {
            members.erase(members.begin() + idx);
            saveMembers();
            cout << "  [✓] Member deleted.\n";
        } else {
            cout << "  Deletion cancelled.\n";
        }
    }

    // ════════════════ ISSUE & RETURN ════════════════

    void issueBook() {
        printHeader("ISSUE BOOK");
        int bid = getInt("  Enter Book ID  : ");
        int bidx = findBook(bid);
        if (bidx == -1) { cout << "  [!] Book not found.\n"; return; }

        if (books[bidx].getAvailable() <= 0) {
            cout << "  [!] Book is currently unavailable.\n";
            return;
        }

        int mid = getInt("  Enter Member ID: ");
        int midx = findMember(mid);
        if (midx == -1) { cout << "  [!] Member not found.\n"; return; }

        // Check: member already has this book
        for (auto &r : issues)
            if (r.getBookId() == bid && r.getMemberId() == mid && !r.isReturned()) {
                cout << "  [!] Member already has this book issued.\n";
                return;
            }

        books[bidx].issueOne();
        saveBooks();

        issues.push_back(IssueRecord(nextIssueId(), bid, mid));
        saveIssues();

        cout << "\n  [✓] Book issued successfully!\n";
        cout << "  Book   : " << books[bidx].getTitle() << "\n";
        cout << "  Member : " << members[midx].getName() << "\n";
        cout << "  Date   : " << currentDateTime() << "\n";
    }

    void returnBook() {
        printHeader("RETURN BOOK");
        int bid = getInt("  Enter Book ID  : ");
        int mid = getInt("  Enter Member ID: ");

        int bidx = findBook(bid);
        int midx = findMember(mid);

        if (bidx == -1) { cout << "  [!] Book not found.\n"; return; }
        if (midx == -1) { cout << "  [!] Member not found.\n"; return; }

        bool found = false;
        for (auto &r : issues) {
            if (r.getBookId() == bid && r.getMemberId() == mid && !r.isReturned()) {
                r.markReturned();
                books[bidx].returnOne();
                saveBooks();
                saveIssues();
                cout << "\n  [✓] Book returned successfully!\n";
                cout << "  Book   : " << books[bidx].getTitle() << "\n";
                cout << "  Member : " << members[midx].getName() << "\n";
                cout << "  Date   : " << currentDateTime() << "\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "  [!] No active issue record found for this book/member.\n";
    }

    void viewIssues() {
        printHeader("ISSUE RECORDS");
        if (issues.empty()) { cout << "  No records found.\n"; return; }

        cout << "  [1] All Records\n  [2] Active Issues Only\n  [3] Returned Only\n";
        int ch = getInt("\n  Choice: ");
        int cnt = 0;
        for (auto &r : issues) {
            if (ch == 2 && r.isReturned()) continue;
            if (ch == 3 && !r.isReturned()) continue;
            r.display();
            cnt++;
        }
        cout << "\n  Showing " << cnt << " record(s).\n";
    }

    // ════════════════ MENUS ════════════════

    void bookMenu() {
        while (true) {
            printHeader("BOOK MANAGEMENT");
            cout << "  [1] Add Book\n";
            cout << "  [2] Display All Books\n";
            cout << "  [3] Search Books\n";
            cout << "  [4] Update Book\n";
            cout << "  [5] Delete Book\n";
            cout << "  [0] Back\n";
            int ch = getInt("\n  Choice: ");
            clearScreen();
            switch (ch) {
            case 1: addBook();        break;
            case 2: displayAllBooks();break;
            case 3: searchBooks();    break;
            case 4: updateBook();     break;
            case 5: deleteBook();     break;
            case 0: return;
            default: cout << "  [!] Invalid choice.\n";
            }
            pause();
            clearScreen();
        }
    }

    void memberMenu() {
        while (true) {
            printHeader("MEMBER MANAGEMENT");
            cout << "  [1] Add Member\n";
            cout << "  [2] Display All Members\n";
            cout << "  [3] Search Member\n";
            cout << "  [4] Update Member\n";
            cout << "  [5] Delete Member\n";
            cout << "  [0] Back\n";
            int ch = getInt("\n  Choice: ");
            clearScreen();
            switch (ch) {
            case 1: addMember();        break;
            case 2: displayAllMembers();break;
            case 3: searchMember();     break;
            case 4: updateMember();     break;
            case 5: deleteMember();     break;
            case 0: return;
            default: cout << "  [!] Invalid choice.\n";
            }
            pause();
            clearScreen();
        }
    }

    void issueMenu() {
        while (true) {
            printHeader("ISSUE & RETURN");
            cout << "  [1] Issue Book\n";
            cout << "  [2] Return Book\n";
            cout << "  [3] View Issue Records\n";
            cout << "  [0] Back\n";
            int ch = getInt("\n  Choice: ");
            clearScreen();
            switch (ch) {
            case 1: issueBook();  break;
            case 2: returnBook(); break;
            case 3: viewIssues(); break;
            case 0: return;
            default: cout << "  [!] Invalid choice.\n";
            }
            pause();
            clearScreen();
        }
    }

    void mainMenu() {
        while (true) {
            cout << "\n";
            printLine('=');
            cout << "          LIBRARY MANAGEMENT SYSTEM\n";
            printLine('=');
            cout << "\n";
            cout << "  [1]  Book Management\n";
            cout << "  [2]  Member Management\n";
            cout << "  [3]  Issue & Return Books\n";
            cout << "  [0]  Exit\n";
            printLine();

            int ch = getInt("\n  Your choice: ");
            clearScreen();
            switch (ch) {
            case 1: bookMenu();   break;
            case 2: memberMenu(); break;
            case 3: issueMenu();  break;
            case 0:
                cout << "\n  Thank you for using Library Management System!\n\n";
                return;
            default:
                cout << "  [!] Invalid choice.\n";
                pause();
                clearScreen();
            }
        }
    }
};

// ───────────────────── MAIN ─────────────────────

int main() {
    clearScreen();
    cout << "\n";
    cout << "  ====================================================\n";
    cout << "       LIBRARY MANAGEMENT SYSTEM  v1.0\n";
    cout << "  ====================================================\n";
    cout << "  Loading data...\n";

    LibraryManager lms;

    cout << "  Data loaded successfully!\n";
    pause();
    clearScreen();

    lms.mainMenu();
    return 0;
}
