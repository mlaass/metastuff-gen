# Metatool

Clang based templated code generator from C/C++ sources. Useful for serialization/deserialization, introspection and interfacing with embedded languages etc.

This is a fork of [metastuff-clang-generator](https://github.com/btaczala/metastuff-clang-generator)

Extensible using templates written in either xml or my own template format t8.

the templates subdirectory contains a selection of templates ready to use. currenlt supporting the following targets:
* [nlohmann::json](https://github.com/nlohmann/json) JSON for Modern C++
* [sol2](https://github.com/ThePhD/sol2) Lua API wrapper for C++
* [metastuff](https://github.com/eliasdaler/MetaStuff)

## Prerequisites for building

Cmake and [clang](https://clang.llvm.org/) & [llvm](https://llvm.org/) version 17 is required to be installed as generating is based on [libTooling](https://clang.llvm.org/docs/LibTooling.html) from LLVM/Clang.
Also gcc version 12 is expected (at least on my current system)
e.g. on linux

```sh
sudo apt update
sudo apt install llvm-17-dev libclang-17-dev gcc-12 g++-12
```

## Building

For building you need to have llvm & clang installed then standard cmake stuff:

```sh
git clone
cd metatool
git submodule update --init --recursive
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Usage
 ```sh
 cd build/
 ./metatool -t ../templates/nlohmann-json.t8 -o test-json ../test/test.cpp -- -std=c++20
 ```
If you run intro trouble with includes, you might need to a create a compilation database, which is a json file called `compile_commands.json`.

You can do that with with cmake like this: `cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON` you can pass the file into clang using `-p`

 ```sh
 ./metatool -t ../templates/nlohmann-json.xml -o test-json ../test/test.cpp -- -p /path/to/compile_commands.json
 ```

Or you need to figure out your includes are for clang and pass them to metatool command line. See example:

 ```sh
 ./metatool -p ../test -t ../templates/sol2-lua.xml -o test-lua  ../test/test.cpp -- -I/include/path/here -std=c++17
 ```

 where test.cpp:

 ```cpp
#include <iostream>

struct A {
  int a;
  float b;
  std::string s;
  void fun(int x){;}
};

/**
 * @meta-ignore-struct;
 * whole struct gets ignored
 */
struct B {
  int a;
  float b;
};

/**
 * @meta-ignore-members: b,c;
 * only specified members get ignored
 */
struct C {
  int a;
  // ignored
  float b;
  // ignored
  float c;
  // not ignored
  float d;

  B bB;
  A* pA;
};

int main() { std::cout << "hello" << std::endl; }
```

The generated code will be printed to stdout or to the file specified with the `-o` option
For given example the generated code is:

test-json.hpp
```cpp
/** autogenerated from
../test/test.cpp
 **/
#ifndef POOODAOUSRTIPO_HPP
#define POOODAOUSRTIPO_HPP

#include <json.hpp>


template <>  struct nlohmann::adl_serializer<A>  {
	static void to_json(nlohmann::json &j, const struct A &input);
	static void from_json(const nlohmann::json &j, A &output);
};
template <>  struct nlohmann::adl_serializer<C>  {
	static void to_json(nlohmann::json &j, const struct C &input);
	static void from_json(const nlohmann::json &j, C &output);
};


#endif POOODAOUSRTIPO_HPP
```

test-json.cpp
```cpp
/** autogenerated file: test-json.cpp**/
#include <json.hpp>
#include "test-json.hpp"

void nlohmann::adl_serializer<A>::to_json(nlohmann::json &j, const struct A &input) {

  j["a"] = input.a;
  j["ua"] = input.ua;
  j["b"] = input.b;
  j["db"] = input.db;
  j["cc"] = input.cc;
  j["isBool"] = input.isBool;
  j["s"] = input.s;

}

void nlohmann::adl_serializer<A>::from_json(const nlohmann::json &j, A &output) {

	output.a = j["a"].get<int>();
	output.ua = j["ua"].get<uint>();
	output.b = j["b"].get<float>();
	output.db = j["db"].get<double>();
	output.cc = j["cc"].get<char>();
	output.isBool = j["isBool"].get<bool>();
	output.s = j["s"].get<std::string>();

}
void nlohmann::adl_serializer<C>::to_json(nlohmann::json &j, const struct C &input) {

  j["a"] = input.a;
  j["d"] = input.d;
  j["bB"] = input.bB;
  j["pA"] = input.pA;

}

void nlohmann::adl_serializer<C>::from_json(const nlohmann::json &j, C &output) {

	output.a = j["a"].get<int>();
	output.d = j["d"].get<float>();
	output.bB = j["bB"].get<B>();
	output.pA = j["pA"].get<A *>();

}



```

## Detailed usage

```sh
$ metatool --help

USAGE: metatool [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

metatoolerator options:

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -p=<string>                - Build path
  -x=<filename>              - path to xml template file
  -t=<filename>              - path to t8 template file

-p <build-path> is used to read a compile command database.

	For example, it can be a CMake build directory in which a file named
	compile_commands.json exists (use -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	CMake option to get this output). When no build path is specified,
	a search for compile_commands.json will be attempted through all
	parent paths of the first input file . See:
	http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html for an
	example of setting up Clang Tooling on a source tree.

<source0> ... specify the paths of source files. These paths are
	looked up in the compile command database. If the path of a file is
	absolute, it needs to point into CMake's source tree. If the path is
	relative, the current working directory needs to be in the CMake
	source tree and the file must be in a subdirectory of the current
	working directory. "./" prefixes in the relative files will be
	automatically removed, but the rest of a relative path must be a
	suffix of a path in the compile command database.
```
