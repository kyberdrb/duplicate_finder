#pragma once

#include "File.h"
#include "StringComparatorAscending.h"

#include <map>
#include <memory>
#include <string>

class Directory {

public:
    explicit Directory(std::string pathToDirectory);

    const std::string& getPathToDirectory() const;

    // scan the directory
    // - iterate all files in a directory
    //   - compute hash for each file
    //   - if hash exists already in the original files (map? because I want to store only unique files?)
    //      - add the file pathToDirectory to the duplicate files (multimap? because there might be multiple duplicate files with the same hash?)
    //      - continue
    //   - add the file to the original files
    void scan();

    // sort files by path and list original files first, but perform sorting outside of the loop with std::sort, instead of sorting at insertion
    //  because of lower algorithmic complexity: N*N with sorting at insertion vs N [insertion] + N * log(N) [sorting] for separated insertion and sorting
    void sort();

    void findDuplicates();

    // generate report
    // - iterate all duplicate files
    //   - display duplicate file pathToDirectory with hash
    //   - display original file pathToDirectory with the same hash from the original files
    void generateReport() const;

    // tidy_up
    //   - move the duplicate file to trash OR make a directory named '.TO_TRASH' in the searched directory and move the duplicate file to the '.TO_TRASH' directory
    void moveDuplicatesToSeparateDirectory();

private:
    std::string pathToDirectory;

    std::vector<std::unique_ptr<File>> allFilesInDirectory;

    // Using a lambda expression for string (or object-type key) comparison
//    auto stringComparator = [](const std::string& a, const std::string& b) mutable { return a < b; };
//    std::map<std::reference_wrapper<
//        std::reference_wrapper<const std::string>>,                 // reference to 'hash' member from File object from the vector
//        std::reference_wrapper<const File>, // reference to the File object from the vector
//        decltype(stringComparator)> originalFiles (stringComparator);

    std::map<
            const std::reference_wrapper<const std::string>,        // reference to 'hash' member from File object from the vector
            const std::reference_wrapper<const File>,               // reference to the File object from the vector
            StringComparatorAscending
    >
            originalFiles;

    std::multimap<
            const std::reference_wrapper<const std::string>,    // reference to 'hash' member from File object from the vector
            const std::reference_wrapper<const File>,
            StringComparatorAscending
    >
            duplicateFiles;
};
