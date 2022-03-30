void sha1_C_style_static_alloc(const char* filePath) {
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

int main() {
    std::string basePath = "/home/laptop/backup-sony_xa1/apps";
    fs::path aPath {basePath};

    for (const auto& entry : fs::directory_iterator(aPath)) {
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            std::cout << "file: " << absolutePathForFile << '\n';

            std::cout << "sha1-c-static:  ";
            sha1_C_style_static_alloc(absolutePathForFile.c_str() );
            std::cout << '\n';

            std::cout << "---" << '\n';
        }
    }
}