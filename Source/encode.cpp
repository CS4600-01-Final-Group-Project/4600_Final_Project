#include "main.h"

std::string encode()
{

    std::string messageToEncode = getMessage();
    std::string encodedText;
    // std::string keyExample = clientContact.getPublicKey;   idk these are just ideas
    // std::string keyExample = otherContact.getPrivateKey;
    

	
	return encodedText;
}


std::string getMessage()
{

    std::ifstream inputFile("Source/sending.txt");
    if (!inputFile.is_open())
    {

        std::cerr << "Error: Could not open the file.\n";
        return 0;
    }

    std::string line;
    std::string message;
    while (std::getline(inputFile, line))
    {

        message = line;
    }

    inputFile.close();

    return message;
}