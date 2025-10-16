cmake_minimum_required(VERSION 3.16)
project(RepetitiveCodeDetector VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Utils)
include_directories(${CMAKE_SOURCE_DIR}/Utils/Model)

# Source files
set(SOURCES
    main.cpp
    Utils/Model/SA.cpp
    Utils/Model/ST.cpp
    FileType/FileType.cpp
    FileType/Specific/FileType_CSharp.cpp
    FileType/Specific/FileType_Lua.cpp
    FileType/Specific/FileType_Shader.cpp
)

# Header files
set(HEADERS
    common.h
    Utils/Model/SA.h
    Utils/Model/ST.h
    FileType/FileType.h
    FileType/Specific/FileType_CSharp.h
    FileType/Specific/FileType_Lua.h
    FileType/Specific/FileType_Shader.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Windows-specific settings
if(WIN32)
    if(MSVC)
        # Set console code page to UTF-8 for proper Chinese character display
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
    else()
        # For MinGW, use different linker flags
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "-Wl,--subsystem,console"
        )
    endif()
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Create a simple test target
enable_testing()

# Add custom target for running the detector
add_custom_target(run_detector
    COMMAND ${PROJECT_NAME} ${CMAKE_SOURCE_DIR}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running repetitive code detector on source directory"
)

# Print configuration info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
