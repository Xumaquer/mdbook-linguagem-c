# Literais
Literais são valores de um determinado tipo que podem ser escritos diretamente no código fonte.

Diferente da crença de alguns, TODOS valores literais, sem exceção, tem um tipo.

Logo até mesmo escrever números diretamente no código fonte como `1`, implicam um tipo de variável que é decidido de acordo com algumas regras.

## Literais de caracteres
Os literais de caracteres já foram explicados no [capítulo sobre caracteres](./4-01-caracteres.md#literais-de-caractere).

Vamos apenas recapitular os tipos : 

```c
'A'   // char
L'A'  // wchar_t
u'A'  // char16_t (C11)
U'A'  // char32_t (C11)
u8'A' // char8_t  (C23)
```

## Literais de inteiros
Em literais de inteiros, os prefixos são utilizados para determinar a base númerica (decimal, hexadecimal, binário, octal) enquanto os sufixos são utilizados para determinar os tipos (`unsigned int`, `int`, `long`, `long long`, etc).

Prefixos:

| Prefixo            | Base Númerica   | Exemplo |
| :----------------: | :-------------: | :-----: |
| Nenhum             | Decimal         | 1       | 
| `0`                | Octal           | 01      |
| `0x` ou `0X`       | Hexadecimal     | 0x1     |
| `0b` ou `0B`       | Binário (`C23`) | 0b1     |

Sufixos:

| Sufixo             | Tipo / Modificador   |
| :----------------: | :------------------: |
| Nenhum             | `int`                | 
| `U` ou `u`         | `unsigned`           |
| `L` ou `l`         | `long`               | 
| `LL` ou `ll`       | `long long` (`C99`)  |
| `wb` ou `WB`       | `_BitInt(N)` (`C23`) |

Os sufixos de tipo podem ser utilizados juntos com o sufixo `U` que indica que o tipo não tem sinal.

Aconselha-se o uso do sufixo `U` antes dos outros sufixos (`UL` para `unsigned long` ao invés de `LU`), pois antes do `C23` utilizar o sufixo `U` por último em conjunto com outro sufixo não estava no padrão da linguagem.

No caso do sufixo `WB`, teremos um `_BitInt(N)` onde `N` é o menor número de bits necessários para representar o valor, este sufixo também pode ser utilizado em conjunto com `U`.

Vale lembrar que não existem "literais de inteiros negativos", todos literais inteiros SEMPRE são positivos, existem duas formas de conseguir um valor negativo:
```c
//Colocar um menos na frente (convertendo-o para negativo)
-5;                

//Realizar uma conversão implicita ou explicita para um tipo com sinal
(signed char)0xFF;  
``` 

Todos literais de inteiros podem ser "promovidos" para tipos maiores, caso o valor escrito esteja "acima" dos limites, mas há algumas diferenças de comportamento :
- Todos literais de tipos sem sinal só podem ser promovidos para tipos sem sinal (`unsigned int` -> `unsigned long` -> `unsigned long long`)
- Antes do `C99` literais decimais de tipos com sinal poderiam ser promovidos até o tipo `unsigned long` caso `long` não fosse o suficiente para representar o valor.
- Desde o `C99`, literais decimais de tipos com sinal só podem ser promovidos para tipos com sinal (`int` -> `long` -> `long long`).
- Literais com prefixo (binário, octal ou hexadecimal) sem o sufixo `U` podem ser promovidos para tipos sem sinal (`int` -> `unsigned int` -> `long` -> `unsigned long` -> `long long` -> `unsigned long long`).

Para o exemplo abaixo, vamos considerar `int` como 16bits, `long` como 32bits e `long long` como 64bits : 
```c
2147483648; //(Pre-C99) unsigned long
            //(C99) long long

//32768 é 0x8000 em decimal
32768;   //long
0x8000;  //unsigned int

0x1FFFF; //long
```

Desde o `C23` também podemos utilizar o caractere `'` como separador opcional entre digitos (que será ignorado pelo compilador) : 
```c
int reais = 29'99;
```

Devido a forma como os compiladores são implementados, utilizar um número hexadecimal terminando em `E` seguido do operador `+` ou `-` causa um erro de compilação:
```c
0xE+2;   //Errado
0xE + 2; //Correto
```

## Literais de ponto flutuante
Nos literais de ponto flutuante, temos a seguinte sintaxe :
```
prefixo numero . fracao expoente sufixo 
```

Assim como nos literais de ponto flutuante, o prefixo determina a base númerica (decimal ou hexadecimal) enquanto o sufixo controla o tipo (`float`, `double` ou `long double`), enquanto o expoente pode ser utilizado para facilitar a escrita de alguns números.

Prefixos :

| Prefixo              | Base Númerica   | Exemplo |
| :------------------: | :-------------: | :-----: |
| Nenhum               | Decimal         | 1.0     | 
| `0x` ou `0X` (`C99`) | Hexadecimal     | 0x1p0   |

Expoentes : 

| Expoente           | Multiplica-se por | Exemplo |
| :----------------: | :--------------:  | :-----: |
| Nenhum             | Inalterado        | 1.0     |
| `e` ou `E`         | 10 elevado a X    | 1e-1    | 
| `p` ou `P` (`C99`) | 2  elevado a X    | 0x1p1   |

Sufixos :

| Sufixo             | Tipo                  |
| :----------------: | :-------------------: |
| Nenhum             | `double`              | 
| `F` ou `f`         | `float`               |
| `L` ou `l`         | `long double`         |
| `df` ou `DF`       | `_Decimal32` (`C23`)  |
| `dd` ou `DD`       | `_Decimal64` (`C23`)  |
| `dl` ou `DL`       | `_Decimal128` (`C23`) |

Algumas regras a respeito dos literais de ponto flutuante: 
- O expoente `e` ou `E` só pode ser utilizado em literais decimais, mas é opcional.
- O expoente `p` ou `P` só pode ser utilizado em literais hexadecimais, e é obrigatório (`0x1f` é `int`, mas `0x1p0f` é `float`).
- O sufixos para tipos `_DecimalX` não podem ser utilizado com o prefixo de hexadecimal.
- Literais decimais de ponto flutuante precisam ter o expoente `e`/`E` ou um ponto (`e3` é inválido, `1e1` é válido, `.1` é válido).

Exemplo demonstrando vários literais diferentes : 
```c
.05L;     //long double
1;        //int
1.0;      //double
0x1p1;    //double, tem valor = 2 (1 * 2 elevado a 1)
1ef;      //inválido
1e0f;     //float
0x1e3;    //int
0x2p-1;   //double, tem valor = 1 (2 * 2 elevado a -1)
123.45dd; //_Decimal64 (C23)
```
Assim como em literais de inteiros : 
- Podemos utilizar `'` no `C23` como separador entre digitos em literais de ponto flutuante (eles serão ignorados pelo compilador).
- Não existem literais de ponto flutuante negativos, eles são "gerados" usando o operador `-`.

No geral operações aritméticas são realizadas levando em consideração o tipo da variável, o que pode levar o usuário ao erro  : 
```c
double a = 5 / 2;   //2   (int/int    = divisão de inteiro)
double b = 5 / 2.0; //2.5 (int/double = divisão com double)

//Considerando int 32bits e complemento de 2
//O valor resultante será -2147483648 devido a um overflow
double c = 2147483648 + 1;   //-2147483648 (int + int)
double d = 2147483648.0 + 1; //2147483649  (double + int)
```

## Literais de string

As literais de string já foram explicadas no [capítulo sobre strings](./9-strings.md#tipos-de-strings-literais), mas aqui vamos relembrar os tipos existentes :

```c
"T1"   //char*
u8"T2" //char* (C11) ou char8_t* (C23)
u"T3"  //char16_t* (C11)
U"T4"  //char32_t* (C11)
L"T5"  //wchar_t*
```

## Literais extras
Desde o `C23` : 
- `true` e `false` foram introduzidos como palavras chave da linguagem e tem efetivamente o tipo `bool`.
- `nullptr` foi introduzido como palavra chave da linguagem e tem efetivamente o tipo `nullptr_t`.

## Literais Compostos
Desde o `C99`, podemos escrever literais compostos, que são utilizados para escrever valores literais para qualquer tipo que podem ser usados como `lvalue`.

`lvalue` vem de "left value", ou seja, "valor a esquerda", indicando que podemos atribuir valores a expressão e seu endereço pode ser utilizado.

A sintaxe para definição de literais compostos é similar a inicialização de arrays e structs : 
```c
(modificadores tipo){lista de inicialização}
```
- Desde o `C23`, podemos adicionar os `modificadores`: `constexpr`, `static`, `register` ou `thread_local`.
- Em `tipo` podemos colocar qualquer tipo que não seja um array de tamanho variável, não é necessário inicializar o array.
- Na `lista de inicialização` podemos colocar um ou mais valores separados com virgula que inicializam o tipo descrito, desde o `C23` podemos deixar essa lista vazia, tendo uma inicialização similar a de arrays e estruturas (campos não preenchidos viram 0).

Exemplo de uso para criar um array de strings e outro de inteiros : 
```c
#include <stdlib.h>
#include <stdio.h>

//É relativamente comum utilizar um ponteiro NULL para indicar o "fim" de um array de strings
void escreveStrings(const char **strings)
{
    while(*strings) { 
        printf("%s\n", *strings);
        strings++;
    }
}

void escreveNumeros(int *numeros, unsigned int tam)
{
    while(tam--)
        printf("%d\n", *numeros++);
}

int main()
{
   escreveStrings((const char*[]){"Ola","Como","Vai","Voce?", NULL});
   escreveNumeros((int[]){1,2,3,4}, 4);
}
```

Também podemos utilizar inicializadores designados para inicializar um literal composto.

O exemplo abaixo demonstra vários literais compostos : 
```c
//Pegando endereço de um literal composto
char *test = &(char){'A'}; 

struct GString gstr;

//É possível utilizar literais compostos para atribuir uma estrutura
//Permitindo uma atribuição com sintaxe similar a inicialização!
gstr = (struct GString) {
    .str = "Tutorial",
    .len = sizeof("Tutorial"),
    .allocated_len = sizeof("Tutorial")
};

// Inicializadores designados, outros elementos serão 0, array terá tamanho 8
(int[]) {
    [1] = 5,
    [3] = 10,
    [5] = 15,
    [7] = 20,
};

//Dependendo da implementação, pode ser 1, pois o padrão do C permite
//reutilizar a mesma string entre literais compostos e strings literais
(const char[]){"abc"} == "abc";
```

No geral, todos os literais compostos criados dentro de algum bloco tem [`duração automática`](./4-00-variaveis.md#modificadores-de-armazenamento) e fora de blocos no escopo global tem [`duração estática`](./4-00-variaveis.md#modificadores-de-armazenamento) a menos que utilizemos o modificador `static` para forçar `duração estática`.
