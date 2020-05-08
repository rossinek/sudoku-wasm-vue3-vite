# WASM Sudoku Generator

Legacy C project from school compiled to WebAssembly with [Vue 3](https://github.com/vuejs/vue-next) UI built with [vite](https://github.com/vuejs/vite).

## Let's play sudoku (demo)

https://rossinek.github.io/sudoku-wasm-vue3-vite/

## Technologies

- legacy C code
- [emscripten](https://emscripten.org/docs/getting_started/downloads.html) – compile C code into js/wasm
- [vuejs/vue-next](https://github.com/vuejs/vue-next) – Vue 3.0 (currently in beta)
- [vuejs/vite](https://github.com/vuejs/vite) – Great development experience provider 🔥
- TypeScript – no configuration is needed with Vue 3.0 + vite setup 💚

## Build and development

### Compile C core

Note: you can skip this step. Compiled modules are attached in repository.

Follow [Emscripten SDK installation instructions](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions).

After installation run
```sh
make
```

### Web app

```sh
# install dependencies
yarn

# run blazing fast dev server (vite)
yarn dev

# build (vite)
yarn build
```

## Background

I remembered that a few years ago I did a sudoku generator as a student project.
It was a time when I started my programming adventure and that was the first "big project" I made.
Apart from the sentimental aspect I like this project also because it is quite good when it comes to performance (even though the algorithm is quite naive).
One of the tricks that made the code work super fast is the use of very cheap bit operations. I described it a little "bit" (🤓) below.

## About C code and a performance trick

There are 3 basic rules that defines valid (fully completed) sudoku (`n × n`):

1. Each row contains all numbers from `1` to `n` without repetition
2. Each column contains all numbers from `1` to `n` without repetition
3. Each "small square" contains all numbers from `1` to `n` without repetition

While generating sudoku it is important to know what are the valid numbers for a given field.
The easiest way to get such information for a field `(x,y)` (according to those three rules) is to get
set of missing numbers in row `y` (lets call it `R(y)`), set of missing numbers in column `x` (lets call it `C(x)`) and set of missing numbers in small square (lets call it `S(x,y)`)
and compute intersection of those sets: `R(y) ⋂ C(y) ⋂ S(x,y)`.
This sudoku generator implementation often performs such an operation therefore it should be very fast.

And it is. Explanation by example:

Let assume that there is a sudoku like this and we want to find `X` (field `(6,3)`):

```
     1 2 3   4 5 6   7 8 9
   +-------+-------+-------+
1  | 5     |   8   |   4 9 |
2  |       | 5     |   3   |
3  |   6 7 | 3   X |     1 |
   +-------+-------+-------+
4  | 1 5 8 |       |       |
5  |       | 2   8 |       |
6  |       |       |   1 8 |
   +-------+-------+-------+
7  | 7     |     4 | 1 5   |
8  |   3   |     2 |       |
9  | 4 9   |   5   |     3 |
   +-------+-------+-------+
```

Sets of missing numbers in row `3`, column `6` and third small square looks like this:

```
R(3) =   [    2     4  5        8  9 ]
C(6) =   [ 1     3     5  6  7     9 ]
S(6,3) = [ 1  2     4     6  7     9 ]
```

This weird formatting is for a purpose – our intersection `R(3) ⋂ C(6) ⋂ S(6,3)` is the set with values that are repeated in all 3 sets.
We can easily find out that it is `[ 9 ]` so the only valid answer is that `X = 9`.

Ok, "we" can easily find it out, but what about computer? How to keep those sets in memory and compute this intersection in the fastest possible way?

The trick here is that one set can be represented as a regular integer. To be more specific: binary representation of such integer.
In C world 32 bit integer is just an ordered list of 32 boolean values. Using this property let's save our sets in a slightly different way:
```
binary(R(3)) =   010110011
binary(C(6)) =   101011101
binary(S(6,3)) = 110101101
```
where `1` at position `i` in binary representation `binary(X)` of set `X` means that number `i ∈ X`.

Now there is a time for magic:

```
binary(R(3)) & binary(C(6)) & binary(S(6,3)) = binary(R(3) ⋂ C(6) ⋂ S(6,3))
```
where `&` is binary AND operation.

Why is it so amazing?

Binary AND operation is one of the basic CPU operations which means it takes literally no time to execute 💃
