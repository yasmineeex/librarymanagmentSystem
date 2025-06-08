#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <ctime>
#include <iomanip>
#include <memory>

// Forward declarations
class Resource;
class User;
class Loan;
class Reservation;

// Date utility class
class Date {
private:
    int day, month, year;

public:
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }

    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    Date addDays(int days) const {
        // Simplified date addition (for demonstration)
        Date newDate = *this;
        newDate.day += days;
        if (newDate.day > 30) {
            newDate.month += newDate.day / 30;
            newDate.day = newDate.day % 30;
            if (newDate.day == 0) newDate.day = 30;
        }
        if (newDate.month > 12) {
            newDate.year += newDate.month / 12;
            newDate.month = newDate.month % 12;
            if (newDate.month == 0) newDate.month = 12;
        }
        return newDate;
    }

    bool isOverdue(const Date& current) const {
        if (year < current.year) return true;
        if (year > current.year) return false;
        if (month < current.month) return true;
        if (month > current.month) return false;
        return day < current.day;
    }

    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        return os << date.toString();
    }
};

// Base Resource class
class Resource {
protected:
    static int nextId;
    int id;
    std::string title;
    std::string author;
    int publicationYear;
    std::string category;
    bool isAvailable;

public:
    Resource(const std::string& t, const std::string& a, int year, const std::string& cat)
        : id(nextId++), title(t), author(a), publicationYear(year), category(cat), isAvailable(true) {}

    virtual ~Resource() = default;

    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    int getPublicationYear() const { return publicationYear; }
    std::string getCategory() const { return category; }
    bool getAvailability() const { return isAvailable; }

    // Setters
    void setTitle(const std::string& t) { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setPublicationYear(int year) { publicationYear = year; }
    void setCategory(const std::string& cat) { category = cat; }
    void setAvailability(bool available) { isAvailable = available; }

    virtual std::string getType() const = 0;
    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Title: " << title << ", Author: " << author
                  << ", Year: " << publicationYear << ", Category: " << category
                  << ", Available: " << (isAvailable ? "Yes" : "No") << std::endl;
    }

    virtual std::string toCSV() const {
        return std::to_string(id) + "," + getType() + "," + title + "," + author + "," +
               std::to_string(publicationYear) + "," + category + "," + (isAvailable ? "1" : "0");
    }
};

int Resource::nextId = 1;

// Derived classes for different resource types
class Book : public Resource {
private:
    std::string isbn;
    int pages;

public:
    Book(const std::string& t, const std::string& a, int year, const std::string& cat, const std::string& i, int p)
        : Resource(t, a, year, cat), isbn(i), pages(p) {}

    std::string getType() const override { return "Book"; }
    std::string getISBN() const { return isbn; }
    int getPages() const { return pages; }

    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "ISBN: " << isbn << ", Pages: " << pages << std::endl;
    }

    std::string toCSV() const override {
        return Resource::toCSV() + "," + isbn + "," + std::to_string(pages);
    }
};

class Article : public Resource {
private:
    std::string journal;
    int volume;

public:
    Article(const std::string& t, const std::string& a, int year, const std::string& cat, const std::string& j, int v)
        : Resource(t, a, year, cat), journal(j), volume(v) {}

    std::string getType() const override { return "Article"; }
    std::string getJournal() const { return journal; }
    int getVolume() const { return volume; }

    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "Journal: " << journal << ", Volume: " << volume << std::endl;
    }

    std::string toCSV() const override {
        return Resource::toCSV() + "," + journal + "," + std::to_string(volume);
    }
};

class Thesis : public Resource {
private:
    std::string degree;
    std::string university;

public:
    Thesis(const std::string& t, const std::string& a, int year, const std::string& cat, const std::string& d, const std::string& u)
        : Resource(t, a, year, cat), degree(d), university(u) {}

    std::string getType() const override { return "Thesis"; }
    std::string getDegree() const { return degree; }
    std::string getUniversity() const { return university; }

    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "Degree: " << degree << ", University: " << university << std::endl;
    }

    std::string toCSV() const override {
        return Resource::toCSV() + "," + degree + "," + university;
    }
};

class DigitalContent : public Resource {
private:
    std::string format;
    double fileSize; // in MB

public:
    DigitalContent(const std::string& t, const std::string& a, int year, const std::string& cat, const std::string& f, double size)
        : Resource(t, a, year, cat), format(f), fileSize(size) {}

