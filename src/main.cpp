#include "File.h"
#include "HashGenerator.h"

#include <iostream>

#include <map>
#include <functional>

int main() {
    std::string basePath = "/home/laptop/backup-sony_xa1/apps";
    std::cout << basePath << std::endl;

    // auto directory = std::make_unique<Directory>(pathToDirectory);
    // directory->scan();

    // look_up
    // - iterate all files in a directory
    //   - compute hash for each file
    //   - if hash exists already in the original files (map? because I want to store only unique files?)
    //      - add the file basePath to the duplicate files (multimap? because there might be multiple duplicate files with the same hash?)
    //      - continue
    //   - add the file basePath to the original files
    fs::path aPath {basePath};

    std::vector<std::unique_ptr<File>> allFilesInDirectory;
    std::unique_ptr<HashGenerator> hashGenerator{};

    std::cout << "Parent basePath: " << aPath.parent_path() << std::endl;
    std::cout << "Filename: " << aPath.filename() << std::endl;
    std::cout << "Extension: " << aPath.extension() << std::endl;

    for (const auto& entry : fs::directory_iterator(aPath)) {
        //const auto filenameStr = entry.path().filename().string();
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            //std::cout << "file: " << absolutePathForFile << '\n';

            std::string shaAsString{};

            shaAsString = hashGenerator->sha256_CPP_style(absolutePathForFile);

//            std::cout << "sha1-c-static:  ";
//            hashGenerator->sha1_C_style_static_alloc(absolutePathForFile.c_str() );
//            std::cout << '\n';

            // C-style hash generation
//            char* sha1AsString = sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );
//            auto fileInDirectory = std::make_unique<File>(entry, sha1AsString);

            // for 'sha1AsString' as 'char*'
//            if (sha1AsString != NULL) {
//                free(sha1AsString);
//                sha1AsString = NULL;
//            }

            // C-Plus-Plus-ified C-style hash generation
//            shaAsString = hashGenerator->sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );

            auto fileInDirectory = std::make_unique<File>(entry, shaAsString);

            allFilesInDirectory.emplace_back(std::move(fileInDirectory));
        }
        //else if (entry.is_directory()) {
        //    std::cout << "dir:  " << filenameStr << '\n';
        //}
        //else
        //    std::cout << "??    " << filenameStr << '\n';
    }

    struct FilePathsComparator {
        bool operator()(const std::unique_ptr<File>& firstFile, const std::unique_ptr<File>& secondFile) const {
            return (firstFile.get()->getModifiedAbsolutePath() < secondFile.get()->getModifiedAbsolutePath());
            //return (firstFile.get()->getAbsolutePath() < secondFile.get()->getAbsolutePath());
        }
    };

    // sort files by path and list original files first, but perform sorting outside of the loop with std::sort, instead of sorting at insertion
    //  because of lower algorithmic complexity: N*N with sorting at insertion vs N [insertion] + N * log(N) [sorting] for separated insertion and sorting
    std::sort(allFilesInDirectory.begin(), allFilesInDirectory.end(), FilePathsComparator());

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

    // directory->findDuplicates();

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

        const_cast<File&>(originalFiles.at(file->getHash()).get())
            .addDuplicateFile(*(file.get()));
    }

    // auto reportGenerator = std::make_unique<ReportGenerator>(directory);
    // reportGenerator->generateTerminalReport();
    // reportGenerator->generateFileReport();
    std::cout << "\n===================================================================\n\n";
    std::cout << "ORIGINAL FILES - C++17 iteration\n\n";

    std::vector<std::reference_wrapper<const File>> originalFilesInAnotherForm;

    for (const auto& [hash, file] : originalFiles) {
        std::cout << hash.get() << "\t" << file.get().getAbsolutePath() << "\n";
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
