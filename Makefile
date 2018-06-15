#****************************************
# Makefile - Chained Lists exercises
#****************************************
# Author : Adrien CARBONARO
#****************************************

# Variables

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wno-unused-variable -Wno-unused-but-set-variable
ADDFLAGS =

SRC += chainList.c
SRC += chainListBackward.c

INCL=

OBJ=$(SRC:.c=.o)

EXEC =$(SRC:.c=)

# Execution
all: $(EXEC)
	@echo "[Make] ALL DONE"

# Create binary file ************************
%: %.o
	@echo "[Make] Creating binary file"
	@$(CC) $(CFLAGS) -o $@ $< $(ADDFLAGS)
	@echo "[Make] DONE"

# Create obj files **************************
%.o: %.c
	@echo "[Make] Creating obj files"
	@$(CC) -o $@ -c $< $(CFLAGS) $(ADDFLAGS)
	@echo "[Make] DONE"

# Remove all objects and binary files *******
cleanall: clean mrproper

# Remove all obj files **********************
clean:
	@echo "[Make] Removing Obj files"
	@rm -fv *.o
	@echo "[Make] DONE"

# Remove binary file ************************
mrproper:
	@echo "[Make] Removing Binary file"
	@rm -fv $(EXEC)
	@echo "[Make] DONE"
