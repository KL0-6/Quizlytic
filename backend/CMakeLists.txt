cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(AIFlashcardBackend LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -frtti")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries as shared libraries" FORCE)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG origin/master)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG origin/master)
FetchContent_MakeAvailable(nlohmann_json)

find_package(Drogon REQUIRED)

add_executable(AIFlashcardBackend
    main.cpp

    common/env/env.cpp
    common/base64/base64.cpp
    common/cryptography/cryptography.cpp

    routes/test/test.cpp
    routes/chat/chat.cpp
    routes/getflashcards/getflashcards.cpp
    routes/clerk/usercreated/usercreated.cpp
    routes/clerk/userdeleted/userdeleted.cpp
)

set_target_properties(AIFlashcardBackend PROPERTIES CXX_STANDARD 17)

target_link_libraries(AIFlashcardBackend PRIVATE 
    Drogon::Drogon 
    cpr::cpr
    nlohmann_json::nlohmann_json
)

target_include_directories(AIFlashcardBackend PRIVATE 
    ${nlohmann_json_SOURCE_DIR}/single_include
)