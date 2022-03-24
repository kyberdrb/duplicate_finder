#include <iostream>
#include <map>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

using std::cout;
using std::endl;

int main() {
    std::string path = "/home/laptop/backup-sony_xa1/apps";
    std::cout << path << std::endl;

    // look_up
    // - iterate all files in a directory
    //   - compute hash for each file
    //   - if hash exists already in the original files (map? because I want to store only unique files?)
    //      - add the file path to the duplicate files (multimap? because there might be multiple duplicate files with the same hash?)
    //      - continue
    //   - add the file path to the original files

    fs::path aPath {path};

    cout << "Parent path: " << aPath.parent_path() << endl;
    cout << "Filename: " << aPath.filename() << endl;
    cout << "Extension: " << aPath.extension() << endl;

    for (const auto& entry : fs::directory_iterator(aPath)) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_directory()) {
            std::cout << "dir:  " << filenameStr << '\n';
        } else if (entry.is_regular_file()) {
            std::cout << "file: " << filenameStr << '\n';
        } else
            std::cout << "??    " << filenameStr << '\n';
    }

    // clean_up
    // - iterate all duplicate files
    //   - display duplicate file path with hash
    //   - display original file path with the same hash from the original files
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
