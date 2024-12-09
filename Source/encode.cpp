#include "main.h"
#include <openssl/bio.h>

bool EncryptData(const unsigned char* InData, size_t InDataLen, const std::string& InPublicKey, unsigned char*& OutData, size_t& OutDataLen);

std::string encode(const Contact& clientContact)
{
    std::string messageToEncode = getMessage();

    const unsigned char* messageAsUnsignedChar = reinterpret_cast<const unsigned char*>(messageToEncode.c_str());

    // Necessary AES encryption variables
    unsigned char key[32]; //, IV[16];
    EVP_CIPHER_CTX* encryption_ctx = EVP_CIPHER_CTX_new();

    // Assign random values to our key
    RAND_bytes(key, 32);
    std::cout << "Known key from encode: ";
    for (unsigned char byte : key) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // Initialize the AES-256-ECB cipher
    EVP_EncryptInit_ex(encryption_ctx, EVP_aes_256_ecb(), NULL, key, NULL);

    int input_len = messageToEncode.length();
    int c_len = input_len + AES_BLOCK_SIZE;
    unsigned char* ciphertext = new unsigned char[c_len];
    int out_len = 0, encrypted_len = 0;

    // Encrypt Update
    if (EVP_EncryptUpdate(encryption_ctx, ciphertext, &out_len, messageAsUnsignedChar, input_len) != 1) {
        std::cerr << "EVP_EncryptUpdate failed" << std::endl;
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(encryption_ctx);
        return "";
    }
    encrypted_len += out_len;

    // Encrypt Final
    if (EVP_EncryptFinal_ex(encryption_ctx, ciphertext + encrypted_len, &out_len) != 1) {
        std::cerr << "EVP_EncryptFinal_ex failed" << std::endl;
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(encryption_ctx);
        return "";
    }
    encrypted_len += out_len;

    std::string recipientPublicKeyPath = clientContact.getOnlineContact(contacts, clientContact.getName()) + "KeyPublic.pem";
    std::cout << "Trying to read recipient public key from file: " << recipientPublicKeyPath << std::endl;

    
    unsigned char* encryptedKey = nullptr;
    size_t encryptedKeyLength = 0;
    bool test = EncryptData(key, 32, recipientPublicKeyPath, encryptedKey, encryptedKeyLength);

    if (test) {
        std::cout << "Encrypted Key test success: " << std::string(reinterpret_cast<char*>(encryptedKey), encryptedKeyLength) << std::endl;
    }
    else {
        std::cout << "Encrypted Key test failed" << std::endl;
    }

    // base64 encode the ciphertext
    std::vector<unsigned char> encodedCiphertext((4 * encrypted_len / 3 + 4));
    int encoded_len = EVP_EncodeBlock(encodedCiphertext.data(), ciphertext, encrypted_len);

    // base64 encode the key
    std::vector<unsigned char> encodedKey((4 * encryptedKeyLength / 3 + 4));
    int encoded_key_len = EVP_EncodeBlock(encodedKey.data(), encryptedKey, encryptedKeyLength);

    // Clean up resources
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(encryption_ctx);

    std::string encodedCiphertextString(reinterpret_cast<char*>(encodedCiphertext.data()), encoded_len);
    std::string keyAsString(reinterpret_cast<char*>(encodedKey.data()), encoded_key_len);

    return encodedCiphertextString + "|" + keyAsString;
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

bool EncryptData(const unsigned char* InData, size_t InDataLen, const std::string& InPublicKey, unsigned char*& OutData, size_t& OutDataLen) {
    // Load key using BIO
    BIO* bio = BIO_new_file(InPublicKey.c_str(), "r");
    if (!bio) {
        // Handle error if file couldn't be opened
        std::cerr << "Failed to open public key file." << std::endl;
        return false;
    }

    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio); // Release the BIO object after reading the key

    if (!pkey) {
        // Handle error if the key couldn't be read
        std::cerr << "Failed to read public key from file." << std::endl;
        return false;
    }

    // Create/initialize context
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to create encryption context." << std::endl;
        return false; // Handle context creation failure
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to initialize encryption." << std::endl;
        return false; // Handle encryption initialization failure
    }

    // Encryption (calculate ciphertext length)
    if (EVP_PKEY_encrypt(ctx, NULL, &OutDataLen, InData, InDataLen) <= 0) {
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to determine ciphertext size." << std::endl;
        return false; // Handle encryption size query failure
    }

    // Allocate memory for ciphertext
    OutData = (unsigned char*)OPENSSL_malloc(OutDataLen);
    if (!OutData) {
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        std::cerr << "Failed to allocate memory for ciphertext." << std::endl;
        return false; // Handle allocation failure
    }

    // Perform encryption
    if (EVP_PKEY_encrypt(ctx, OutData, &OutDataLen, InData, InDataLen) <= 0) {
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        OPENSSL_free(OutData);
        std::cerr << "Encryption failed." << std::endl;
        return false; // Handle encryption failure
    }

    // Release memory
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    return true; // Encryption successful
}