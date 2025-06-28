# Estruturas
Estruturas são tipos de dados compostos por um conjunto de outros tipos existentes, que são alocados de forma sequencial na memória, respeitando a mesma ordem declarada na estrutura.

Estruturas também são chamadas de "registro" que é o nome mais comum no português para se referir ao conceito de programação de um tipo de variável composto por um conjunto de outros tipos, no geral em C é muito comum nos referirmos como apenas `struct`, que é o mesmo nome da palavra chave utilizada para declarar uma estrutura.

Cada estrutura é composta de campos nomeados que podem ser de qualquer tipo, esses campos normalmente são chamados de "membros" e serão referidos dessa forma ao longo do texto.

## Declaração de estruturas

Estruturas são declaradas utilizando a seguinte sintaxe : 
```c
struct atributos nomeDaEstrutura {
    listaCampos
} variaveis;
```
Onde : 
- `atributos` : São atributos introduzidos no `C23` que são opcionais, porém caso sejam incluídos, devem ser declarados nessa posição da declaração da estrutura.
- `nomeDaEstrutura` : Indica o nome da estrutura, usado para definir novas variáveis do tipo da estrutura, este campo é opcional, caso esteja ausente a estrutura gerada é "anônima" e não pode ser criada fora da declaração da estrutura (neste caso seu tipo não tem nome mas existe).
- `listaCampos` : Uma lista de zero ou mais campos definindo os membros de uma estrutura, onde cada membro é composto pelo nome do tipo, nome do membro e um ponto e virgula.
- `variaveis` : é opcional e indica uma lista separada por virgula de variáveis que serão criadas junto da declaração da estrutura com o tipo que está sendo declarado, é a única forma de criar variáveis do tipo de uma estrutura anônima (além claro, do uso de `typeof` com uma variável criada desta forma).

Lembrando que os campos de uma estrutura devem ter tamanho conhecido portanto não podem ser tipos incompletos, com a única exceção do campo de array flexível, que será detalhado depois.

Vale lembrar que toda declaração de uma estrutura tem escopo, portanto é possível, assim como em um `enum`, declarar estruturas locais que só existem dentro de um bloco.

## Inicialização de Estruturas
De forma similar a arrays, estruturas também podem ser inicializadas utilizando chaves `{}` e desde o `C99`, podem utilizar inicializadores designados com o nome dos campos.

Exemplo de declaração de uma estrutura definido dados de uma pessoa, junto com 3 formas diferentes de inicialização : 
```c
#include <string.h>

//Numa implementação onde "int" é 4 bytes, sizeof(struct Pessoa) tem 104 bytes.
struct Pessoa {
    char nome[85];
    char cpf[15]; //XXX.XXX.XXX-XX
    int idade;
};

int main() 
{
    //Campos são preenchidos na mesma ordem da estrutura
    //e se não forem preenchidos, terão o valor 0
    struct Pessoa pessoa1 = {"Joao", "123.456.789-10", 25};
    
    struct Pessoa pessoa2 = {
        .nome = "Lucia",
        .cpf  = "987.173.762-28",
        .idade = 22
    };

    struct Pessoa pessoa3;
    strcpy(pessoa3.nome, "Carlos");
    strcpy(pessoa3.cpf,  "982.238.372-59");
    pessoa3.idade = 35;
}
```  

Vale lembrar que no C++ os inicializados designados precisam estar na mesma ordem de declaração dos membros da estrutura, porém, no C eles podem estar fora de ordem.

Antes do `C23`, de forma similar a arrays, para inicializar todos elementos de uma estrutura em 0, é necessário utilizar `{0}`.

## Estruturas incompletas
Mesmo se uma estrutura ainda não foi declarada, ponteiros para ela podem ser criados, neste caso, a estrutura é efetivamente um tipo incompleto, impossibilitando o uso de `sizeof` ou acesso aos seus valores.

Ponteiros para estruturas incompletas não podem ser utilizados para ler/escrever valores, mas podem ser repassados para outras funções, o que é muito comum no padrão "pimpl".

Exemplo de declarações envolvendo estruturas incompletas : 
```c

//Neste caso, uma lista encadeada tem um ponteiro
//do próprio tipo (que durante a declaração ainda é incompleto)
struct ListaEncadeada{
    struct ListaEncadeada *proximo;
    void *dado;
};

//Neste caso é feito um ponteiro para "struct Pessoa" 
//Antes da definição, o tipo é inicialmente incompleto,
//mas é completado após a definição de "struct Pessoa"
struct VetorPessoa {
    size_t quantidade;
    struct Pessoa *pessoas;
};

struct Pessoa {
    char nome[85];
    char cpf[15]; //XXX.XXX.XXX-XX
    int idade;
};
```