    std::string getType() const override { return "Digital"; }
    std::string getFormat() const { return format; }
    double getFileSize() const { return fileSize; }

    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "Format: " << format << ", Size: " << fileSize << " MB" << std::endl;
    }

    std::string toCSV() const override {
        return Resource::toCSV() + "," + format + "," + std::to_string(fileSize);
    }
};

// User class
class User {
private:
    static int nextId;
    int id;
    std::string name;
    std::string email;
    std::string userType; // Student, Faculty, etc.

public:
    User(const std::string& n, const std::string& e, const std::string& type)
        : id(nextId++), name(n), email(e), userType(type) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getUserType() const { return userType; }

    void displayInfo() const {
        std::cout << "User ID: " << id << ", Name: " << name << ", Email: " << email
                  << ", Type: " << userType << std::endl;
    }

    std::string toCSV() const {
        return std::to_string(id) + "," + name + "," + email + "," + userType;
    }
};

int User::nextId = 1;

// Loan class
class Loan {
private:
    static int nextId;
    int id;
    int userId;
    int resourceId;
    Date borrowDate;
    Date dueDate;
    Date returnDate;
    bool isReturned;

public:
    Loan(int uId, int rId, int loanDays = 14)
        : id(nextId++), userId(uId), resourceId(rId), borrowDate(), isReturned(false) {
        dueDate = borrowDate.addDays(loanDays);
    }

    // Getters
    int getId() const { return id; }
    int getUserId() const { return userId; }
    int getResourceId() const { return resourceId; }
    Date getBorrowDate() const { return borrowDate; }
    Date getDueDate() const { return dueDate; }
    bool getIsReturned() const { return isReturned; }

    void returnResource() {
        isReturned = true;
        returnDate = Date();
    }

    bool isOverdue() const {
        if (isReturned) return false;
        return dueDate.isOverdue(Date());
    }

    void extendDueDate(int days) {
        dueDate = dueDate.addDays(days);
    }

    void displayInfo() const {
        std::cout << "Loan ID: " << id << ", User ID: " << userId << ", Resource ID: " << resourceId
                  << ", Borrow Date: " << borrowDate << ", Due Date: " << dueDate
                  << ", Returned: " << (isReturned ? "Yes" : "No");
        if (isOverdue() && !isReturned) std::cout << " (OVERDUE)";
        std::cout << std::endl;
    }

    std::string toCSV() const {
        return std::to_string(id) + "," + std::to_string(userId) + "," + std::to_string(resourceId) + "," +
               borrowDate.toString() + "," + dueDate.toString() + "," + (isReturned ? "1" : "0");
    }
};

int Loan::nextId = 1;

// Reservation class
class Reservation {
private:
    static int nextId;
    int id;
    int userId;
    int resourceId;
    Date reservationDate;
    bool isActive;

public:
    Reservation(int uId, int rId)
        : id(nextId++), userId(uId), resourceId(rId), reservationDate(), isActive(true) {}

    // Getters
    int getId() const { return id; }
    int getUserId() const { return userId; }
    int getResourceId() const { return resourceId; }
    Date getReservationDate() const { return reservationDate; }
    bool getIsActive() const { return isActive; }

    void deactivate() { isActive = false; }

    void displayInfo() const {
        std::cout << "Reservation ID: " << id << ", User ID: " << userId << ", Resource ID: " << resourceId
                  << ", Date: " << reservationDate << ", Active: " << (isActive ? "Yes" : "No") << std::endl;
    }

    std::string toCSV() const {
        return std::to_string(id) + "," + std::to_string(userId) + "," + std::to_string(resourceId) + "," +
               reservationDate.toString() + "," + (isActive ? "1" : "0");
    }
};

int Reservation::nextId = 1;

// Notification class
class Notification {
private:
    std::string message;
    Date date;
    std::string type; // "due", "available", "overdue", "new_acquisition"

public:
    Notification(const std::string& msg, const std::string& t)
        : message(msg), date(), type(t) {}

    void display() const {
        std::cout << "[" << date << "] " << type << ": " << message << std::endl;
    }

    std::string getMessage() const { return message; }
    std::string getType() const { return type; }
};

// Library Management System class
class LibraryManagementSystem {
private:
    std::vector<std::unique_ptr<Resource>> resources;
    std::vector<std::unique_ptr<User>> users;
    std::vector<std::unique_ptr<Loan>> loans;
    std::vector<std::unique_ptr<Reservation>> reservations;
    std::vector<Notification> notifications;
    std::map<std::string, std::string> libraryEvents;

public:
    LibraryManagementSystem() {
        void loadData();
        void initializeLibrarySchedule();
    }

