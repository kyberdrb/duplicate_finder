#include "File.h"

#include <iostream>

#include <fstream>
#include "openssl/sha.h"
#include <vector>
#include <array>
#include <sstream>
#include <cstdint>
#include <iomanip>

#include <map>
#include <functional>
#include <algorithm>

// TODO template this for algorithm type - maybe later, when I will create a library for hashing which will be much simpler to use than the raw Crypto++ or OpenSSL libraries, like this for example: std::string sha256HashOfFile = Hasher::sha256sum(filePath);
std::string sha256_CPP_style(const std::string& filePath) {
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    std::ifstream file(filePath, std::ios::binary);
    const size_t CHUNK_SIZE = 1024;
    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire usage
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

    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire usage
    //std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH, '\0');
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash{'\0'};
    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream hashInHexadecimalFormat;
    for(auto chunk : hash) {
        hashInHexadecimalFormat << std::hex << std::setw(2) << std::setfill('0') << (uint32_t) chunk;
    }

    return hashInHexadecimalFormat.str();
}

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
    std::cout << basePath << std::endl;

    // look_up
    // - iterate all files in a directory
    //   - compute hash for each file
    //   - if hash exists already in the original files (map? because I want to store only unique files?)
    //      - add the file basePath to the duplicate files (multimap? because there might be multiple duplicate files with the same hash?)
    //      - continue
    //   - add the file basePath to the original files

    fs::path aPath {basePath};

    // Preferring list instead of vector because I'll sort entries at insertion/emplacement
    // which has quadratic n^2 complexity for a vector and linear complexity n for a list
    //std::vector<std::unique_ptr<File>> allFilesInDirectory;
    std::vector<std::unique_ptr<File>> allFilesInDirectory;

    std::cout << "Parent basePath: " << aPath.parent_path() << std::endl;
    std::cout << "Filename: " << aPath.filename() << std::endl;
    std::cout << "Extension: " << aPath.extension() << std::endl;

    for (const auto& entry : fs::directory_iterator(aPath)) {
        //const auto filenameStr = entry.path().filename().string();
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            //std::cout << "file: " << absolutePathForFile << '\n';

            std::string shaAsString = sha256_CPP_style(absolutePathForFile);
            //std::cout << "sha256-cpp:     " << shaAsString << '\n';

            //std::cout << "sha1-c-static:  ";
            //sha1_C_style_static_alloc(absolutePathForFile.c_str() );
            //std::cout << '\n';

            char* sha1AsString = sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );
            //std::cout << "sha1-c-dynamic: " << sha1AsString;
            free(sha1AsString);
            sha1AsString = NULL;
            //std::cout << '\n';

            // for constructor with initializing by moving - modern C++ style
            auto fileInDirectory = std::make_unique<File>(entry, shaAsString);

            // for constructor with initializing by copying - C and obsolete C++ style; doesn't work because I freed and nullified the 'sha1AsString' variable which results in crash at runtime for dereferencing a null pointer
            //auto fileInDirectory = std::make_unique<File>(entry, sha1AsString);

            // TODO sort outside of the loop with std::sort - lower complexity: N*N with sorting at insertion vs N + N * log(N) for separating insertion and sorting
            // sort files by path - so that the files with names that contain '(copy N)' will be after the original file, thus the original file remains and the '(copy N)' files will be marked as duplicates if they have the same has and moved to a separate directory designated for duplicate files
            allFilesInDirectory.emplace_back(std::move(fileInDirectory));

//            auto iterator = allFilesInDirectory.begin();
//            for (; iterator != allFilesInDirectory.end(); ++iterator) {
//                if (fileInDirectory->getModifiedAbsolutePath() < iterator->get()->getModifiedAbsolutePath()) {
//                    break;
//                }
//            }
//            allFilesInDirectory.emplace(iterator, std::move(fileInDirectory));
        }
        //else if (entry.is_directory()) {
        //    std::cout << "dir:  " << filenameStr << '\n';
        //}
        //else
        //    std::cout << "??    " << filenameStr << '\n';
    }

    struct compareFilePaths {
        bool operator()(const std::unique_ptr<File>& firstFile, const std::unique_ptr<File>& secondFile) const {
            return (firstFile.get()->getModifiedAbsolutePath() < secondFile.get()->getModifiedAbsolutePath());
            //return (firstFile.get()->getAbsolutePath() < secondFile.get()->getAbsolutePath());
        }
    };

    std::sort(allFilesInDirectory.begin(), allFilesInDirectory.end(), compareFilePaths());

    std::map<std::string, int> myMap;
    myMap.emplace(std::make_pair("earth", 1));
    myMap.emplace(std::make_pair("moon", 2));

    std::map<int, std::reference_wrapper<const File>> mb1;
    const File& f1 = *(allFilesInDirectory.begin()->get());
    mb1.emplace(0, std::cref(f1));

    std::map<std::string, std::reference_wrapper<const File>> mb2;
    //std::map<std::string, std::reference_wrapper<const File>, StringComparator> mb2;  // works without and with explicitly defined comparator
    const File& f2 = *(allFilesInDirectory.begin()->get());
    mb2.emplace("hello", f2);
    mb2.emplace("hi", *(allFilesInDirectory.begin()->get()));

    // Using a Functor for string (or object-type key) comparison
    struct StringComparator {
        bool operator()(const std::string& a, const std::string& b) const {
            return a < b;
        }
    };

    //std::map<std::reference_wrapper<const std::string>, std::reference_wrapper<const File>> mb3;                      // Apparently for std::reference
                                                                                                                        // /usr/include/c++/11.2.0/bits/stl_function.h:386:20: error: no match for ‘operator<’ (operand types are ‘const std::reference_wrapper<const std::__cxx11::basic_string<char> >’ and ‘const std::reference_wrapper<const std::__cxx11::basic_string<char> >’)
                                                                                                                        //  386 |       { return __x < __y; }
                                                                                                                        //      |                ~~~~^~~~~

    std::map<std::reference_wrapper<const std::string>, std::reference_wrapper<const File>, StringComparator> mb3;
    const File& f3 = *(allFilesInDirectory.begin()->get());
    const std::string key {"hello"};
    mb3.emplace(key, std::cref(f3));

    // Using a lambda expression for string (or object-type key) comparison
