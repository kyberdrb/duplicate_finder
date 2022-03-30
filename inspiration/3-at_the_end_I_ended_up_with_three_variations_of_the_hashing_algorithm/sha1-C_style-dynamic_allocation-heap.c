char* sha1_C_style_dynamic_alloc(const char* filePath) {
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

int main() {
    std::string basePath = "/home/laptop/backup-sony_xa1/apps";
    fs::path aPath {basePath};

    for (const auto& entry : fs::directory_iterator(aPath)) {
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            std::cout << "file: " << absolutePathForFile << '\n';

            char* sha1AsString = sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );
            std::cout << "sha1-c-dynamic: " << sha1AsString;
            free(sha1AsString);
            sha1AsString = NULL;
            std::cout << '\n';

            std::cout << "---" << '\n';
        }
    }
}