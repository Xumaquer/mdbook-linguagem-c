# Compilação Condicional
Existem algumas diretivas de preprocessamento que permitem decidir condicionalmente o código que será levado a etapa de compilação, possibilitando que um determinado código seja excluido da compilação.

Para isso temos várias diretivas que funcionam de forma similar a `if` e `else` : 
- `#if` : Checa se uma expressão é verdadeira, se for, o código dentro deste bloco será válido para compilação.
- `#ifdef`: Um atalho para `#if defined`.
- `#ifndef`: Um atalho para `#if ! defined`.
- `#elif`: Similar a `else if`, uma junção de `#else` com `#if`, onde a expressão é checada apenas se todas anteriores falharem.
- `#elifdef`: Adicionado no `C23`, atalho para `#elseif defined`.
- `#elifndef`: Adicionado no `C23`, atalho para `#elseif ! defined`.
- `#else`: Similar ao `else`, o código dentro deste bloco só será válido se todos condicionais anteriores falharem.
- `#endif` : Finaliza o bloco iniciado por `#if`, `#ifdef` ou `#ifndef`.

No geral as seguintes funcionalidades adicionais podem ser utilizadas em conjunto com `#if` e `#elif` :
- `defined(X)`: Checa se a macro `X` existe, resultando em 1 caso ela exista
- `__has_include(X)`: Adicionado no `C23`, checa se o arquivo `X` pode ser encontrado (busca nas pastas de `include`).
- `__has_embed(X)`: Adicionado no `C23`, checa se o arquivo de recurso `X` pode ser encontrado, resultando em :
    - `__STDC_EMBED_NOT_FOUND__` : Caso o arquivo não exista.
    - `__STDC_EMBED_EMPTY__`: Caso o arquivo exista, mas esteja vazio.
    - `__STDC_EMBED_FOUND__`: Caso o arquivo existe e não esteja vazio.
- `__has_c_attribute(X)`: Adicionado no `C23`, checa se o atributo especificado existe, resultado em um número que indica quando ele foi adicionado ao C, qualquer valor diferente de 0 indica que o atributo existe.

Exemplos de uso das diretivas : 
```c
//Definição indicando que é uma versão de debug
#ifndef NDEBUG 
    #define DEBUG_BUILD
#endif 

#if defined(DEBUG_BUILD)
    #ifdef _WIN32 //Checa se estamos no windows
        #define DBG_BREAKPOINT() __debugbreak()
    #else //No linux e macOs utilizamos SIGTRAP
        #include <signal.h>
        #define DBG_BREAKPOINT() raise(SIGTRAP)
    #endif
    #define DBG_ASSERT(X) if(!(X)) DBG_BREAKPOINT()
#else
    #define DBG_BREAKPOINT()
    #define DBG_ASSERT(X)
#endif 

//Exemplo ficticio de uso
static char buffer[2048];
size_t tam = recebeDados(sock,buffer, sizeof(buffer));

//Gera um breakpoint caso receba mais do que 20 bytes
DBG_ASSERT(tam >= 20); 
```

É muito comum o uso das diretivas `#ifndef` para implementação de `include guards` (explicados no capítulo sobre `include`): 
```c
#ifndef MINHA_BIBLIOTECA_H
#define MINHA_BIBLIOTECA_H
    static int test;
#endif 

#ifndef MINHA_BIBLIOTECA_H
#define MINHA_BIBLIOTECA_H
    static int test;
#endif 

//Mesmo incluindo o mesmo código duas vezes, o "include guard"
//Impede que ele seja avaliado duas vezes...
```

Apenas expressões constantes podem estar num `#if` ou `#elif`, isso significa que variáveis `constexpr`, macros e valores de `enum` são permitidos, bem como `sizeof`.

## Definições úteis
Aqui vamos citar algumas definições úteis que estão presentes em vários compiladores, lembrando que nenhuma dessas definições está presente no padrão do C e qualquer compilador PODE escolher não implementar algumas delas.

Macros para detecção de sistema operacional : 
- `_WIN32`: Macro definida quando compilado para o sistema operacional Windows (tanto 32bits quanto 64bits).
- `_WIN64`: Macro definida quando compilado para Windows 64bits.
- `__linux__`: Macro definida em sistemas linux.
- `__APPLE__`: Detecta que é um sistema operacional da apple.
- `__MACH__`: Detecta que o sistema baseado em Mach, kernel da apple.
- `BSD`: Definido em sistemas como `DragonFly BSD`, `FreeBSD`, `OpenBSD`, `NetBSD`.
- `__unix__`: Detecta sistemas derivados de UNIX, pode não estar presente nos dispositivos da apple apesar deles serem variantes.
- `_POSIX_VERSION`: Ao incluir a biblioteca `unistd.h` em sistemas UNIX, podemos checar a versão do POSIX.
- `__CYGWIN__`: POSIX no Windows utilizando Cygwin (neste caso, `_WIN32` não estará definido).

Macros para detecção de compiladores: 
- `__clang__`: Detecta o compilador Clang.
- `__GNUC__`: Detecta o compilador GCC.
- `__GNUG__`: Detecta o compilador G++ (GCC para C++).
- `_MSC_VER`: Detecta o compilador MSVC.
- `__INTEL_COMPILER`: Detecta o compilador da intel.
- `__IBMC__`: Detecta o compilador da IBM para C.
- `__IBMCPP__`: Detecta o compilador da IBM para C++.

Também é importante mencionar que o C++ define a macro `__cplusplus` com a versão do C++, similar a macro `__STDC_VERSION__`, podemos utilizar ela para verificar se nosso código está sendo compilado por um compilador de C++, algo que muitas bibliotecas fazem.
