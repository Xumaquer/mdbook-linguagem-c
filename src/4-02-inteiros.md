# Inteiros
Números inteiros são um conjunto de tipos de variaveis primitivas, indicam números sem casas decimais que podem ter valores negativos e positivos, ou limitados para apenas números positivos com o modificador `unsigned`.

A única diferença entre os diferentes tipos de inteiros são as regras para definição do seu tamanho e limites e algumas particularidades exclusivas dos tipos `char`, `signed char` e `unsigned char` que serão explicados em detalhes na seção sobre ponteiros.

Existem vários tipos de números inteiros, sendo eles (em ordem crescente de tamanho) : 
- `char` 
- `short` 
- `int` 
- `long` 
- `long long (adicionado no C99)` 

## Regras para tamanho de inteiros

No geral o padrão do C não dá muitas garantias quanto aos tamanhos em bytes de inteiros, a única garantia real é que `char` é 1 byte e que os tipos "maiores" precisam atender a alguns requisitos mínimos.

Na prática a maioria dos sistemas modernos atende aos padrões conhecidos como "modelos de dados" que são os conjuntos de tamanhos de cada variável : 
- `LP32`: Utilizado pelo windows 16bits (não é mais tão moderno assim...)
- `ILP32`: Utilizado pelo windows 32bits e sistemas UNIX 32bits (Linux,MacOs e afins)
- `LLP64`: Utilizado pelo windows 64bits
- `LP64`: Utilizado por sistemas UNIX 64bits (Linux, MacOs e afins)

Com isso é possível montar a tabela abaixo (relacionando a quantidade de bits de cada tipo) : 

| Tipo        | Padrão C       | LP32 | ILP32 | LLP64 | LP64 |  
| ----------- | -------------- | :--: | :---: | :---: | :--: |
| `char`      | Pelo menos 8   |  8   |   8   |   8   |   8  |
| `short`     | Pelo menos 16  |  16  |   16  |  16   |  16  |
| `int`       | Pelo menos 16  |  16  |   16  |  32   |  32  |
| `long`      | Pelo menos 32  |  32  |   32  |  32   |  64  |
| `long long` | Pelo menos 64  |  64  |   64  |  64   |  64  |

Se atendo um pouco mais aos detalhes, o padrão do C não obriga 1 byte a ser 8bits, na verdade um byte é o menor valor endereçável da arquitetura, portanto apesar de todas as arquiteturas modernas usarem 8bits por byte, o padrão do C aceita arquiteturas que não seguem isso e expõe a definição `CHAR_BIT` que indica o número de bits em um byte.

Isso permitiria por exemplo bizarrices como arquiteturas que tem um byte com 64bits, o que possibilitaria que todos os tipos tivessem apenas 1 byte, logo, quanto a bytes a regra a ser seguida pelo C é a seguinte : 

`1` == `sizeof(char)` <= `sizeof(short)` <= `sizeof(int)` <= `sizeof(long)` <= `sizeof(long long)`

Mas mesmo assim, a maioria das pessoas não está preocupada com arquiteturas obscuras que provavelmente nunca vão ver na vida, na prática as arquiteturas modernas e mesmo sistemas embarcados de hoje em dia em sua grande maioria usam 8bits por byte e a representação de complemento de dois para inteiros com sinal.

## Limites de inteiros
Constantes relacionadas a limites de inteiros podem ser encontradas na biblioteca `limits.h`.

Ao assumir complemento de dois para os inteiros com sinal, podemos definir os seguintes limites para os dados : 

| Tamanho em bits | Tem sinal | Limite inferior      | Limite superior      |
| :-------------: | :-------: | :------------------: | :------------------: |
|        8        |    Sim    | -128                 | 127                  |
|       16        |    Sim    | -32768               | 32768                |
|       32        |    Sim    | -2147483648          | 2147483647           |
|       64        |    Sim    | -9223372036854775808 | 9223372036854775807  |
|        8        |    Não    | 0                    | 255                  |
|       16        |    Não    | 0                    | 65535                |
|       32        |    Não    | 0                    | 4294967295           |
|       64        |    Não    | 0                    | 18446744073709551616 |

