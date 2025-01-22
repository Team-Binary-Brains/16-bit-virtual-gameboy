cmake_minimum_required(VERSION 3.12)
project(gbvm)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wswitch-enum -std=c11 -pedantic")

include_directories(
    ./libs/headers
    ./SASM/headers
    ./O/headers
    ./VM/headers
)

set(G_CODE
    ./libs/src/univ_cmdutils.c
    ./libs/src/univ_errors.c
    ./libs/src/univ_fileops.c
    ./libs/src/univ_strings.c
)

set(A_CODE
    ./SASM/src/sasm_alu.c
    ./SASM/src/sasm_assembler.c
    ./SASM/src/sasm_flags.c
    ./SASM/src/sasm_instructions.c
)

set(C_CODE
    ./O/src/O_lexer.c
    ./O/src/O_parser.c
    ./O/src/O_codegenerator.c
)

set(M_CODE
    ./VM/src/gbvm.c
)

add_executable(sasm sasm.c ${G_CODE} ${A_CODE})
add_executable(occ occ.c ${G_CODE} ${C_CODE})
add_executable(gbvm main.c ${G_CODE} ${M_CODE} ${A_CODE} ${C_CODE})

add_custom_target(removeBuildDir
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
)
