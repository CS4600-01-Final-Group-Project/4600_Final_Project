#include "main.h"

std::string encode()
{

    std::string messageToEncode = getMessage();

    const unsigned char* messageAsUnsignedChar = reinterpret_cast<const unsigned char*>(messageToEncode.c_str());

    // Necessary AES encryption variables
    unsigned char key[32]; //, IV[16];
    EVP_CIPHER_CTX* encryption_ctx = EVP_CIPHER_CTX_new();

    // Assign random values to our key and IV
    RAND_bytes(key, 32);
    // RAND_bytes(IV, 16); - might be fine to do just ECB 

    // Initialize the AES-256-CBC cipher
    EVP_EncryptInit_ex(encryption_ctx, EVP_aes_256_ecb(), NULL, key, NULL);

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

    // base64 encode the ciphertext
    std::vector<unsigned char> encodedCiphertext((4 * (encrypted_len + 2)));
    int encoded_len = EVP_EncodeBlock(encodedCiphertext.data(), ciphertext, encrypted_len);

    // base64 encode the key
    std::vector<unsigned char> encodedKey((4 * (32 + 2)));
    int encoded_key_len = EVP_EncodeBlock(encodedKey.data(), key, 32);

    // Clean up resources
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(encryption_ctx);

    std::string encodedCiphertextString(reinterpret_cast<char*>(encodedCiphertext.data()), encoded_len);
    std::string keyAsString(reinterpret_cast<char*>(encodedKey.data()), encoded_key_len);

    return  encodedCiphertextString + "|" + keyAsString;
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