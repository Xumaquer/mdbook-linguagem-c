# Macros
Macros são diretivas de preprocessamento que realizam uma substituição textual.

Macros não tem escopo, portanto mesmo se ela for definida dentro de uma função, poderá ser utilizada fora dela.

A sintaxe para definirmos novas macros é : 
```c
//Macro sem conteúdo
#define BUILD_DEBUG

//Macro sem parâmetros
#define TEXTO_ANTIGO "Texto novo"

//Macro com parâmetros
#define MAXIMO(X,Y) (((X) > (Y)) ? (X) : (Y))
```

Dessa forma quando escrevermos : 
```c
BUILD_DEBUG;  //Será substituido por nada
TEXTO_ANTIGO; //Será substituido por "Texto novo"

//Resultará em 5 
//expressão ((5) > (3) ? (5) : (3))
int test = MAXIMO(5,3); 
```

Lembrando que no `C23`, podemos substituir o uso de macros para definições de constantes utilizando variáveis como `constexpr` ou até `enum`, o problema de `constexpr` é que ele é bem recente, acaba sendo problemático quando desejamos fazer bibliotecas que funcionem com versões anteriores do C, no C++ essa noção já é mais comum pois `constexpr` foi adicionado desde o `C++11`.

## Funções-Macros
Devido a falta de algumas funcionalidades alto nível na linguagem, é normal que programadores utilizem macros com parâmetros para implementar algumas funcionalidades utilizando macros.

A vantagem de utilizar macros é que podemos fazer coisas que normalmente não seriam possíveis utilizando apenas funções, a desvantagem, é que abrimos portas para que o programador possa implementar coisas realmente imprevisíveis, difíceis de testar, etc.

É preferível utilizar funções normais quando elas suficientes, mas as funções macros tem seu espaço, o importante é saber utiliza-las de forma consciente, algo que programadores adquirem com experiência.

Para macros de múltiplas linhas é comum o uso de `do while(0)`, que pode inicialmente parecer estranho por ser um "loop que não se repete" mas é a única forma de garantir que a macro funcione em um `if` de uma linha : 

```c
#include <stdio.h>
#include <stdbool.h>

#define print_forced1(X) {printf(X); fflush(stdout);}
#define print_forced2(X) \
    do { \
        printf(X); fflush(stdout); \
    } while(0)

//Neste exemplo, ocorre um "erro" de compilação
//Pois "print_forced1" expande para uma chave com 
//ponto e virgula no final, causando erros
bool forcePrint = true;
if(forcePrint)
    print_forced1("Teste");
else
    printf("Teste");

//Enquanto isso, utilizar "print_forced2" funciona
//Pois o loop "do while(0)" mantem as chaves dentro dele 
// se tornando um único statement
if(forcePrint)
    print_forced2("Teste");
else
    printf("Teste");
```

Existem também dois erros que são muito "comuns" quando utilizamos macros:
- A falta de parenteses, que pode causar uma grande dor de cabeça devido as diferentes ordens de precedências dos operadores
- A possibilidade de repetirmos parâmetros quando há uma chamada de função.

Podemos exemplificar os dois casos ao implementar uma função-macro para obter o valor máximo : 
```c
#include <math.h>
#define MATH_MAX_1(X,Y) (X > Y ? X : Y)
#define MATH_MAX_2(X,Y) ((X) : (Y) ? (X) : (Y))

//Resulta em 3, pois teremos
//1 ^ (10 > 3) -> 0, pois XOR inverte o bit
// 0 ? 11 : 3  -> 3, pois temos 0 no ternário
const int test = MATH_MAX_1(1 ^ 10, 3);

//Resulta em 11, pois teremos
//1^10 = 11
// (11) > (3) ? (11) : (3)
const int test2 = MATH_MAX_2(1 ^ 10, 3);

//Neste caso, teremos TRÊS chamadas
//de função, pois expandirá para: 
// sqrtf(2) > pow(1.19,2) ? sqrtf(2) : pow(1.19,2)
float test3 = MATH_MAX_2(sqrtf(2), pow(1.19,2));
```

