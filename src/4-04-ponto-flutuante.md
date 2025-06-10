# Ponto Flutuante
Variáveis de ponto flutuante são tipos utilizados para representar números reais, que podem ter casas depois da virgula, ou mesmo para representar números extremamente grandes.

Os tipos padrões de ponto flutuantes existentes são:

- `float`: Ponto flutuante de precisão única.
- `double`: Ponto flutuante de precisão dupla.
- `long double`: Ponto flutuante de precisão extendida.

Geralmente os números de ponto flutuante obedecem os formatos definidos pelo padrão IEEE-754, apesar de isso não ser garantido pela especificação do C.

Logo o tipo `float` geralmente tem 32bits e obedece o formato [IEEE 754 binary32](https://en.wikipedia.org/wiki/Single-precision_floating-point_format).

O tipo `double` geralmente tem 64bits e obedece o formato [IEEE 754 binary64](https://en.wikipedia.org/wiki/Double-precision_floating-point_format).

Já o tipo `long double` geralmente utiliza um dos seguintes formatos : 
- [IEEE 754 binary128](https://en.wikipedia.org/wiki/Quadruple-precision_floating-point_format)
- [IEEE 754 binary64 extendido](https://en.wikipedia.org/wiki/Extended_precision)
- [IEEE 754 binary64](https://en.wikipedia.org/wiki/Double-precision_floating-point_format)

No geral as implementações na arquitetura x86 e x86-64 normalmente utilizada em PCs, tendem a utilizar o formato `binary64 extendido` que tem 80bits suportado nativamente pelo processador, porém alguns compiladores já utilizam o formato `binary128`.

O compilador da microsoft MSVC, é uma exceção a regra, no qual ele implementa mesmo em x86 e x86-64, `long double` como `binary64` tendo a mesma representação de `double`.

Além disso, como é a microsoft quem escreve os runtimes padrões do C para windows, geralmente funções como `printf` vão levar em consideração que `long double` é 64bits (assim como no MSVC), de forma que nenhum tipo `long double` seja corretamente escrito quando usado em outros compiladores no windows, exigindo o uso de definições como `__USE_MINGW_ANSI_STDIO` que forçam o compilador a utilizar suas próprias implementações dessas funções.

Já processadores ARM (utilizados normalmente em celulares e embarcados), geralmente usam o formato `binary64` em suas versões 32bits e o formato `binary128` nas suas versões 64bits.


## Limites dos ponto flutuantes

A seguir, veremos os limites de cada formato de ponto flutuante.

O campo "Maior inteiro" é utilizado para indicar o maior número inteiro que pode ser representado sem utilizar aproximações.

Lembrando que todos os formatos podem representar os mesmos valores com sinal positivos ou negativo, de forma que os limites "inferiores" e "superiores" descritos se refiram ao menor valor positivo e maior valor positivo respectivamente.

O `binary64 extendido` e o `binary128` tem a mesma quantidade de bits reservadas para o expoente, de forma que o range do `binary64 extendido` seja o mesmo, mas a precisão seja menor.

- `binary32` 
    - Limite inferior : \\(1.40129846432481707092372958328991613 * 10{-45}\\)
    - Limite superior : \\(3.40282346638528859811704183484516925 * 10{38}\\) 
    - Maior inteiro : \\(2^{24}-1\\)
- `binary64`
    - Limite inferior : \\(4.94065645841246544176568792868221372 * 10{-324}\\) 
    - Limite superior : \\(1.79769313486231570814527423731704357 * 10{308}\\)
    - Maior inteiro : \\(2^{53} -1\\)
- `binary64 extendido`
    - Limite inferior : \\(3.64519953188247460252840593361941982 * 10^{-4951}\\)
    - Limite superior : \\(1.18973149535723176502126385303097021 * 10^{4932}\\)
    - Maior inteiro : \\(2^{64} - 1\\)
- `binary128`
    - Limite inferior : \\(3.64519953188247460252840593361941982 * 10^{-4951}\\)
    - Limite superior : \\(1.18973149535723176502126385303097021 * 10^{4932}\\)
    - Maior inteiro :  \\(2^{113} - 1\\)

## Entendendo os limites
Mas agora a pergunta que não quer calar é... como os ponto flutuantes conseguem ter uma faixa tão grande de valores, mesmo tendo uma quantidade de bits similar a inteiros que representam faixas bem menores?

Assumindo o padrão dos formatos do IEEE 754, a verdade é que eles guardam bits separados para sinal, expoente e número, de forma que alguns valores não possam exatamente serem representados com precisão.

Ou seja, para vários números os tipos de ponto flutuante oferecem uma aproximação, inclusive o campo de "Maior inteiro" da seção anterior buscava mostrar o maior valor representável sem aproximações.

O resultado disso na prática é que existem clássicos exemplos de como essas aproximações podem dar errado ou nos levar a erros. 

Por exemplo, o código a seguir : 
```c
#include <stdio.h>
#include <stdbool.h>

int main() 
{
    bool resultado = (0.1 + 0.2) == 0.3;

    if(resultado)
        puts("O resultado é verdadeiro");
    else
        puts("O resultado é falso");
}
```

Talvez [você fique surpreso](https://www.youtube.com/watch?v=O6FsyQZzJJ0) em descobrir que o resultado deste programa é : 
```
O resultado é falso
```

A forma recomendada para indicar que dois números de ponto flutuante são iguais é :
```c
#include <stdbool.h>
#include <float.h>

bool mesmoNumero(double a, double b)
{
   if(a == b)
      return true;

    return (fabs(a-b) <= DBL_EPSILON);
}

int main() 
{
    bool resultado = mesmoNumero(0.1 + 0.2, 0.3);

    if(resultado)
        puts("O resultado é verdadeiro");
    else
        puts("O resultado é falso");
}
```

Neste caso o resultado é :
``` 
O resultado é verdadeiro
```

A constante definida como `DBL_EPSILON` ou `FLT_EPSILON` indica a menor diferença entre 1 e o valor após 1 que é representável, porém essa diferença não é exatamente a diferença mínima entre dois números quaisquer pois ela pode variar um pouco de acordo com o expoente atual.

Geralmente implementações mais avançadas da comparação costumam passar um `EPSILON` diferente dependendo da faixa de valores que será usada em cada caso.


## Valores especiais para ponto flutuante
Os tipos de ponto flutuante tem alguns valores especiais, que podem indicar estados de erro e de operações que normalmente seriam tratadas como exceções.

- Infinito : A macro `INFINITY` (que pode ter um valor positivo ou negativo e é definida em `math.h`), geralmente é o resultado ao realizar divisões por zero.
- 0 negativo : O valor `-0.0` é geralmente suportado, a expressão `-0.0 == 0` é `Verdadeira`, mas `5 / -0.0` gera o valor equivalente a `-INFINITY` por exemplo.
- Não é um número: Geralmente conhecido como `NaN`, pode ter várias representações, mas basicamente indica que o número é indefinido/inválido e todas as operações aritméticas envolvendo ele devem resultar em `NaN` também. Além disso o `NaN` também obrigatóriamente deve gerar `Falso` quando comparado com ele mesmo, sendo uma forma possível de identificar se o valor é `NaN`.

## Erros de ponto flutuante
Operações de ponto flutuante e funções envolvendo ponto flutuante geralmente reportam erros envolvendo operações (overflow, underflow, divisão por zero, valor inválido, resulto inexato, erro de faixa, etc).

A biblioteca `math.h` oferece a definição `math_errhandling` (que por algum motivo, não é suportada no windows), que indica as formas como erros são reportados em um bitmask : 
- `MATH_ERRNO`: Tem o valor `1`, se este bit em `math_errhandling` estiver definido, significa que erros de ponto flutuante e funções relacionadas a elas são reportadas através do `errno`, que é uma variável local do thread que indica o código do último erro ocorrido.
- `MATH_ERREXCEPT`: Tem o valor `2`, se este bit em `math_errhandling` estiver definido, significa que erros de ponto flutuante e funções relacionadas a elas são reportadas ao chamar `fetestexcept` e podem ser limpas ao chamar `feclearexcept`. 

Lembrando que ambos modos podem ser utilizados para reportar erros, sendo necessário checar.

No geral compiladores no windows não fornecem implementação dessas macros, mas usa o modo definido por `MATH_ERREXCEPT`.

Uma sugestão para compatibilidade no windows sugere utilizar o seguinte código : 
```c
#include <math.h>

#if !defined(MATH_ERRNO)
    #  define MATH_ERRNO 1
#endif
#if !defined(MATH_ERREXCEPT)
    #  define MATH_ERREXCEPT 2
#endif
#if !defined(math_errhandling)
    #  define math_errhandling MATH_ERREXCEPT
#endif
```

Código exemplo testando exceções : 
```c
#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

void testarErrno()
{  
   if(errno == EDOM)
      puts("Valor inválido");
   else if(errno == ERANGE)
      puts("Valor fora da faixa");
   
   errno = 0; //Limpa o errno para o próximo uso
}


void testarExcecoes()
{
   if(fetestexcept(FE_INVALID))   puts("Operação inválida");
   if(fetestexcept(FE_DIVBYZERO)) puts("Divisão por zero");
   if(fetestexcept(FE_OVERFLOW))  puts("Overflow");
   if(fetestexcept(FE_UNDERFLOW)) puts("Underflow");
   if(fetestexcept(FE_INEXACT))   puts("Resultado inexato");

   //Limpa as exceções, evitando reportar duas vezes
   feclearexcept(FE_ALL_EXCEPT);
}

void checaErrosPontoFlutuante(const char *texto)
{
   puts(texto);

   if(math_errhandling & MATH_ERREXCEPT)
      testarExcecoes();
   else if(math_errhandling & MATH_ERRNO)
      testarErrno();
}

int main()
{
   double test;
   errno = 0; //Garante que errno está limpo

   test = 0.0 / 0.0;
   checaErrosPontoFlutuante("1 - 0/0");

   test = 5.0 / -0.0;
   checaErrosPontoFlutuante("2 - 5/-0");

   test = 2 * DBL_MAX;
   checaErrosPontoFlutuante("3 - 2 * MAXIMO");

   test = DBL_TRUE_MIN / 2;
   checaErrosPontoFlutuante("4 - MINIMO / 2");

   test = sqrt(2);
   checaErrosPontoFlutuante("5 - Raiz de 2");
}
``` 

Um teste no windows resultou no seguinte (foi necessário adicionar o código de sugerido para compatibilidade): 
```
1 - 0/0
Operação inválida
2 - 5/-0
Divisão por zero
3 - 2 * MAXIMO
Overflow
Resultado inexato
4 - MINIMO / 2
5 - Raiz de 2
```

## Tipos extras de ponto flutuante
Nos casos onde há uma FPU (Floating Point Unit), uma unidade dedicada para processamento de números de ponto flutuante dentro do processador, é possível que a precisão de certos tipos de ponto flutuante seja menor do que a precisão oferecida nativamente pelo hardware para realizar os cálculos.

Nesses casos, utilizar os tipos de maior precisão suportados em hardware para realizar os cálculos é mais eficiente, além de, claro, ser mais preciso.

Para isso, foram criadas novas definições de tipos de ponto flutuante na biblioteca `math.h`, aconselháveis para variáveis que guardem cálculos intermediários utilizados para produzir um valor final do tipo indicado : 

- `float_t`, como o tipo mais eficiente que tem pelo menos o tamanho de `float`
- `double_t`, como o tipo mais eficiente que tem pelo menos o tamanho de `double`

Alguns detalhes adicionais sobre a implementação são evidenciados pelos diferentes valores da macro `FLT_EVAL_METHOD` : 
- `0`: `float_t` e `double_t` são equivalentes a `float` e `double` respectivamente
- `1`: `float_t` e `double_t` são equivalentes a `double`
- `2`: `float_t` e `double_t` são equivalentes a `long double`
- `outro`: o formato de `float_t` e `double_t` são definidos pela implementação

## Números complexos e imaginários
Desde o `C99`, foram adicionados números complexos a linguagem, que devem ser, idealmente, acessados adicionando a biblioteca `complex.h`, pois algumas das funcionalidades dependem de macros do `complex.h` que podem incluir extensões de compilador (de forma que incluir `complex.h` seja a forma mais portável). 

Os números complexos são acessados através do modificador `complex` que deve ser escrito após o tipo de ponto flutuante (ex: `double complex` ou `float complex`) e literais de números imaginários podem ser escritos utilizando a macro `I`.

Tipos de variáveis para números imaginários podem ser escritos utilizando o modificador `imaginary` da mesma forma que `complex`. 

A biblioteca `complex.h` também oferece algumas funções para operar com números imaginários e complexos e o `printf` apresenta formatadores para realizar a formatação de tais números.

Uma implementação também pode se recusar a fornecer suporte a números imaginários, neste caso indica-se:

- Antes do `C11`: A definição de `__STDC_IEC_559_COMPLEX__` é recomendada para indicar que a implementação suporta números imaginários, mas não obrigatória, o POSIX recomenda checar se `_Imaginary_I` está definido para identificar suporte a números complexos
- `C11`: Números imaginários são suportados se `__STDC_IEC_559_COMPLEX__` estiver definido
- `C23`: Números imaginários são suportados se `__STDC_IEC_60559_COMPLEX__` estiver definido

## Números decimais
Adicionados no `C23`, os números decimais são números de ponto flutuante com precisão decimal que seguem os formatos especificados pela IEEE 754.

Nele temos os tipos 
- `_Decimal32` : Segue o formato `decimal32` da IEEE 754
- `_Decimal64` : Segue o formato `decimal64` da IEEE 754
- `_Decimal128`: Segue o formato `decimal128` da IEEE 754

Os valores obedecem aos seguintes limites : 
- `decimal32`
    - Limite mínimo : \\(±1.000000 * 10^{−95}\\)
    - Limite máximo : \\(±9.999999 * 10^{96}\\)
    - Precisão: até 7 digitos.
- `decimal64`
    - Limite mínimo : \\(±1.000000000000000 * 10^{−383}\\)
    - Limite máximo : \\(±9.999999999999999 * 10^{384}\\)
    - Precisão: até 16 digitos.
- `decimal128` 
    - Limite mínimo : \\(±0.000000000000000000000000000000000 * 10^{−6143}\\)
    - Limite máximo : \\(±9.999999999999999999999999999999999 * 10^{6144}\\)
    - Precisão: até 34 digitos.

O suporte a tipos decimais pode ser checado verificando a definição de `__STDC_IEC_60559_DFP__`.



