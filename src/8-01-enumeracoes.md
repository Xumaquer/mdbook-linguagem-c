# Enumerações
Enumerações são tipos de dados que definem um intervalo limitado de números inteiros. Eles são efetivamente equivalentes a tipos inteiros primitivos.

Utilizar enumerações geralmente permite uma integração melhor com ferramentas de debug e avisos do compilador, demonstrando a intenção do programador de usar um número que faz parte de um conjunto limitado de valores.

## Declaração de enum

Novos tipos de enum podem ser gerados utilizando a palavra chave `enum` e suas definições tem escopo (ou seja, um `enum` gerado dentro de um bloco `{}` só existe dentro dele).

No C, ao declarar um novo tipo enum usando, por exemplo, `enum NovoTipo`, é necessário utilizar `enum NovoTipo` no tipo da variável para criar uma variável que tenha o tipo do enum criado, enquanto que no C++, pode ser utilizado tanto `enum NovoTipo` como apenas `NovoTipo`.

As definições de enum seguem a seguinte sintaxe : 
```c
enum NomeDoEnum{
    ConstantesDoEnum
}VariaveisCriadas;
```
- `NomeDoEnum` indica o nome do enum, usado para definir novas variáveis do tipo do enum, este campo é opcional, caso esteja ausente o enum gerado é "anônimo" e não pode ser criado fora da declaração do enum (neste caso seu tipo não tem nome mas existe).
- `ConstantesDoEnum` é uma lista, que deve conter ao menos um elemento, de nomes de constantes que serão geradas, separadas por virgula (geralmente uma em cada linha), que podem, opcionalmente, ser inicializados com uma expressão constante (isso será explicado em mais detalhes a seguir).
- `VariaveisCriadas` é opcional e indica uma lista separada por virgula de variáveis que serão criadas junto da declaração do enum com o tipo que está sendo declarado, é a única forma de criar variáveis do tipo de um enum anônimo (além claro, do uso de `typeof` com uma variável criada desta forma).

Logo uma enumeração que busca ter valores inteiros indicando dias da semana pode ser escrita da seguinte forma: 
```c
enum DiasDaSemana{
    SEGUNDA_FEIRA,
    TERCA_FEIRA,
    QUARTA_FEIRA,
    QUINTA_FEIRA,
    SEXTA_FEIRA,
    SABADO,
    DOMINGO
};
```

Neste caso os valores seguem uma sequência, com `SEGUNDA_FEIRA` sendo 0 e `DOMINGO` sendo 6.

O primeiro campo do enum sempre começa em 0, a menos que seu valor seja definido com uma expressão constante, já os outros campos tem o valor do campo anterior + 1.

É possível, declarar os possíveis valores de enums com qualquer expressão constante usando `= VALOR` após a declaração do nome do valor do enum, onde `VALOR` é uma expressão constante que pode incluir constantes definidas previamente na mesma declaração de enum.

Isso pode ser visto no exemplo abaixo: 
```c
enum VersoesWindows{
    WINDOWS98 = 98,
    WINDOWS2000 = 2000,

    WINDOWS7 = 7,
    WINDOWS8,      //Tem valor 8
    WINDOWS10 = 10,
    WINDOWS11,    //Tem valor 11

    WINDOWS_MAIS_RECENTE = WINDOWS11, //Usa uma constante declarada neste mesmo enum
};
```

## Regras para tipo compatível
Até antes do `C23`, todas as constantes definidas em um `enum` são consideradas como do tipo `int` a não ser que elas não possam ser representadas por ele, neste caso os valores são ajustados para o menor tipo de inteiro com sinal ou sem sinal que consiga representar o número. 

Vale lembrar que apesar das constantes do `enum` serem normalmente do tipo `int`, o tipo efetivo que uma variável do tipo de um enum pode ser qualquer tipo inteiro, incluindo `char`, que tenha capacidade suficiente para guardar qualquer valor definido no enum. 

Vários compiladores geralmente oferecem flags de compilação especificas para forçar tipos `enum` a serem menores ou não, oferecendo opção para que tipos menores do que `int` sejam utilizados caso seja possível.

Devido a falta de garantia quanto ao tamanho real de um enum, é comum que as pessoas evitem `enum` onde é necessário um tamanho fixo e garantido, ou escrevam constantes com valores altos que forcem um determinado limite (como por exemplo uma constante com `0x7FFFFFFF` para forçar um `enum` a ter ao menos 32bits).

No `C23`, as regras anteriores ainda se aplicam, porém, existe uma nova forma de declaração que inclui `: TIPO`, onde `TIPO` é o tipo que será utilizado para representação do `enum`.

Todas as constantes de um `enum` declarado desse forma tem o tipo que foi definido explicitamente.

