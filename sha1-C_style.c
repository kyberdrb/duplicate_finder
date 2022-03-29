void sha1(const char* filePath) {
    SHA_CTX SHA1_context;
    SHA1_Init(&SHA1_context);

    uint32_t bytes;
    unsigned char data[1024];
    FILE* file = fopen(filePath, "rb");

    while( (bytes = fread(data, 1, 1024, file) ) != 0 ) {
        SHA1_Update(&SHA1_context, data, bytes);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1_Final(hash, &SHA1_context);

    unsigned char result[2*SHA_DIGEST_LENGTH];
    for(int32_t chunkSize=0; chunkSize < SHA_DIGEST_LENGTH;chunkSize++) {
        sprintf( (char*) &(result[chunkSize * 2]), "%02x",hash[chunkSize] );
    }

    printf("%s\n",result);

    fclose(file);

    //return ??? not local variable 'result' but something dynamically allocated that the caller code will need to deallocate.
}