cmake_minimum_required(VERSION 3.21)
project(duplicate_finder)

set(CMAKE_CXX_STANDARD 17)

# linking 'openssl' library: https://cppsecrets.com/users/41129711010797106994610011511264103109971051084699111109/OpenSSL-generate-hash-of-the-file.php
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcrypto")

# linking '-lssl' unnecessary: at function 'SHA1_INIT' the linker still produces an error "/usr/bin/ld: CMakeFiles/duplicate_finder.dir/src/HashGenerator.cpp.o: undefined reference to symbol 'SHA1_Init@@OPENSSL_1_1_0'"
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcrypto -lssl")

# linking 'std::filesystem' library: https://stackoverflow.com/questions/55318924/how-to-avoid-stdfilesystem-linker-errors-with-qt#55328592
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcrypto -lssl -lstdc++fs")

add_executable(duplicate_finder src/main.cpp src/File.cpp src/File.h src/HashGenerator.cpp src/HashGenerator.h src/Directory.cpp src/Directory.h src/ReportGenerator.cpp src/ReportGenerator.h src/HashComparatorAscending.h src/Hash.cpp src/Hash.h)
