#include "encoding.h"
#include "../primitives/bitBuffer.h"

AC_Huffman_Code ac_table[162] = {
    {0, 0, "1010", 4},
    {0, 1, "00", 3},
    {0, 2, "01", 4},
    {0, 3, "100", 6},
    {0, 4, "1011", 8},
    {0, 5, "11010", 10},
    {0, 6, "111000", 12},
    {0, 7, "1111000", 14},
    {0, 8, "1111110110", 18},
    {0, 9, "1111111110000010", 25},
    {0, 10, "1111111110000011", 26},
    {1, 1, "1100", 5},
    {1, 2, "111001", 8},
    {1, 3, "1111001", 10},
    {1, 4, "111110110", 13},
    {1, 5, "11111110110", 16},
    {1, 6, "1111111110000100", 22},
    {1, 7, "1111111110000101", 23},
    {1, 8, "1111111110000110", 24},
    {1, 9, "1111111110000111", 25},
    {1, 10, "1111111110001000", 26},
    {2, 1, "11011", 6},
    {2, 2, "11111000", 10},
    {2, 3, "1111110111", 13},
    {2, 4, "1111111110001001", 20},
    {2, 5, "1111111110001010", 21},
    {2, 6, "1111111110001011", 22},
    {2, 7, "1111111110001100", 23},
    {2, 8, "1111111110001101", 24},
    {2, 9, "1111111110001110", 25},
    {2, 10, "111111111000111",26},
    {3, 1, "111010", 7},
    {3, 2, "111110111", 11},
    {3, 3, "11111110111", 14},
    {3, 4, "1111111110010000", 20},
    {3, 5, "1111111110010001", 21},
    {3, 6, "1111111110010010", 22},
    {3, 7, "1111111110010011", 23},
    {3, 8, "1111111110010100", 24},
    {3, 9, "1111111110010101", 25},
    {3, 10, "1111111110010110", 26},
    {4, 1, "111011", 7},
    {4, 2, "1111111000", 12},
    {4, 3, "1111111110010111", 19},
    {4, 4, "1111111110011000", 20},
    {4, 5, "1111111110011001", 21},
    {4, 6, "1111111110011010", 22},
    {4, 7, "1111111110011011", 23},
    {4, 8, "1111111110011100", 24},
    {4, 9, "1111111110011101", 25},
    {4, 10, "1111111110011110", 26},
    {5, 1, "1111010", 8},
    {5, 2, "1111111001", 12},
    {5, 3, "1111111110011111", 19},
    {5, 4, "1111111110100000", 20},
    {5, 5, "1111111110100001", 21},
    {5, 6, "1111111110100010", 22},
    {5, 7, "1111111110100011", 23},
    {5, 8, "1111111110100100", 24},
    {5, 9, "1111111110100101", 25},
    {5, 10, "1111111110100110", 26},
    {6, 1, "1111011", 8},
    {6, 2, "11111111000", 13},
    {6, 3, "1111111110100111", 19},
    {6, 4, "1111111110101000", 20},
    {6, 5, "1111111110101001", 21},
    {6, 6, "1111111110101010", 22},
    {6, 7, "1111111110101011", 23},
    {6, 8, "1111111110101100", 24},
    {6, 9, "1111111110101101", 25},
    {6, 10, "1111111110101110", 26},
    {7, 1, "11111001", 9},
    {7, 2, "11111111001", 13},
    {7, 3, "1111111110101111", 19},
    {7, 4, "1111111110110000", 20},
    {7, 5, "1111111110110001", 21},
    {7, 6, "1111111110110010", 22},
    {7, 7, "1111111110110011", 23},
    {7, 8, "1111111110110100", 24},
    {7, 9, "1111111110110101", 25},
    {7, 10, "1111111110110110", 26},
    {8, 1, "11111010", 9},
    {8, 2, "111111111000000", 17},
    {8, 3, "1111111110110111", 19},
    {8, 4, "1111111110111000", 20},
    {8, 5, "1111111110111001", 21},
    {8, 6, "1111111110111010", 22},
    {8, 7, "1111111110111011", 23},
    {8, 8, "1111111110111100", 24},
    {8, 9, "1111111110111101", 25},
    {8, 10, "1111111110111110", 26},
    {9, 1, "111111000", 10},
    {9, 2, "1111111110111111", 18},
    {9, 3, "1111111111000000", 19},
    {9, 4, "1111111111000001", 20},
    {9, 5, "1111111111000010", 21},
    {9, 6, "1111111111000011", 22},
    {9, 7, "1111111111000100", 23},
    {9, 8, "1111111111000101", 24},
    {9, 9, "1111111111000110", 25},
    {9, 10, "1111111111000111", 26},
    {10, 1, "111111001", 10},
    {10, 2, "1111111111001000", 18},
    {10, 3, "1111111111001001", 19},
    {10, 4, "1111111111001010", 20},
    {10, 5, "1111111111001011", 21},
    {10, 6, "1111111111001100", 22},
    {10, 7, "1111111111001101", 23},
    {10, 8, "1111111111001110", 24},
    {10, 9, "1111111111001111", 25},
    {10, 10, "1111111111010000", 26},
    {11, 1, "111111010", 10},
    {11, 2, "1111111111010001", 18},
    {11, 3, "1111111111010010", 19},
    {11, 4, "1111111111010011", 20},
    {11, 5, "1111111111010100", 21},
    {11, 6, "1111111111010101", 22},
    {11, 7, "1111111111010110", 23},
    {11, 8, "1111111111010111", 24},
    {11, 9, "1111111111011000", 25},
    {11, 10, "1111111111011001", 26},
    {12, 1, "1111111010", 11},
    {12, 2, "1111111111011010", 18},
    {12, 3, "1111111111011011", 19},
    {12, 4, "1111111111011100", 20},
    {12, 5, "1111111111011101", 21},
    {12, 6, "1111111111011110", 22},
    {12, 7, "1111111111011111", 23},
    {12, 8, "1111111111100000", 24},
    {12, 9, "1111111111100001", 25},
    {12, 10, "1111111111100010", 26},
    {13, 1, "11111111010", 12},
    {13, 2, "1111111111100011", 18},
    {13, 3, "1111111111100100", 19},
    {13, 4, "1111111111100101", 20},
    {13, 5, "1111111111100110", 21},
    {13, 6, "1111111111100111", 22},
    {13, 7, "1111111111101000", 23},
    {13, 8, "1111111111101001", 24},
    {13, 9, "1111111111101010", 25},
    {13, 10, "1111111111101011", 26},
    {14, 1, "111111110110", 13},
    {14, 2, "1111111111101100", 18},
    {14, 3, "1111111111101101", 19},
    {14, 4, "1111111111101110", 20},
    {14, 5, "1111111111101111", 21},
    {14, 6, "1111111111110000", 22},
    {14, 7, "1111111111110001", 23},
    {14, 8, "1111111111110010", 24},
    {14, 9, "1111111111110011", 25},
    {14, 10, "11111111111100100", 26},
    {15, 0, "111111110111", 12},
    {15, 1, "1111111111110101", 17},
    {15, 2, "1111111111110110", 18},
    {15, 3, "1111111111110111", 19},
    {15, 4, "1111111111111000", 20},
    {15, 5, "1111111111111001", 21},
    {15, 6, "1111111111111010", 22},
    {15, 7, "1111111111111011", 23},
    {15, 8, "1111111111111100", 24},
    {15, 9, "1111111111111101", 25},
    {15, 10, "1111111111111110", 26}    
};

