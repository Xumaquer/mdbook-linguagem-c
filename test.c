#ifndef ARQUIVO_MAPEADO_H
#define ARQUIVO_MAPEADO_H

    //Define um typedef que será utilizado pelas funções
    typedef struct ArquivoMapeado ArquivoMapeado;

    ArquivoMapeado *mapearArquivoEmMemoria(const char *caminho);

    void *acessarArquivoMapeado(ArquivoMapeado *mapa);

    size_t tamanhoArquivoMapeado(ArquivoMapeado *mapa);

    void fecharArquivoMapeado(ArquivoMapeado *mapa);
#endif 


//Código do .c para Windows
#include <stdlib.h>
#include <windows.h>

struct ArquivoMapeado {
    HANDLE objeto;  /* Objeto do mapa de memoria */
    void *dados;    /* Ponteiro para dados*/
    size_t tamanho; /* Tamanho do arquivo */
};

struct ArquivoMapeado *mapearArquivoEmMemoria(const char *caminho)
{
    
    HANDLE arquivo = CreateFileA(caminho, GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL, NULL);
    

    if(arquivo == INVALID_HANDLE_VALUE)
        return NULL;

    struct ArquivoMapeado *mapa = malloc(sizeof(*mapa));
    mapa->objeto = CreateFileMappingW(arquivo, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 0, NULL);
    mapa->dados  = NULL;
        
    if(mapa->objeto == NULL)
        goto mapa_falhou;
    
    DWORD tamAlto;
    mapa->dados   = MapViewOfFile(mapa->objeto, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    mapa->tamanho = GetFileSize(arquivo, &tamAlto);
    if(sizeof(size_t) == 8) //Caso "size_t" seja 64bits, adiciona a parte alta do tamanho
        mapa->tamanho |= (size_t)tamAlto << 32;

mapa_falhou:
    CloseHandle(arquivo);
    return mapa;
}

void *acessarArquivoMapeado(ArquivoMapeado *mapa)
{
    return mapa->dados;
}

size_t tamanhoArquivoMapeado(ArquivoMapeado *mapa) 
{
    return mapa->tamanho;
}

void fecharArquivoMapeado(ArquivoMapeado *mapa)
{
    if(mapa == NULL)
        return; //Proteção contra ponteiro nulo

    if(mapa->dados != NULL) 
        UnmapViewOfFile(mapa->dados);
    if(mapa->objeto != NULL)
        CloseHandle(mapa->objeto);
    free(mapa);
}