Fica a dica : 
- Sempre certifique-se de botar parenteses pois isso garante um funcionamento adequado das macros para qualquer entrada 
- Evite usar funções como parâmetros em macros, ou certifique-se de documentar quando o usuário pode utilizá-las ou deve evitá-las com a sua macro
- Se for fazer uma função macro, certifique-se que ela se comporta como uma chamada de função, utilizando `do while(0)` caso necessário (evitando imprevisibilidades)
- Sempre que pensar em fazer uma função-macro, pense bem se essa solução é viável e se não existe outra forma melhor

## Macros com argumentos variádicos
Desde o `C99` podemos utilizar argumentos variádicos em macros, de forma similar aos argumentos variádicos em funções.

Ao utilizar `...` como o último parâmetro de uma macro, podemos colocar quantos argumentos quisermos naquele ponto, os argumentos podem ser acessados na macro ao digitar `__VA_ARGS__`.

Exemplo : 
```c
#include <stdio.h>

#define PRINT_ERRO(FORMAT, ...) fprintf(stderr, FORMAT, __VA_ARGS__)

//Esta macro
PRINT_ERRO("Erro %d no arquivo %s\n", 30, "main.c");

//Será substituida por
fprintf(stderr, "Erro %d no arquivo %s\n", 30, "main.c");
``` 

Observamos também que no exemplo acima, como foi colocada uma virgula na macro, se `...` for vazio, teremos um erro de compilação : 
```c
//Erro de compilação! 
PRINT_ERRO("Erro!\n");

//Pois isso expande para
fprintf(stderr, "Erro!\n",);
```

Para isso, desde o `C23`, podemos utilizar `__VA_OPT__` para indicar algo que só será adicionado caso `__VA_ARGS__` não seja vazio :
```c
#include <stdio.h>
#define PRINT_ERRO(FORMAT, ...) fprintf(stderr, FORMAT __VA_OPT__(,) __VA_ARGS__)

PRINT_ERRO("Erro!\n");
``` 

## Manipulação de argumentos de macro
Existem duas operações que podem ser realizadas utilizando argumentos de macros : 
- Concatenação
- Conversão para string

Para concatenar um argumento de macro, utilizamos `##` dentro da macro : 
```c
#define JUNTA_NOME(X,Y) X##Y

//Neste caso geramos um nome com "Vector_Grow"
void JUNTA_NOME(Vector, _Grow)()
{
    //Código da função
}
```

As coisas se tornam um pouco mais complicadas caso quisermos concatenar o conteúdo de macros e não seus nomes, para isso é necessário usar uma macro para forçar a expansão das macros antes de realizar a operação:
```c
#define JUNTA_NOMES_INTERNO1(X,Y) X##Y
#define JUNTA_NOMES(X,Y) JUNTA_NOMES_INTERNO1(X,Y)

int main()
{
    //Causa um erro, pois "JUNTA_NOMES_INTERNO1"
    //juntará ambos, gerando "__DATE____FILE__"
    JUNTA_NOMES_INTERNO1(__DATE__,__FILE__);

    //Neste caso, as macros são repassadas a outra macro
    //De forma que "__DATE__" e "__FILE__" sejam expandidas
    //E se tornem realmente strings com a data e nome do arquivo
    JUNTA_NOMES(__DATE__,__FILE__);
}
```


Para converter simbolos para string, utilizamos `#` : 
```c
#define NOME_VARIAVEL(X) #X

int main()
{
    int test = 5;

    //Esta expressão se tornará
    const char *nome = NOME_VARIAVEL(test);

    //Isso : 
    const char *nome = "test";
}
```

## Remover definições de macros
Podemos utilizar a diretiva `#undef` para remover uma macro já criada.

O uso é muito simples, basta utilizar `#undef NOME_MACRO` e ela será removida.

Isso é útil por exemplo quando queremos limitar uma macro a uma única função, pois macros não tem escopo.

## Macros pre-definidas
Algumas macros são predefinidas pelo padrão do C e estão sempre presentes : 

