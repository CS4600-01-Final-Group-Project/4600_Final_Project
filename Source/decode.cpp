#include "main.h"

std::string decode(std::string encryptedMessage)
{
	int firstColon = encryptedMessage.find("|");
	int secondColon = encryptedMessage.find("|", firstColon + 1);

	std::string encodedMessage = encryptedMessage.substr(0, firstColon);
	std::string base64EncodedKey = encryptedMessage.substr(firstColon + 1, secondColon);

	std::cout << "Encoded Message: " << encodedMessage << std::endl;
	std::cout << "Base64 Encoded Key: " << base64EncodedKey << std::endl;

	return encryptedMessage;
}