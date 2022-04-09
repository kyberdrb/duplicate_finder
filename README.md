# duplicate_finder

Finds duplicate files in a single directory non-recursively/shallowly.

The duplicate files are moved to a separate directory.

A report is generated and saved in the separate directory with duplicate files. The report shows the name/path of the duplicate file and its corresponding original file.

## Dependencies

- compiler supporting C++17 standard or newer
- packages: `openssl` for generating hash

## Design

- HashGenerator
- File
- Hash
- Directory
- FilePathsComparator - for vector of all files in dir
- StringComparatorAscending - for string/object-type keys in maps
- DuplicateFilesHandler
- ReportGenerator

## Hints

- Sledovat typovu zhodu a konstantnost typu pre:  
typ elementu v kontaineri `<=>` typ elementu v cykle pri iterovani `<=>` typ elementu v navratovej hodnote `<=>` typ elementu pri vkladani `<=>` typ parametra vo funkcii v ktorej element chceme pouzit
- `reference_wrapped` prijma aj rvalue referencie
    
      duplicateFiles.emplace(file->gethash(), *(file.get()));
- These routines for erasing character from string are equivalent

    Remove characters with `replace`

      int numberOfCharacters = 1;
      while ((position = modifiedAbsolutePath.find("(")) != std::string::npos) {
          modifiedAbsolutePath.replace(position, 1, "");
      }

    Remove characters with `erase`

      while ((position = modifiedAbsolutePath.find(")")) != std::string::npos) {
          modifiedAbsolutePath.erase(position, numberOfCharacters);
      }

## Sources

- filesystem
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+iterate+all+files+in+directory&ia=web
    - https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c/
    - https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c/56376599#56376599
    - https://www.cppstories.com/2019/04/dir-iterate/
    - https://www.cppstories.com/2019/04/dir-iterate/#using-c17
    - https://duckduckgo.com/?q=c%2B%2B+include+filesystem+example&t=ffab&ia=web
    - https://carlosvin.github.io/langs/en/posts/recursive-directory-iterator/
    - https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c/32889434#32889434
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+filesystem+move+file&ia=web
    - https://stackoverflow.com/questions/22201663/find-and-move-files-in-c
    - https://stackoverflow.com/questions/22201663/find-and-move-files-in-c/48614612#48614612
    - https://duckduckgo.com/?t=ffab&q=std%3A%3Afilesystem+c%2B%2B+linker&ia=web&iax=qa
    - https://stackoverflow.com/questions/55318924/how-to-avoid-stdfilesystem-linker-errors-with-qt#55328592
