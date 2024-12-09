#include "main.h"
#include <vector>
#include <iostream>
#include <openssl/evp.h>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

void printHex(const unsigned char* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl;  // Return to decimal for future printing
}

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string);
bool DecryptData(const unsigned char* InData, size_t InDataLen, const std::string& InPublicKey, unsigned char*& OutData, size_t& OutDataLen);

std::string decode(std::string encryptedMessage, const Contact& clientContact)
{
    std::cout << "received message to decode: " << encryptedMessage << std::endl;

    int firstColon = encryptedMessage.find("|");
    int secondColon = encryptedMessage.find("|", firstColon + 1);

    std::string encodedMessage = encryptedMessage.substr(0, firstColon);
    std::string base64EncodedKey = encryptedMessage.substr(firstColon + 1, secondColon);

    std::cout << std::endl;
    std::cout << "encoded Key from decode: " << base64EncodedKey << std::endl;

    std::cout << std::endl;

    std::cout << "encoded message from decode: " << encodedMessage << std::endl;

    // Decode the key and message
    std::string decodedKey = base64_decode(base64EncodedKey);
    std::string decodedMessage = base64_decode(encodedMessage);

    // Debug: Print decoded key and message in hex
    std::cout << std::endl;
    std::cout << "Decoded Key from decode: " << decodedKey << std::endl;
 
    std::cout << std::endl;

    std::cout << "Decoded message from decode: " << decodedMessage << std:: endl;

    // Convert key and message into correct data type for cipher

    const unsigned char* decodedMessageAsCharArray = reinterpret_cast<const unsigned char*>(decodedMessage.c_str());
    const unsigned char* decodedFromBase64Key = reinterpret_cast<const unsigned char*>(decodedKey.c_str());


    // get path of private key // clientContact.getOnlineContact(contacts, clientContact.getName())
    std::string recipientPrivateKey = clientContact.getName() + "Key.pem";
    std::cout << "Trying to read clients private key from: " << recipientPrivateKey << std::endl;

    unsigned char* decryptedKey = nullptr;
    size_t decryptedKeyLength = 0;
    bool test = DecryptData(decodedFromBase64Key, decodedKey.size(), recipientPrivateKey, decryptedKey, decryptedKeyLength);

    std::cout << "Key length = " << decryptedKeyLength << std::endl;

    if (test) {
        std::cout << "Decrypted Key test success: " << std::endl;
        for (size_t i = 0; i < decryptedKeyLength; ++i) {
        std::cout << std::hex << static_cast<int>(decryptedKey[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    }
    else {
        std::cout << "Decrypted Key test failed" << std::endl;
    }

    // decrypt the AES key with private key
    
    // Necessary AES decryption variables
    EVP_CIPHER_CTX* decryption_ctx = EVP_CIPHER_CTX_new();

    if (EVP_DecryptInit_ex(decryption_ctx, EVP_aes_256_ecb(), NULL, decryptedKey, NULL) != 1) {
        std::cerr << "EVP_DecryptInit_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }

    int input_len = decodedMessage.size();
    int p_len = input_len + AES_BLOCK_SIZE; // Ensure buffer is large enough for padding
    unsigned char* plaintext = new unsigned char[p_len];  // Use vector for automatic memory management
    int out_len = 0, decrypted_len = 0;

    // Decrypt Update
    if (EVP_DecryptUpdate(decryption_ctx, plaintext, &out_len, decodedMessageAsCharArray, input_len) != 1) {
        std::cerr << "EVP_DecryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }
    decrypted_len += out_len;

    // Decrypt Final
    
    if (EVP_DecryptFinal_ex(decryption_ctx, plaintext + decrypted_len, &out_len) != 1) {
        std::cerr << "EVP_DecryptFinal_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(decryption_ctx);
        return "";
    }
    decrypted_len += out_len;

    // Convert to std::string and ensure null-termination
    std::string plaintextAsString(reinterpret_cast<char*>(plaintext), decrypted_len);

    // Clean up
    EVP_CIPHER_CTX_free(decryption_ctx);

    std::cout << "plaintext: " << plaintextAsString << std::endl;

    return plaintextAsString;
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

bool DecryptData(const unsigned char* InData, size_t InDataLen, const std::string& InPrivateKey, unsigned char*& OutData, size_t& OutDataLen) {
    // Load key using BIO
    BIO* bio = BIO_new_file(InPrivateKey.c_str(), "r");
    if (!bio) {
        // Handle error if file couldn't be opened
        std::cerr << "Failed to open private key file." << std::endl;
        return false;
    }

    EVP_PKEY* privateKey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    BIO_free(bio); // Release the BIO object after reading the key

    if (!privateKey) {
        // Handle error if the key couldn't be read
        std::cerr << "Failed to read private key from file." << std::endl;
        return false;
    }

    // Create/initialize context
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privateKey, NULL);
    if (!ctx) {
        EVP_PKEY_free(privateKey);
        std::cerr << "Failed to create decryption context." << std::endl;
        return false; // Handle context creation failure
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        EVP_PKEY_free(privateKey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to initialize decryption." << std::endl;
        return false; // Handle decryption initialization failure
    }

    // Decryption (calculate plaintext length)
    if (EVP_PKEY_decrypt(ctx, NULL, &OutDataLen, InData, InDataLen) <= 0) {
        EVP_PKEY_free(privateKey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to determine plaintext size." << std::endl;
        return false; // Handle decryption size query failure
    }

    // Allocate memory for plaintext
    OutData = (unsigned char*)OPENSSL_malloc(OutDataLen);
    if (!OutData) {
        EVP_PKEY_free(privateKey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to allocate memory for plaintext." << std::endl;
        return false; // Handle allocation failure
    }

    // Perform decryption
    if (EVP_PKEY_decrypt(ctx, OutData, &OutDataLen, InData, InDataLen) <= 0) {
        EVP_PKEY_free(privateKey);
        EVP_PKEY_CTX_free(ctx);
        OPENSSL_free(OutData);
        std::cerr << "Decryption failed." << std::endl;
        return false; // Handle decryption failure
    }

    // Release memory
    EVP_PKEY_free(privateKey);
    EVP_PKEY_CTX_free(ctx);

    return true; // Decryption successful
}
    