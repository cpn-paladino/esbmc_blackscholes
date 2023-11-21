# Minimum cmake version
cmake_minimum_required(VERSION 3.10)
project(ESBMC_BlackScholes)

# Require C++14 or higher
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
elseif(CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 14 or higher")
endif()

# CASE WINDOWS
if(MSVC)
    # For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
    cmake_policy(SET CMP0091 NEW)

    # Configure MSVC runtime library settings for Debug and Release builds using MTd
    set(CMAKE_BUILD_TYPE MTd_StaticDebug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

    # --- WINDOWS LOCAL settings ---
    # You must adjust according to your environment

    # Set Boost lib path
    set(Boost_INCLUDE_DIR "C:/local/boost_1_81_0")    
    # Set the Quantlib PATH according your machine
    set(QUANTLIB_PATH "D:/workspace/cplusplus/QuantLib-1.32")
    # Set the Quantlib NAME according your machine (check the debug name)
    set(QUANTLIB_LIB_NAME "QuantLib-x64-mt-sgd")

    # include Boost
    include_directories(${Boost_INCLUDE_DIR})
    # Headers, libs of QuantLib
    include_directories(${QUANTLIB_PATH})
    link_directories(${QUANTLIB_PATH}/build/ql/Debug)

# CASE LINUX
else()
    # Check Boost
    find_package(Boost REQUIRED)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif()

    # Check QuantLib
    find_package(QuantLib REQUIRED)
    if(QuantLib_FOUND)
        include_directories(${QuantLib_INCLUDE_DIRS})
        link_directories(${QuantLib_LIBRARY_DIRS})
    endif()
endif()

# Create exe for current project
add_executable(ESBMC_BlackScholes src/black_scholes_check.cpp)


# Link with the QuantLib
# CASE WINDOWS
if(MSVC)
    target_link_libraries(ESBMC_BlackScholes ${QUANTLIB_LIB_NAME})
else()
    target_link_libraries(ESBMC_BlackScholes ${QuantLib_LIBRARIES})
endif()