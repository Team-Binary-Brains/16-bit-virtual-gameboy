
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


_A_HEAD = 		sasm_assembler.h	sasm_instructions.c		sasm_memory.h
_A_CODE = 		sasm_assembler.c	sasm_instructions.c		sasm_memory.c

_G_HEAD = 		univ_cmdutils.h 	univ_errors.h		    univ_fileops.h		univ_strings.h		univ_hashmap.h		univ_defs.h 
_G_CODE = 		univ_cmdutils.c		univ_errors.c		    univ_fileops.c 		univ_strings.c		univ_hashmap.c

_C_HEAD =		O_lexer.h			O_parser.h				O_token_types.h		O_parse_tree.h		O_symbol_table.h
_C_CODE = 		O_lexer.c			O_parser.c				O_token_types.c		O_parse_tree.c		O_symbol_table.c

_M_HEAD = 		gbvm.h
_M_CODE =		gbvm.c

G_HEAD = 		$(patsubst %, 		$(IGDIR)/%,				$(_G_HEAD))
G_CODE = 		$(patsubst %, 		$(CGDIR)/%,				$(_G_CODE))

A_HEAD = 		$(patsubst %, 		$(IADIR)/%,				$(_A_HEAD))
A_CODE = 		$(patsubst %, 		$(CADIR)/%,				$(_A_CODE))

C_HEAD =		$(patsubst %, 		$(ICDIR)/%,				$(_C_HEAD))
C_CODE = 		$(patsubst %, 		$(CCDIR)/%,				$(_C_CODE))

M_HEAD = 		$(patsubst %, 		$(IMDIR)/%,				$(_M_HEAD))
M_CODE =		$(patsubst %, 		$(CMDIR)/%,				$(_M_CODE))

.PHONY: clean
MAKEFLAGS += --no-print-directory


CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -Wfatal-errors -Wswitch-enum -std=c99 -pedantic -I$(IGDIR) -I$(IADIR) -I$(ICDIR) -I$(IMDIR) 
LIBS =

sasm: 	sasm.c $(G_CODE) $(A_CODE)
		@$(CC) -o $@ $^   $(CFLAGS) $(LIBS)
		@echo -e ""
		@echo -e "SASM MADE SUCCESSFULLY"
		@echo -e ""

occ:	occ.c  $(G_CODE) $(C_CODE) 
		@$(CC) -o $@ $^   $(CFLAGS) $(LIBS)
		@echo -e ""
		@echo -e "COMPILER MADE SUCCESSFULLY"
		@echo -e ""

gbvm: 	main.c $(G_CODE) $(M_CODE) $(A_CODE) $(C_CODE)
		@make occ
		@make sasm
		@echo -e ""
		@echo -e "VM MADE SUCCESSFULLY"
		@echo -e ""
	  	@$(CC) -o $@ $^   $(CFLAGS) $(LIBS)

clean:
		@rm -f $(CGDIR)/*.o $(CADIR)/*.o $(CCDIR)/*.o main.o gbvm sasm occ
		@echo -e ""
		@echo -e "CLEANED ALL OBJECT FILES AND EXECUTABLES"
		@echo -e ""
