// TODO template this for algorithm type - maybe later, when I will create a library for hashing which will be much simpler to use than the raw Crypto++ or OpenSSL libraries, like this for example: std::string sha256HashOfFile = Hasher::sha256sum(filePath);
std::string sha256_CPP_style(const std::string filePath) {
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    std::ifstream file(filePath, std::ios::binary);
    const size_t CHUNK_SIZE = 1024;
    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire usage
    //std::vector<char> chunkBuffer(CHUNK_SIZE, '\0');
    std::array<char, CHUNK_SIZE> chunkBuffer{'\0'};

    while ( file.read( chunkBuffer.data(), chunkBuffer.size() ) ) {
        auto bytesRead = file.gcount();
        SHA256_Update(&sha256_context, chunkBuffer.data(), bytesRead);
    }

    // Evaluate the last partial chunk
    // `fin.read(...)` evaluates to false on the last partial block (or eof). You need to process partial reads outside of the while loop `gcount()!=0` - https://stackoverflow.com/questions/35905295/reading-a-file-in-chunks#comment59488065_35905524
    if (file.gcount() != 0) {
        SHA256_Update(&sha256_context, chunkBuffer.data(), file.gcount());
    }

    // preferring array instead of vector because the size of the buffer will stay the same throughout the entire usage
    //std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH, '\0');
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash{'\0'};
    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream hashInHexadecimalFormat;
    for(auto chunk : hash) {
        hashInHexadecimalFormat << std::hex << std::setw(2) << std::setfill('0') << (uint32_t) chunk;
    }

    return hashInHexadecimalFormat.str();
}

int main() {
    std::string basePath = "/home/laptop/backup-sony_xa1/apps";
    fs::path aPath {basePath};

    for (const auto& entry : fs::directory_iterator(aPath)) {
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            std::cout << "file: " << absolutePathForFile << '\n';

            std::string shaAsString = sha256_CPP_style(absolutePathForFile);
            std::cout << "sha256-cpp:     " << shaAsString << '\n';

            std::cout << "---" << '\n';
        }
    }
}