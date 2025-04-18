# <img src="doc/colgm.svg" height="45px"/> Colgm Compiler Project

[![bootstrap](https://github.com/colgm/colgm/actions/workflows/ci.yml/badge.svg)](https://github.com/colgm/colgm/actions/workflows/ci.yml)
[![nightly-build](https://github.com/colgm/colgm/actions/workflows/release.yml/badge.svg)](https://github.com/colgm/colgm/actions/workflows/release.yml)

```rust
use std::libc::puts;

pub func main() -> i32 {
    puts("hello world!");
    return 0;
}
```

## Repo Content

- [bootstrap](./bootstrap/README.md) : bootstrap compiler.
- [src](./src/README.md) : self-host compiler.
- [test](./test): test cases.

## Tutorial

Welcome to try. See simple tutorial in [bootstrap/README.md](./bootstrap/README.md).

## Bootstrap Compiler

- [macos-aarch64-nightly-build](https://github.com/colgm/colgm/releases/tag/macOS_nightly)

- [linux-x86_64-nightly-build](https://github.com/colgm/colgm/releases/tag/linux_nightly)

- [windows-x86_64-nightly-build __[WIP]__](https://github.com/colgm/colgm/releases/tag/windows_nightly)

Bootstrap compiler locates at `{repo_path}/bootstrap`, written by C++(`-std=c++17`):

Use these commands to build the bootstrap compiler, use `-DCMAKE_BUILD_TYPE=Debug` if want debug version:

```sh
mkdir build && cd build
cmake ../bootstrap -DCMAKE_BUILD_TYPE=Release && make -j6
```

The executable is located as `build/colgm`.

Then use this command to build self-host compiler:

```sh
make colgm.ll
```

The self-host compiler should be `{repo_path}/colgm.ll`.
If having `lli`(LLVM JIT interpreter), you could try this to
execute self-host compiler:

```sh
lli colgm.ll main.colgm --library .
```

LLVM JIT interpreter maybe unstable on some platforms,
if `lli` crashed, try using `clang` to build to executable
(suggested):

```sh
clang colgm.ll -o colgm
```

If want to make `panic` print stack trace with function name, remember to use `-rdynamic`:

```sh
clang -rdynamic colgm.ll -o colgm
```

On macOS this option is not necessary because attribute
`frame-pointer=non-leaf` is added after function definition.

## Self-Host Compiler

Learn more about [Self-host compiler](./src/README.md)

## Features and Roadmap

1. support foreach/forindex:
    - forindex loop, container should have `size()` method
    - foreach loop, container should have iterator-like stuff
        - `iter()`, `next()` and `is_end()` method
2. array type like `[i32; 128]`
    - syntax like `var a: [i32; 128] = [1, 2, 3];`
    - syntax like `var a: [i8*; 128] = ["foo", "bar", tmp];`
3. feature: [tagged union](./doc/spec/tagged_union.md)
4. feature: [tuple](./doc/spec/tuple.md)
