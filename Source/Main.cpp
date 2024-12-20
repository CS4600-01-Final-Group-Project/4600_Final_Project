#include "Main.h"

std::vector<Contact> contacts;
Contact clientContact;
Contact* otherContact;
std::string onlineContact;
TextBox textBox(363, 910, 595, 50);

bool  contactSelected = false;

int main()
{
    // Loads in example Contact information from a File, Randomly selects an non online
    // contact and marks it as online in the file and as a client in the running program.
    // The Contacts.txt is visible to both instances of the program for simplicity.

    contacts = Contact::loadContacts("Source/Contacts.txt");
    clientContact = Contact::selectRandomOfflineClient(contacts);

    Contact::saveContacts(contacts, "Source/Contacts.txt");
    clientContact.setState("Client");

    onlineContact = Contact::getOnlineContact(contacts, clientContact.getName());

    otherContact = Contact::findContactByName(contacts, onlineContact);

    // Once the "User" is selected the program may begin.
    cleanCache();
    SFMLApp app;
    app.run();
    return 0;
}

// Initilize SFML Object
SFMLApp::SFMLApp() : window(sf::VideoMode(960, 960), "Secure Messager", sf::Style::Titlebar | sf::Style::Close)
{

   window.setFramerateLimit(60);
}

// Main Application Loop
void SFMLApp::run()
{

    while (window.isOpen())
    {

        update();           // Possibly not needed
        render();           // Render to the screen
        processEvents();    // Handle clicks and keyboard inputs
    }
}

// SFMLApp Destructor
SFMLApp::~SFMLApp()
{

    // SFML's textures and sprites automatically clean up when the object goes out of scope.

    
    // Update the state of the chosen client in the contacts list
    for (auto& contact : contacts)
    {
        // Match the chosen client by unique attributes (e.g., name and email)
        if (contact.getName() == clientContact.getName() &&
            contact.getEmail() == clientContact.getEmail())
        {
            contact.setState("Offline");
            break; // Found and updated, no need to check further
        }
    }

    // Save the updated list of contacts back to the file
    Contact::saveContacts(contacts, "Source/Contacts.txt");
    cleanCache();
    std::cout << "Client contact " << clientContact.getName() << " set to Offline." << std::endl;

}

void cleanCache()
{
    // Erase the contents of the file by opening it in truncation mode
    std::ofstream outFile2("Source/MessageCache.txt", std::ios::trunc);
    if (outFile2.is_open())
    {
        // File is successfully cleared
        std::cout << "MessageCache.txt has been cleared.\n";
        outFile2.close();
    }
    else
    {
        // Handle the error if the file could not be opened
        std::cerr << "Error: Unable to open MessageCache.txt for clearing.\n";
    }
}