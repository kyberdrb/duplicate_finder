#pragma once

#include "Hash.h"

#include <filesystem>
#include <vector>

class File {
public:
    File(const std::filesystem::directory_entry fileInDirectory, std::unique_ptr<Hash> hash);

    std::string getFilename() const;

    std::string getAbsolutePath() const;

    std::string getModifiedAbsolutePath() const;

    const Hash& getHash() const;

    void addDuplicateFile(std::reference_wrapper<const File> duplicateFile);

    bool hasDuplicates() const;

    friend std::ostream& operator<<(std::ostream& out, const File& file) {
        out << file.getHash() << "\t" << file.getAbsolutePath() << "\n";

        bool hasFileDuplicates = file.duplicateFiles.size() > 0;
        if (hasFileDuplicates) {
            for (const auto& duplicateFile : file.duplicateFiles) {
                out << "- duplicate file: " << duplicateFile.get().getHash() << "\t" << duplicateFile.get().getAbsolutePath() << "\n";
            }
        }

        return out;
    }

private:
    std::filesystem::directory_entry fileInDirectory;
    std::unique_ptr<Hash> hash;

    std::vector<std::reference_wrapper<const File>> duplicateFiles;
};
