#===============
# CONSTANTES
#===============
# Compilador e flages
CC = gcc
CFLAGS = -Wall -std=c99 -lm

# Diretórios de código fonte e destino
SRC_DIR = src
PRIM_DIR = src/primitives
COMP_DIR = src/compressor
FMAN_DIR = src/fileManager
TEST_DIR = src/test
OBJ_DIR = obj

# Nome do binário final
TARGET = main

# Criando uma lista de variáveis para os arquivos .c
# e associa cada um a uma lista de .o
SRC_FILES = $(wildcard $(SRC_DIR)/main.c) \
            $(wildcard $(PRIM_DIR)/*.c) \
            $(wildcard $(COMP_DIR)/*.c) \
            $(wildcard $(FMAN_DIR)/*.c) \
			$(wildcard $(TEST_DIR)/*.c)

# Convertendo os arquivos .c em arquivos .o correspondentes
# Cada diretório de origem será convertido para um caminho correto para o diretório de objetos
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/$(notdir %.o), $(SRC_FILES))


#===============
# Regras
#===============

# Criando o binário final
# A função acima está ajeitando o nome dos
$(TARGET): $(OBJ_FILES)
	$(CC) obj/*.o $(CFLAGS) -o $(TARGET)

# Regra para compilar os arquivos .c em .o
# Cria OBJ_DIR se necessário
# Compila cada .o associado ao .c do qual depende
# $< Variável que representa o primeiro pré-requisito (o .c)
# $@: O nome do arquivo .o será o mesmo do .c correspondente
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  
	$(CC) -c $< -o $(OBJ_DIR)/$(notdir $@)

# Deleta os arquivos .o e o binário final
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Forçando o Makefile a entender que se houver arquivos com o nome
# dessas regras, eles devem ser ignorados
.PHONY: clean