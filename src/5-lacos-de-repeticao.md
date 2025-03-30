# Laços de repetição
Laços de repetição são formas de controlar o fluxo de execução, de forma que uma parcela de código possa ser executada novamente.

Ao aliar laços de repetição com a modificação de variaveis, podemos efetivamente ter um comportamento diferente mesmo repetindo os mesmos comandos.

Da mesma forma que o `if`, as palavras chave `while`, `do while` e `for`, podem executar um único statement sem a necessidade de usar chaves (`{}`) ou vários statements ao colocar eles entre chaves.

## Controle de fluxo dos laços de repetição

Ao executar laços de repetição, existem duas palavras chave usadas para controlar o fluxo em um laço.

Uma delas é o `break`, no português "quebrar", usada para sair do laço de repetição forçadamente.

A segunda é o `continue`, no português "continuar", usada para pular para a próxima iteração.

O termo iteração se refere a uma das execuções da repetição, de forma que executar 3 vezes o conteúdo de um loop sejam "3 iterações", ou acessar todos elementos de uma lista em um laço de repetição seja normalmente dito como "iterar uma lista".

## While
A palavra chave `while`, no português "enquanto", é de certa forma similar ao `if`, porém ao final de cada execução, o fluxo de execução novamente volta a antes da condição ser checada.

Portanto, o `while` repete a execução do código enquanto sua condição for `Verdadeira`, e a condição é avaliada uma vez por iteração.


Abaixo um programa que calcula tabuada, usando `while` :

```c
#include <stdlib.h>
#include <stdio.h>

int lerNumero(const char *texto)
{
    char linha[1024];
    fgets(linha, sizeof(linha), stdin);
    return (int) strtol(linha, NULL, 10);
}

int main()
{
    int valor = lerNumero("Digite um número de 1 a 10:");
    if(valor < 1 || valor > 10) {
        puts("Digitou um valor inválido");
        return 1; //Finaliza o programa
    }

    int n = 1;
    while(n <= 10) {
        printf("%d x %d = %d\n", valor, n, valor * n);
        n++;
    }
}

```

Lembrando que é possível realizar um laço de repetição infinito utilizando `while(1)` por exemplo, nesses casos o normal é que haja uma condição com `break` para sair do loop.

## Do While
O `do while` é bastante similar ao `while`, a diferença é que a condição do laço é checada no final, sendo mais eficiente/ideal para casos onde a condição naturalmente se encaixa no fim.

A ordem para escrita do `do while` é `do conteudo while(condição)`, de forma que `do x++; while(x < 10);` seja uma forma válida para escrever, mas o mais comum é usando chaves no lugar do `conteudo`.

Exemplo de uso do `do while` : 

```c
#include <stdio.h>

int main()
{
    int i = 0;
    do {
        printf("%d\n", i);
        i++;
    } while (i < 5);
}
```

## For
A palavra chave `for`, no português "para", é normalmente utilizada para iterar sobre listas, arrays, strings (textos), etc.

Porém o `for` é separado em 3 campos: 

`for(pre-execução; condição; pós-iteração)`

- O campo de pre-execução, que pode ser vazio, inclui um único statement que é executado antes de checar o campo `condição` pela primeira vez, ele também pode conter uma declaração de variável (que só vai existir dentro do `for`).
- O campo de condição, é similar ao campo de condição de um `while`, porém quando este campo está vazio, considera-se um loop infinito (similar a `while(1)`).
- O campo da pós-iteração, que pode ser vazio, é um único statement (que não deve ter `;`) que é executado após cada iteração do laço, geralmente utilizado para modificar alguma variável usada dentro do laço.

Lembrando que no `for`, os statements não devem ter `;`, e o `;` é meramente o separador entre os campos e deve estar incluso, mesmo se o campo estiver vazio.

Dessa forma, um loop infinito com `for` pode ser feito utilizando `for(;;)`.

Exemplo do laço de repetição `for` usado para escrever todos valores de um array : 

```c
//Esta é uma "macro" que calcula o tamanho do array
#define ARRAY_SIZE(X) (sizeof(X)/sizeof(*X))

int main()
{
    //Este é um array, uma variável que indica uma lista de valores sequenciais
    //Vamos falar mais sobre ele mais tarde
    int numeros[5] = {53,37,84,28,39};

    //É muito comum o uso do nome "i" para criar um indice ao iterar
    //sobre arrays, o padrão (criar indice; checar faixa; incrementar) é muito comum
    for(int i = 0; i < ARRAY_SIZE(numeros); i++) {
        printf("%d\n", numeros[i]);
    }
}
```
