
IGDIR = ./headers
CGDIR = ./src

IADIR = ./assembler/headers
CADIR = ./assembler/src

_GLOBAL_HEADERS = 		gbvm_errors.h 	gbvm_fileops.h 		gbvm_strings.h 	gbvm.h 					global_definitions.h 	global_cmdlineops.h
_GLOBAL_CODE = 			gbvm_errors.c 	gbvm_fileops.c 		gbvm_strings.c 	gbvm.c 					global_cmdlineops.c

_ASSEMBLER_HEADERS = 	gbvm_alu.h 		gbvm_assembler.h 	gbvm_flags.h 	gbvm_instructions.h 	gbvm_memory.h
_ASSEMBLER_CODE = 		gbvm_alu.c 		gbvm_assembler.c 	gbvm_flags.c 	gbvm_instructions.c

GLOBAL_HEADERS = 		$(patsubst %, 	$(IGDIR)/%,			$(_GLOBAL_HEADERS))
GLOBAL_CODE = 			$(patsubst %, 	$(CGDIR)/%,			$(_GLOBAL_CODE))

ASSEMBLER_HEADERS = 	$(patsubst %, 	$(IADIR)/%,			$(_ASSEMBLER_HEADERS))
ASSEMBLER_CODE = 		$(patsubst %, 	$(CADIR)/%,			$(_ASSEMBLER_CODE))

.PHONY: clean

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wswitch-enum -std=c2x -pedantic -I$(IGDIR) -I$(IADIR)
LIBS =

gbasm: 	gbasm.c $(ASSEMBLER_CODE) $(GLOBAL_CODE)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

gbvm: 	main.c $(GLOBAL_CODE) $(ASSEMBLER_CODE)
	  	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -f $(CGDIR)/*.o $(CADIR)/*.o main.o gbvm gbasm