Aqui temos um exemplo de como ficariam as flags da função [SetWindowPos](https://learn.microsoft.com/pt-br/windows/win32/api/winuser/nf-winuser-setwindowpos) da API do Windows, em um `enum`, utilizando as funcionalidades do `C23` para forçar o valor como inteiro sem sinal de 32bits:

```c
#include <inttypes.h>

enum SetWindowPosFlags : uin32_t { 
    SWP_NOSIZE         = 0x1,
    SWP_NOMOVE         = 0x2,
    SWP_NOZORDER       = 0x4,
    SWP_NOREDRAW       = 0x8,
    SWP_NOACTIVATE     = 0x10,
    SWP_FRAMECHANGED   = 0x20,
    SWP_HIDEWINDOW     = 0x80,
    SWP_NOCOPYBITS     = 0x100,
    SWP_NOREPOSITION   = 0x200,
    SWP_NOSENDCHANGING = 0x400,
    SWP_DEFERERASE     = 0x2000,
    SWP_ASYNCWINDOWPOS = 0x4000,
};
```

## Uso inteligente de enums
Existem algumas técnicas interessantes que podem ser feitas com `enum`, essa seção busca compartilhar essas ideias para que você possa aplicar em seu código, algumas dessas dicas provavelmente envolvem constantes no geral e podem ser realizadas da mesma forma usando o preprocessador com `#define`.

### 1 - `enum` para constantes de números inteiros 

Antes do advento da palavra chave `constexpr`, utilizar `enum` era uma das únicas formas de definir constantes de números inteiros em C sem depender do preprocessador (`#define`), e mesmo com `constexpr` ainda é uma forma muito simples e menos verbosa para declaração de múltiplas constantes relacionadas.

Ao definir um `enum` anônimo e não declarar nenhuma variável do tipo do `enum`, temos efetivamente algo muito similar a criação de constantes.

```c
enum{
    RODAS_CARRO = 4,
    RODAS_MOTO  = 2,
    RODAS_ONIBUS = 8,
    RODAS_CAMINHAO = 14,
};
```

### 2 - Tamanho do `enum`

É muito comum utilizarmos `enum` para indicar um entre uma lista de possíveis etapas, valores, objetos, etc.

Em alguns casos é importante sabermos quantos itens existem no total, porém a linguagem C não oferece nenhuma funcionalidade para tal, ao menos nenhuma que independa de extensões de compilador.

Como toda enumeração sempre começa em 0, caso nenhuma declaração de constante no `enum` pule valores, é possível saber o tamanho ao definir um último item adicional e se limitar a adicionar novos itens sempre antes deste último item.

O exemplo abaixo demonstra isso : 

```c
enum UtensiliosCozinha {
    UTEN_COZINHA_FACA,
    UTEN_COZINHA_GARFO,
    UTEN_COZINHA_COLHER,
    UTEN_COZINHA_ESPATULA,
    UTEN_COZINHA_PEGADOR_MASSA,

    /* Adicionar novos itens acima */ 
    UTEN_COZINHA_QUANTIDADE, //Sempre tem o número de itens do enum
};
```

### 3 - `enum` com bitmask

Neste caso é bem comum em vários headers em C utilizar `#define` para isso, da mesma forma `enum` pode ser utilizado nos mesmos casos, portanto o que está descrito aqui é muito mais uma técnica genérica usada com constantes do que efetivamente com `enum`.

O nome "bitmask" significa "máscara de bits", e é uma técnica que envolve o uso dos bits para codificar valores, geralmente com cada bit representando um valor [booleano](./4-03-booleanos.md).

É possível, por exemplo, colocar até 32 valores booleanos simultâneos em um mesmo bitmask que resida em um inteiro de 32bits, isso significa que é possível condensar múltiplas opções e estados em um mesmo valor e possivelmente checar qualquer combinação deles de forma muito mais simples e eficiente.

A ideia é que assim como descrito nos [operadores bit a bit](./3-operadores.md#operadores-bit-a-bit), é possível realizar operações separadas que checam bits em separado de um mesmo valor.

No exemplo abaixo temos um uso de bitmask para indicar status negativos de um RPG: 

```c
#include <stdio.h>
//É comum utilizarmos (1 << X) ou números em hexadecimal ao definirmos bitmasks
enum StatusNegativo{
    ESTADO_PETRIFICADO = 1 << 0,
    ESTADO_CONGELADO   = 1 << 1,
    ESTADO_CONFUSO     = 1 << 2,
    ESTADO_QUEIMANDO   = 1 << 3,
    ESTADO_ENVENENADO  = 1 << 4,
    ESTADO_SANGRANDO   = 1 << 5,
    ESTADO_DOENTE      = 1 << 6,

    //Caos ocorre quando temos os 3 estados (confusão, queimando e doente)
    ESTADO_CAOS = ESTADO_DOENTE | ESTADO_CONFUSO | ESTADO_QUEIMANDO,
};

void reportaStatus(enum StatusNegativo status)
{
    if(status & ESTADO_PETRIFICADO) puts("Você está petrificado");
    if(status & ESTADO_CONGELADO)   puts("Você está congelado");
    if(status & ESTADO_CONFUSO)     puts("Você está confuso");
    if(status & ESTADO_QUEIMANDO)   puts("Você está queimando");
    if(status & ESTADO_ENVENENADO)  puts("Você está envenenado");
    if(status & ESTADO_SANGRANDO)   puts("Você está sangrando");
    if(status & ESTADO_DOENTE)      puts("Você está doente");

    if((status & ESTADO_CAOS) == ESTADO_CAOS)
        puts("Você está no estado CAOS!");
}

int main()
{
    reportaStatus(ESTADO_DOENTE | ESTADO_CONFUSO);
}
```


### 4 - `enum` local

Como o `enum` obedece o escopo atual, é possível utilizá-lo para definir constantes que só existem dentro de uma função, logo são privadas e não afetam o escopo global.

Exemplo : 
```c
void teste()
{
    enum{
        ETAPA1,
        ETAPA2,
        ETAPA3,
        ETAPA4,

        NUM_ETAPAS
    };

    for(int i = ETAPA1; i < NUM_ETAPAS; i++)  {
        //Código ...
    }
}

```