//Calcula a categoria de um valor (n de bits sem zeros a esquerda)
int category(int value) {
    int num = abs(value);
    if (num == 0){return 0;}
    int cat = 0;
    while (num) {num >>= 1; cat++;} //Conta bits com bit shifting
    return cat;
}

//Busca o código AC na tabela de Huffman
char* searchACTable(int run, int cat) {
    // Verifica se o run e o level estão dentro dos limites
    if (run < 0 || run > 15 || cat < -1024 || cat > 1023) {
        printf("Run ou Level fora dos limites.\n");
        return NULL;
    }
    // Retorna o código AC correspondente
    for (int i = 0; i < 162; i++) {
        if (ac_table[i].run == run && ac_table[i].category == cat) {
            return ac_table[i].prefix; // Retorna a string do código AC
        }
    }
    printf("Código AC não encontrado.\n");
    return NULL;
}

// Converte uma string de '0's e '1's em um vetor de inteiros
VECTOR* string_to_code(const char *str) {
    // Calcula o tamanho do código
    int size = strlen(str);
    VECTOR* code = vectorCreateAs(unsigned int, NULL);
    // Converte a string em um vetor de inteiros
    for (int i = 0; i < size; i++) {
        int pb = str[i] - '0';
        vectorPushBack(code, &pb); // Converte '0' e '1' para 0 e 1
    }
    return code;
}

