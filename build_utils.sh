#!bin/bash

# dir name
project_dir_name="project"
build_dir_name="build"
include_dir_name="include"
src_dir_name="src"
bin_dir_name="bin"

# dir path
original_dir=$(pwd)
project_dir="${HOME}/${project_dir_name}"
include_dir="${project_dir}/${include_dir_name}"
src_dir="${project_dir}/${src_dir_name}"
build_dir="${project_dir}/${build_dir_name}"
bin_dir="${project_dir}/${bin_dir_name}"

function build() {
    cd "${project_dir}"
    if [ ! -d "${build_dir_name}" ]; then
        mkdir "${build_dir_name}"
    fi

    cmakelists_path="${project_dir}/CMakeLists.txt"
    cd "${build_dir}"
    if [ ! -f "${cmakelists_path}" ]; then
        echo "E : ${cmakelists_path} does not exist";
        exit 1
    fi

    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cd "${original_dir}"
}

function create_executable() {
    cd "${build_dir}"
    make
    cd "${original_dir}"
}
