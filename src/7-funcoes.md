# Funções
Funções são parcelas de código "reutilizável", escritas para que possamos dar um nome a um pedaço de código e reutilzá-lo em diversas situações, possívelmente com parâmetros diferentes.

As principais vantagens ao utilizar funções são : 
- Reutilização de código
- Organização do código por meio de nomes bem definidos
- Possibilidade de parametrizar um código 
- Diminuição de dependências externas

Funções são declaradas utilizando a sintaxe `tipoRetorno NomeFunção(parametros)` : 
- `tipoRetorno` indica o tipo do valor resultante de uma função, que pode ser atribuido a uma variável ou usado em outras expressões, podemos escrever `void` para indicar que a função não resulta em um valor.
- `NomeFunção` é o nome dado a função, utilizado ao chamar
- `parametros` são os parâmetros que devem ser repassados ao chamar a função, é uma lista com a definição de zero ou mais variaveis, separadas por virgula.

Para retornar um valor em uma função é utilizado o operador `return`, ele finaliza a execução da função, similar a forma como o operador `break` finaliza um laço de repetição.

Porém ao finalizar uma função que resulta em um valor, é necessário informar o valor ao escrever `return` e o uso da palavra chave `return` em funções assim é obrigatório (todos os caminhos devem ter um retorno).

Exemplos de funções : 
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//realiza a soma de dois números, retornando a soma
int soma(int v1, int v2)
{
    return v1 + v2; 
}

/* Em muitos casos, quando queremos retornar mais de um valor,
   utilizamos um "parâmetro como ponteiro" para retornar valores 
   adicionais, neste caso temos uma função bhaskara que retorna se 
   a operação deu certo, e preenche x1 e x2 que são passados via ponteiro */
bool bhaskara(double a, double b, double c, 
              double *restrict x1, double *restrict x2)
{
    const double delta = b*b - 4*a*c;
    
    //Raiz de número negativo é um número imaginário!
    if(delta < 0)
        return false; 

    const double raizDelta = sqrt(delta);

    if(x1 != NULL)
        *x1 = (-b + raizDelta) / (2 * a);
    if(x2 != NULL)
        *x2 = (-b - raizDelta) / (2 * a);

    return true;
}

//Função main testando ambas
int main()
{
    int resultado1 = soma(2,3); //5
    printf("A soma resultou em %d\n", resultado1);

    double r1;
    double r2;

    if(bhaskara(2, -3, -5, &r1, &r2)) 
        printf("Bhaskara resultou em %.2f e %.2f\n", r1, r2);
    else
        printf("Bhaskara deu negativo!\n");
}
```

Também é importante mencionar que desde o `C99`, é possível acessar o nome da função atual utilizando a macro `__func__`, que é tratado como uma variável constante e de duração estática (que claramente só será incluida na memória do programa se for utilizada).

## Ponto de entrada
Em todas as aplicações feitas em C, exceto as que não usam o ambiente de execução padrão do C, precisam escrever uma função especial denominada `main`.

O `main` é o chamado "ponto de entrada" do programa, é onde o seu programa em C começa a executar.

Existem duas formas "padronizadas" de escrever o ponto de entrada.

A primeira, sem parâmetros : 

```c
int main() { /* conteúdo */ }
```  

A segunda, que recebe :
- `argc`, indica a quantidade de argumentos da linha de comando
- `argv`, uma lista com os argumentos da linha de comando

```c
int main(int argc, char *argv[]) {/* conteúdo */}
```

Os argumentos da linha de comando são os valores repassados a um programa quando ele inicia.

Quando iniciamos um programa pelo terminal, a linha de texto do comando utilizado para chamar o programa é enviada ao programa e cada argumento separado por espaço vira um elemento diferente de `argv` e a quantia de argumentos é repassada através de `argc`.

Historicamente o primeiro argumento da linha de comando de um programa é o caminho do arquivo utilizado para chamar o programa.

Porém, apesar dessa regra ser seguida no geral pelos terminais de qualquer sistema operacional, vale lembrar que isso não é totalmente garantido.

Qualquer programa pode diretamente chamar as funções de baixo nível para iniciar um processo diretamente como a [CreateProcessW](https://learn.microsoft.com/pt-br/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw) no windows, [execve](https://man7.org/linux/man-pages/man2/execve.2.html) no linux ou [posix_spawn](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/posix_spawn.2.html) presente no linux e implementada como chamada de sistema no macOs.

Ao chamar essas funções diretamente, é possível ignorar essa convenção histórica, o que apesar de incomum, é uma possibilidade.

Por exemplo ao executar um programa escrevendo o seguinte :
```
programa teste.txt -t 50
```

Teremos `argc = 4` e os valores de `argv` serão:
```
argv[0] = "programa"
argv[1] = "teste.txt"
argv[2] = "-t"
argv[3] = "50" 
```  

## Retorno da função main

A função `main` retorna um valor do tipo `int` que indica um status de finalização que é retornado ao sistema operacional. 

Retornar da primeira chamada da função `main` é equivalente a chamar a função `exit`, onde o retorno do `main` será o código de status repassado ao sistema.

Para indicar uma execução bem sucedida, usa-se o valor `0` ou a macro `EXIT_SUCCESS` definida na `stdlib.h`.

Para indicar um erro ou falha na execução, usa-se um valor diferente de `0`, geralmente positivo ou a macro `EXIT_FAILURE` definida também na `stdlib.h`.

Esses códigos de status podem ser acessados pelo terminal diretamente após executar um programa utilizando a variável `%errorlevel%` no Windows ou `$?` no bash em linux ou macOs.

Podemos fazer um programa que justamente testa isso : 
```c
#include <stdlib.h>