- hash
    - **https://duckduckgo.com/?t=ffab&q=c%2B%2B+sha256+checksum&ia=web** - according to this I decided to use OpenSSL library to generate checksum, because `openssl` package is a dependency for `openssh` package, which is more widely used
    - https://stackoverflow.com/questions/31586701/generate-sha256-in-c
    - https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+open+binary+file+as+plain+text+fstream&ia=web&iax=qa
    - https://duckduckgo.com/?t=ffab&q=convert+ifstream+to+string&ia=web&iax=qa
    - https://stackoverflow.com/questions/13551911/read-text-file-into-string-c-ifstream
    - https://www.codegrepper.com/code-examples/cpp/C%2B%2B+ifstream+to+string
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+undefined+reference+to+%60SHA256_Init%27+openssl&ia=web
    - https://stackoverflow.com/questions/10085945/set-cflags-and-cxxflags-options-using-cmake
    - https://stackoverflow.com/questions/13784434/how-to-use-openssls-sha256-functions
    - https://stackoverflow.com/questions/15100351/changing-cmake-cxx-flags-in-project
    - https://cryptopp.com/wiki/HMAC
    - https://cryptopp.com/docs/ref/class_s_h_a256.html
    - https://archlinux.org/packages/community/x86_64/crypto++/
    - https://archlinux.org/packages/?sort=&q=crypto&maintainer=&flagged=
    - https://archlinux.org/packages/extra/x86_64/botan/
    - https://aur.archlinux.org/packages?O=0&SeB=nd&K=crypto&outdated=&SB=p&SO=d&PP=50&submit=Go
    - https://duckduckgo.com/?q=crypto%2B%2B+example+hash+file&t=ffab&ia=web
    - **https://cpp.hotexamples.com/examples/cryptopp/SHA512/-/cpp-sha512-class-examples.html#0x4505bc1003699f447c397e8c358071ae1c57f38b04bd859368e15592cc6ebe1f-13,,27,** - another alternative to generate SHA hash from file: with Crypto++ library
    - https://stackoverflow.com/questions/62216156/how-to-generate-sha256-hashes-of-hexadecimal-binary-data-in-c
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+sha256sum&ia=web
    - http://www.zedwood.com/article/cpp-sha256-function
    - https://duckduckgo.com/?q=crypto%2B%2B+example+hash+std%3A%3Afilesystem+path+file&t=ffab&ia=web
    - https://en.cppreference.com/w/cpp/filesystem/path/hash_value
    - https://stackoverflow.com/questions/53021188/hash-file-recursive-and-save-into-vector-cryptopp
    - https://duckduckgo.com/?q=c%2B%2B+hash+openssl+fstream&t=ffab&ia=web
    - **https://cppsecrets.com/users/931049711497106109971151165748485664103109971051084699111109/C00-Program-to-Find-Hash-of-File.php** - one of the examples that shaped the final C algorithm
    - **https://cppsecrets.com/users/41129711010797106994610011511264103109971051084699111109/OpenSSL-generate-hash-of-the-file.php** - helpful for linking libraries (adding options for linking to linker in CMake) for successful compiling
    - https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    - **https://duckduckgo.com/?q=cmake+lcrypto&t=ffab&ia=web** - because I use CMake for this project, and Crypto++ library has problems with CMake apparently, I decided to stay with OpenSSL for hash generating 
    - https://duckduckgo.com/?q=cmake+add+link+flags&t=ffab&ia=web
    - https://stackoverflow.com/questions/11783932/how-do-i-add-a-linker-or-compile-flag-in-a-cmake-file
    - **https://duckduckgo.com/?t=ffab&q=c%2B%2B+hash+binary+file&ia=web** - finally this search offered me a link with a accurate implementation of the SHA algorithm (SHA1)
    - **https://stackoverflow.com/questions/29416549/getting-hash-of-a-binary-file-c/29418717#29418717** - **_the base source code for the generation of the SHA hash for binary files_**
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+fread+fstream&ia=web
    - https://stackoverflow.com/questions/13487461/fread-equivalent-with-fstream
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+read+file+by+chunks&ia=web
    - **https://stackoverflow.com/questions/35905295/reading-a-file-in-chunks/35905524#35905524** - **one of the source codes that helped me to rewrite the base C hash algorithm into C++**
    - **https://stackoverflow.com/questions/35905295/reading-a-file-in-chunks#comment59488065_35905524** - **_this comment helped me to adjust the C++ hash algorithm for accuracy by taking consideration for the last bytes into the hash update algorithm_**
    - https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
    - https://www.programiz.com/cpp-programming/library-function/cstdio/sprintf
    - **https://duckduckgo.com/?q=c+corrupted+size+vs.+prev_size+sprintf&t=ffab&ia=web&iax=qa** - this brought me to make a self-reflection and take into consideration that the array of characters needs one place more for a null terminator character in order to be a correct string representation in C - confirmed with the link _https://cppsecrets.com/users/931049711497106109971151165748485664103109971051084699111109/C00-Program-to-Find-Hash-of-File.php_
    - https://duckduckgo.com/?t=ffab&q=uint8_t+header&ia=web
    - https://www.cplusplus.com/reference/cstdint/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+array&ia=web
    - https://en.cppreference.com/w/cpp/container/array
- vector, sorting, comparators
    - https://duckduckgo.com/?t=ffab&q=create+empty+vector+c%2B%2B&ia=web&iax=qa
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+create+custom+comparator+sort&ia=web
    - https://stackoverflow.com/questions/16894700/c-custom-compare-function-for-stdsort
    - https://www.technicalkeeda.in/2020/10/Sorting-a-vector-of-custom-objects-using-Cpp-stl.html
    - https://duckduckgo.com/?q=sort+vector+custom+comparator&t=ffab&ia=web
    - https://newbedev.com/typescript-c-sort-vector-with-custom-comparator-code-example
    - https://duckduckgo.com/?q=c%2B%2B+vector+sort+at+insert+emplace+push&t=ffab&ia=web
    - https://www.geeksforgeeks.org/vector-emplace-function-in-c-stl/
    - https://duckduckgo.com/?t=ffab&q=iterator+to+int&ia=web&iax=qa
    - https://stackoverflow.com/questions/26995725/how-to-convert-vector-iterator-to-int-in-c#26995815
    - 