### pimpl 

O padrão pimpl (pointer to implementation ou "ponteiro para implementação" no português), é utilizado para encapsular e remover ou limitar o acesso aos campos da estrutura, impedindo efetivamente que código externo dependa da forma como os dados da estrutura são organizados.

O exemplo mais comum desse padrão sendo aplicado na prática é o próprio tipo `FILE*` presente na biblioteca padrão do C na `stdio.h`, isso ocorre pois a forma de representar um arquivo pode ser diferente entre sistemas operacionais ou até entre diferentes versões da biblioteca padrão.

Esse padrão normalmente é aplicado ao definir em um arquivo de cabeçário, o protótipo de funções que operam com um ponteiro da estrutura, sem declarar efetivamente a estrutura, a mantendo como um tipo incompleto. Enquanto, ao mesmo tempo, define um arquivo que implementa e define as funções e a estrutura.

Exemplo abaixo do uso do padrão "pimpl" para mapear um arquivo em memória no windows:
```c
//Código do .h (cabeçário)
#ifndef ARQUIVO_MAPEADO_H
#define ARQUIVO_MAPEADO_H

    //Define um typedef que será utilizado pelas funções
    typedef struct ArquivoMapeado ArquivoMapeado;

    ArquivoMapeado *mapearArquivoEmMemoria(const char *caminho);

    void *acessarArquivoMapeado(ArquivoMapeado *mapa);

    size_t tamanhoArquivoMapeado(ArquivoMapeado *mapa);

    void fecharArquivoMapeado(ArquivoMapeado *mapa);
#endif 
```

``` c
//Código do .c para Windows
#include "arquivoMapeado.h"
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
    mapa->objeto = CreateFileMappingW(arquivo, NULL, PAGE_READWRITE | SEC_COMMIT,
                                      0, 0, NULL);
    mapa->dados  = NULL;
        
    if(mapa->objeto == NULL)
        goto mapa_falhou;
    
    DWORD tamAlto;
    mapa->dados   = MapViewOfFile(mapa->objeto, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    mapa->tamanho = GetFileSize(arquivo, &tamAlto);

    //Caso "size_t" seja 64bits, adiciona a parte alta do tamanho
    if(sizeof(size_t) == 8) 
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
```

No caso descrito acima, poderiamos implementar outra versão do arquivo `.c` que funcione para linux sem a necessidade de mudarmos o código que chama essas funções, oferecendo uma flexibilidade e facilidade para tornar o código portável.

## Membro de array flexível
Desde o `C99`, a definição de uma estrutura pode incluir como seu último membro um campo de array "incompleto", onde o tamanho não é especificado.

O tamanho de tal membro não é incluido no tamanho da estrutura ao utilizar `sizeof`, esta estratégia normalmente é utilizada quando planejamos alocar dinamicamente a estrutura, reservando um espaço variável para o último campo.

Exemplo de uso do membro de array flexível para criação de um vetor dinâmico que guarda o tamanho do array :
```c
#include <stdlib.h>

#define TAM_VETOR 20

struct VetorDouble{
    size_t tam;
    double dados[];
};

int main()
{
    struct VetorDouble *vetor = malloc(sizeof(*vetor) + TAM_VETOR * sizeof(double));
    vetor->tam = TAM_VETOR;
    for(size_t i = 0; i < vetor->tam; i++)
        vetor->dados[i] = (double)(i * i); 
}
```  

## Membro de estrutura ou união anônima
Desde o `C11`, cada membro de uma estrutura ou união anônima que foi declarada dentro de uma estrutura é considerado como um membro direto dela.

Essa definição funciona de forma recursiva, portanto se uma estrutura ou união anônima tiver mais membros na mesma condição, todos os membros serão incorporados pela estrutura não anônima.

Exemplificando, a forma de acesso da estrutura a seguir :
```c
struct Teste1 {
    int a;
    struct {
        int b;
        int c;
    };
    int d;
};

//Exemplo de uso
struct Teste1 test;
test.c = 10;
```

É exatamente a mesma forma com que acessariamos :
```c
struct Teste2 {
    int a;
    int b;
    int c;
    int d;
};

//Exemplo de uso
struct Teste2 test;
test.c = 10;
``` 

No geral, essa regra normalmente é utilizada em conjunto com uniões para simplificar o acesso a alguns membros.

Alguns lugares usam macros especificas para lidar com versões anteriores do C que não suportam membros de estruturas ou uniões anônimas, a microsoft por exemplo utiliza macros como `DUMMYSTRUCTNAME` e `DUMMYUNIONNAME` que definem um nome caso não haja esse suporte.

