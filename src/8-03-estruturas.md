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

Lembrando que os campos de uma estrutura devem ter tamanho conhecido, com a única exceção do campo de array flexível, que será detalhado depois.

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

## Pre-declaração de estruturas
Estruturas também podem ser pre-declaradas utilizando apenas a palavra chave `struct` e o nome da estrutura.

Ao pre-declarar uma estrutura, ela se torna um tipo incompleto, onde o tamanho não está disponível com `sizeof` mas pode ser utilizada em uma declaração com `typedef`.

Exemplo de pre-declaração de estrutura : 
```c
//Pre-declaração normal
struct Nodo; //Desnecessário fazer assim, pois já a declaramos na linha do typedef

//Dessa forma, o tipo "struct Nodo" pode ser acessado com apenas o nome "Nodo"
typedef struct Nodo Nodo;

struct Nodo {
    Node *proximo;
    int dado;
};
```

Na maioria das vezes, exceto quando utilizamos `typedef`, pre-declarar uma estrutura dessa forma é desnecessário, pois quando declaramos um ponteiro para uma estrutura ela também já é automáticamente pre-declarada.

Exemplo de estrutura pre-declarada ao utilizar ponteiros : 

```c
struct Nodo {
    struct Nodo *proxima;
    struct DadoNodo *dados;
};

struct DadoNodo {
    int num;
    char nome[16];
};
```
Neste caso, mesmo que `struct DadoNodo` ainda não exista, é possível declarar um ponteiro para ela normalmente, e depois ao declarar efetivamente o tipo, ele se torna um tipo completo que pode ser usado com `sizeof` e poderá ser acessado através do campo `dados` da estrutura `Nodo`.

### Membro de array flexível
A definição de uma estrutura pode incluir como seu último membro um campo de array "incompleto", onde o tamanho não é especificado.

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

### Campos de bits
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

Dito isso, há vários detalhes sobre campos de bits que são "definidos pela implementação" :
- Se um campo de bits do tipo `int` tem ou não sinal
- Se tipos além dos mencionados acima são suportados em campos de bits (ex: `short`, `long`, `char`, etc)
- Se tipos atômicos são permitidos em um campo de bits
- Se a ordem dos bits de campos de bits em sequência é da esquerda para direita ou da direita para esquerda.

### Alinhamento de estruturas
Ao definir estruturas, compiladores são livres para adicionar bytes extras de forma a garantir que os membros da estrutura estejam alinhados.

Cada tipo primitivo tem um número de bytes considerado como "requisito de alinhamento" diferente e diz-se que um campo está "alinhado" quando seu endereço e tamanho são múltiplos do requisito de alinhamento.

O padrão do C apenas especifica que bytes extras podem ser adicionados por questão de alinhamento, mas não entra em detalhes sobre a forma como isso deve ser feito, mantendo esse detalhe como "definido pela implementação", repassando a responsabilidade aos compiladores e especificações de ABI para cada plataforma.

Para quem não sabe, ABI é uma sigla para "Application Binary Interface" e especifica a forma em comum que diferentes programas devem se comunicar em uma mesma plataforma, geralmente especificando convenções de chamada, requisitos de alinhamento e vários outros detalhes.

A forma mais precisa e efetiva de descobrir como exatamente o alinhamento é lidado, é ler documentos que especificam esse comportamento, como por exemplo : 
- [`ABI x86-64 Windows`](https://learn.microsoft.com/pt-br/cpp/build/x64-software-conventions?view=msvc-170)
- [`ABI x86-64 SystemV`](https://gitlab.com/x86-psABIs/x86-64-ABI)
- [`ABI ARM Windows`](https://learn.microsoft.com/pt-br/cpp/build/overview-of-arm-abi-conventions?view=msvc-170#alignment)
- [`ABI ARM64 Windows`](https://learn.microsoft.com/pt-br/cpp/build/arm64-windows-abi-conventions?view=msvc-170#alignment)

No geral é possível saber quando uma estrutura teve bytes extras adicionais devido ao alinhamento quando o tamanho resultante de `sizeof` da estrutura, excede a soma total do tamanho de todos os seus membros.

Também é importante citar que a maioria dos compiladores oferece alguma extensão que permite forçar o alinhamento de uma estrutura, isso é particularmente útil quando estamos lendo um formato de arquivo ou realizando uma comunicação entre diferentes aplicações e queremos forçar o compilador a não alinhar os membros da estrutura.

#### Comportamento convencional ao alinhar estruturas
Apesar do padrão do C não especificar nada, vamos falar do comportamento mais convencional das implementações em relação ao alinhamento, que pode ou não estar correto para a sua plataforma (mas provavelmente está).

##### Requisitos de alinhamento

No geral o requisito de alinhamento de cada tipo geralmente é exatamente o tamanho do maior tipo primitivo ou ponteiro que o compõem.

Logo todo tipo primitivo ou ponteiro tem requisito de alinhamento igual ao seu tamanho em bytes, no caso dos ponteiros, me refiro a `sizeof(void*)` e não o tamanho do tipo que o ponteiro representa.

Arrays terão sempre requisito de alinhamento igual ao tipo usado para formar um array, arrays de `char` terão o mesmo requisito de um `char`, arrays de `int` terão o requisito de um `int` e assim por diante.

Estruturas seguem a mesma regra, terão o requisito de alinhamento igual ao membro com o maior requisito de alinhamento.

##### Ordem de declaração e alinhamento
No geral, "buracos de bytes" são adicionados logo após o membro que deseja ser alinhado.

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