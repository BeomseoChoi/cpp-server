cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 설정
project(cpp-server)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include 디렉토리 설정
include_directories(${PROJECT_SOURCE_DIR}/include)

# pugixml include 디렉토리 추가
include_directories(${PROJECT_SOURCE_DIR}/lib/pugixml-1.14/include)

# 재귀적으로 src 디렉토리와 하위 디렉토리의 모든 .cpp 파일 찾기
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# pugixml 소스 파일 추가
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/lib/pugixml-1.14/src/pugixml.cpp)
include_directories(${PROJECT_SOURCE_DIR}/lib/pugixml-1.14/include)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/lib/xxml/src/xxml.cpp)
include_directories(${PROJECT_SOURCE_DIR}/lib/xxml/include)

# 출력 디렉토리 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 실행 파일 생성
add_executable(cpp-server ${SOURCES})

# PCH 파일 설정 (PROJECT_SOURCE_DIR을 이용하여 include 디렉토리 경로를 설정)
target_precompile_headers(cpp-server PRIVATE ${PROJECT_SOURCE_DIR}/include/pch.h)
