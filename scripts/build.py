#!/usr/bin/env python3
import subprocess, os
from utils.pushd import pushd
from pathlib import Path

build_dir = "build"
Path(build_dir).mkdir(parents=True, exist_ok=True)

with pushd(build_dir):
    print("Installing dependencies...")
    subprocess.run('conan install --build=missing -s compiler.libcxx=libstdc++11 ..'.split(),
            check=True)
    print("\nConfiguring build...")
    subprocess.run([
        'cmake',
        '-DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake',
        '-DCMAKE_MODULE_PATH=' + os.getcwd(),
        '-DSANITIZE_ADDRESS=ON',
        '-DSANITIZE_UNDEFINED_BEHAVIOR=ON',
        '..'],
        check=True)
    print("\nBuilding...")
    subprocess.run(['cmake', '--build', '.'], check=True)
    print("\nExecuting tests...")
    subprocess.run(['cmake', '--build', '.', '--target', 'test'], check=True)

