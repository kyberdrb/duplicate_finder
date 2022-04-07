#include "Directory.h"

#include <iostream>

#include <map>
#include <functional>

#include "HashGenerator.h"

int main(int argc, char** argv) {
    std::string pathToDirectory = argv[1];

    if (pathToDirectory.empty()) {
        std::cout << "Usage: duplicate_find <PATH_TO_DIRECTORY>";
        exit(1);
    }

    std::cout << pathToDirectory << std::endl;

    auto directory = std::make_unique<Directory>(std::move(pathToDirectory));
    directory->scan();
    directory->sort();
    directory->findDuplicates();

    // TODO delegate report generation on separate class 'ReportGenerator'
    // auto reportGenerator = std::make_unique<ReportGenerator>(directory);
    // reportGenerator->generateTerminalReport();
    // reportGenerator->generateFileReport();
    directory->generateReport();

    directory->moveDuplicatesToSeparateDirectory();

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
