# Litan

Litan is a dynamically typed programming language with a C-like syntax.

## Hello World

```js
function main() {
    std::println("Hello World");
}
```

## Lamdas and Higher-Order-Functions

Litan has fully-featured lambda expressions and supports higher-order-functions.

```js
function use_lambda(fx) {
    return fx(1, 2);
}

function make_lambda(c) 
    => lambda[c](a, b) => (a + b + c)

function main() {
    std::println(use_lambda(make_lambda(3))); // 6
}
```

## Reflections

Reflections allow you to gather infos about your programm at compile-time.

```js
function main() {
    std::println("Line: " + std::str(reflect(line))); // Line 2
}
```

[Learn more](/doc/language/Reflection.md)

## Structs

Litan uses structs whose members can added or removed dynamically.

```js
function main() {
    var object = std::struct();
    object.foo = 42;
    object.bar = 1337;
    std::println(object.foo);
    std::println(object.bar);
}
```

## Useful Libraries

- [Sphinx](https://github.com/JeneLitsch/Sphinx) (Unit test framework)

## Build, Use and Install

### Requirements

- CMake 3.13 or newer
- A modern C++ compiler with C++20 support
  - Clang++-10 or newer
  - GCC-10 or newer
- stdxx library

### Use as Standalone

1. Clone the repository recursively.
2. Run `./build.sh` to compile the compiler, vm and everything needed. 
   - Note: Clang++ is choosen by default but this can be changed.
Just run `./build.sh COMPILER_GOES_HERE` in this case.
3. Run `./install.sh` to install ltn, ltnc, ltnvm and ltninfo into the /usr/local/bin/ directory.


## Special Thanks

Bob Nystrom: For his great book "Crafting Interpreters" which provided a large portion of the information I needed to realize this project. 