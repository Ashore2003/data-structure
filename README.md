# Data-structure

Use C++ to implement some common data structures, such as array,queue,trie_hash, hash table, linked list, doubly linked list, fully self-balancing tree, etc.
:warning:️ :construction: Thread safety is not yet supported
Use Conan as the package manager,For details, please refer to:
[Conan](https://github.com/conan-io/conan)

## How to Use?

first:

```Bash
conan install conanfile.py --build=missing --profile=default-debug
```

Then:

```Bash
 cmake --preset debug
 ```

## Note

 If you want to add multiple CMake preset configurations to your project, such as providing different configurations for different build types (such as Debug and Release) or different platforms, you can define more preset objects in the configurePresets array. Each preset object can specify different settings to suit specific build needs or environments.
 example:

 ```JSON
     "configurePresets": [
        {
            "name": "debug",
            "description": "Use Debug build settings with GNU Make and Conan integration",
            "hidden": false,
            "generator": "Unix Makefiles",
            "binaryDir": "${sourceDir}/build/Debug",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug",
                "CMAKE_POLICY_DEFAULT_CMP0091": "NEW",
                "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/build/Debug/generators/conan_toolchain.cmake"
            }
        },
        {
            "name": "release",
            "description": "Use Release build settings with GNU Make and Conan integration",
            "hidden": false,
            "generator": "Unix Makefiles",
            "binaryDir": "${sourceDir}/build/Release",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "CMAKE_POLICY_DEFAULT_CMP0091": "NEW",
                "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/build/Release/generators/conan_toolchain.cmake"
            }
        }
    ],
```

Then execute:

```Bash
cmake --preset debug or cmake --preset release
```

## Tasks

Implemented

- array
- vector
- list
- queue
- set
- trie
- hash table

Not implemented

- heap
- deque
- stack
- ring buffer
- some avl...
- There is no std implementation of the data structure, which can be used on MCUs such as Arduino and ESP32
- Data structures implemented in C language

## Final Thoughts

 Executable files and  Executable files in buid/Debug or build/Release