## Campos de bits
Campos de bits ou no inglês "bit fields", são uma forma alternativa de declarar membros em uma estrutura, permitindo a escolha da quantidade de bits que o campo deverá ocupar.

Com isso é possível ter vários membros que compartilham do mesmo byte de outros membros, portanto o endereço de um membro declarado com campo de bits não pode ser utilizado.

Campos de bits são declarados com a seguinte sintaxe : 
```c
tipo identificador : tamanho;
```

- `tipo` é o tipo do membro de campo de bits declarado, lembrando que há várias limitações no tipo de um campo de bit, que serão detalhadas em seguida.
- `identificador` é o nome do membro, que é opcional, pois uma declaração de campo de bits também pode ser utilizada apenas para inserir um "buraco" com a quantidade de bits especificada na estrutura.
- `tamanho` é a quantidade de bits que o campo deve ter.

Os tipos que um campo de bits podem ter são limitados a : 
- `unsigned int` : O campo de bits é um inteiro sem sinal (ex: `unsigned int b : 3;` tem os limites `[​0​, 7]`)
- `signed int` : O campo de bits é um inteiro com sinal, reservando 1 dos bits para o sinal (`signed int b : 3;` tem os limites `[-4,3]`)
- `int` : O campo de bits é um inteiro que pode ou não ter sinal, dependendo da implementação do compilador.
- `bool` : Pode ser utilizado para campos de bits de tamanho igual a 1, segue o mesmo comportamento de uma variável booleana.
- `_BitInt`: Também pode ser utilizado para campos de bits com ou sem o modificador `unsigned`.

No geral, apesar de ser definido pela implementação, o comportamento mais comum com campos de bits é que os valores são agrupados para compartilhar os bytes de um mesmo tipo.

Ao definir o tipo do bit field, a quantidade de bytes que o tipo fora de um bit field normalmente ocuparia é reservada para todos os campos de bit seguintes que usem o mesmo tipo, ou sua respectiva versão com ou sem sinal.

Para entender melhor, os exemplos a seguir, vamos levar em consideração : 
-  `C23` ou a biblioteca `stdbool.h` já incluida
- `bool` com 1 byte 
- `short` com 2 bytes e permitido em bit fields
- `int` com 4 bytes 
- Arquitetura onde 1 byte é 8bits
- Desconsiderar bytes extras colocado para alinhar membros e a estrutura

Neste exemplo teriamos uma estrutura com 5 bytes, pois 4 bytes foram reservados para os próximos bitfields de tipo `int`/`unsigned int`/`signed int`, porém como logo em seguida encontramos um do tipo `bool` que é diferente, houve uma outra reserva de mais 1 byte.
```c
struct BitField1 {
    unsigned int membro1 : 1;
    bool membro2 : 1;
};
```

Neste outro exemplo, teriamos uma estrutura de 1 byte, pois utilizamos apenas tipo `bool` em todos os campos, o ocupando totalmente : 
```c 
struct BitField2 {
    bool membro1 : 1;
    bool membro2 : 1;
    bool membro3 : 1;
    bool membro4 : 1;
    bool membro5 : 1;
    bool membro6 : 1;
    bool membro7 : 1;
    bool membro8 : 1;
};
```

Neste caso, temos 6 bytes, pois os membros 1 até o 3 ocupam totalmente os 32bits ocupados por um `int` e os membros 4 a 5 ocupam os 16bits ocupados por um `short`.
```c
struct BitField3 {
    unsigned int   membro1 : 4;
    signed int     membro2 : 16;
    unsigned int   membro3 : 12;
    unsigned short membro4 : 8;
    signed short   membro5 : 8;
}
```

Dito isso, há vários outros detalhes sobre campos de bits que também são "definidos pela implementação" :
- Se um campo de bits do tipo `int` tem ou não sinal
- Se tipos além dos mencionados acima são suportados em campos de bits (ex: `short`, `long`, `char`, etc)
- Se tipos atômicos são permitidos em um campo de bits
- Se a ordem dos bits de campos de bits em sequência é da esquerda para direita ou da direita para esquerda.

## Alinhamento de estruturas
Ao definir estruturas, compiladores são livres para adicionar bytes extras de forma a garantir que os membros da estrutura estejam alinhados.

Cada tipo primitivo tem um número de bytes considerado como "requisito de alinhamento" diferente e diz-se que um campo está "alinhado" quando seu endereço e tamanho são múltiplos do requisito de alinhamento.

