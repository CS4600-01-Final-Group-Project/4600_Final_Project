#include "main.h"

std::string encode()
{

    std::string messageToEncode = getMessage();

    const unsigned char* messageAsUnsignedChar = reinterpret_cast<const unsigned char*>(messageToEncode.c_str());

    // Necessary AES encryption variables
    unsigned char key[32], IV[16];
    EVP_CIPHER_CTX* encryption_ctx = EVP_CIPHER_CTX_new();

    // Assign random values to our key and IV
    RAND_bytes(key, 32);
    RAND_bytes(IV, 16);

    // Initialize the AES-256-CBC cipher
    EVP_EncryptInit_ex(encryption_ctx, EVP_aes_256_cbc(), NULL, key, IV);

    int input_len = messageToEncode.length();
    int c_len = input_len + AES_BLOCK_SIZE;
    unsigned char* ciphertext = new unsigned char[c_len];
    int out_len = 0, encrypted_len = 0;

    // Encrypt Update
    if (EVP_EncryptUpdate(encryption_ctx, ciphertext, &out_len, messageAsUnsignedChar, input_len) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(encryption_ctx);
        return "";
    }
    encrypted_len += out_len;

    // Encrypt Final
    if (EVP_EncryptFinal_ex(encryption_ctx, ciphertext + encrypted_len, &out_len) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(encryption_ctx);
        return "";
    }
    encrypted_len += out_len;

    // Convert to std::string
    std::string encodedText(reinterpret_cast<char*>(ciphertext), encrypted_len);

    // Clean up resources
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(encryption_ctx);

	
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