# ECE650 : Assignment 2

Replace FIRST_NAME, LAST_NAME, WATIAM, and STUDENT_ID, EMAIL in
`user.yml` by your first and last name, WatIAM id, student number, and
email, respectively. Whenever possible, please use ASCII characters.

Do not change the format `user.yml`. We will parse it
automatically. Only enter the information requested.

The main file for your solution to the assignment should be
`ece650-a2.cpp`. You can use `gtest_example.cpp` for your unit tests.

You might need to modify `CMakeLists.txt` if you are adding additional
source files for your code and/or tests.

Commit your changes and submit on GitHub.

## Compilation Instructions

### Using default compiler
```bash

$ mkdir build 
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ../
$ make
```

### Using Clang compiler

```bash
$ mkdir build 
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../
$ make
```

### With AddressSanitizer

AddressSanitizer is a run-time checker that monitors your program for potential
memory errors. It is very useful for thoroughly testing your C/C++ code at the
early stages of development. It is recommended that you always compile with
AddressSantizer enabled and fix any of the errors that it uncovers.


```bash
$ mkdir build 
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DWITH_ASAN=ON ../
$ make
```

