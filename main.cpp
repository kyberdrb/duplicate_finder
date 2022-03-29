#include <iostream>
#include <map>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

#include "openssl/sha.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>

using std::cout;
using std::endl;
using std::string;

std::string sha256_CPP_style(const std::string filePath) {
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    std::ifstream fin(filePath, std::ios::binary);
    std::vector<char> x(1024, 0);

    while (fin.read(x.data(), x.size())) {
        std::streamsize bytes = fin.gcount();
        SHA256_Update(&sha256_context, x.data(), bytes);
    }

    if (fin.gcount() != 0) {
        SHA256_Update(&sha256_context, x.data(), fin.gcount());
    }

    std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH, 0);
    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream result;
    for(auto chunk : hash) {
        result << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)chunk;
    }

    return result.str();
}

void sha1_C_style(const char* filePath) {
    SHA_CTX SHA1_context;
    // TODO rename SHA1_context to sha1_context
    //SHA_CTX* SHA1_context = (SHA_CTX*) calloc(1, sizeof(SHA_CTX));
    SHA1_Init(&SHA1_context);

    uint32_t bytes;
    unsigned char data[1024];
    //char* data = (unsigned char*) calloc(1024, sizeof(unsigned char));
    FILE* file = fopen(filePath, "rb");

    while( (bytes = fread(data, 1, 1024, file) ) != 0 ) {
        SHA1_Update(&SHA1_context, data, bytes);
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    //unsigned char* hash = (unsigned char*) calloc(SHA_DIGEST_LENGTH, sizeof(unsigned char));
    SHA1_Final(hash, &SHA1_context);

    char result[2*SHA_DIGEST_LENGTH];
    //char* result = (char*) calloc(SHA_DIGEST_LENGTH * 2 + 1, sizeof(char));   // + 1 for the null character '\0' for terminating string
    for(int32_t chunkPosition=0; chunkPosition < SHA_DIGEST_LENGTH; ++chunkPosition) {
        sprintf(&(result[chunkPosition * 2]), "%02x", hash[chunkPosition] );
    }

    printf("%s",result);

    fclose(file);

    //return ??? not local variable 'result' but something dynamically allocated that the caller code will need to deallocate.
    result = NULL;
    free(result);
}

int main() {
    std::string basePath = "/home/laptop/backup-sony_xa1/apps";
    std::cout << basePath << std::endl;

    // look_up
    // - iterate all files in a directory
    //   - compute hash for each file
    //   - if hash exists already in the original files (map? because I want to store only unique files?)
    //      - add the file basePath to the duplicate files (multimap? because there might be multiple duplicate files with the same hash?)
    //      - continue
    //   - add the file basePath to the original files

    fs::path aPath {basePath};

    cout << "Parent basePath: " << aPath.parent_path() << endl;
    cout << "Filename: " << aPath.filename() << endl;
    cout << "Extension: " << aPath.extension() << endl;

    for (const auto& entry : fs::directory_iterator(aPath)) {
        const auto filenameStr = entry.path().filename().string();
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file()) {
            // show absolute file basePath
            std::cout << "file: " << absolutePathForFile << '\n';

            // generate hash from a file, which is read as text, regardless whether the file is in a plain-text or binary format
            //auto fileAsString = convertFileToString(absolutePathForFile);
            //auto sha256AsString = sha256_CPP_style(absolutePathForFile);
            //std::cout << "hash: " << sha256AsString << '\n';

            //getFileHash(absolutePathForFile.c_str());

            //auto shaAsString = getFileHash(absolutePathForFile.c_str());
            auto shaAsString = sha256_CPP_style(absolutePathForFile);
            std::cout << "hash: " << shaAsString << '\n';

            std::cout << "hash: ";
            sha1_C_style(absolutePathForFile.c_str());
            std::cout << '\n';

            std::cout << "---" << '\n';
        }
        //else if (entry.is_directory()) {
        //    std::cout << "dir:  " << filenameStr << '\n';
        //}
        //else
        //    std::cout << "??    " << filenameStr << '\n';
    }

    // clean_up
    // - iterate all duplicate files
    //   - display duplicate file basePath with hash
    //   - display original file basePath with the same hash from the original files
    //   - move the duplicate file to trash OR make a directory named '.TO_TRASH' in the searched directory and move the duplicate file to the '.TO_TRASH' directory

    // verify
    // - iterate all files in a directory again
    //   - if file is present in the searched directory
    //     - continue
    //   - display error "File not matched"
    //
    // - compare the number of entries in original files and the number of files in the searched directory
    // - if they're matching
    //   - display message "In the directory are present only unique files"
    //   - exit 0
    // - display message "The files in the directory do not match the original file list"
    // exit 1

    auto filesWithHashes = std::map<std::string, std::string>();
    //auto originalFiles = std::map<Hash, Path>();

    auto duplicateFiles = std::multimap<std::string, std::string>();
    //auto duplicateFiles = std::map<Hash, Path>();

    return 0;
}
