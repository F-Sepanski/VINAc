# O Arquivador VINAc
O objetivo deste trabalho é implementar o programa vinac, que consiste em um arquivador (archiver) com suporte a compressão, isto é, um programa que salva em sequência uma coleção de arquivos (denominados membros), planos ou comprimidos, dentro de outro arquivo (denominado archive) cujaestrutura permite recuperar os arquivos originais individualmente.
Os programas tar, zip, rar e arj são arquivadores populares. Além de
arquivar arquivos, a maioria deles também comprime os dados dos mesmos,
para ocupar menos espaço de armazenamento.

## Descrição das Operações
O pacote de software a ser construído deve gerar o executável chamado
vina, que deve ser executado da seguinte forma:

`vinac <opção> <archive> [membro1 membro2 ...]`

Onde a opção pode ser:

**-ip (ou -p)** : insere/acrescenta um ou mais membros *sem compressão* ao
archive. Caso o membro já exista no archive, ele deve ser substituído.
Novos membros são inseridos respeitando a ordem da linha de comando, ao
final do archive;

**-ic (ou -i)** : insere/acrescenta um ou mais membros *com compressão* ao
archive. Caso o membro já exista no archive, ele deve ser substituído.
Novos membros são inseridos respeitando a ordem da linha de comando, ao
final do archive;

**-m membro** : move o membro indicado na linha de comando para imediatamente depois do membro target existente em archive. A movimentação deve ocorrer na seção de dados do archive; para mover para o início, o target deve ser NULL.

**-x** : extrai os membros indicados de archive. Se os membros não forem
indicados, todos devem ser extraídos. A extração consiste em ler o membro de archive e criar um arquivo correspondente, com conteúdo idêntico, em disco;

**-r** : remove os membros indicados de archive;

**-c** : lista o conteúdo de archive em ordem, incluindo as propriedades de
cada membro (nome, UID, tamanho original, tamanho em disco e data de
modificação) e sua ordem no arquivo.

## Formato do Arquivo
Cada aluno é livre para inventar seu próprio formato para o archive, desde que respeitando as seguintes restrições:

- O início ou final do archive (extensão .vc) deve ter uma área de diretório, como mostra a figura:

![[Pasted image 20250430075559.png]]

- Todas as informações sobre os membros, necessárias para a manipulação do archive, devem estar armazenadas única e exclusivamente na área de diretório. A parte restante deve conter apenas os dados dos membros.

- Para cada membro, devem ser armazenadas, no diretório, as seguintes informações, na ordem indicada: nome (sem espaços, com tamanho máximo de 1024B), UID, tamanho original, tamanho em disco, data de modificação, ordem no arquivo e localização (offset a partir do início).

- A ordem de um membro no arquivo é dada pela ordem de inserção, e pode ser alterada pela opção -m.

- O conteúdo da área de diretório pode ser manipulado em memória RAM ou em disco, a critério da equipe.

- O conteúdo dos membros do archive deve ser manipulado diretamente em disco, não sendo permitida a alocação de um buffer maior do que a quantidade de bytes utilizado pelo maior membro presente no archive para manipulação dos conteúdos dos membros – isto é, a leitura e escrita deve ser, no máximo, de um membro por vez.

- A compressão que deve ser utilizada é a LZ.

- A compressão deve ser realizada, quando requisitada na inserção de um membro, para cada arquivo de entrada separadamente – se o resultado da compressão for maior ou igual ao do arquivo original, então você deve armazenar os dados descomprimidos.

- No armazenamento dos arquivos descomprimidos, o tamanho original deve ser igual ao tamanho em disco.

- Considerando os requisitos anteriores, a descompressão deve também ser feita arquivo a arquivo.

## Biblioteca de Compressão
A biblioteca de compressão que deve ser utilizada no trabalho está disponível no moodle, como material auxiliar.

## Entrega
O arquivo de entrega deve conter:
- Todos os arquivos de código-fonte do programa desenvolvido;
- Um LEIAME;
- Um Makefile.

O arquivo de texto LEIAME deve conter as seguintes informações:
- autoria do software (GRR e nome do autor);
- lista dos arquivos e diretórios contidos no pacote e sua descrição (breve);
- uma seção descrevendo os algoritmos e as estruturas de dados utilizadas, as alternativas de implementação consideradas e/ou experimentadas e os motivos que o levaram a optar pela versão entregue, as dificuldades encontradas e as maneiras pelas quais foram contornadas.
- bugs conhecidos.

O arquivo Makefile deve possuir as regras necessárias para compilar os módulos individualmente e gerar o programa executável. As seguintes regras devem estar presentes:
- all: compila e produz um executável chamado vina no diretório login/;
- clean: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (.o, executável, etc.).