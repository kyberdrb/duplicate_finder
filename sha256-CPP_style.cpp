std::string sha256(const std::string filePath) {
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    std::ifstream fin(filePath, std::ios::binary);
    std::vector<char> x(1024, 0);

    while ( fin.read(x.data(), x.size() ) {
        std::streamsize bytes = fin.gcount();
        SHA256_Update(&sha256_context, x.data(), bytes);
    }

    if (fin.gcount() != 0) {
        SHA256_Update(&sha256_context, x.data(), fin.gcount());
    }

    std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH, 0);
    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream result;
    for(auto chunk : hash) {
        result << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)chunk;
    }

    return result.str();
}