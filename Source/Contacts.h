#ifndef CONTACTS_H
#define CONTACTS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>

class Contact
{
public:
    // Default constructor
    Contact()
        : name(""), bio(""), number(""), email(""), state("Offline") {}

    // Constructor
    Contact(const std::string& name, const std::string& bio, const std::string& number, const std::string& email, std::string& state);

    static void saveContacts(const std::vector<Contact>& contacts, const std::string& filePath);

    // Getters
    std::string getName() const;
    std::string getBio() const;
    std::string getNumber() const;
    std::string getEmail() const;
    std::string getState() const;

    // Setters
    void setState(const std::string& newState);

    // Static Methods
    static std::vector<Contact> loadContacts(const std::string& filePath);
    static Contact selectRandomOfflineClient(std::vector<Contact>& contacts);

private:
    std::string name;
    std::string bio;
    std::string number;
    std::string email;
    std::string state;
};

#endif // CONTACTS_H