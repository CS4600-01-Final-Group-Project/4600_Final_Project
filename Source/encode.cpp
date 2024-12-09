#include "main.h"
#include <openssl/bio.h>

bool EncryptData(const unsigned char* InData, size_t InDataLen, const std::string& InPublicKey, unsigned char*& OutData, size_t& OutDataLen);
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

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



    std::string decodedString(reinterpret_cast<const char*>(ciphertext), encrypted_len);
    std::cout << "Decoded string from encode: " << decodedString << std::endl;

    if (test) {
        std::cout << "Encrypted Key test success: " << std::string(reinterpret_cast<char*>(encryptedKey), encryptedKeyLength) << std::endl;
    }
    else {
        std::cout << "Encrypted Key test failed" << std::endl;
    }

    std::string encodedCiphertextString = base64_encode(ciphertext, encrypted_len);
    std::string keyAsString = base64_encode(encryptedKey, encryptedKeyLength);

    // Clean up resources
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(encryption_ctx);


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

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;

}