Mas e se eu te disser que você não precisa perder seu tempo decorando a tabela acima? 

Todos esses valores podem ser facilmente calculados.

Para inteiros sem sinal o limite é : 

\\[-\frac{2^N}{2} \text{ até } \frac{2^N}{2}-1\\]

Enquanto que para inteiros sem sinal, o limite é 

\\[0 \text{ até } 2^N-1\\]

Onde `N` é o número de bits, logo fica fácil "descobrir" os limites de um tipo inteiro tendo uma calculadora a disposição.

## Tipos de tamanho específico 
Para facilitar o manuseio de inteiros, existem algumas definições específicas de tipos que estão presentes na biblioteca padrão pela `inttypes.h` que foi adicionada no `C99`.

Lá existem definições para inteiros de 8,16,32 e 64bits e suas respectivas versões com ou sem sinal. Utilizar estes tipos garante um tamanho fixo conhecido e facilita o manuseio dos mesmos além de compatibilidade maior com outros compiladores e processadores.

Eu poderia listar todos eles, mas é mais fácil entender a regra dos nomes, considere que pedaços em colchetes `[]`, são opcionais e `/` são alternativas ao mesmo valor. 

`[u]int[_fast/_least]X_t`

- `X` é o número de bits, podendo ser 8, 16, 32 ou 64
- `u` é o modificador para uma versão sem sinal do tipo
- `_fast` é o modificador para obter o tipo "mais eficiente para manuseio" que tenha pelo menos o tamanho especificado
- `_least` é o modificador para obter o menor tipo que tenha "pelo menos aquele tamanho"
- Na ausência de `fast` e `least`, o tipo tem EXATAMENTE a quantidade de bits em `X`. 

Exemplos : 
- `uint8_t` : Tipo sem sinal com exatamente 8bits
- `int_fast32_t`: Tipo com sinal com o tipo mais eficiente que tenha pelo menos 32bits
- `uint_least64_t`: Inteiro sem sinal com pelo menos 64bits

Além destes tipos, existe o `intmax_t` que contêm o maior tipo inteiro com sinal  e `uintmax_t` que contêm o maior tipo inteiro sem sinal.

## Tipos inteiros adicionais
Além destes, existem alguns tipos inteiros adicionais presentes nas bibliotecas `inttypes.h` e `stddef.h` (incluida junto com `stdlib.h`).

Estes tipos são : 

| Tipo          | Descrição                                                            |
| ------------- | -------------------------------------------------------------------- |
| `ptrdiff_t`   | Tipo resultante ao subtrair dois ponteiros                           | 
| `size_t`      | Tipo que pode guardar o tamanho máximo teórico que um array pode ter |
| `max_align_t` | Tipo que com o maior requisito de alinhamento possível (`C11`)       |
| `intptr_t`    | Inteiro com sinal capaz de guardar qualquer ponteiro                 |
| `uintptr_t`   | Inteiro sem sinal capaz de guardar qualquer ponteiro                 |

O tipo `size_t` é o tipo resultante de toda expressão `sizeof`, e é o tipo ideal para guardar o tamanho máximo que objetos, arrays ou qualquer dado possa ter.

O tipo `ptrdiff_t` é utilizado para guardar diferenças entre ponteiros, e pode ser considerado como uma versão com sinal de `size_t`, visto que o tipo `ssize_t` normalmente definido e presente em sistemas POSIX não faz parte do padrão do C.

O propósito do POSIX ao utilizar `ssize_t` é a possibilidade de utilizar os valores negativos para indicar erros, reservando um bit do valor para erros.

`max_align_t` no geral é utilizado junto com o operador `alignof`.

## Tipo _BitInt
Adicionado apenas no `C23`, o tipo é declarado como `_BitInt(N)` onde `N` é o número de bits que o tipo deve ter com cada valor de `N` sendo considerado um tipo diferente.

Os tipos `_BitInt` ainda podem ter o modificador `unsigned` e para valores com sinal, o número de `N` inclui o bit de sinal, de forma que `_BitInt(1)` não seja um tipo válido (pois não sobra nenhum bit pro valor).