O padrão do C apenas especifica que bytes extras podem ser adicionados por questão de alinhamento, mas não entra em detalhes sobre a forma como isso deve ser feito, mantendo esse detalhe como "definido pela implementação", repassando a responsabilidade aos compiladores e especificações de ABI para cada plataforma.

Para quem não sabe, ABI é uma sigla para "Application Binary Interface" e especifica a forma em comum que diferentes programas devem se comunicar em uma mesma plataforma, geralmente especificando convenções de chamada, requisitos de alinhamento e vários outros detalhes.

A forma mais precisa e efetiva de descobrir como exatamente o alinhamento é lidado, é ler documentos que especificam esse comportamento, como por exemplo : 
- [`ABI x86-64 Windows`](https://learn.microsoft.com/pt-br/cpp/build/x64-software-conventions?view=msvc-170)
- [`ABI x86-64 SystemV`](https://gitlab.com/x86-psABIs/x86-64-ABI)
- [`ABI ARM Windows`](https://learn.microsoft.com/pt-br/cpp/build/overview-of-arm-abi-conventions?view=msvc-170#alignment)
- [`ABI ARM64 Windows`](https://learn.microsoft.com/pt-br/cpp/build/arm64-windows-abi-conventions?view=msvc-170#alignment)
- [`Detalhes de implementação do GCC para ARM`](https://developer.arm.com/documentation/dui0491/i/C-and-C---Implementation-Details/Structures--unions--enumerations--and-bitfields)

No geral é possível saber quando uma estrutura teve bytes extras adicionais devido ao alinhamento quando o tamanho resultante de `sizeof` da estrutura, excede a soma total do tamanho de todos os seus membros.

Também é importante citar que a maioria dos compiladores oferece alguma extensão que permite forçar o alinhamento de uma estrutura, isso é particularmente útil quando estamos lendo um formato de arquivo ou realizando uma comunicação entre diferentes aplicações e queremos forçar o compilador a não alinhar os membros da estrutura.

### Comportamento convencional ao alinhar estruturas
Apesar do padrão do C não especificar nada, vamos falar do comportamento mais convencional das implementações em relação ao alinhamento, que pode ou não estar correto para a sua plataforma (mas provavelmente está).

Então as seções seguintes não devem ser levadas ao pé de letra, pois podem não refletir todos os ambientes e compiladores disponíveis.

#### Requisitos de alinhamento

No geral o requisito de alinhamento de cada tipo geralmente é exatamente o tamanho do maior tipo primitivo ou ponteiro que o compõem.

Logo todo tipo primitivo ou ponteiro tem requisito de alinhamento igual ao seu tamanho em bytes, no caso dos ponteiros, me refiro a `sizeof(void*)` e não o tamanho do tipo que o ponteiro representa.

Arrays terão sempre requisito de alinhamento igual ao tipo usado para formar um array, arrays de `char` terão o mesmo requisito de um `char`, arrays de `int` terão o requisito de um `int` e assim por diante.

Estruturas seguem a mesma regra, terão o requisito de alinhamento igual ao membro com o maior requisito de alinhamento.

#### Ordem de declaração e alinhamento
No geral, "buracos de bytes" são adicionados logo após o membro que deseja ser alinhado e tanto os seus membros quanto a estrutura como um todo geralmente são alinhados.

Devido ao comportamento de uma estrutura de manter os seus elementos sequenciais na memória EXATAMENTE na forma como foram declarados, a ordem dos elementos pode alterar o tamanho da estrutura.

Observe o exemplo abaixo que demonstra isso : 
```c
#include <assert.h>
#include <inttypes.h>

struct teste1{
    int16_t a;
    int32_t b;
    int16_t c;
};

struct teste2 {
    int32_t b;
    int16_t a;
    int16_t c;
};

int main()
{
    static_assert(sizeof(struct teste1) == sizeof(struct teste2), 
                  "Alinhamento diferente");
}
```

No caso descrito acima, o código não compila, pois o `static_assert` falha indicando que o tamanho das duas estruturas são diferentes. 

Devido a regra de que os elementos de uma estrutura são colocados exatamente na ordem declarada, existe uma necessidade de adicionar 2 bytes adicionais após `a` e `c` na estrutura `teste1`, devido ao requisito de alinhamento de 4 bytes.

Ao mesmo tempo, a estrutura `teste2` não precisa de alinhamento, pois os valores `a` e `c` estão seguidos um do outro na memória, formando, juntos, 4 bytes.

Dessa forma `sizeof(struct teste1)` será 12 devido ao requisito de alinhamento, enquanto `sizeof(struct teste2)` será 8.

## Offset de membros
A biblioteca `stddef.h` (incluida junto com )