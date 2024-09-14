cmake_minimum_required (VERSION 3.10)

# 프로젝트 이름 설정
project (cpp-server)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# include 디렉토리 설정
include_directories(${PROJECT_SOURCE_DIR}/include)

# 소스 파일 리스트 설정
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 출력 디렉토리 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(cpp-server ${SOURCES})
