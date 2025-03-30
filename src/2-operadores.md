# Operadores
Os operadores são simbolos utilizados para operar com valores e variaveis.

Os usos dos operadores são diversos e os mesmos são separados em diferentes categorias.

## Operadores de Atribuição
O principal operador de atribuição é o `=`, ele funciona de forma bastante simples : 
```c
variavel = valor;
```  

A variável (ou local na memória) a esquerda, recebe o valor a direita.

No geral os operadores de atribuição, são em sua maioria junções do operador `=` com outros operadores já existentes, portanto vou apenas listar os que existem (aconselho que veja o resto do texto logo a seguir, que explica cada categoria de operadores com mais detalhes)

Para que um valor possa ficar no lado esquerdo de uma atribuição (o lado que recebe), ele deve ao menos indicar um local no qual o valor que está sendo recebido será guardado (seja uma variável, ou o conteúdo em um endereço de memória).

No geral é comum vermos menções disso nos seguintes termos em inglês: 
- `lvalue` : Valor que pode sofrer uma atribuição, vem de `Left Value` (valor a esquerda)
- `rvalue` : Valor que só pode ser usado para leitura, vem de `Right Value` (Valor a direita)

| Operador | Descrição                                      |
| -------- | ---------------------------------------------- |
| `=`      | Atribuição                                     |
| `+=`     | Soma e atribuição                              |
| `-=`     | Subtração e atribuição                         |
| `*=`     | Multiplicação e atribuição                     |
| `/=`     | Divisão e atribuição                           |
| `%=`     | Resto da divisão e atribuição                  |
| `<<=`    | Deslocamento de bit para esquerda e atribuição |
| `>>=`    | Deslocamento de bit para direita e atribuição  |
| `&=`     | AND bit a bit e atribuição                     |
| `^=`     | XOR e atribuição                               |
| `⏐=`     | OR bit a bit e atribuição                      |

Os operadores de atribuição também geram como "resultado", o valor atribuido, de forma que seja possível utilizar eles em outras expressões : 
```c
    //Com isso, x = 10 e y = 15 
    int x, y, z; 
    y = (x = 10) + 5; 

    /*
        Todas as três variaveis são iguais a 20 agora
        Como a atribuição é avaliada da direita para esquerda,
        podemos dizer que o que ocorre é :  

        Z recebe 20, Y recebe Z, X recebe Y 
    */
    x = y = z = 20;
```

## Operadores Aritméticos
Os operadores aritméticos são utilizados para efetuar as quatro operações matemáticas básicas, junto da operação de resto de divisão, que está intimamente relacionada a divisão.

| Operador | Descrição        |
| -------- | ---------------- |
| `+`      | Soma             |
| `-`      | Subtração        |
| `*`      | Multiplicação    |
| `/`      | Divisão          |
| `%`      | Resto da divisão |

Acredito que os operadores aritméticos sejam bem intuitivos para quem já estudou matemática, mas na dúvida eis aqui um exemplo demonstrando todos:
```c
    int soma, sub, mul, div, rest;
    int x = 10;
    int y = 5;

    soma = x + y; //x + y = 15
    sub  = x - y; //x - y = 5
    mul  = x * y; //x * y = 50
    div  = x / y; //x / y = 2
    rest = x % y; //x % y = 0 (não há resto)
```  

## Operadores de Incremento/Decremento
Estes são operadores que são de certa forma especiais, seu funcionamento é bastante simples, mas suas implicações são muitas, pois causam "efeitos colaterais", assim como os operadores de atribuição.

No geral dizemos que uma expressão ou função causa efeitos colaterais quando ela tem algum efeito observável além de produzir um valor, um grande exemplo disso seria uma operação que modifica o valor operado, ou realiza operações adicionais como escritas em
arquivos.

| Operador | Descrição        |
| -------- | ---------------- |
| `++`     | Incremento       |
| `--`     | Decremento       |

O operador de incremento, aumenta em 1 o valor de uma variável, e o operador de decremento, diminuí em 1 o valor de uma variável, o que é diferente dos operadores de soma e subtração que não modificam a variável e apenas geram um valor. 