//Converte o texto do primeiro argumento em inteiro e retorna ele no main
//De forma que o código de status do sistema seja igual ao número passado de argumento
int main(int argc, char **argv)
{
    return (argc > 1) ? (int) strtol(argv[1]) : EXIT_SUCCESS;
}
```

Para testar no windows :
```batch
testaerro.exe 5
echo %errorlevel%
```

Para testar no linux ou macOs : 
```bash
./testaerro 5
echo $?
```

Também é importante lembrar que desde o `C99`, não é necessário escrever `return` na função `main`, pois a ausência de `return` indica que o valor retornado será `0` (só se aplica ao `main`).

## Declaração de funções
Se uma função é definida no C, ela geralmente deve ser definida antes da função que a utiliza, sem isso, compilar o programa resulta em um erro de compilação.

Porém, existe uma forma de burlar isso, declarando a função sem definir ou implementar ela, esse tipo de declaração também é chamado como uma declaração de um "protótipo de função".

Exemplo : 
```c
//Declaração, a forma como a função é
int soma(int v1, int v2);

int main()
{
    printf("1 + 2 = %d\n", soma(1,2));
}

//Definição, escreve a implementação da função
int soma(int v1, int v2)
{
    return v1 + v2;
}
```

Quando declaramos uma função, estamos especificando como a função deve ser chamada e quais parâmetros ela deve receber, logo toda informação relevante para chamar a função está presente.

Mas a explicação real do que realmente acontece quando declaramos uma função é um pouco mais complexa.

Ao declarar uma função, estamos basicamente dizendo ao compilador "confia em mim, essa função claramente existe", o compilador, por sua vez, decide acreditar em você até os últimos momentos.

Mesmo se a função não existir, a etapa de compilação que compila o código de uma unidade de tradução ainda vai funcionar, e seu código é compilado indicando que ele tem uma dependência em uma função com aquele nome.

O problema real ocorre quando chegamos a última etapa, a junção de todos os códigos compilados de todas as unidades de tradução, é neste momento que o compilador checa se a função que você queria existe em algum lugar, se não existir, teremos o erro `undefined reference` (referência indefinida), que indica que algo que deveria estar lá, na verdade não está.

Isso significa na prática que uma declaração de função também é uma forma de importar funções externas, declarando que ela existe no seu código.

Por exemplo o seguinte código funciona apenas se compilarmos ambos arquivos juntos : 
```c
//Arquivo1.c 
#include <stdio.h>
void dizerOi()
{
    puts("Ola mundo!");
}
//Fim do arquivo1.c

//Arquivo2.c
void dizerOi();