A mesma regra descrita nos [limites de inteiros](#limites-de-inteiros) se aplicam para calcular os limites de um valor do tipo `_BitInt`.

## Inteiros definidos pela implementação
Desde o `C99`, existe a possibilidade dos compiladores terem tipos inteiros adicionais adicionados a linguagem, tipos como `__uint128` e `__int128` que simbolizam inteiros de 128 bits, porém o suporte e existência desses tipos depende da arquitetura e do compilador utilizado.

## Overflow e underflow
Ao tratarmos de números inteiros, é comum o uso dos seguintes termos : 
- `overflow` : Em tradução literal, seria um "transbordamento", esse termo é utilizado para indicar quando chegamos em um valor que vai além do limite que uma variável suporta.
- `underflow`: Em tradução literal, seria um "subtransbordamento", esse termo é utilizado para indicar quando chegamos um valor abaixo do limite mínimo que uma variável suporta.

O comportamento padrão para calcularmos o valor resultante quando ocorre `overflow` ou `underflow` é : 
- `overflow`:  `VALOR_MAXIMO + X` se tornará `VALOR_MINIMO + (X - 1)` para `X > 0`
- `underflow`: `VALOR_MINIMO - X` se tornará `VALOR_MAXIMO - (X - 1)` para `X > 0`

Para inteiros com sinal, o comportamento de `overflow` e `underflow` é indefinido, portanto qualquer checagem como `i + 1 < i` é transformada em `false` durante as otimizações, porém na prática quando um `overflow` ou `underflow` acontece e usamos complemento de dois (o que é o padrão), temos o comportamento descrito acima.

O teste abaixo demonstra um exemplo ao escrever um `overflow` e `underflow` de um inteiro com sinal de 8bits (lembre-se que pelas regras da linguagens, estamos invocando um comportamento indefinido):
```c
#include <stdio.h>
#include <limits.h>

int main()
{
    signed char test1 = SCHAR_MAX;
    signed char test2 = SCHAR_MIN;

    //"%hhd" é o modificador para escrever um signed char no printf
    printf("%hhd %hhd\n", test1 + (signed char)1, test2 - (signed char)1);
}
```

Já para inteiros sem sinal, o `overflow` e `underflow` são definidos, e seguem o mesmo comportamento descrito, o que pode ser preocupante pois `0 - 1` gera o valor máximo, logo o perigo de `underflow` é muito maior. É por esse motivo que alguns autores aconselham evitar o uso de inteiros sem sinal, justamente pela facilidade de gerar um `underflow` ao realizar subtrações, no qual devemos ter muito cuidado sempre que realizamos subtrações.

Exemplo de `overflow` e `underflow` com inteiros sem sinal (neste caso, este comportamento é definido pela linguagem, garantindo que funcione em qualquer lugar):

```c
#include <stdio.h>
#include <limits.h>

int main()
{
    unsigned int test1 = UINT_MAX;
    unsigned int test2 = 0;

    //"%u" é o modificador para escrever um unsigned int no printf
    printf("%u %u\n", test1 + 1U, test2 - 1U);
}
``` 

## Dicas para uso consciente de inteiros
No geral, aconselho utilizar `signed char` e `unsigned char` para representar bytes, `int`/`unsigned int` para inteiros genéricos onde o tamanho não é problema (todas as plataformas modernas tem geralmente um `int` de pelo menos 32bits, a menos que sejam processadores embarcados de 16/8 bits ou arquiteturas super específicas).

Em casos onde operações com bits ou tamanhos fixos são necessários, os tipos `uintx_t` e `intx_t` são utilizados para especificar um tamanho fixo.

O tipo `size_t` é importante também pois é normalmente utilizado para indicar tamanhos ou um inteiro do maior tamanho suportado nativamente pela plataforma.

Já o tipo `ptrdiff_t` é mais raro, mas pode ser usado de maneira similar ao `ssize_t` definido pelo POSIX (um tamanho de arrays/dados que também pode representar erros, utilizado geralmente em retorno de funções) ou realmente para diferenças entre ponteiros.






