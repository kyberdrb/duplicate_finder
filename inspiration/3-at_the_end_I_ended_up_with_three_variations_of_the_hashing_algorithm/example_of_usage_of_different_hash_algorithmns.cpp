void Directory::scan() {
    fs::path aPath {this->getPathToDirectory()};

    std::unique_ptr<HashGenerator> hashGenerator{};

//    std::cout << "Parent pathToDirectoryAsText: " << aPath.parent_path() << "\n";
//    std::cout << "Filename: " << aPath.filename() << "\n";
//    std::cout << "Extension: " << aPath.extension() << "\n";

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

            // C-style hashAsText generation
//            char* sha1AsString = sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );
//            auto fileInDirectory = std::make_unique<File>(entry, sha1AsString);

            // for 'sha1AsString' as 'char*'
//            if (sha1AsString != NULL) {
//                free(sha1AsString);
//                sha1AsString = NULL;
//            }

            // C-Plus-Plus-ified C-style hashAsText generation
//            shaAsString = hashGenerator->sha1_C_style_dynamic_alloc(absolutePathForFile.c_str() );

            //auto fileInDirectory = std::make_unique<File>(entry, shaAsString);

            // TODO test object type Hash hashAsText and use it instead of string type hashAsText
            auto hashAsText = std::make_unique<Hash>(shaAsString);
            auto fileInDirectory = std::make_unique<File>(entry, std::move(hashAsText));

            this->allFilesInDirectory.emplace_back(std::move(fileInDirectory));
        }
        //else if (entry.is_directory()) {
        //    std::cout << "dir:  " << filenameStr << '\n';
        //}
        //else
        //    std::cout << "??    " << filenameStr << '\n';
    }
}