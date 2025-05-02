# LEIAME.md

# Autoria do Software
- GRR: [Your GRR Here]
- Nome do Autor: [Your Name Here]

# Descrição dos Arquivos e Diretórios
- **src/**: Contém os arquivos de código-fonte do projeto.
  - **main.c**: Ponto de entrada da aplicação VINAc. Gerencia argumentos de linha de comando e fluxo principal do programa.
  - **archiver.c**: Implementação das funções do arquivador, incluindo adição, extração e remoção de membros no arquivo.
  - **archiver.h**: Declarações das funções e estruturas de dados usadas nas operações de arquivamento.
  - **directory.c**: Implementa funções para gerenciar a estrutura do diretório do arquivo, incluindo leitura e escrita de informações dos membros.
  - **directory.h**: Declarações das funções e estruturas de dados para gerenciamento do diretório.
  - **member.c**: Implementação das funções relacionadas aos membros, como criação, leitura e escrita de dados dos membros no arquivo.
  - **member.h**: Declarações das funções e estruturas de dados relacionadas aos membros.
  - **lz/**: Contém a implementação do algoritmo de compressão LZ77.
    - **lz.c**: Implementação do algoritmo de compressão e descompressão LZ77.
    - **lz.h**: Declarações das funções de compressão e descompressão LZ77.
  
- **include/**: Contém os arquivos de cabeçalho do projeto.
  - **vinac.h**: Interface principal do projeto, incluindo definições comuns, constantes e protótipos de funções.

- **test/**: Contém os arquivos de teste do projeto.
  - **test_archiver.c**: Testes unitários para as funções do arquivador.
  - **test_directory.c**: Testes unitários para as funções de gerenciamento do diretório.
  - **test_member.c**: Testes unitários para as funções relacionadas aos membros.

- **Makefile**: Instruções de compilação do projeto, especificando como compilar os arquivos de origem e criar o executável.

- **README.md**: Documentação do projeto, incluindo visão geral, instruções de instalação e exemplos de uso.

# Algoritmos e Estruturas de Dados Utilizadas
- O projeto utiliza o algoritmo de compressão LZ77 para compactar dados. O algoritmo é implementado em `src/lz/lz.c` e suas funções são declaradas em `src/lz/lz.h`.
- Estruturas de dados são utilizadas para gerenciar informações dos membros e do diretório, garantindo que as operações de adição, extração e remoção sejam realizadas de forma eficiente.

# Dificuldades Encontradas
- [Descreva aqui as dificuldades que você encontrou durante o desenvolvimento do projeto e como foram contornadas.]

# Bugs Conhecidos
- [Liste aqui quaisquer bugs conhecidos que ainda não foram resolvidos.]