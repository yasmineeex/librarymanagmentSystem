Library Management System
Overview
This Library Management System is a comprehensive C++ application designed to manage library resources, users, loans, and reservations. The system provides functionalities for librarians to efficiently track library operations and for users to access library services.

Features:
------Resource Management----------
Add, edit, and remove various types of resources (Books, Articles, Theses, Digital Content)

Search resources by keyword or category

View detailed information about all resources

User Management
Add new users (Students, Faculty, Staff)

View all registered users

Borrowing System
Borrow available resources

Return borrowed resources

Renew loans (if no reservations exist)

View borrowing history

Reservation System
Reserve currently borrowed resources

View active reservations

Automatic notifications when reserved items become available

Notifications
View recent system notifications

Automatic overdue item detection


Main Menu:
The system presents a main menu with options for different modules

Navigation:
Use the numbered options to navigate through the system

Data Entry:
Follow on-screen prompts to enter required information

System Architecture
--------Key Classes--------
Resource: Base class for all library resources

Book

Article

Thesis

DigitalContent

User: Represents library users

Loan: Tracks resource borrowing

Reservation: Manages resource reservations

Notification: Handles system notifications

LibraryManagementSystem: Main system controller

Data Management
All data is stored in memory during runtime

Testing
The system includes a comprehensive main function that allows testing all features through an interactive menu. See the test cases in the main function documentation for verification procedures.

Future Enhancements
Implement data persistence (file storage/database)

Add user authentication system

Enhance search functionality with filters

Add reporting features

Implement fine calculation for overdue items

Add bulk import/export capabilities

Authors:
DAHAOUI YASMINE

Built With:
C++ programing language 

License
This project is open-source and available for educational purposes.
/////////////////////////////////////////////