int main()
{
    dizerOi();
}
//Fim do arquivo2.c
``` 

No exemplo acima, o código no `arquivo2.c` importa a função presente no `arquivo1.c` e a chama, se compilarmos apenas `arquivo1.c` teremos um código que carece da função `main` e se compilarmos apenas `arquivo2.c` teremos um erro de `undefined reference`.

De forma que esse código só funcione se ambos forem compilados juntos.

## Funções variádicas
Funções variádicas são funções que tem a capacidade de receber um número variável de argumentos.

Para escrever funções variádicas, devemos colocar `...` como o último argumento de uma função, antes do `C23` era necessário ter ao menos um argumento além do `...`, porém no `C23` essa obrigatoriedade foi removida.

Exemplo de função variadica : 
```c
    int printf(const char *restrict format, ...);
```

Para utilizar os argumentos de uma função variádica, é necessário utilizar as macros definidas na biblioteca `stdarg.h` junto do tipo `va_list` que indica a lista de argumento variádicos.

Descrições das macros, bem como dos seus argumentos (que estão descritos entre parenteses):

- `va_start(LISTA,INICIO)`: a macro `va_start` inicializa a variável `LISTA` do tipo `va_list`
que aparece logo após o argumento `INICIO`, sendo necessário informar o argumento `INICIO` sempre que houver outro argumento antes dos argumentos variádicos (o que é obrigatório antes do `C23`).

- `va_arg(LISTA,TIPO)`: a macro `va_arg` retorna o próximo valor do tipo informado em `TIPO` da variável `LISTA` do tipo `va_list` que foi inicializada com `va_start`, a ideia é que a cada chamada de `va_arg` um argumento é extraido e a `va_list` "avança de posição".

- `va_copy(DESTINO,FONTE)`: Adicionado no `C99`, copia a variável `FONTE` do tipo `va_list` para a variável `DESTINO` também do tipo `va_list`, sendo necessário chamar `va_end` para cada uma das listas.

- `va_end(LISTA)`: Finaliza a variável `LISTA` que foi inicializada com `va_start`,  a ideia é que normalmente essa funcionalidade é implementada usando a stack e usar `va_end` limpa a stack utilizada por `LISTA`.


Abaixo um exemplo de uma função de argumentos variádicos que calcula uma média aritmética.
```c
#include <stdarg.h>
#include <stdio.h>

double calculaMedia(int quantidade, ...)
{
    double soma = 0;
    va_list argumentos;
    va_start(argumentos); 

    for(int i = 0; i < quantidade; i++) 
        soma += va_arg(argumentos, double);

    va_end(argumentos);
    
    //Evita divisão por zero ou com números negativos...
    if(quantidade <= 0)
        return 0; 
    
    return (soma / quantidade);
}

int main()
{
    const int quant = 3;
    const double prova1 = 5.7;
    const double prova2 = 8.4;
    const double prova3 = 9.2;

    double media = calculaMedia(quant, prova1, prova2, prova3);

    printf("A media das notas é %.2f\n", media);
}
```

Apesar do `C23` ter removido a obrigatoriedade de outros argumentos, é normal que seja necessário adicionar ao menos um argumento obrigatório, para que seja possível saber quantos argumentos são.

Também é interessante saber que algumas funções do padrão do C como `printf` e `scanf` e suas variações, que normalmente são funções variádicas, também tem outras variações que começam com `v` como `vprintf` e `vscanf` que aceitam um parâmetro do tipo `va_list`.

Isso pode ser utilizado por exemplo, para implementar uma função variádica que faça algum tratamento adicional antes de chamar essas funções, por exemplo : 

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/*
    A função "scanf" normalmente é extremamente problemática para 
    leitura de entrada do usuário no terminal, pois pode manter coisas
    pendentes no buffer de leitura
    
    A própria documentação do manpages aponta isso e relembra a dificuldade 
    de usar "scanf" corretamente
    
    Utilizar "fgets" para leitura e possivelmente depois uma função separada
    de conversão como "sscanf"/"strtol"/"strtod"/"strtof" é melhor para ler 
    entrada do usuário, porém é mais complexo 
    
    Aqui vamos mostrar como podemos fazer a combinação "fgets" + "sscanf" ficar 
    transparente de forma que funcione como um "scanf" porém sem o problema de 
    leitura pendente.

    Vale lembrar, que o "scanf" é capaz de lidar com entradas enormes e as lê 
    sob demanda, enquanto aqui estou tentando ler a linha inteira de uma vez, 
    portanto "scanf" é ideal para leituras maiores (acima dos 4KB que defini)
    enquanto essa função se adequa melhor para leituras simples escritas pelo
    usuário.
*/
int scanf_user(const char *restrict format, ...)
{
    char line[4096];
    if(fgets(line, sizeof(line), stdin) == NULL)
        return feof(stdin) ? EOF : 0;

    va_list args;
    va_start(args, format);
    int result = vsscanf(line, format, args);
    va_end(args);

    return result;
}


int main()
{
    int test1, test2;
    scanf_user("%d\n", &test1);
    scanf_user("%d\n", &test2);

    printf("%d e %d\n", test1, test2);
}
```