O curioso desses operadores é que, a posição onde são colocados altera o comportamento, portanto `++x` se comporta diferente de `x++`. Essa diferença se da pelo valor resultante da expressão, pois ao utilizarmos o operador como prefixo (`++x` ou `--x`), o valor é modificado primeiro e depois lido, e ao utilizarmos um sufixo (`x++` ou `x--`), o valor é lido e depois modificado.

```c
    int i = 0;

    i++; //Aumenta "i" em 1
    i--; //Diminui "i" em 1

    //Neste ponto, i é zero
    int x = ++i;  //"x" é 1 e "i" é 1 (pois i foi incrementado e depois lido)
    int y = i++;  //"y" é 1 e "i" é 2 (pois i foi lido e depois incrementado)
    int z = --i;  //"z" é 0 e "i" é 1 (pois i foi decrementado e depois lido)
    int w = i--;  //"w" é 0 e "i" é 0 (pois i foi lido e depois decrementado)
```

Poderiamos dizer que :
- `++x` é equivalente a `(x+=1)`
- `--x` é equivalente a `(x-=1)`
- `x++` é equivalente a ler `x` e depois executar `x+=1` em outra linha de código
- `x--` é equivalente a ler `x` e depois executar `x-=1` em outra linha de código

## Operadores Booleanos
Todas as operações booleanas resultam apenas em `Verdadeiro` ou `Falso`, dizer que um valor é "Booleano" significa que ele só pode ter um desses dois valores.

As definições de `Verdadeiro` e `Falso` : 
- `Falso` : Valor igual a zero
- `Verdadeiro` : Qualquer valor diferente de zero

Essas regras são válidas para qualquer lugar que deseja ler um valor qualquer como booleano.

Porém, para que seja possível atribuir `Verdadeiro` como um valor único, o valor utilizado para atribuir `Verdadeiro` na linguagem C é `1`.
Portanto todos os operadores booleanos sempre resultam em 0 (`Falso`) ou 1 (`Verdadeiro`).


| Operador | Descrição        |
| -------- | ---------------- |
| `==`     | Igual a          |
| `!=`     | Diferente de     |
| `>`      | Maior que        |
| `>=`     | Maior ou igual a |
| `<`      | Menor que        |
| `<=`     | Menor ou igual a |
| `!`      | NOT lógico       |
| `&&`     | AND lógico       |
| `⏐⏐`     | OR lógico        |

A maioria dos operadores são auto explicativos ao considerarmos a descrição juntamente ao fato de que todos operadores só retornam `Verdadeiro` ou `Falso`.

Com exceção, claro, dos três últimos operadores da tabela:

- `Not lógico` : Opera com apenas um valor, invertendo seu estado de `Verdadeiro`/`Falso`, de forma que `0` vire `1` e valores diferentes de zero, virem `0`, sendo literalmente a lógica ao usar `Não`. 
- `AND lógico`: Resulta em `Verdadeiro` quando ambos valores sejam `Verdadeiro`, sendo literalmente a lógica ao usar `E`. 
- `OR lógico`: Resulta em `Verdadeiro` quando um dos valores é `Verdadeiro`, sendo literalmente a lógica ao usar `Ou`.

> Agora, para exemplificar, vamos pegar de exemplo a seguinte frase :
>
> "João é inteligente, mas não passou na prova"
>
> Logo, "João é inteligente" = `Verdadeiro` e "João passou na prova" = `Falso`
>
> Ao usar `Not lógico`, "João `não` é inteligente", uma preposição que era `Verdadeira` se tornou `Falsa` pois invertemos o sentido dela.
>
> Ao usar `AND lógico`, "João é inteligente `e` passou na prova", vemos que a frase se torna `Falsa` pois João não passou na prova.
>
> Ao usar `OR lógico`, "João é inteligente `ou` passou na prova", a frase se torna `Verdadeira`, pois mesmo que não tenha passado, acertamos que João é inteligente.

