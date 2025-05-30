# Uniões
Uniões são tipos de dados compostos por um conjunto de outros tipos existentes, similar a estruturas, mas que compartilham a mesma memória para todos os seus membros.

O tamanho efetivo de um `union` é igual ao tamanho do seu maior membro.

A sintaxe para definição de uma união é exatamente a mesma utilizada para definição de estruturas, porém ao invés de escrevermos `struct`, utlizaremos a palavra chave `union`.

A regra para estruturas anônimas também se aplicam para definição de `union`.

## Inicialização de uniões
Ao inicializar uma união utilizando chaves `{}`, apenas o primeiro membro de uma união é inicializado a menos que um inicializador designado (adicionado no `C99`) seja utilizado.

```c
#include <inttypes.h>
union Num32 {
    int32_t  i32;
    uint32_t u32;
    float    f32;
};

int main()
{
    //Inicializará o campo "i32"
    union Num32 a = {5};        

    //Inicializará o campo "f32"
    union Num32 b = {.f32 = 1.0f}; 

    //Não recomendado, pois tentará inicializar "i32" com um float
    union Num32 c = {5.0f};        
}
```

## Reinterpretação de tipos
Um uso muito comum de uniões é a possibilidade de realizar o que chamamos de reinterpretação de tipos, o que é comumente chamado no inglês de "type punning".

O uso de uniões para esse propósito é permitido desde o `C99`, mas é comportamento indefinido no C++ e em versões anteriores do C.

A reinterpretação de tipos seria efetivamente ler o valor de um tipo como se ele fosse outro, ao escrever em um dos membros do `union` e ler o valor por outro.

No exemplo abaixo, o uso de um `union` para reinterpretar um `float` e ler sua representação interna :  
```c
#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>
union ieee754_float {
    float f32;
    struct { 
        unsigned int fracao   : 23;
        unsigned int expoente : 8;
        unsigned int sinal    : 1;
    };
};

float lerFloat() 
{
    char linha[1024];
    fgets(linha, sizeof(linha), stdin);
    return strtof(linha, NULL);
}

int main()
{
    union ieee754_float valor;
    printf("Escreva um ponto flutuante: ");
    valor.f32 = lerFloat();
    
    printf("sinal = %u\n"
           "expoente = %u\n"
           "fracao = %u\n", 
           valor.sinal, valor.expoente, valor.fracao);
}
```

## Usos de uniões
Existem algumas técnicas interessantes que podem ser feitas com `union`, essa seção busca compartilhar essas ideias para que você possa aplicar em seu código.

### 1 - Bitmask e booleanas
Geralmente ao utilizar o padrão de bitmask e separar várias flags em um único inteiro, temos um padrão que é um pouco mais complexo de utilizar do que bit fields de campos booleanos, mas que é mais flexível para definir ou checar múltiplas variaveis de uma vez.

Utilizando uniões, podemos juntar ambos em um único tipo, o exemplo abaixo demonstra isso extendendo o exemplo de RPG utilizado em enumerações: 

```c
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

enum {
    ESTADO_PETRIFICADO = 1 << 0,
    ESTADO_CONGELADO   = 1 << 1,
    ESTADO_CONFUSO     = 1 << 2,
    ESTADO_QUEIMANDO   = 1 << 3,
    ESTADO_ENVENENADO  = 1 << 4,
    ESTADO_SANGRANDO   = 1 << 5,
    ESTADO_DOENTE      = 1 << 6,

    ESTADO_CAOS = ESTADO_DOENTE | ESTADO_CONFUSO | ESTADO_QUEIMANDO,
};

struct DadosPersonagem { 
    const char *nome;
    int vida;

    //União e estrutura anônima
    union { 
        struct {
            bool petrificado : 1;
            bool congelado : 1;
            bool confuso : 1;
            bool queimando : 1;
            bool envenenado : 1;
            bool sangrando : 1;
            bool doente : 1;
        };
        uint32_t estado;
    };    
};

int main() 
{
    struct DadosPersonagem personagem = {
        .nome = "Jonas",
        .vida = 100,
        .confuso = true,
        .doente = true,
        .queimando = true
    };
    
    //É possível checar multiplos valores de uma vez
    if((personagem.estado & ESTADO_CAOS) == ESTADO_CAOS)
        puts("Personagem no estado CAOS");

    //Mas também podemos checar individualmente cada bit
    if(personagem.doente)
        puts("Personagem está doente");
}
```


### 2 - Pseudo Polimorfismo
É possível implementar um pseudo polimorfismo utilizando uniões, geralmente utilizando um número inteiro ou enumeração para indicar qual tipo é atualmente representado.

Isso é extremamente útil para representar tipos variantes, campos utilizados para repassar mensagens ou dados similares.

Um exemplo com definições de tipos que poderiam ser usados para representar um [JSON](https://developer.mozilla.org/pt-BR/docs/Learn_web_development/Core/Scripting/JSON) : 
```c
enum TipoJson{
    TIPO_JSON_NUMERO,
    TIPO_JSON_STRING,
    TIPO_JSON_OBJETO,
    TIPO_JSON_BOOLEANA,
    TIPO_JSON_LISTA,
    TIPO_JSON_NULL,
};

struct CampoJson {
    enum TipoJson tipo;

    //Todos os tipos da união compartilham a mesma memória
    union { 
        double numero;
        const char *string;
        bool booleana;
        struct CampoJson *lista;
        struct ObjetoJson *objeto;
    };
};

struct ChaveValorJson {
    const char *chave;
    struct CampoJson valor;
};

struct ObjetoJson {
    size_t tamanho;
    struct ChaveValorJson *campos;
};
```

Outros exemplos de uso real de `union` para este propósito : 
- A estrutura [`INPUT`](https://learn.microsoft.com/pt-br/windows/win32/api/winuser/ns-winuser-input) do windows utilizada na função [`SendInput`](https://learn.microsoft.com/pt-br/windows/win32/api/winuser/nf-winuser-sendinput) para representar entrada de mouse, teclado ou hardware. 
- A união [`SDL_Event`](https://wiki.libsdl.org/SDL3/SDL_Event) utilizada para representar qualquer evento de janela ou do sistema pela biblioteca SDL.
- A união [`XEvent`](https://tronche.com/gui/x/xlib/events/structures.html#XEvent), utilizada pelo X11, gerenciador de janelas normalmente utilizado no linux, para representar eventos de janela.
- A união [`iwreq_data`](https://docs.huihoo.com/doxygen/linux/kernel/3.7/uapi_2linux_2wireless_8h_source.html#l00909) utilizada em conjunto com a função [`ioctl`](https://man7.org/linux/man-pages/man2/ioctl.2.html) (que receberá um número indicando o comando, sendo o inteiro que diferencia o tipo neste caso) para receber/enviar dados para o driver de rede no linux.