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

using std::cout;
using std::endl;
using std::string;

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string convertFileToString(std::string filePath){
    //std::ifstream file(filePath, std::ios::binary);
    std::ifstream file(filePath);
    string fileStr;

    std::istreambuf_iterator<char> inputIt(file), emptyInputIt;
    std::back_insert_iterator<string> stringInsert(fileStr);

    std::copy(inputIt, emptyInputIt, stringInsert);

    return fileStr;
}

//std::string getFileHash(const char *fileName) {
void getFileHash(const char *fileName) {

    unsigned char result[2*SHA_DIGEST_LENGTH];
    unsigned char hash[SHA_DIGEST_LENGTH];
    int i;
    FILE *f = fopen(fileName,"rb");
    SHA_CTX mdContent;
    int bytes;
    unsigned char data[1024];

    if(f == NULL){
        printf("%s couldn't open file\n",fileName);
        exit(1);
    }

    SHA1_Init(&mdContent);
    while((bytes = fread(data, 1, 1024, f)) != 0){

        SHA1_Update(&mdContent, data, bytes);
    }

    SHA1_Final(hash,&mdContent);

//    for(i=0;i<SHA_DIGEST_LENGTH;i++){
//        printf("%02x",hash[i]);
//    }
//    printf("\n");

    /** if you want to see the plain text of the hash */
    for(i=0; i < SHA_DIGEST_LENGTH;i++){
        sprintf((char *)&(result[i*2]), "%02x",hash[i]);
    }

    printf("hash: %s\n",result);

    fclose(f);
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
            //auto sha256AsString = sha256(absolutePathForFile);
            //std::cout << "hash: " << sha256AsString << '\n';

            getFileHash(absolutePathForFile.c_str());

            //auto shaAsString = getFileHash(absolutePathForFile.c_str());
            //std::cout << "hash: " << shaAsString << '\n';

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

    auto duplicateFiles = std::map<std::string, std::string>();
    //auto duplicateFiles = std::map<Hash, Path>();

    return 0;
}
