# Zdefiniuj projekt o nazwie "...".
# Korzystaj ze standardu C++17
cmake_minimum_required(VERSION 3.8)
project(Net)

set(CMAKE_CXX_STANDARD 17)
# Ustaw flagi kompilacji
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

# Miejsca w których kompilator ma szukać plików nagłówkowych .h i .hpp
include_directories(include                     #nasz folder include
        googletest-master/googlemock/include    #foldery google test
        googletest-master/googletest/include)

# Tu będą wszystkie pliki .cpp projektu (na razie nie mamy żadnych)
set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        src/types.cpp)

# Tu będą pliki .cpp używane tylko podczas testów jednostkowych
set(TEST_SOURCES
        test/main_test.cpp) #plik z funkcją main() do testów

#dodaje bibliotekę Google Test, kompiluje ją z użyciem CMakeLists.txt z folderu googletest-master
add_subdirectory(googletest-master)
link_libraries(gmock)

#Tworzy konfigurację o nazwie konfig_run (nazwę możemy dowolnie zmienić)
# do której dołącza plik main.cpp z funkcją main()
add_executable(Net_Sim ${SOURCE_FILES} main.cpp)

#Tworzy konfigurację o nazwie konfig_test służącą do uruchamiania testów jednostkowych
add_executable(Net_Sim__test ${SOURCE_FILES} ${TEST_SOURCES})