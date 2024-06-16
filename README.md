# data-structure
Use C++ to implement some common data structures, such as array, hash table, linked list, doubly linked list, fully self-balancing tree, etc.
# Package management tools used
For details, please refer to:
[Conan](https://github.com/conan-io/conan)
# Use
first:
```Bash
conan install conanfile.py --build=missing --profile=default-debug
```
Then:
```Bash
 cmake --preset conan-debug
 ```
 Executable files in buid/Debug