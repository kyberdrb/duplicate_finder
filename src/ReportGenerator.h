#pragma once

#include "Directory.h"

#include <functional>

class ReportGenerator {
public:
    explicit ReportGenerator(const std::reference_wrapper<const Directory> directory);

    // generate report
    // - iterate all duplicate files
    //   - display duplicate file pathToDirectoryAsText with hashAsText
    //   - display original file pathToDirectoryAsText with the same hashAsText from the original files
    void generateTerminalReport() const;

private:
    std::reference_wrapper<const Directory> directory;
};
