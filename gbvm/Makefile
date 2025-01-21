
#* NAMING FORMAT
#* I = INCLUDE, C = CODE
#* G = GLOBAL, A = ASSEMBLER, C = COMPILER, M = MACHINE
#* DIR = DIRECTORY

IGDIR = ./libs/headers
CGDIR = ./libs/src

IADIR = ./SASM/headers
CADIR = ./SASM/src

ICDIR =	./O/headers
CCDIR =	./O/src

IMDIR = ./VM/headers
CMDIR = ./VM/src


_A_HEAD = 		sasm_alu.h			sasm_assembler.h	sasm_flags.h			sasm_instructions.c		sasm_memory.h
_A_CODE = 		sasm_alu.c			sasm_assembler.c	sasm_flags.c			sasm_instructions.c					 		 				 	

_G_HEAD = 		univ_cmdutils.h 	univ_errors.h		univ_fileops.h 		 	univ_strings.h			univ_defs.h 
_G_CODE = 		univ_cmdutils.c		univ_errors.c		univ_fileops.c 		 	univ_strings.c

_C_HEAD =		O_lexer.h			O_parser.h			O_codegenerator.h 	
_C_CODE = 		O_lexer.c			O_parser.c			O_codegenerator.c 	

_M_HEAD = 		gbvm.h
_M_CODE =		gbvm.c

G_HEAD = 		$(patsubst %, 	$(IGDIR)/%,			$(_G_HEAD))
G_CODE = 		$(patsubst %, 	$(CGDIR)/%,			$(_G_CODE))

A_HEAD = 		$(patsubst %, 	$(IADIR)/%,			$(_A_HEAD))
A_CODE = 		$(patsubst %, 	$(CADIR)/%,			$(_A_CODE))

C_HEAD =		$(patsubst %, 	$(ICDIR)/%,			$(_C_HEAD))
C_CODE = 		$(patsubst %, 	$(CCDIR)/%,			$(_C_CODE))

M_HEAD = 		$(patsubst %, 	$(IMDIR)/%,			$(_M_HEAD))
M_CODE =		$(patsubst %, 	$(CMDIR)/%,			$(_M_CODE))

.PHONY: clean

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wswitch-enum -std=c2x -pedantic -I$(IGDIR) -I$(IADIR) -I$(ICDIR) -I$(IMDIR)
LIBS =

sasm: 	sasm.c $(G_CODE) $(A_CODE)
		$(CC) -o $@ $^   $(CFLAGS) $(LIBS)

occ:	occ.c  $(G_CODE) $(C_CODE) 
		$(CC) -o $@ $^   $(CFLAGS) $(LIBS)

gbvm: 	main.c $(G_CODE) $(M_CODE) $(A_CODE) $(C_CODE)
	  	$(CC) -o $@ $^   $(CFLAGS) $(LIBS)

clean:
		rm -f $(CGDIR)/*.o $(CADIR)/*.o $(CCDIR)/*.o main.o gbvm sasm occ
