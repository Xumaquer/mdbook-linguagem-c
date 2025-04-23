# Palavras Chave
Palavras chave são palavras reservadas da linguagem que não podem ser usadas em lugares onde um nome arbitrário pode ou deve ser definido. Essas palavras são utilizadas para identificar comandos ou especificações únicas da linguagem.

## Lista de palavras chave

Algumas das palavras chave da linguagem foram introduzidas depois, em novos padrões da linguagem, nesses casos, os nomes dos padrões que introduziram a palavra chave estão escritos ao lado na tabela.

As palavras chave presentes na linguagem são : 

|                   |                  |                       |                        |
| ----------------- | ---------------- | --------------------- | ---------------------- |
| `alignas` (C23)   | `extern`         | `sizeof`              | `_Alignas` (C11)       |
| `alignof` (C23)   | `false` (C23)    | `static`              | `_AlignOf` (C11)       |
| `auto`            | `float`          | `static_assert` (C23) | `_Atomic` (C11)        |
| `bool`            | `for`            | `struct`              | `_BitInt` (C23)        |
| `break`           | `goto`           | `switch`              | `_Bool` (C99)          |
| `case`            | `if`             | `thread_local` (C23)  | `_Complex` (C99)       |
| `char`            | `inline`         | `true` (C23)          | `_Decimal128` (C23)    |
| `const`           | `int`            | `typedef`             | `_Decimal32` (C23)     |
| `constexpr` (C23) | `long`           | `typeof` (C23)        | `_Decimal64` (C23)     |
| `continue`        | `nullptr` (C23)  | `typeof_unqual` (C23) | `_Generic` (C11)       |
| `default`         | `register`       | `union`               | `_Imaginary` (C99)     |
| `do`              | `restrict` (C99) | `unsigned`            | `_Noreturn` (C11)      |
| `double`          | `return`         | `void`                | `_Static_assert` (C11) |
| `else`            | `short`          | `volatile`            | `_Thread_local` (C11)  |
| `enum`            | `signed`         | `while`               |                        |

A grande maioria das palavras chaves que começam com `_` são normalmente utilizadas junto de bibliotecas que definem macros para utilizar as palavras chaves com outros nomes mais simplificados, os nomes com underline foram introduzidos dessa forma inicialmente para evitar conflitos com código já existente.

No `C23` alguns desses nomes "simplificados" se tornaram palavras chave nativas da linguagem e o uso da palavra chave utilizando `_` se tornou depreciado (suportado mas não recomendado), dispensando também a necessidade de inclusão de uma biblioteca para uso dos nomes "simplificados".

A tabela a seguir mostra as macros e as bibliotecas para cada palavra chave que começa com `_`, detalhando `incorp. C23` para as macros que foram incorporadas a linguagem no `C23` :

| Palavra Chave          | Usado como                    | Definido em     |
| ---------------------- | ----------------------------- | --------------- |
| `_Alignas` (C11)       | `alignas` (incorp. C23)       | `stdalign.h`    |
| `_Alignof` (C11)       | `alignof` (incorp. C23)       | `stdalign.h`    |
| `_Atomic` (C11)        | `atomic_` + nomeTipo          | `stdatomic.h`   |
| `_BitInt` (C23)        | (sem macro)                   |                 |
| `_Bool` (C99)          | `bool` (incorp. C23)          | `stdbool.h`     |
| `_Complex` (C99)       | `complex`                     | `complex.h`     |
| `_Decimal128` (C23)    | (sem macro)                   |                 |
| `_Decimal32` (C23)     | (sem macro)                   |                 |
| `_Decimal64` (C23)     | (sem macro)                   |                 |
| `_Generic` (C11)       | (sem macro)                   |                 |
| `_Imaginary` (C99)     | `imaginary`                   | `complex.h`     |
| `_Noreturn` (C11)      | `noreturn` (incorp. C23)      | `stdnoreturn.h` |
| `_Static_assert` (C11) | `static_assert` (incorp. C23) | `assert.h`      |
| `_Thread_local` (C11)  | `thread_local` (incorp. C23)  | `threads.h`     |

## Sumário das palavras chaves

Muitas das palavras chaves estão relacionadas a outros conceitos e serão explicadas com mais detalhes nos capítulos seguintes.

Essa seção serve como um sumário para demonstrar quais partes do documento apresentam quais palavras chaves.

Sumário das explicações e introduções a palavras chaves presentes no documento : 

- [`Operadores - Operadores especiais`](./3-operadores.md#operadores-especiais) : `sizeof`, `_AlignOf`, `_AlignAs`, `typeof`, `typeof_unqual`.
- [`Variáveis - Modificadores de tipos`](./4-00-variaveis.md#modificadores-de-tipo) : `_Atomic`, `const`, `constexpr`, `restrict`, `volatile`, `signed`, `unsigned`.
- [`Variáveis - Modificadores de armazenamento`](./4-00-variaveis.md#modificadores-de-armazenamento) : `auto`, `register`, `static`, `extern`, `thread_local`. 
- [`Variáveis - Palavra chave typedef`](./4-00-variaveis.md#palavra-chave-typedef) : `typedef`.
- [`Caracteres`](./4-01-caracteres.md) : `char`.
- [`Inteiros`](./4-02-inteiros.md): `char`, `short`, `int`, `long`, `_BitInt`.
- [`Booleanos`](./4-03-booleanos.md): `_Bool`, `bool`, `false`, `true`.
- [`Ponto Flutuante`](./4-04-ponto-flutuante.md): `float`, `double`.
- [`Ponto Flutuante - Números complexos e imaginários`](./4-04-ponto-flutuante.md#números-complexos-e-imaginários): `_Complex`, `_Imaginary`.
- [`Ponto Flutuante - Números decimais`](./4-04-ponto-flutuante.md#números-decimais): `_Decimal32`, `_Decimal64`, `_Decimal128`.
- [`Controle de Fluxo`](./5-controle-de-fluxo.md): `if`, `else`, `switch`, `case`, `default`, `break`, `goto`.
- [`Laços de Repetição`](./6-lacos-de-repeticao.md): `continue`, `while`, `do`, `for`.
- [`Funções`](./7-funcoes.md): `void`, `return`, `inline`, `_Noreturn`.
- [`Enumerações`](./8-01-enumeracoes.md): `enum`.
- [`Estruturas`](./8-03-estruturas.md): `struct`.
- [`Uniões`](./8-04-unioes.md): `union`.

## static_assert
A palavra chave `_Static_assert` foi adicionada no `C11`, mas é acessível através da macro `static_assert` presente na biblioteca `assert.h` que foi incorporada a linguagem no `C23`, dispensando a necessidade de incluir a biblioteca.

Essa palavra chave permite que o usuário certifique que uma condição é `Verdadeira`, caso contrário, um erro de compilação será gerado.

A sintaxe para uso de `static_assert` é  :

```c
#include <assert.h> //Biblioteca para compatibilidade com versões antes do C23

//C11 em diante
static_assert(expressao, mensagem);

//C23 em diante
static_assert(expressao);
```

Onde : 
- `expressao` é a expressão que será avaliada para determinar se um erro de compilação será gerado.
- `mensagem` é um literal de string indicando a mensagem que será exibida quando o erro é gerado, este campo era obrigatório até antes do `C23`.

