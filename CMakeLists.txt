cmake_minimum_required(VERSION 3.16)
project(CryptoCompass VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Include paths ──────────────────────────────────────────────
include_directories(include)

# ── Source files ───────────────────────────────────────────────
set(SOURCES
    src/Crypto.cpp
    src/Database.cpp
    src/Menu.cpp
    src/RecommendationEngine.cpp
    src/Trie.cpp
)

# ── Main executable ────────────────────────────────────────────
add_executable(CryptoCompass src/main.cpp ${SOURCES})

# ── Google Test ────────────────────────────────────────────────
find_package(GTest QUIET)

if(GTest_FOUND)
    enable_testing()

    add_executable(all_tests
        tests/test_trie.cpp
        tests/test_database.cpp
        tests/test_recommendation.cpp
        ${SOURCES}
    )

    target_link_libraries(all_tests GTest::gtest GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(all_tests)

    message(STATUS "Google Test found -- tests enabled")
else()
    message(WARNING "Google Test not found -- skipping tests")
endif()
