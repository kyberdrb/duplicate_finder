#pragma once

#include "File.h"
#include "HashComparatorAscending.h"

#include <map>

using OriginalFilesType = std::map<const std::reference_wrapper<const Hash>, const std::reference_wrapper<const File>, HashComparatorAscending>;
using DuplicateFilesType = std::multimap<const std::reference_wrapper<const Hash>, const std::reference_wrapper<const File>, HashComparatorAscending>;

class Directory {

public:
    explicit Directory(std::string pathToDirectory);

    const std::string& getPathToDirectory() const;

    const OriginalFilesType& getOriginalFiles() const;
    const DuplicateFilesType& getDuplicateFiles() const;

    // scan the directory
    // - iterate all files in a directory
    //   - compute hashAsText for each file
    //   - if hashAsText exists already in the original files (map? because I want to store only unique files?)
    //      - add the file pathToDirectoryAsText to the duplicate files (multimap? because there might be multiple duplicate files with the same hashAsText?)
    //      - continue
    //   - add the file to the original files
    void scan();

    // sort files by path and list original files first, but perform sorting outside of the loop with std::sort, instead of sorting at insertion
    //  because of lower algorithmic complexity: N*N with sorting at insertion vs N [insertion] + N * log(N) [sorting] for separated insertion and sorting
    void sort();

    void findDuplicates();

    // tidy_up
    //   - move the duplicate file to trash OR make a directory named '.TO_TRASH' in the searched directory and move the duplicate file to the '.TO_TRASH' directory
    void moveDuplicatesToSeparateDirectory();

    // verify
    // - compare the number of entries in original files and the number of files in the searched directory
    void verifyFiles();

private:
    std::string pathToDirectoryAsText;

    std::vector<std::unique_ptr<File>> allFilesInDirectory;

    OriginalFilesType originalFiles;
    DuplicateFilesType duplicateFiles;

    std::string pathToDuplicateFilesDirectoryAsText;
};