    ~LibraryManagementSystem() {
        void saveData();
    }

    // Resource Management
    void addResource() {
        std::string title, author, category;
        int year, choice;

        std::cout << "\n=== Add New Resource ===" << std::endl;
        std::cout << "1. Book\n2. Article\n3. Thesis\n4. Digital Content" << std::endl;
        std::cout << "Choose resource type: ";
        std::cin >> choice;
        std::cin.ignore();

        std::cout << "Enter title: ";
        std::getline(std::cin, title);
        std::cout << "Enter author: ";
        std::getline(std::cin, author);
        std::cout << "Enter publication year: ";
        std::cin >> year;
        std::cin.ignore();
        std::cout << "Enter category: ";
        std::getline(std::cin, category);

        try {
            switch (choice) {
                case 1: {
                    std::string isbn;
                    int pages;
                    std::cout << "Enter ISBN: ";
                    std::getline(std::cin, isbn);
                    std::cout << "Enter number of pages: ";
                    std::cin >> pages;
                    resources.push_back(std::make_unique<Book>(title, author, year, category, isbn, pages));
                    break;
                }
                case 2: {
                    std::string journal;
                    int volume;
                    std::cout << "Enter journal name: ";
                    std::getline(std::cin, journal);
                    std::cout << "Enter volume: ";
                    std::cin >> volume;
                    resources.push_back(std::make_unique<Article>(title, author, year, category, journal, volume));
                    break;
                }
                case 3: {
                    std::string degree, university;
                    std::cout << "Enter degree: ";
                    std::getline(std::cin, degree);
                    std::cout << "Enter university: ";
                    std::getline(std::cin, university);
                    resources.push_back(std::make_unique<Thesis>(title, author, year, category, degree, university));
                    break;
                }
                case 4: {
                    std::string format;
                    double fileSize;
                    std::cout << "Enter format: ";
                    std::getline(std::cin, format);
                    std::cout << "Enter file size (MB): ";
                    std::cin >> fileSize;
                    resources.push_back(std::make_unique<DigitalContent>(title, author, year, category, format, fileSize));
                    break;
                }
                default:
                    throw std::invalid_argument("Invalid resource type");
            }
            std::cout << "Resource added successfully!" << std::endl;
            notifications.push_back(Notification("New resource added: " + title, "new_acquisition"));
        } catch (const std::exception& e) {
            std::cout << "Error adding resource: " << e.what() << std::endl;
        }
    }

    void editResource() {
        int id;
        std::cout << "Enter resource ID to edit: ";
        std::cin >> id;

        auto it = std::find_if(resources.begin(), resources.end(),
            [id](const std::unique_ptr<Resource>& r) { return r->getId() == id; });

        if (it != resources.end()) {
            std::string newTitle, newAuthor, newCategory;
            int newYear;

            std::cout << "Current resource details:" << std::endl;
            (*it)->displayInfo();

            std::cin.ignore();
            std::cout << "Enter new title (current: " << (*it)->getTitle() << "): ";
            std::getline(std::cin, newTitle);
            if (!newTitle.empty()) (*it)->setTitle(newTitle);

            std::cout << "Enter new author (current: " << (*it)->getAuthor() << "): ";
            std::getline(std::cin, newAuthor);
            if (!newAuthor.empty()) (*it)->setAuthor(newAuthor);

            std::cout << "Enter new publication year (current: " << (*it)->getPublicationYear() << "): ";
            std::string yearInput;
            std::getline(std::cin, yearInput);
            if (!yearInput.empty()) {
                newYear = std::stoi(yearInput);
                (*it)->setPublicationYear(newYear);
            }

            std::cout << "Resource updated successfully!" << std::endl;
        } else {
            std::cout << "Resource not found!" << std::endl;
        }
    }

    void removeResource() {
        int id;
        std::cout << "Enter resource ID to remove: ";
        std::cin >> id;

        auto it = std::find_if(resources.begin(), resources.end(),
            [id](const std::unique_ptr<Resource>& r) { return r->getId() == id; });

        if (it != resources.end()) {
            // Check if resource is currently borrowed
            bool isBorrowed = std::any_of(loans.begin(), loans.end(),
                [id](const std::unique_ptr<Loan>& loan) {
                    return loan->getResourceId() == id && !loan->getIsReturned();
                });

            if (isBorrowed) {
                std::cout << "Cannot remove resource - it is currently borrowed!" << std::endl;
                return;
            }

            std::cout << "Resource removed: " << (*it)->getTitle() << std::endl;
            resources.erase(it);
        } else {
            std::cout << "Resource not found!" << std::endl;
        }
    }