- `__STDC__` : Tem o valor `1` caso a implementação respeite o padrão do C.
- `__STDC_VERSION__`: Introduzido no `C95`, indica a versão do C utilizada, onde : 
 - `199409L` = `C95`
 - `199901L` = `C99`
 - `201112L` = `C11`
 - `201710L` = `C17`
 - `202311L` = `C23`
- `__STDC_HOSTED__`: Introduzido no `C99`, indica `1` se a versão roda num sistema operacional e `0` caso não haja sistema operacional (chamado freestanding).
- `__FILE__`: Se torna uma string literal com o nome do arquivo atual.
- `__LINE__`: Se torna o número da linha atual do código fonte.
- `__DATE__`: Se torna a data que o programa foi gerado, no formato `Mmmm dd yyyy`, o nome do mês se comporta como se gerado por `asctime`.
- `__TIME__`: Se torna o horário em que o programa foi gerado, no formato `hh:mm:ss`, como se gerado por `asctime`.
- `__STDC_UTF_16__`: Obrigatório desde `C23`, indica `1` se `char16_t` utilizar o encoding `UTF-16`.
- `__STDC_UTF_32__`: Obrigatório desde `C23`, indica `1` se `char32_t` utilizar o encoding `UTF-32`.


Outras macros, podem opcionalmente serem predefinidas pela implementação: 
- `__STDC_ISO_10646__`: Se torna um inteiro no formato `yyyymmL` se `wchar_t` usar Unicode, a data indica a última revisão do Unicode suportada.
- `__STDC_IEC_559__`: Introduzido no `C99`, se torna `1` se `IEC 60559` for suportado (depreciado no `C23`)
- `__STDC_IEC_559_COMPLEX__`: Introduzido no `C99`, se torna `1` se números complexos forem suportados.
- `__STDC_UTF_16___`: Introduzido no `C11`, indica `1` se `char16_t` utilizar o encoding `UTF-16`.
- `__STDC_UTF_32___`: Introduzido no `C11`, indica `1` se `char32_t` utilizar o encoding `UTF-32`.
- `__STDC_MB_MIGHT_NEQ_WC__`: Introduzido no `C99`, indica `1` se comparações de caractere como `'x' == L'x'` podem resultar em falso.
- `__STDC_ANALYZABLE__`:Introduzido no `C11`, indica `1` quando o compilador é limitado a não modificar o comportamento do código em certos casos de comportamento indefinido.
- `__STDC_LIB_EXT1__`: Introduzido no `C11`, se torna `201112L` se as funções "seguras" do Annex K estão disponíveis.
- `__STDC_NO_ATOMICS__`: Introduzido no `C11`, se torna `1` caso tipos atômicos não sejam suportados.
- `__STDC_NO_COMPLEX__`: Introduzido no `C11`, se torna `1` caso números complexos não sejam suportados.
- `__STDC_NO_THREADS__`: Introduzido no `C11`, se torna `1` se as funções padrões de threads não seja suportadas.
- `__STDC_NO_VLA__`: Introduzido no `C11`, se torna `1` se arrays de tamanho variável não forem suportados.
- `__STDC_IEC_60559_BFP__`: Introduzido no `C23`, se torna `202311L` se tipos adicionais de ponto flutuante forem suportados (`_FloatN`, `_FloatN_t`).
- `__STDC_IEC_60559_DFP__`: Introduzido no `C23`, se torna `202311L` se pontos flutuantes decimais forem suportados (`_Decimal32`, `_Decimal64`, `_Decimal128`).
- `__STDC_IEC_60559_COMPLEX__`: Introduzido no `C23`, se torna `202311L` se números complexos forem suportados (`_Complex` e `_Imaginary`).
- `__STDC_IEC_60559_TYPES__`: Introduzido no `C23`, se torna `202311L` se a implementação implementa qualquer um dos tipos do `IEC_60559`.

## Diretiva #line
A diretiva `#line` pode ser utilizada para modificar os valores de `__FILE__` e `__LINE__`, a sintaxe é :
- `#line num_linha` : Modifica o número da linha utilizado em `__LINE__`.
- `#line num_linha "nome arquivo"`: Modifica o nome do arquivo utilizado em `__FILE__` e o número da linha em `__LINE__`.