//    auto stringComparator = [](const std::string& a, const std::string& b) mutable { return a < b; };
//    std::map<std::reference_wrapper<
//        std::reference_wrapper<const std::string>>,                 // reference to 'hash' member from File object from the vector
//        std::reference_wrapper<const File>, // reference to the File object from the vector
//        decltype(stringComparator)> originalFiles (stringComparator);

    std::map<
            std::reference_wrapper<const std::string>,        // reference to 'hash' member from File object from the vector
            std::reference_wrapper<const File>,               // reference to the File object from the vector
            StringComparator
            >
        originalFiles;

    std::multimap<
            std::reference_wrapper<const std::string>,    // reference to 'hash' member from File object from the vector
            std::reference_wrapper<const File>,
            StringComparator
            >
        duplicateFiles;

    for (const auto& file : allFilesInDirectory) {
        std::cout << file->getHash();
        std::cout << "\t";
        std::cout << file->getAbsolutePath();
        std::cout << "\t";
        std::cout << file->getModifiedAbsolutePath();
        std::cout << "\n";
        //std::cout << "---" << '\n';

        // According to the C++ reference docs, "the insertion operation checks whether each inserted element has a key equivalent to the one of an element already in the container, and if so, the element is not inserted"
        // insert hash-File as key-value pair into the original files.
//        const std::string& hash = file->getHash();
//        const File& fileReference = *(file.get());

        // TODO instead of vector being a container of unique_ptrs for Files and map a container of reference to ref_wrap string-ref wrap File pair
        //   make the vector a container of shared_ptrs on Files and the map an container of
        //   weak_ptrs to the string (hash in File) and of weak_ptrs to the file itself
        //   C++ combos:
        //   - 'shared_ptr' and 'weak_ptr'
        //   - 'unique_ptr' and 'reference_wrapper'
//        originalFiles.emplace(hash, fileReference);     // referencing local variables produces unreadable characters and undefined behavior
                                                        // when iterating and printing out contents of the map

        // if the file is missing in the original files
        //   - by checking if the original files container contains the hash key associated with the file -
        //   add it to the original files.
        //   Otherwise add the file to the duplicate files

        bool isFileMissing = originalFiles.count(file->getHash()) == 0;
        if (isFileMissing) {
            originalFiles.emplace(file->getHash(), *(file.get()));
            // or explicitly
            //originalFiles.emplace(file->getHash(), std::cref(*(file.get())));
            continue;
        }

        duplicateFiles.emplace(file->getHash(), *(file.get()));

        //file->addDuplicateFile(*(file.get()));

        const_cast<File&>(originalFiles.at(file->getHash()).get())
            .addDuplicateFile(*(file.get()));
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "ORIGINAL FILES - C++17 iteration\n\n";

    std::vector<std::reference_wrapper<const File>> originalFilesInAnotherForm;

    for (const auto& [hash, file] : originalFiles) {
        std::cout << hash.get() << " has file " << file.get().getAbsolutePath() << "\n";
        originalFilesInAnotherForm.emplace_back(file);
    }

    // clean_up
    // - iterate all duplicate files
    //   - display duplicate file basePath with hash
    //   - display original file basePath with the same hash from the original files
    //   - move the duplicate file to trash OR make a directory named '.TO_TRASH' in the searched directory and move the duplicate file to the '.TO_TRASH' directory

    std::cout << "\n===================================================================\n\n";
    std::cout << "DUPLICATE FILES - C++11 iteration\n\n";

    for (const auto& keyValuePair : duplicateFiles) {
        std::cout << keyValuePair.first.get() << "\t" << keyValuePair.second.get().getAbsolutePath() << "\n";
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "ALL ORIGINAL FILES - sorted by name - WITH THEIR DUPLICATES - if any\n\n";

    struct compareFilePathsForRawFiles {
        bool operator()(const File& firstFile, const File& secondFile) const {
            return (firstFile.getModifiedAbsolutePath() < secondFile.getModifiedAbsolutePath());
        }
    };

    std::sort(originalFilesInAnotherForm.begin(), originalFilesInAnotherForm.end(), compareFilePathsForRawFiles());

    for (const auto& file : originalFilesInAnotherForm) {
        std::cout << file;
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "ONLY ORIGINAL FILES - sorted by name - THAT HAVE DUPLICATES\n\n";

    //std::sort(originalFilesInAnotherForm.begin(), originalFilesInAnotherForm.end(), compareFilePathsForRawFiles());

    for (const auto& file : originalFilesInAnotherForm) {
        if (file.get().hasDuplicates()) {
            std::cout << file;
        }
    }

    // TODO move all duplicate files in a separate (newly created) directory 'DUPLICATE_FILES'

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

    return 0;
}