    void searchResources() {
        std::string keyword, category;
        int choice;

        std::cout << "\n=== Search Resources ===" << std::endl;
        std::cout << "1. Search by keyword\n2. Search by category\n3. Show all resources" << std::endl;
        std::cout << "Choose search option: ";
        std::cin >> choice;
        std::cin.ignore();

        std::vector<Resource*> results;

        switch (choice) {
            case 1:
                std::cout << "Enter keyword: ";
                std::getline(std::cin, keyword);
                for (const auto& resource : resources) {
                    if (resource->getTitle().find(keyword) != std::string::npos ||
                        resource->getAuthor().find(keyword) != std::string::npos) {
                        results.push_back(resource.get());
                    }
                }
                break;
            case 2:
                std::cout << "Enter category: ";
                std::getline(std::cin, category);
                for (const auto& resource : resources) {
                    if (resource->getCategory() == category) {
                        results.push_back(resource.get());
                    }
                }
                break;
            case 3:
                for (const auto& resource : resources) {
                    results.push_back(resource.get());
                }
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
                return;
        }

        if (results.empty()) {
            std::cout << "No resources found!" << std::endl;
        } else {
            std::cout << "\nSearch Results (" << results.size() << " found):" << std::endl;
            std::cout << std::string(80, '-') << std::endl;
            for (const auto& resource : results) {
                resource->displayInfo();
                std::cout << std::string(80, '-') << std::endl;
            }
        }
    }

    // User Management
    void addUser() {
        std::string name, email, userType;

        std::cout << "\n=== Add New User ===" << std::endl;
        std::cin.ignore();
        std::cout << "Enter name: ";
        std::getline(std::cin, name);
        std::cout << "Enter email: ";
        std::getline(std::cin, email);
        std::cout << "Enter user type (Student/Faculty/Staff): ";
        std::getline(std::cin, userType);

        users.push_back(std::make_unique<User>(name, email, userType));
        std::cout << "User added successfully! User ID: " << users.back()->getId() << std::endl;
    }

