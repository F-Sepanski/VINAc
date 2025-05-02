#ifndef MEMBER_H
#define MEMBER_H
#include <stdio.h>
#include <stdlib.h>

// Função para ler um arquivo do disco e retornar seu conteúdo e tamanho
int ler_arquivo(const char *nome, unsigned char **buffer, long *tamanho);

#endif // MEMBER_H
