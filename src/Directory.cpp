#include "Directory.h"

#include "File.h"
#include "HashGenerator.h"

#include <algorithm>
#include <iostream>

Directory::Directory(std::string pathToDirectory) :
    pathToDirectory(std::move(pathToDirectory))
{}

const std::string& Directory::getPathToDirectory() const {
    return pathToDirectory;
}

void Directory::scan() {
    fs::path aPath {this->getPathToDirectory()};
    
    std::unique_ptr<HashGenerator> hashGenerator{};

    std::cout << "Parent pathToDirectory: " << aPath.parent_path() << std::endl;
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

            this->allFilesInDirectory.emplace_back(std::move(fileInDirectory));
        }
        //else if (entry.is_directory()) {
        //    std::cout << "dir:  " << filenameStr << '\n';
        //}
        //else
        //    std::cout << "??    " << filenameStr << '\n';
    }
}

void Directory::sort() {
    struct FilePathsComparator {
        bool operator()(const std::unique_ptr<File>& firstFile, const std::unique_ptr<File>& secondFile) const {
            return (firstFile.get()->getModifiedAbsolutePath() < secondFile.get()->getModifiedAbsolutePath());
            //return (firstFile.get()->getAbsolutePath() < secondFile.get()->getAbsolutePath());
        }
    };

    std::sort(this->allFilesInDirectory.begin(), this->allFilesInDirectory.end(), FilePathsComparator());
}

void Directory::findDuplicates() {
    for (const auto& file : this->allFilesInDirectory) {
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
}

void Directory::generateReport() const {
    std::cout << "\n===================================================================\n\n";
    std::cout
        << "Found " << this->allFilesInDirectory.size() << " files: "
        << this->originalFiles.size() << " original files and "
        << this->duplicateFiles.size() << " duplicate files" << "\n";

    std::cout << "\n===================================================================\n\n";
    std::cout << "ORIGINAL FILES - C++17 iteration\n\n";

    std::vector<std::reference_wrapper<const File>> originalFilesInAnotherForm;

    for (const auto& [hash, file] : this->originalFiles) {
        std::cout << hash.get() << "\t" << file.get().getAbsolutePath() << "\n";
        originalFilesInAnotherForm.emplace_back(file);
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "DUPLICATE FILES - C++11 iteration\n\n";

    for (const auto& keyValuePair : this->duplicateFiles) {
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
}

void Directory::moveDuplicatesToSeparateDirectory() {
    std::cout << "\n===================================================================\n\n";
    std::cout << "MOVING DUPLICATE FILES\n\n";

    if (this->pathToDirectory.at(this->pathToDirectory.size() - 1) == '/') {
        this->pathToDirectory.pop_back();
    }

    std::string pathToDuplicateFilesDirectory = this->pathToDirectory + "/DUPLICATE_FILES/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectory);

    for (const auto& [hash, duplicateFile] : this->duplicateFiles) {
        try {
            std::cout << "Moving duplicate file\n";
            std::cout << "\t" << duplicateFile.get().getAbsolutePath() << "\n";
            std::cout << "to a separate directory for duplicate files\n";
            std::cout << "\t" << pathToDuplicateFilesDirectory + duplicateFile.get().getFilename() << "\n";
            std::cout << "---\n";

            const std::string& from = duplicateFile.get().getAbsolutePath();
            const std::string& to = pathToDuplicateFilesDirectory + duplicateFile.get().getFilename();
            std::filesystem::rename(from, to);

        } catch (std::filesystem::filesystem_error& e) {
            std::cout << e.what() << '\n';
        }
    }
}
