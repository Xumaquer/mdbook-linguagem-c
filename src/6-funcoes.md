## Funções
Funções são parcelas de código "reutilizável", escritas para que possamos dar um nome a um pedaço de código e reutilzá-lo em diversas situações, possívelmente com parâmetros diferentes.

As principais vantagens ao utilizar funções são : 
- Reutilização de código
- Organização do código por meio de nomes bem definidos
- Possibilidade de parametrizar um código 
- Diminuição de dependências externas

Funções são declaradas utilizando a sintaxe `tipoRetorno NomeFunção(parametros)` : 
- `tipoRetorno` indica o tipo do valor resultante de uma função, que pode ser atribuido a uma variavel ou usado em outras expressões, podemos escrever `void` para indicar que a função não resulta em um valor.
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

Também é importante mencionar que desde o `C99`, é possível acessar o nome da função atual utilizando `__func__`, que é tratado como uma variável constante e de duração estática (que claramente só será incluida na memória do programa se for utilizada).

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

Esses códigos de status podem ser acessados pelo terminal diretamente após executar um programa utilizando a variavel `%errorlevel%` no Windows ou `$?` no bash em linux ou macOs.

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

## Definições de funções
Se uma função é declarada no C, ela geralmente deve ser declarada antes da função que a utiliza, sem isso, compilar o programa resulta em um erro de compilação.

Porém, existe uma forma de burlar isso, definindo a função sem declarar.

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

Quando definimos uma função, estamos especificando como a função deve ser chamada e quais parâmetros ela deve receber, logo toda informação relevante para chamar a função está presente.

Mas a explicação real do que realmente acontece quando definimos uma função é um pouco mais complexa.

Ao definir uma função, estamos basicamente dizendo ao compilador "confia em mim, essa função claramente existe", o compilador, por sua vez, decide acreditar em você até os últimos momentos.

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