// Retorna o prefixo AC correspondente ao run e categoria
VECTOR* getACPrefix(int run, int cat){
    return string_to_code(searchACTable(run, cat));
}

// Retorna o complemento de 1 do número em uma categoria específica
VECTOR* get1sComplement(int number, int cat){

    // Verifica se o número está dentro dos limites
    if (number < -1024 || number > 1023) {
        printf("Número fora dos limites.\n");
        return NULL;
    }

    // Calcula o complemento de 1 do número
    int absNumber = abs(number);
    VECTOR* code = vectorCreateAs(unsigned int, NULL);

    if (number > 0) {
        // Para números positivos, apenas converte para binário
        for (int i = 0; i < cat; i++) {
            int pb = absNumber >> (cat - 1 - i) & 1;
            vectorPushBack(code, &pb); // Extrai cada bit
        }
        return code;
    }
    // Para números negativos, converte para complemento de 1
    if (number < 0) {
        // Inverte os bits para o complemento de 1
        for (int i = 0; i < cat; i++) {
            int pb = !((absNumber >> (cat - 1 - i)) & 1);
            vectorPushBack(code, &pb);
        }
        return code;
    }
    return code;
}

//Retorna o prefixo AC concatenado com o complemento de 1 do número
VECTOR* getACCode(int run, int number) {
    int cat = category(number);
    VECTOR *acPrefix = getACPrefix(run, cat);
    VECTOR *complement = get1sComplement(number, cat);
    if (acPrefix == NULL || complement == NULL) {
        return NULL; // Erro ao obter os códigos
    }

    // Cria um novo vetor para armazenar o código AC completo
    VECTOR *acCode = vectorCreateAs(unsigned int, NULL);

    // Adiciona o prefixo AC ao vetor
    for (int i = 0; i < vectorGetSize(acPrefix); i++) {
        vectorPushBack(acCode, &vectorIndexAs(acPrefix, unsigned int, i));
    }

    // Adiciona o complemento de 1 ao vetor
    for (int i = 0; i < vectorGetSize(complement); i++) {
        vectorPushBack(acCode, &vectorIndexAs(complement, unsigned int, i));
    }

    vectorDestroy(&acPrefix);
    vectorDestroy(&complement);

    return acCode;
}

//Função que converte uma matriz 8x8 em um vetor 1D fazendo zigzag com o bloco da DCT para compressão
number* zigZagNxN(int blkSize, int matrix[blkSize][blkSize]) {
    int i = 0, j = 0, index = 0;

    // Alocando o vetor retulstante
    number *zigZag = (number *)malloc(blkSize * blkSize * sizeof(number));
    if (zigZag == NULL) {
        printf("Erro ao alocar memória para o vetor zigZag.\n");
        return NULL;
    }

    // Inicializando o vetor zigZag com zeros
    for (int k = 0; k < blkSize * blkSize; k++) {
        zigZag[k] = 0;
    }

    // Preenchendo o vetor zigZag com os valores da matriz
    while(index < blkSize * blkSize){
        // Verifica se a posição atual está dentro dos limites da matriz
        if(i >= 0 && i < blkSize && j >= 0 && j < blkSize) {
            zigZag[index++] = matrix[i][j];
        }

        // Move na diagonal
        if((i + j) % 2 == 0) {
            if(j == blkSize - 1) {
                i++;
            } else if(i == 0) {
                j++;
            } else {
                i--;
                j++;
            }
        } else {
            if(i == blkSize - 1) {
                j++;
            } else if(j == 0) {
                i++;
            } else {
                i++;
                j--;
            }
        }
    }
    return zigZag;
}

