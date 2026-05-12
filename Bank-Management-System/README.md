# Bank Management System (C++)

## Project Overview

The Bank Management System is a console-based application developed in C++ using Object-Oriented Programming (OOP) concepts and File Handling.

This project simulates basic banking operations such as:

* Creating bank accounts
* Depositing money
* Withdrawing money
* Checking account balance
* Viewing customer details
* Managing transaction records

The application stores account data permanently using text files, making the records available even after restarting the program.

## Features

* Create New Bank Account
* Deposit Money
* Withdraw Money
* Balance Inquiry
* View Account Details
* Transaction History
* Input Validation
* File Handling for Permanent Storage
* Menu-Driven Console Interface
* Secure and Organized Data Management

## Technologies Used

Language: C++

Concepts Used:

* Object-Oriented Programming (OOP)
* Classes and Objects
* Encapsulation
* File Handling
* Input Validation
* Vectors and Strings

---

## Project Structure

```text
Bank-Management-System
│
├── bank_management.cpp
├── accounts.txt
└── README.md
```
## Functionalities

### 1. Create Account

Allows users to create a new bank account with customer details.

### 2. Deposit Money

Users can deposit money into an existing account.

### 3. Withdraw Money

Allows withdrawal of money while checking sufficient balance.

### 4. Balance Inquiry

Displays the current balance of a customer account.

### 5. Transaction History

Stores and displays transaction records with date and time.

### 6. File Storage

All account details are saved in text files for permanent storage.

## Input Validation

The system validates:

* Numeric inputs
* Phone numbers
* Customer names
* Transaction amounts
* Invalid menu selections

This improves reliability and prevents incorrect data entry.

## File Handling

The project uses:

```cpp
fstream
ifstream
ofstream
```

for reading and writing account information to files.

## Concepts Demonstrated

This project demonstrates:

* Object-Oriented Programming
* Class Design
* Data Encapsulation
* File Management
* Modular Programming
* User Input Validation
* Transaction Management

## Learning Outcomes

After completing this project, students can understand:

* Real-world implementation of OOP concepts
* Working with file handling in C++
* Designing menu-driven applications
* Managing persistent data storage
* Building structured console applications

## Author
Thiranex

Developed as a C++ mini project for learning Object-Oriented Programming and File Handling.

## Repository Purpose

This repository is part of the Thiranex Internship Projects Collection, showcasing beginner-to-intermediate level C++ projects.
