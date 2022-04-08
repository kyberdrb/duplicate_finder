#include "HashGenerator.h"

#include "openssl/sha.h"

#include <array>
#include <vector>

#include <fstream>
#include <iomanip>
#include <sstream>

// TODO maybe template this, or abstract this into virtual functions, according to the algorithm type - maybe later, when I will create a library for hashing which will be much simpler to use than the raw Crypto++ or OpenSSL libraries, like this for example: std::string sha256HashOfFile = Hasher::sha256sum(filePath);
std::string HashGenerator::sha256_CPP_style(const std::string& filePath) {
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    std::ifstream file(filePath, std::ios::binary);
    const size_t CHUNK_SIZE = 1024;
    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire hashAsText creation process
    //std::vector<char> chunkBuffer(CHUNK_SIZE, '\0');
    std::array<char, CHUNK_SIZE> chunkBuffer{'\0'};

    while ( file.read( chunkBuffer.data(), chunkBuffer.size() ) ) {
        auto bytesRead = file.gcount();
        SHA256_Update(&sha256_context, chunkBuffer.data(), bytesRead);
    }

    // Evaluate the last partial chunk
    // `fin.read(...)` evaluates to false on the last partial block (or eof). You need to process partial reads outside of the while loop `gcount()!=0` - https://stackoverflow.com/questions/35905295/reading-a-file-in-chunks#comment59488065_35905524
    if (file.gcount() != 0) {
        SHA256_Update(&sha256_context, chunkBuffer.data(), file.gcount());
    }

    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire hashAsText creation process
    //std::vector<uint8_t> hashAsText(SHA256_DIGEST_LENGTH, '\0');
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash{'\0'};
    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream hashInHexadecimalFormat;
    for(auto chunk : hash) {
        hashInHexadecimalFormat << std::hex << std::setw(2) << std::setfill('0') << (uint32_t) chunk;
    }

    return hashInHexadecimalFormat.str();
}

void HashGenerator::sha1_C_style_static_alloc(const char* filePath) {
    SHA_CTX sha1Context;
    SHA1_Init(&sha1Context);

    uint32_t bytesRead;
    uint8_t chunkBuffer[1024];
    const size_t CHUNK_SIZE = 1024;
    FILE* file = fopen(filePath, "rb");

    while( (bytesRead = fread(chunkBuffer, 1, CHUNK_SIZE, file) ) != 0 ) {
        SHA1_Update(&sha1Context, chunkBuffer, bytesRead);
    }

    uint8_t hash[SHA_DIGEST_LENGTH];
    SHA1_Final(hash, &sha1Context);

    char hashInHexadecimalFormat[2 * SHA_DIGEST_LENGTH];
    for(int32_t chunkPosition=0; chunkPosition < SHA_DIGEST_LENGTH; ++chunkPosition) {
        sprintf(&(hashInHexadecimalFormat[chunkPosition * 2]), "%02x", hash[chunkPosition] );
    }

    printf("%s", hashInHexadecimalFormat);

    fclose(file);

    // no return because we would return a pointer to a local variable, which are discarded at the closing curly brace of this function; therefore we print the output to the terminal as a feedback and a side-effect
}

char* HashGenerator::sha1_C_style_dynamic_alloc(const char* filePath) {
    SHA_CTX* sha1Context = (SHA_CTX*) calloc(1, sizeof(SHA_CTX) );
    SHA1_Init(sha1Context);

    uint32_t bytesRead;
    uint8_t* chunkBuffer = (uint8_t*) calloc(1024, sizeof(uint8_t) );
    const size_t CHUNK_SIZE = 1024;
    FILE* file = fopen(filePath, "rb");

    while( (bytesRead = fread(chunkBuffer, 1, CHUNK_SIZE, file) ) != 0 ) {
        SHA1_Update(sha1Context, chunkBuffer, bytesRead);
    }

    free(chunkBuffer);
    chunkBuffer = NULL;     // sanitize dangling pointer

    uint8_t* hash = (uint8_t*) calloc(SHA_DIGEST_LENGTH, sizeof(uint8_t) );
    SHA1_Final(hash, sha1Context);

    free(sha1Context);
    sha1Context = NULL;

    //char* hashInHexadecimalFormat = (char*) calloc(SHA_DIGEST_LENGTH * 2, sizeof(char));   // error: 'Corrupted size vs. prev_size' or 'malloc(): invalid next size (unsorted)'
    char* hashInHexadecimalFormat = (char*) calloc(SHA_DIGEST_LENGTH * 2 + 1, sizeof(char));   // add one extra position at the end of the buffer '+ 1' for the 'null terminator' '\0' that terminates the string, in order to prevent error 'Corrupted size vs. prev_size' and other errors and undefined behaviors - https://cppsecrets.com/users/931049711497106109971151165748485664103109971051084699111109/C00-Program-to-Find-Hash-of-File.php
    for(int32_t chunkPosition=0; chunkPosition < SHA_DIGEST_LENGTH; ++chunkPosition) {
        sprintf( &(hashInHexadecimalFormat[chunkPosition * 2] ), "%02x", hash[chunkPosition] );
    }

    free(hash);
    hash = NULL;

    fclose(file);

    return hashInHexadecimalFormat; // remember to free the pointer in the caller code, i. e. free the pointer on the client side
}
