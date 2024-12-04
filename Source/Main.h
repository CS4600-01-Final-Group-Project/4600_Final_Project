#ifndef MAIN_H
#define MAIN_H

#include "object.h"
#include "contacts.h"
#include "TextBox.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>

class SFMLApp
{
private:
    sf::RenderWindow window;

public:
    SFMLApp();
    ~SFMLApp();
    void run();

private:
    void processEvents();
    void update();
    void render();
    sf::Font font;
};

extern bool isToggled;
extern bool isToggled2;
extern bool contactSelected;
extern Contact clientContact;
extern Contact* otherContact;
extern std::vector<Contact> contacts;
extern TextBox textBox;


#endif