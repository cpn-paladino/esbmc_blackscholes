cmake_minimum_required(VERSION 3.10)

# For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
cmake_policy(SET CMP0091 NEW)

project(ESBMC_BlackScholes)

# Fix Mtd compiler
set(CMAKE_BUILD_TYPE MTd_StaticDebug)

# --- LOCAL settings ---
# You must adjust according to your environment


# Set the Quantlib PATH according your machine
set(QUANTLIB_PATH "D:/workspace/cplusplus/QuantLib-1.32")

# Set the Quantlib NAME according your machine
set(QUANTLIB_LIB_NAME "QuantLib-x64-mt-sgd")

# Set Boost lib path
set(Boost_INCLUDE_DIR "C:/local/boost_1_81_0")

# --------------

# Require C++14 or higher
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
elseif(CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 14 or higher")
endif()


if (NOT DEFINED QL_BOOST_VERSION)
    # Boost 1.75.0 or greater required for compiling with C++20
    if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        set(QL_BOOST_VERSION 1.75.0)
    else()
        set(QL_BOOST_VERSION 1.58.0)
    endif()
endif()

# Configure MSVC runtime library settings for Debug and Release builds using MTd
if(MSVC)  
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# include Boost
include_directories(${Boost_INCLUDE_DIR})

# Headers, libs of QuantLib
include_directories(${QUANTLIB_PATH})
link_directories(${QUANTLIB_PATH}/build/ql/Debug)

# Create exe for current project
add_executable(ESBMC_BlackScholes src/black_scholes_check.cpp)

# Link with the QuantLib
target_link_libraries(ESBMC_BlackScholes ${QUANTLIB_LIB_NAME})