- map
    - https://en.cppreference.com/w/cpp/container/map
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+example&ia=web
    - https://www.freecodecamp.org/news/c-plus-plus-map-explained-with-examples/
    - https://www.cplusplus.com/reference/map/map/
    - https://www.cplusplus.com/reference/map/map/key_comp/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+map+iterate+for+each&ia=web&iax=qa
    - https://stackoverflow.com/questions/2850312/use-of-for-each-on-map-elements#23891386
    - https://stackoverflow.com/questions/2850312/use-of-for-each-on-map-elements/2850354#2850354
    - https://en.cppreference.com/w/cpp/language/structured_binding
- map with `cref`, `reference_wrapper` and a comparator for custom objects
    - https://duckduckgo.com/?q=c%2B%2B+std%3A%3Aref&t=ffab&ia=web
    - https://stackoverflow.com/questions/7707842/stl-containers-with-reference-to-objects
    - https://duckduckgo.com/?q=c%2B%2B+std+reference_wrapper&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+std+reference_wrapper+map&t=ffab&ia=web
    - https://www.cplusplus.com/reference/map/map/
    - https://stackoverflow.com/questions/27174982/use-stdreference-wrapper-in-stdmap
    - https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+map+string+reference+wrapper&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+as+key+map&ia=web
    - https://stackoverflow.com/questions/8374806/stdstring-as-a-key-in-stdmap-using-a-compare-operator
    - https://stackoverflow.com/questions/13858894/c-string-map-key
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+custom+comparator+string&ia=web
    - https://stackoverflow.com/questions/5733254/how-can-i-create-my-own-comparator-for-a-map
    - https://stackoverflow.com/questions/5733254/how-can-i-create-my-own-comparator-for-a-map/5733353#5733353
    - https://stackoverflow.com/questions/5733254/how-can-i-create-my-own-comparator-for-a-map/48613405#48613405
    - https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=msvc-170
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+comparator+lambda&ia=web
    - **https://stackoverflow.com/questions/19461355/stdmap-with-lambda-comparator/19461799#19461799** - abbreviating longer typenames in C\+\+ with an alias
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+lambda+compare+comparator+a+lambda+closure+type+has+a+deleted+default+constructor&ia=web
    - https://stackoverflow.com/questions/14896032/c11-stdset-lambda-comparison-function
    - https://stackoverflow.com/questions/14896032/c11-stdset-lambda-comparison-function/14896195#14896195
    - https://stackoverflow.com/questions/32236599/using-reference-to-lambda-as-comparator-in-a-map-the-right-way
    - https://stackoverflow.com/questions/32236599/using-reference-to-lambda-as-comparator-in-a-map-the-right-way/32237957#32237957
- map find value by key
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+find+key&ia=web
    - https://www.thecodeteacher.com/howto/1454/C++---How-To--Find-a-Key-in-C++-Map
    - https://www.cplusplus.com/reference/map/map/find/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+check+if+map+contains+key&ia=web
    - https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key#11765524
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+get+element+by+key&ia=web
    - https://stackoverflow.com/questions/10699689/how-can-i-get-a-value-from-a-map
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+const_cast&ia=web
    - https://www.ibm.com/docs/en/zos/2.3.0?topic=expressions-const-cast-operator-c-only
- string
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+delete+last+characters+of+string&ia=web
    - https://stackoverflow.com/questions/2310939/remove-last-character-from-c-string
    - https://stackoverflow.com/questions/2310939/remove-last-character-from-c-string#16630332
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+erase+characters+string&ia=web
    - https://www.cplusplus.com/reference/string/string/erase/
    - https://stackoverflow.com/questions/10111279/how-to-erase-characters-in-a-string
    - http://zditect.com/guide/cpp/string-erase-in-cpp.html
- C\+\+ and object-oriented specific nuances
  - https://duckduckgo.com/?t=ffab&q=c%2B%2B+overload+less+than+operator&ia=web&iax=qa
  - https://stackoverflow.com/questions/18646526/overloading-less-than-operator#18646541
  - https://en.cppreference.com/w/cpp/language/type_alias
  - https://duckduckgo.com/?q=const+string+is+const+top+level+readability&t=ffab&ia=web
  - https://stackoverflow.com/questions/63972019/understanding-const-at-top-level-which-may-reduce-code-readability-without-i
  - https://stackoverflow.com/questions/63972019/understanding-const-at-top-level-which-may-reduce-code-readability-without-i/63972327#63972327
  - https://duckduckgo.com/?t=ffab&q=member+stream+output+operator&ia=web
  - https://docs.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170
  - https://duckduckgo.com/?t=ffab&q=multiple+definition+of+%60operator%3C%3C(std%3A%3Aostream%26&ia=web
  - https://stackoverflow.com/questions/12802536/c-multiple-definitions-of-operator
  - https://www.cplusplus.com/reference/cassert/assert/
  - 
