# 16 bit virtual gameboy

## UPDATE

### It has been confirmed that the previous MakeFile worked on windows as well, So this "CMake Transfer" has been paused for an indefinite amount of time

## AIM

-   A fun project where we aim to create a virtual machine with it's own instruction set, a language in that instruction set and then a simple game(Tetris?) in that language

## References

-   [Tsoding](https://www.youtube.com/playlist?list=PLpM-Dvs8t0VY73ytTCQqgvgCWttV3m8LM)
-   [Dr Birch](https://www.youtube.com/@dr-Jonas-Birch)
-   [Low Byte Productions](https://www.youtube.com/playlist?list=PLP29wDx6QmW5DdwpdwHCRJsEubS5NrQ9b)
-   [Cobb Coding](https://www.youtube.com/playlist?list=PLRnI_2_ZWhtCxHQ_3zDfW0-RgiWo8ftyj)

## PBL

-   [Weekly Report](https://docs.google.com/spreadsheets/d/1VjYzkm8S0E-dAcf4HTZTpgma7ViCjvB43XC4-JkVbVo/edit?gid=449988072#gid=449988072)

## Commands

-   CMakeLists is in the gbvm folder

```
cd gbvm
```

-   Creates a build folder in current directory which will contains executables

```
cmake -S . -B build
```

-   Build the actual executable

```
cmake --build build --target <target>
```

-   --target is optional, if not specified, builds all executables

| Options        | working                   |
| -------------- | ------------------------- |
| removeBuildDir | removes the build dir     |
| gbvm           | contains the entire thing |
| occ            | build the compiler        |
| sasm           | build the assembler       |

-   the executables will be in gbvm/build/