Para testar a diferença prática do código acima, experimente escrever `abacate` na primeira entrada e `10` na segunda, e depois trocar as chamadas de `scanf_user` para `scanf` e testar novamente do mesmo jeito.

## Modificadores
Ainda existem 2 modificadores utilizados em funções, que fornecem dicas para o compilador, sendo eles `inline` e `_Noreturn`.

### Inline
O modificador `inline` é utilizado como uma dica para o compilador, indicando que chamar a função, deve idealmente evitar uma chamada "real" da função e apenas inserir as instruções contidas nela diretamente no local onde ela foi chamada.

Em funções com vinculação interna, o modificador `inline` pode ser usado normalmente (`static inline`).

A regra do C é que "se todas as declarações de funções especificam `inline` sem `extern` então a definição naquela unidade de tradução é uma definição `inline`", logo se em algum lugar de uma unidade de tradução escrevermos `extern inline void funcao1();` por exemplo, todas as definições da função `funcao1` mesmo tendo `inline`, não serão tratadas como `inline`. 

Na prática isso significa que o ideal é utilizarmos sempre `static inline` e opcionalmente, podemos fornecer em uma das unidades de traduções compiladas, uma única versão "não inline" da função que tenha o mesmo nome.

#### Motivação por trás do uso de inline

Chamar funções muitas vezes é custoso comparado com algumas instruções mais simples e chamar funções bilhões de vezes pode ter um custo relevante comparado a executar diretamente suas instruções.

Porém, o compilador é livre para ignorar a presença ou ausência desse modificador, fazendo o `inline` de funções que são pequenas e não tem o modificador `inline` ou decidindo mesmo numa função que o tenha que é mais eficiente chamar a função do que gerar cópias enormes de um código que é chamado em vários lugares e é grande.

Os compiladores de hoje em dia são cada vez mais inteligentes, portanto é difícil saber se `inline` realmente fará alguma diferença, a melhor maneira de saber isso é testando, para isso o [Compiler Explorer](https://gcc.godbolt.org/) é uma ótima alternativa que permite visualizar o assembly gerado ao compilar.

### _Noreturn
O modificador `_Noreturn` pode ser utilizado como `noreturn` ao adicionar a biblioteca `stdnoreturn.h`, no `C23` ele foi depreciado e substituito pelo atributo `[[noreturn]]`.

Este modificador indica que a função não vai retornar sob hipótese alguma, pois provavelmente vai utilizar `exit` ou algum outro mecanismo como `longjmp` que modifique o fluxo de execução impedindo o retorno.

Se existir a remota possibilidade da função retornar, como no caso de funções como o [execve](https://man7.org/linux/man-pages/man2/execve.2.html) do linux, que apesar de não retornar caso bem sucedida, ainda pode falhar, o ideal é não marcar a função como `noreturn`.

Existem dois motivos principais para marcar uma função como `noreturn`, a possibilidade de otimizações adicionais pelo compilador e avisos melhores por parte do compilador, que pode possivelmente indicar quando código é escrito após a chamada de uma função `noreturn` ou mesmo o uso da palavra chave `return` dentro dela.