number** unZigZagNxN(number *zigZag) {
    number **matrix = (number **)malloc(BLK_SIZE * sizeof(number *));
    for (int i = 0; i < BLK_SIZE; i++) {
        matrix[i] = (number *)malloc(BLK_SIZE * sizeof(number));
    }
    int i = 0, j = 0, index = 0;
    // Preenche a matriz com os valores do vetor zigZag
    while(index < BLK_SIZE * BLK_SIZE){
        // Verifica se a posição atual está dentro dos limites da matriz
        if(i >= 0 && i < BLK_SIZE && j >= 0 && j < BLK_SIZE) {
            matrix[i][j] = zigZag[index++];
        }

        // Move na diagonal
        if((i + j) % 2 == 0) {
            if(j == BLK_SIZE - 1) {
                i++;
            } else if(i == 0) {
                j++;
            } else {
                i--;
                j++;
            }
        } else {
            if(i == BLK_SIZE - 1) {
                j++;
            } else if(j == 0) {
                i++;
            } else {
                i++;
                j--;
            }
        }
    }
    return matrix;
}

//codificação por diferença do vetor zigZag
number* zigZagDifference(number *zigZag) {
    number *zigZagDiff = (number *)malloc(BLK_SIZE * BLK_SIZE * sizeof(number));
    if (zigZagDiff == NULL) {
        printf("Erro ao alocar memória para o vetor zigZagDiff.\n");
        return NULL;
    }
    // Inicializa o vetor zigZagDiff com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZagDiff[k] = 0;
    }
    // Preenche o vetor zigZagDiff com as diferenças
    zigZagDiff[0] = zigZag[0];
    for (int i = 1; i < BLK_SIZE * BLK_SIZE; i++) {
        zigZagDiff[i] = zigZag[i] - zigZag[i - 1];
    }
    return zigZagDiff;
}

number* unZigZagDifference(number *zigZag) {
    number *zigZagDiff = (number *)malloc(BLK_SIZE * BLK_SIZE * sizeof(number));
    if (zigZagDiff == NULL) {
        printf("Erro ao alocar memória para o vetor zigZagDiff.\n");
        return NULL;
    }
    // Inicializa o vetor zigZagDiff com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZagDiff[k] = 0;
    }
    // Preenche o vetor zigZagDiff com as diferenças
    zigZagDiff[0] = zigZag[0];
    for (int i = 1; i < BLK_SIZE * BLK_SIZE; i++) {
        zigZagDiff[i] = zigZag[i] + zigZagDiff[i - 1];
    }
    return zigZagDiff;
}

//Cria um RLEPairs
RLEPairs createRLEPairs(int size) {
    RLEPairs rlePairs = (RLEPairs)malloc(sizeof(rlepairs));
    if (rlePairs == NULL) {
        printf("Erro ao alocar memória para RLEPairs.\n");
        return NULL;
    }
    rlePairs->pairs = (RLEPair)malloc(size * sizeof(rlepair));
    if (rlePairs->pairs == NULL) {
        printf("Erro ao alocar memória para os pares RLE.\n");
        free(rlePairs);
        return NULL;
    }
    rlePairs->size = size;
    return rlePairs;
}


//Run Length Encoding (RLE) para compressão de dados
RLEPairs runLengthEncoding(number *zigZag, int *size) {
    RLEPairs rle = createRLEPairs(BLK_SIZE * BLK_SIZE);
    int index = 0;
    int count = 0;
    for (int i = 0; i < BLK_SIZE * BLK_SIZE; i++) {
        if (zigZag[i] == 0) {
            count++;
            if(count == 16) {
                rle->pairs[index].run = 15;
                rle->pairs[index].level = 0;
                index++;
                if(i != 63) count = 0; 
            }
        } else {
            rle->pairs[index].run = count;
            rle->pairs[index].level = zigZag[i];
            index++;
            count = 0;
        }
    }
    rle->pairs[index].run = 0; // Último run
    rle->pairs[index].level = 0; // Indica o fim do bloco
    index++;

    rle->size = index; // Tamanho do vetor RLE
    return rle;
}

