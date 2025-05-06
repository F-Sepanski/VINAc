#!/bin/bash
set -e

VINA=../vina
ARCHIVE=test_archive.vc

# Limpeza inicial
echo "[Limpeza inicial]"
rm -f $ARCHIVE file1.txt file2.txt file3.txt file1_out.txt file2_out.txt file3_out.txt

# Cria arquivos de teste
echo "Conteudo do arquivo 1" > file1.txt
echo "Conteudo do arquivo 2" > file2.txt
echo "Conteudo do arquivo 3" > file3.txt

# Teste: Inserção de membros
echo "==== Teste: Inserção de membros ===="
$VINA -p $ARCHIVE file1.txt file2.txt
echo

# Teste: Listagem do archive
echo "==== Teste: Listagem do archive ===="
$VINA -c $ARCHIVE
echo

# Teste: Inserção de novo membro
echo "==== Teste: Inserção de novo membro ===="
$VINA -p $ARCHIVE file3.txt
echo

# Teste: Listagem após inserção
echo "==== Teste: Listagem após inserção ===="
$VINA -c $ARCHIVE
echo

# Teste: Extração de todos os membros
echo "==== Teste: Extração de todos os membros ===="
rm -f file1.txt file2.txt file3.txt
$VINA -x $ARCHIVE
echo

# Teste: Verificação de integridade
echo "==== Teste: Verificação de integridade ===="
diff file1.txt <(echo "Conteudo do arquivo 1")
diff file2.txt <(echo "Conteudo do arquivo 2")
diff file3.txt <(echo "Conteudo do arquivo 3")
echo "Arquivos extraídos idênticos aos originais."
echo

# Teste: Remoção de um membro
echo "==== Teste: Remoção de um membro ===="
$VINA -r $ARCHIVE file2.txt
$VINA -c $ARCHIVE
echo

# Teste: Movimentação de membro
echo "==== Teste: Movimentação de membro ===="
$VINA -m $ARCHIVE file3.txt file1.txt
$VINA -c $ARCHIVE
echo

# Limpeza final
echo "[Limpeza final]"
rm -f $ARCHIVE file1.txt file2.txt file3.txt

echo "==== Todos os testes passaram! ===="