    void viewUsers() {
        if (users.empty()) {
            std::cout << "No users found!" << std::endl;
            return;
        }

        std::cout << "\n=== All Users ===" << std::endl;
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    // Borrowing System
    void borrowResource() {
        int userId, resourceId;

        std::cout << "\n=== Borrow Resource ===" << std::endl;
        std::cout << "Enter user ID: ";
        std::cin >> userId;
        std::cout << "Enter resource ID: ";
        std::cin >> resourceId;

        try {
            // Validate user
            auto userIt = std::find_if(users.begin(), users.end(),
                [userId](const std::unique_ptr<User>& u) { return u->getId() == userId; });
            if (userIt == users.end()) {
                throw std::invalid_argument("User not found");
            }

            // Validate resource
            auto resourceIt = std::find_if(resources.begin(), resources.end(),
                [resourceId](const std::unique_ptr<Resource>& r) { return r->getId() == resourceId; });
            if (resourceIt == resources.end()) {
                throw std::invalid_argument("Resource not found");
            }

            // Check availability
            if (!(*resourceIt)->getAvailability()) {
                throw std::invalid_argument("Resource is not available");
            }

            // Create loan
            loans.push_back(std::make_unique<Loan>(userId, resourceId));
            (*resourceIt)->setAvailability(false);

            std::cout << "Resource borrowed successfully!" << std::endl;
            std::cout << "Due date: " << loans.back()->getDueDate() << std::endl;

            // Add notification
            notifications.push_back(Notification(
                "Resource borrowed: " + (*resourceIt)->getTitle(), "borrow"));

        } catch (const std::exception& e) {
            std::cout << "Error borrowing resource: " << e.what() << std::endl;
        }
    }

    void returnResource() {
        int loanId;
        std::cout << "\n=== Return Resource ===" << std::endl;
        std::cout << "Enter loan ID: ";
        std::cin >> loanId;

        auto loanIt = std::find_if(loans.begin(), loans.end(),
            [loanId](const std::unique_ptr<Loan>& l) { return l->getId() == loanId; });

        if (loanIt != loans.end() && !(*loanIt)->getIsReturned()) {
            (*loanIt)->returnResource();

            // Make resource available
            int resourceId = (*loanIt)->getResourceId();
            auto resourceIt = std::find_if(resources.begin(), resources.end(),
                [resourceId](const std::unique_ptr<Resource>& r) { return r->getId() == resourceId; });
            if (resourceIt != resources.end()) {
                (*resourceIt)->setAvailability(true);
            }

            std::cout << "Resource returned successfully!" << std::endl;

            // Check for reservations
            checkReservations(resourceId);

        } else {
            std::cout << "Loan not found or already returned!" << std::endl;
        }
    }

    void renewResource() {
        int loanId;
        std::cout << "\n=== Renew Resource ===" << std::endl;
        std::cout << "Enter loan ID: ";
        std::cin >> loanId;

        auto loanIt = std::find_if(loans.begin(), loans.end(),
            [loanId](const std::unique_ptr<Loan>& l) { return l->getId() == loanId; });

        if (loanIt != loans.end() && !(*loanIt)->getIsReturned()) {
            // Check if there are reservations for this resource
            int resourceId = (*loanIt)->getResourceId();
            bool hasReservation = std::any_of(reservations.begin(), reservations.end(),
                [resourceId](const std::unique_ptr<Reservation>& r) {
                    return r->getResourceId() == resourceId && r->getIsActive();
                });

            if (hasReservation) {
                std::cout << "Cannot renew - resource has reservations!" << std::endl;
            } else {
                (*loanIt)->extendDueDate(14);
                std::cout << "Resource renewed successfully!" << std::endl;
                std::cout << "New due date: " << (*loanIt)->getDueDate() << std::endl;
            }
        } else {
            std::cout << "Loan not found or already returned!" << std::endl;
        }
    }

    void viewBorrowHistory() {
        int userId;
        std::cout << "Enter user ID: ";
        std::cin >> userId;

        std::cout << "\n=== Borrow History for User " << userId << " ===" << std::endl;
        bool found = false;
        for (const auto& loan : loans) {
            if (loan->getUserId() == userId) {
                loan->displayInfo();
                found = true;
            }
        }

        if (!found) {
            std::cout << "No borrow history found for this user!" << std::endl;
        }
    }

    // Reservation System
    void reserveResource() {
        int userId, resourceId;

        std::cout << "\n=== Reserve Resource ===" << std::endl;
        std::cout << "Enter user ID: ";
        std::cin >> userId;
        std::cout << "Enter resource ID: ";
        std::cin >> resourceId;

        try {
            // Validate user and resource
            auto userIt = std::find_if(users.begin(), users.end(),
                [userId](const std::unique_ptr<User>& u) { return u->getId() == userId; });
            if (userIt == users.end()) {
                throw std::invalid_argument("User not found");
            }

            auto resourceIt = std::find_if(resources.begin(), resources.end(),
                [resourceId](const std::unique_ptr<Resource>& r) { return r->getId() == resourceId; });
            if (resourceIt == resources.end()) {
                throw std::invalid_argument("Resource not found");
            }

            // Check if resource is available
            if ((*resourceIt)->getAvailability()) {
                std::cout << "Resource is available - you can borrow it directly!" << std::endl;
                return;
            }

            // Check if user already has a reservation for this resource
            bool alreadyReserved = std::any_of(reservations.begin(), reservations.end(),
                [userId, resourceId](const std::unique_ptr<Reservation>& r) {
                    return r->getUserId() == userId && r->getResourceId() == resourceId && r->getIsActive();
                });

            if (alreadyReserved) {
                throw std::invalid_argument("You already have a reservation for this resource");
            }

            reservations.push_back(std::make_unique<Reservation>(userId, resourceId));
            std::cout << "Resource reserved successfully!" << std::endl;

        } catch (const std::exception& e) {
            std::cout << "Error reserving resource: " << e.what() << std::endl;
        }
    }

    void viewReservations() {
        int userId;
        std::cout << "Enter user ID: ";
        std::cin >> userId;

        std::cout << "\n=== Reservations for User " << userId << " ===" << std::endl;
        bool found = false;
        for (const auto& reservation : reservations) {
            if (reservation->getUserId() == userId && reservation->getIsActive()) {
                reservation->displayInfo();
                found = true;
            }
        }

        if (!found) {
            std::cout << "No active reservations found for this user!" << std::endl;
        }
    }

    void checkReservations(int resourceId) {
        auto reservationIt = std::find_if(reservations.begin(), reservations.end(),
            [resourceId](const std::unique_ptr<Reservation>& r) {
                return r->getResourceId() == resourceId && r->getIsActive();
            });

        if (reservationIt != reservations.end()) {
            int userId = (*reservationIt)->getUserId();
            auto userIt = std::find_if(users.begin(), users.end(),
                [userId](const std::unique_ptr<User>& u) { return u->getId() == userId; });

            if (userIt != users.end()) {
                std::cout << "Notifying user " << (*userIt)->getName() 
                         << " that reserved resource is now available!" << std::endl;
                notifications.push_back(Notification(
                    "Reserved resource is now available", "available"));
            }
        }
    }

    // Notifications
    void viewNotifications() {
        std::cout << "\n=== Recent Notifications ===" << std::endl;
        if (notifications.empty()) {
            std::cout << "No notifications!" << std::endl;
            return;
        }

        // Show last 10 notifications
        int start = std::max(0, static_cast<int>(notifications.size()) - 10);
        for (int i = start; i < notifications.size(); ++i) {
            notifications[i].display();
        }
    }
    void checkOverdueItems() {
        std::cout << "\n=== Overdue Items ===" << std::endl;
        bool found = false;
        for (const auto& loan : loans) {
            if (loan->isOverdue()) {
                loan->displayInfo();
                found = true;
                
                // Add overdue notification
                int userId = loan->getUserId();
                auto userIt = std::find_if(users.begin(), users.end(),
                    [userId](const std::unique_ptr<User>& u) { 
                        return u->getId() == userId; 
                    });
                
                if (userIt != users.end()) {
                    notifications.push_back(Notification(
                        "Overdue item for " + (*userIt)->getName(), "overdue"));
                }
            }}}};
            // Main function
int main() {
    LibraryManagementSystem library;
    int choice;
    
    std::cout << "=== Library Management System ===" << std::endl;
    
    do {
        std::cout << "\nMain Menu:\n";
        std::cout << "1. Resource Management\n";
        std::cout << "2. User Management\n";
        std::cout << "3. Borrow/Return Operations\n";
        std::cout << "4. Reservation System\n";
        std::cout << "5. View Notifications\n";
        std::cout << "6. Check Overdue Items\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                // Resource Management
                int resourceChoice;
                std::cout << "\nResource Management:\n";
                std::cout << "1. Add Resource\n";
                std::cout << "2. Edit Resource\n";
                std::cout << "3. Remove Resource\n";
                std::cout << "4. Search Resources\n";
                std::cout << "0. Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> resourceChoice;
                
                switch(resourceChoice) {
                    case 1: library.addResource(); break;
                    case 2: library.editResource(); break;
                    case 3: library.removeResource(); break;
                    case 4: library.searchResources(); break;
                    case 0: break;
                    default: std::cout << "Invalid choice!\n";
                }
                break;
            }
            case 2: {
                // User Management
                int userChoice;
                std::cout << "\nUser Management:\n";
                std::cout << "1. Add User\n";
                std::cout << "2. View All Users\n";
                std::cout << "0. Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> userChoice;
                
                switch(userChoice) {
                    case 1: library.addUser(); break;
                    case 2: library.viewUsers(); break;
                    case 0: break;
                    default: std::cout << "Invalid choice!\n";
                }
                break;
            }
            case 3: {
                // Borrow/Return Operations
                int borrowChoice;
                std::cout << "\nBorrow/Return Operations:\n";
                std::cout << "1. Borrow Resource\n";
                std::cout << "2. Return Resource\n";
                std::cout << "3. Renew Resource\n";
                std::cout << "4. View Borrow History\n";
                std::cout << "0. Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> borrowChoice;
                
                switch(borrowChoice) {
                    case 1: library.borrowResource(); break;
                    case 2: library.returnResource(); break;
                    case 3: library.renewResource(); break;
                    case 4: library.viewBorrowHistory(); break;
                    case 0: break;
                    default: std::cout << "Invalid choice!\n";
                }
                break;
            }
            case 4: {
                // Reservation System
                int reserveChoice;
                std::cout << "\nReservation System:\n";
                std::cout << "1. Reserve Resource\n";
                std::cout << "2. View My Reservations\n";
                std::cout << "0. Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> reserveChoice;
                
                switch(reserveChoice) {
                    case 1: library.reserveResource(); break;
                    case 2: library.viewReservations(); break;
                    case 0: break;
                    default: std::cout << "Invalid choice!\n";
                }
                break;
            }
            case 5:
                library.viewNotifications();
                      break;
            case 6:
                library.checkOverdueItems();
                break;
            case 0:
                std::cout << "Exiting system...\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);
    
    return 0;
}
/////////////////////////////////////