//RunLength Decoding (RLD) para descompressão de dados
number* runLengthDecoding(RLEPairs rle) {
    number *zigZag = (number *)malloc(BLK_SIZE * BLK_SIZE * sizeof(number));
    if (zigZag == NULL) {
        printf("Erro ao alocar memória para o vetor zigZag.\n");
        return NULL;
    }
    // Inicializa o vetor zigZag com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZag[k] = 0;
    }
    int index = 0;
    for (int i = 0; i < rle->size; i++) {
        for (int j = 0; j < rle->pairs[i].run; j++) {
            zigZag[index++] = 0;
        }
        if (rle->pairs[i].level != 0) {
            zigZag[index++] = rle->pairs[i].level;
        } else {
            break;
        }
    }
    return zigZag;
}

// Função para codificar o buffer de bits usando a tabela de Huffman
bool huffman_encoding(bitBuffer* buffer, RLEPairs rle){

    for (int i = 0; i < rle->size; i++) {
        VECTOR *acCode = getACCode(rle->pairs[i].run, rle->pairs[i].level);
        if (acCode == NULL) {
            bitBufferDestroy(&buffer);
            return false; // Erro ao obter o código AC
        }
        // Adiciona o código AC ao buffer de bits
        bitBufferInsert(buffer, acCode);
        vectorDestroy(&acCode);
    }
    return true;
}

// Decodifica um símbolo AC (run, category) do bitBuffer usando busca na tabela ac_table
// Guarda posição atual em *pos e avança conforme leitura de bits
// Retorna false em caso de erro (posição inválida ou código não encontrado)
static bool decodeACSymbol(bitBuffer* buffer, int* pos, int* outRun, int* outCategory) {
    char codeBuf[17];
    int len = 0;

    while (true) {
        // Verifica limites
        if (*pos < 0 || *pos >= bitBufferGetOccupiedBits(buffer)) {
            return false;
        }
        // Lê bit na posição atual e avança
        bool bit = bitBufferReadBit(buffer, *pos);
        (*pos)++;

        // Acumula no buffer de código (máx. 16 bits)
        if (len < 16) {
            codeBuf[len++] = bit ? '1' : '0';
            codeBuf[len] = '\0';
        } else {
            // Prefixo maior que o suportado
            return false;
        }

        // Busca sequencial na tabela AC
        for (int i = 0; i < 162; ++i) {
            if (ac_table[i].total_length == len && strcmp(ac_table[i].prefix, codeBuf) == 0) {
                *outRun = ac_table[i].run;
                *outCategory = ac_table[i].category;
                return true;
            }
        }
        // Caso não encontre, continua lendo o próximo bit
    }
}

// Decodifica os bits de magnitude recuperando o valor do coeficiente
static int decodeMagnitude(bitBuffer* buffer, int* pos, int category) {
    if (category == 0) {
        return 0;
    }
    unsigned int bits = 0;
    // Lê category bits sequencialmente
    for (int i = 0; i < category; ++i) {
        if (*pos < 0 || *pos >= bitBufferGetOccupiedBits(buffer)) {
            break;
        }
        bool bit = bitBufferReadBit(buffer, *pos);
        (*pos)++;
        bits = (bits << 1) | (bit ? 1 : 0);
    }
    unsigned int threshold = 1u << (category - 1);
    if (bits >= threshold) {
        return (int)bits;
    }
    // Ajuste para representar valores negativos
    return (int)(bits - ((1u << category) - 1));
}

// Decodifica todo o buffer em um vetor de pares RLE (RLEPairs)
// Itera símbolos até encontrar EOB (0,0) ou processar ZRL (15,0)
RLEPairs huffman_decoding(bitBuffer* buffer) {
    RLEPairs rle = createRLEPairs(BLK_SIZE * BLK_SIZE);
    int index = 0;
    int pos = 0;  // posição atual em bits

    while (true) {
        int run, category;
        if (!decodeACSymbol(buffer, &pos, &run, &category)) {
            // Erro na decodificação Huffman
            break;
        }

        // EOB: (0,0)
        if (run == 0 && category == 0) {
            rle->pairs[index].run = 0;
            rle->pairs[index].level = 0;
            index++;
            break;
        }
        // ZRL: (15,0) -> representa 16 zeros
        if (run == 15 && category == 0) {
            rle->pairs[index].run = 15;
            rle->pairs[index].level = 0;
            index++;
            continue;
        }
        // Coeficiente não-zero
        int level = decodeMagnitude(buffer, &pos, category);
        rle->pairs[index].run = run;
        rle->pairs[index].level = level;
        index++;
    }

    rle->size = index;
    return rle;
}
