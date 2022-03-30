//string sha256(const string str) {
//    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH, 0);
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);

    //SHA256_Update(&sha256_context, str.c_str(), str.size());

//    size_t bytes;
//    unsigned char data[1024];
//    FILE* file = fopen(filePath.c_str(), "rb");
//
//    while( (bytes = fread(data, 1, 1024, file) ) != 0 ){
//        SHA256_Update(&sha256_context, data, bytes);
//    }

//    size_t bytes;
//    //std::array<char, 1024> data;
//    unsigned char data[1024];
//
//    file.read(&data[0], 1024);
//    bytes = file.gcount();

    std::ifstream fin(filePath, std::ios::binary);
    std::vector<char> x(1024, 0);

    while (fin.read(x.data(), x.size()))
    {
        std::streamsize bytes = fin.gcount();

        SHA256_Update(&sha256_context, x.data(), bytes);
    }

    if (fin.gcount() != 0)
    {
        SHA256_Update(&sha256_context, x.data(), fin.gcount());
    }

    SHA256_Final(hash.data(), &sha256_context);

    std::stringstream result;
//    for(int chunkNumber = 0; chunkNumber < SHA256_DIGEST_LENGTH; chunkNumber++)
//    {
//        result << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)hash[chunkNumber];
//    }

    for(auto chunk : hash) {
        result << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)chunk;
    }

    return result.str();