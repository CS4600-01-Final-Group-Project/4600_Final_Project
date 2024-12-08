#include "main.h"
#include <vector>
#include <iostream>
#include <openssl/evp.h>

std::vector<unsigned char> base64Decode(const std::string& base64EncodedString);

std::string decode(std::string encryptedMessage, const Contact& clientContact)
{
    int firstColon = encryptedMessage.find("|");
    int secondColon = encryptedMessage.find("|", firstColon + 1);

    std::string encodedMessage = encryptedMessage.substr(0, firstColon);
    std::string base64EncodedKey = encryptedMessage.substr(firstColon + 1, secondColon);

    std::cout << "Encoded Message: " << encodedMessage << std::endl;
    std::cout << "Base64 Encoded Key: " << base64EncodedKey << std::endl;

    // Decode the key and message
    std::vector<unsigned char> decodedKey = base64Decode(base64EncodedKey);
    std::vector<unsigned char> decodedMessage = base64Decode(encodedMessage);

    // Debug: Print decoded key and message in hex
    std::cout << std::endl;
    std::cout << "Decoded Key: ";
    for (unsigned char byte : decodedKey) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    std::cout << "Decoded message: ";
    for (unsigned char byte : decodedMessage) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // Convert key and message into correct data type for cipher
    const unsigned char* decodedKeyForCipher = decodedKey.data();
    const unsigned char* decodedMessageForCipher = decodedMessage.data();

    // Necessary AES decryption variables
    EVP_CIPHER_CTX* decryption_ctx = EVP_CIPHER_CTX_new();

    if (EVP_DecryptInit_ex(decryption_ctx, EVP_aes_256_ecb(), NULL, decodedKeyForCipher, NULL) != 1) {
        std::cerr << "EVP_DecryptInit_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }

    int input_len = decodedMessage.size();
    int p_len = input_len + AES_BLOCK_SIZE; // Ensure buffer is large enough for padding
    unsigned char* plaintext = new unsigned char[p_len];  // Use vector for automatic memory management
    int out_len = 0, decrypted_len = 0;

    // Decrypt Update
    if (EVP_DecryptUpdate(decryption_ctx, plaintext, &out_len, decodedMessageForCipher, input_len) != 1) {
        std::cerr << "EVP_DecryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }
    decrypted_len += out_len;

    // Decrypt Final

    /*
    if (EVP_DecryptFinal_ex(decryption_ctx, plaintext + decrypted_len, &out_len) != 1) {
        std::cerr << "EVP_DecryptFinal_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }
    decrypted_len += out_len;
    */

    

    // Convert to std::string and ensure null-termination
    std::string plaintextAsString(reinterpret_cast<char*>(plaintext), decrypted_len);

    // Clean up
    EVP_CIPHER_CTX_free(decryption_ctx);

    return plaintextAsString;
}

std::vector<unsigned char> base64Decode(const std::string& base64EncodedString)
{
    int length = base64EncodedString.size();
    std::vector<unsigned char> decodedBytes((length * 3) / 4);

    int decodedLength = EVP_DecodeBlock(decodedBytes.data(), reinterpret_cast<const unsigned char*>(base64EncodedString.c_str()), length);
    decodedBytes.resize(decodedLength);

    return decodedBytes;
}
    