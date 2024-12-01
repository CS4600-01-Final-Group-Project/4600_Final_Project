#include "Contacts.h"

// Constructor Implementation
Contact::Contact(const std::string& name, const std::string& bio, const std::string& number, const std::string& email, std::string& state)
    : name(name), bio(bio), number(number), email(email), state(state) {}

// Getters
std::string Contact::getName() const { return name; }
std::string Contact::getBio() const { return bio; }
std::string Contact::getNumber() const { return number; }
std::string Contact::getEmail() const { return email; }
std::string Contact::getState() const
{
    return state;
}

// Setters
void Contact::setState(const std::string& newState)
{
    state = newState;  // Directly setting the string state
}

std::vector<Contact> Contact::loadContacts(const std::string& filePath)
{
    std::vector<Contact> contacts;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string name, bio, number, email, stateStr;

        // Read the contact details from the file
        std::getline(stream, name, ',');
        std::getline(stream, bio, ',');
        std::getline(stream, number, ',');
        std::getline(stream, email, ',');
        std::getline(stream, stateStr); // Read state as a string

        // Create the contact with the state as a string
        contacts.emplace_back(name, bio, number, email, stateStr);
    }

    return contacts;
}

void Contact::saveContacts(const std::vector<Contact>& contacts, const std::string& filePath)
{
    std::ofstream file(filePath, std::ios::trunc); // Open file in truncate mode to overwrite

    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for writing.");
    }

    for (const auto& contact : contacts)
    {
        file << contact.getName() << ","
            << contact.getBio() << ","
            << contact.getNumber() << ","
            << contact.getEmail() << ","
            << contact.getState() << "\n"; // Save the state as a string
    }

    file.close();
}

Contact Contact::selectRandomOfflineClient(std::vector<Contact>& contacts)
{
    std::vector<Contact*> offlineClients;

    // Iterate through the contacts and check for "Offline" state
    for (auto& contact : contacts)
    {
        
        // Assuming the state is stored as a string in getState()
        if (contact.getState() == "Offline") // Compare against the string "Offline"
        {
            offlineClients.push_back(&contact);
        }
    }

    if (!offlineClients.empty())
    {
        // Select a random offline client
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, offlineClients.size() - 1);

        Contact* selected = offlineClients[dist(gen)];

        // Update the state of the selected client to "Online"
        selected->setState("Online"); // Change state to "Online"
        return *selected;
    }

    throw std::runtime_error("No offline clients available to select.");
}