### Avaliação de curto circuito 
Os operadores `AND lógico` e `OR lógico` apresentam um comportamento extremamente único, a chamada "avaliação de curto circuito", com ela, os valores só são avaliados quando isso é necessário.

Como assim quando isso é "necessário" ? bem se eu sei que um `AND lógico` só será `Verdadeiro` se ambos os valores forem `Verdadeiros`, ao saber que o primeiro dos valores é `Falso`, não há necessidade de avaliar o segundo valor para saber o resultado da expressão.

O mesmo se aplica para o `OR lógico`, se o primeiro valor é `Verdadeiro`, eu não preciso saber o resultado do segundo.

Logo caso o primeiro valor já seja o suficiente para estabelecer o resultado da expressão, a avaliação do segundo valor é ignorada, isso se torna ainda mais poderoso quando juntamos isso com [Funções](./6-funcoes.md).

```c
    /*
        Neste caso, não faria sentido se conectar ao servidor 
        se ele não estivesse funcionando, a ordem das checagens importa
        e a avaliação de curto circuito impede o programa de perder tempo
        tentando conectar num servidor que não está de pé
    */
    if(servidorEstaVivo() && conectarServidor())
        printf("Conectei!\n");
    
```

## Operadores Bit a Bit 
Antes de olhar esta parte sobre operadores bit a bit, aconselho fortemente que veja a seção sobre [Números Binários](#x-01-numeros-binarios.md), ou deixe este pedaço para depois.

Assume-se ao menos um conhecimento acerca de números binários e que o leitor saiba o que são BITS.

| Operador | Descrição                         |
| -------- | --------------------------------- |
| `~`      | NOT bit a bit                     |
| `&`      | AND bit a bit                     |
| `⏐`      | OR bit a bit                      |
| `^`      | XOR bit a bit                     |
| `<<`     | Deslocamento de bit para esquerda |
| `>>`     | Deslocamento de bit para direita  |

- `NOT bit a bit` : Realiza uma inversão de cada bit do valor individualmente.
- `AND bit a bit` : Realiza uma operação similar ao `AND lógico` em cada bit, gerando um resultado onde apenas os bits em comuns que os dois valores compartilham estão ativos.
- `OR bit a bit` : Realiza uma operação similar ao `OR lógico` em cada bit, gerando um resultado onde os bits estão ativos a não ser que ambos os valores não tenham aquele bit ativo.
- `XOR bit a bit` : Traduzido, é um `OU Não Exclusivo`, seu papel é similar ao `OR lógico` porém se ambos os valores forem `Verdadeiros`, o resultado é `Falso`, porém o `XOR` em C não tem versão lógica, portanto ele é sempre bit a bit.
- `Deslocamento de bit para esquerda`: Desloca os bits para esquerda, resultando geralmente, numa multiplicação por 2 elevado na `X`, sendo `X` o número do deslocamento (claro que para valores com sinal, isso muda pois podemos acabar gerando um número negativo ao alcançar o bit do sinal)
- `Deslocamento de bit para direita`: Desloca os bits para direita, resultando geralmente, numa divisão por 2 elevado na `X`, sendo `X` o número do deslocamento.

Operações comuns utilizando os operadores bit a bit: 
```c
    //Conseguir o valor que equivale ao bit Nº X (começando por bit 0)
    bit = 1 << X;

    //Ativar um bit
    valor |= bit; 

    //Remover um bit
    valor &= ~bit;

    //Checar se um bit está ativo
    if(valor & bit)
        printf("Está ativo\n");

    //Inverter o estado do bit (com XOR)
    valor ^= bit; 
```

## Operadores Especiais
A linguagem C também apresenta alguns operadores adicionais que não se encaixam nas outras categorias, estes operadores geralmente fornecem funcionalidades únicas.

| Operador   | Descrição                         |
| ---------- | --------------------------------- |
| `()`       | Chamada de função                 |
| `,`        | Operador virgula                  |
| `(tipo)`   | Conversão de tipo                 |
| `? :`      | Ternário                          |
| `sizeof`   | Tamanho de                        |
| `_AlignOf` | Alinhamento de (`C11`)            |
| `_Alignas` | Alinhar como (`C11`)              |
| `typeof`   | Tipo de (`C23`)                   |

### Chamada de função 
 Ao chamar uma [função](./6-funcoes.md) , utilizamos o operador `()`, preenchido com uma lista separada por virgulas de zero ou mais argumentos, toda chamada de função é precedida de uma expressão que resulta num ponteiro de função (seja o nome direto da função, ou uma variável guardando o endereço de uma função).

### Operador virgula
Este operador é utilizado para adicionar uma expressão adicional, ignorando o resultado da expressão anterior, mas ainda levando em consideração os seus efeitos colaterais. Por exemplo ao escrevermos `a,b`, os efeitos colaterais de `a` são aplicados, porém, o resultado da expressão como um todo será o resultado de `b`.

```c
    int x;
    
    // O efeito colateral de (x = 10) é aplicado, tornando o valor de X como 10
    // Porém o valor lido, é o da expressão  X + 5, que será 15.
    const int y = x = 10, x + 5;

    // Neste caso, Z será 70, pois os efeitos colaterais de ambas
    // expressões são aplicadas e o resultado da expressão será
    // a última, (2 * x) que é 70
    const int z = x = 20, x += 15, 2 * x;
``` 
### Conversão de tipos
    
Este operador é utilizado para converter um tipo para outro, algumas conversões tem comportamentos especificos como por exemplo : 
  
- `Para void`: É possível realizar conversões para o tipo `void` que simboliza uma ausência de tipo/valor, isso é útil por exemplo para ignorar o retorno de algumas funções intencionalmente quando, por exemplo, configuramos o compilador para avisar quando um retorno de função é ignorado.
- `Inteiros maiores para menores`: Ao converter números inteiros que usam mais bytes para tipos inteiros menores, o valor é truncado, cortando fora os bytes que excedem o tamanho do tipo menor.
- `Ponteiro para inteiro`: É possível converter ponteiros para tipos inteiros, no geral é aconselhável utilizar o tipo `intptr_t` ou `uintptr_t` que naturalmente tem o mesmo tamanho em bytes e desde o `C99` funcionam de forma garantida se a implementação fornece esses tipos, a conversão de um ponteiro nulo não necessariamente precisa dar 0 ao ser convertida para um inteiro.
- `Inteiro para ponteiro`: É possível converter números inteiros para ponteiros, porém não há nenhuma garantia que o valor esteja alinhado ou aponte para uma variável do tipo certo.
- `Ponteiro de valor para ponteiro de valor`: Qualquer ponteiro de valor (que não seja de função) pode ser convertido para um tipo ponteiro de "caractere" (`char`, `signed char`,`unsigned char`) para que seja possível copiar o objeto ou ler sua representação interna em bytes, outras conversões são aceitas mas idealmente devem ter um alinhamento adequado.
- `Ponteiro de função <-> ponteiro de valor`: Conversões entre esses dois tipos são proibidas, porém, vários compiladores suportam eles como extensões e ao menos no Windows e Linux, eles são necessários para carregar funções de bibliotecas dinâmicas utilizando [`GetProcAddress`](https://learn.microsoft.com/pt-br/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress) e [`dlsym`](https://man7.org/linux/man-pages/man3/dlsym.3.html) respectivamente.
- `Ponteiros para ponto flutuante`: Conversões entre ponteiros e variaveis de ponto flutuante são proibidas.


```c
    #include <inttypes.h>

    //Ignorando retorno
    (void)printf("Teste...\n");

    //"valor2" é limitado a apenas a metade baixa de "valor1"
    int32_t valor1 = 0xF7FFF;
    int16_t valor2 = (int16_t)valor1; //0x7FFF = 32767

    //dlsym é do tipo "void*", isso efetivamente só funciona
    //como uma extensão da linguagem pelos compiladores.
    fazCafe = (void (*)(int)) dlsym(biblioteca,"fazerCafe");
    fazCafe(4);
```

### Ternário
O operador ternário apresenta a sintaxe `CONDIÇÃO ? VALOR_SE_VERDADE : VALOR_SE_FALSO`, e de acordo com o valor de `CONDIÇÃO`, escolhe qual valor será avaliado e usado como resultado da expressão. 

A ideia do operador ternário é substituir o uso de condicionais por algo menor que possa ser utilizado em outras expressões.

Exemplos: 
```c
    //O seguinte bloco ternário : 
    valor = condicao ? a : b;

    //Tem o mesmo comportamento de : 
    if(condicao) { 
        valor = a;
    } else { 
        valor = b; 
    }

    //Este bloco ternario : 
    valor = condicao1 ? a : 
            condicao2 ? b : 
            condicao3 ? c 
            /* else */: d;
    
    //Tem o mesmo comportamento de : 
    if(condicao1) { 
        valor = a;
    } else if(condicao2) {
        valor = b;
    } else if(condicao3) {
        valor = c;
    } else { 
        valor = d;
    }
```

### Operador sizeof
O operador `sizeof` pode ser usado para obter o tamanho de qualquer tipo, variável ou valor, diretamente em bytes.

No geral mesmo quando um valor é passado ao `sizeof`, o valor não é realmente avaliado e sim o tamanho que o tipo daquele valor teria, portanto expressões que ocasionariam em erros ou crashs no programa ao serem avaliadas, não o causam, pois não são realmente avaliadas. 

Na maioria dos casos, com exceção de arrays de tamanho variável, o operador `sizeof` é avaliado durante a compilação, portanto seu uso não afeta o tempo de execução (nesses casos ele é efetivamente uma constante), o tipo do valor resultante do `sizeof` é sempre do mesmo tipo de `size_t`.

```c
    int arr[40];

     //igual a 40 * sizeof(int)
    sizeof(arr);

    //geralmente 4 nos processadores/compiladores modernos
    sizeof(int); 

    /* É possível utilizar "sizeof" na mesma linha que uma variável
       é declarada e mesmo que "*pessoa" seja indefinido, o operador 
       "sizeof" funciona normalmente                                 */    
    struct Pessoa *pessoa = malloc(sizeof(*pessoa));
```

### Operador _AlignOf
O operador `_AlignOf` foi adicionando no `C11` junto da biblioteca `stdalign.h` que incluia definições para usar `_AlignOf` com o nome `alignof`.

No `C23`, o operador `alignof` foi adicionado a linguagem, dispensando o `stdalign.h`.

Este operador funciona de forma similar ao `sizeof`, porém ao invés de obter o tamanho em bytes do tipo, ele obtêm a quantidade de bytes consideradas o requisito de alinhamento do tipo.

O requisito de alinhamento é um número do tipo `size_t` que indica o número de bytes do qual o endereço deve ser múltiplo para que esse
objeto possa ser alocado.

> Algumas arquiteturas proibem alguns acessos a valores não alinhados
> (por exemplo ler 2 bytes de uma vez de um endereço que tem um valor impar).

### Operador _Alignas
Da mesma forma que o operador `_AlignOf`, foi adicionada no `C11`com a `stdalign.h` para permitir seu uso como `alignas`, que foi incorporado a linguagem no `C23`, dispensando o `stdalign.h`.

Este operador é usado para forçar um requisito de alinhamento diferente em variaveis.

A utilidade desse operador é um tanto difícil de entender, mas ele basicamente funciona como uma forma de controlar o alinhamento para possivelmente melhorar a performance.

Um exemplo do seu uso seria alinhar ao tamanho do cache do processador, de forma que variaveis diferentes caiam em linhas diferentes do cache (o que pode otimizar o acesso quando múltiplos threads estão acessando os elementos).

Manter objetos na mesma linha do cache poderia forçar diferentes threads a competir pelo acesso de uma mesma linha de cache (fazendo com que um tenha que "esperar" o outro). 

```c
#include <stdalign.h>
#include <stdio.h>

/**
    Digamos que cada linha do cache tem 64 bytes
    Estou forçando este objeto a estar alinhado ao cache,
    logo threads nunca vão "competir" para acessar elementos
    próximos
*/
struct alignas(64) Arquivo {
    FILE   *arq; /* identificador do arquivo */
    size_t tam;  /* tamanho do arquivo */
};
```

### Operador typeof
O operador `typeof` foi adicionado apenas no `C23`, ele sempre resulta num tipo de variável, que pode ser utilizado em todos lugares onde um tipo de variável normalmente seria utilizado (seja na definição de variaveis, parâmetros de função, membros de uma estrutura,etc).

De forma similar ao operador `sizeof`, ele funciona com tipos e variaveis, e não avalia os valores das variaveis que ele recebe, apenas
as usa para extrair o tipo.

```c
int a;
typeof(a) b; //igual a "int b"


struct Pessoa pessoa;
typeof(&pessoa) ptr = &pessoa; 


//Algumas declarações também podem ser feitas 
// de uma forma "diferente" usando "typeof" : 

//Array de ponteiros de função do tipo "int FUNCAO(int)"
int (*arr[])(int);
//OU
typeof(int(int)) *arr[];

//Três ponteiros para inteiro
int *p,*q,*r;
//OU
typeof(int*) p,q,r;
```

## Ordem de precedência

A ordem de precedência é a prioridade com a qual os operadores são aplicados.

A associatividade é a ordem que os operandos são avaliados e será abreviada para simbolos de forma a simplificar a tabela : 
- `>` : Esquerda para direita
- `<` : Direita para esquerda

Os itens mais no topo da tabela a seguir, são os itens de maior prioridade (ordem decrescente):

| Operador(es)      | Descrição                                        | Associatividade |
| ----------------- | ------------------------------------------------ | :-------------: |
| `++`/`--`         | Incremento/Decremento (sufixo)                   |        >        |
| `()`              | Chamada de função                                |        >        |
| `[]`              | Subscrição de array/matriz                       |        >        |
| `.`               | Acesso de membro de estrutura/união              |        >        |
| `->`              | Acesso de membro de estrutura/união via ponteiro |        >        |
| `(tipo){lista}`   | Literal composto (`C99`)                         |        >        |
| `++`/`--`         | Incremento/Decremento (prefixo)                  |        <        |
| `+`/`-`           | Sinal de valor (ex: +5 ou -5)                    |        <        |
| `!`/`~`           | NOT lógico e bit a bit                           |        <        |
| `(tipo)`          | Conversão de tipo                                |        <        |
| `*`               | Indireção (desreferenciar)                       |        <        |
| `&`               | Endereço de                                      |        <        |
| `sizeof`          | Tamanho de                                       |        <        |
| `_AlignOf`        | Alinhamento de (`C11`)                           |        <        |
| `*`/`/`/`%`       | Multiplicação, divisão e resto                   |        >        |
| `+`/`-`           | Soma e subtração                                 |        >        |
| `<<`/`>>`         | Deslocamento de bit                              |        >        |
| `<`/`<=`/`>`/`>=` | Comparações de maior/menor                       |        >        |
| `==`/`!=`         | Igual a ou diferente de                          |        >        |
| `&`               | AND bit a bit                                    |        >        |
| `^`               | XOR bit a bit                                    |        >        |
| `⏐`               | OR bit a bit                                     |        >        |
| `&&`              | AND lógico                                       |        >        |
| `⏐⏐`              | OR lógico                                        |        >        |
| `?:`              | Condição ternária                                |        <        |
| `=`               | Atribuição                                       |        <        |
| `+=`/`-=`         | Soma/subtração e atribuição                      |        <        |
| `*=`/`/=`/`%=`    | Multiplicação/divisão/resto e atribuição         |        <        |
| `<<=`/`>>=`       | Deslocamento de bit e atribuição                 |        <        |
| `&=`/`^=`/`⏐=`    | AND/XOR/OR bit a bit e atribuição                |        <        |
| `,`               | Operador virgula                                 |        >        |

> No geral muitos programadores utilizam parenteses para forçar uma certa ordem de precedência.
> Isso diminui a necessidade de um estudo mais minucioso dessa tabela, ao mesmo tempo que facilita a leitura do código
> para pessoas que não a decoraram, ou que tem pouca experiência