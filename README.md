# SimpleCCompressor

Um compressor de imagens de tom contínuo no formato BMP baseado no algorítimo JPEG e implementado em C. Foi desenvolvido na disciplina SCC 0261 - Múltimidia.

## Alunos

* Gabriel Antunes Afonso de araujo - 14571077
* Renan Parpinelli Scarpin - 14712188

## Sobre o projeto

O objetivo desse projeto é implementar as etapas comuns de compressão com perda para imagens BMP, baseadas no algorítimo JPEG, assim como o retorno da descompressão, aplicando conhecimentos de codificação multimídia, algorítimos de compressão, modularização e programação em C.

### Estrutura da implementação

```
PastaRaiz/
  |- docs/
  |- img/              # Imagens BMP de teste
  |- src/
  |   |- compression   # Implementação dos algorítimos de compressão
  |   |- fileManager   # Gerenciamento dos arquivos e da compressão
  |   |- image         # Estruturas de dados para imagens
  |   |- primitives    # Estrutura de dados auxiliares e arquivos comuns
  |   |- test          # Códigos de teste usados no desenvolvimento
  |
  |- main.c            # Interface e interação com o usuário
```

### Fluxo de execução

Quanto aos módulos, o ponto de acesso da main para as funcionalidades se da no fileManager, mais especificamente no compressor.c, responsável por agrupar algorítimos, estruturas e fileManager.c para realizar as tarefas de compressão e descompressão

Quanto ao fluxo dos algorítimos na implementação, temos:

``` Compressão
Leitura BMP --> Conversão YCbCr --> downSample em Cb e Cr --> DCT e Quantização --> Codificação RLE --> Huffman --> Escrita do arquivo binário
```

``` Descompressão
Leitura arquivo binário --> Huffman --> Decodificação RLE --> inversas da quantização de DCT --> upSample --> Conversão RGB --> Escrita BMP
```

### Decisões importantes

Quanto ao processo de codificação, optou-se por divir a imagem em blocos 8x8 após o Downsample e tratar cada canal em um vetor próprio. Ao fim da codificação, os dados dos blocos comprimidos de cada canal são agrupados em uma estrutura que os armazena bit a bit e faz a correta leitura e escrita do arquivo binário final (o bitBuffer). Ademais, criamos um cabeçalho customizado para facilitar a descompressão com alguns metadados do resultado da compressão:

```C
typedef struct cmpHeader {
    int yBlocks;               // Número de blocos em y
    int cbBlocks;              // Número de blocos em cb
    int crBlocks;              // Número de blocos em cr
    int cromPdWidth;           // Padding na largura de cb e cr
    int cromPdHeigth;          // Padding na altura de cb e cr
    int cmpBytes;              // Número de bytes comprimidos
} CMPHEADER;
```

Por fim, Decidiu-se optar por um equilibrio entre o uso de dados na memória estática e dinâmica, com a represetação de estruturas de tamanho fixo (blocos 8x8 dos canais) e alguns dos tipos de dados da imagem (cabeçalhos, pixel) sendo passados na STACK, enquanto estruturas mais complexas e de tamanho variável foram gerenciados na HEAP (vetores, matriz geral da imagem, bitBuffer).

## Como utilizar?

### Pré-requisitos

- Sistema:
    * GCC
    * Make

- Imagens:
    * Arquivos BMP sem compressão
    * 24 bits para cor
    * Altura e largura múltiplos de 8
    * Dimensões mínimas: 8 x 8 pixels
    * Dimensões máximas: 1280 x 800 pixels

### Compilando e executando

Usando o Makefile fornecido na pasta raiz do projeto, o binário final será gerado nela, enquanto os demais arquivos .o ficaram em uma pasta específica (.bin)

```bash
make
```

Após isso, basta chamar o arquivo "main" gerado, passando a flag da ação que pretender executar e os arquivos de entrada e saída do programa.

- Para compressão:
    ```bash
    ./main -c <arquivo-entrada> <arquivo-saida>
    ```

- Para descompressão:
    ```bash
    ./main -d <arquivo-entrada> <arquivo-saida>
    ```

