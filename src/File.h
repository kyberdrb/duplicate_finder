#pragma once

#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

class File {
public:
    File(std::filesystem::directory_entry absolutePath, std::string hash);

    std::string getAbsolutePath() const;

    // assuming all files have the same extension separated by a dot character '.'
    std::string getModifiedAbsolutePath() const;

    const std::string& getHash() const;

    void addDuplicateFile(std::reference_wrapper<const File> duplicateFile);

    bool hasDuplicates() const;

    friend std::ostream& operator<<(std::ostream& out, const File& file);

private:
    std::filesystem::directory_entry fileOnFilesystem;
    std::string hash;

    std::vector<std::reference_wrapper<const File>> duplicateFiles;
};

inline std::ostream& operator<<(std::ostream& out, const File& file) {
    out << file.getHash() << "\t" << file.getAbsolutePath() << "\n";

    // if file has any duplicated
    bool hasFileDuplicates = file.duplicateFiles.size() > 0;
    if (hasFileDuplicates) {
        // for each file in duplicate files
        for (const auto& duplicateFile : file.duplicateFiles) {
            out << "- duplicate file: "<< duplicateFile.get().getHash() << "\t" << duplicateFile.get().getAbsolutePath() << "\n";
        }
    }

    return out;
}
