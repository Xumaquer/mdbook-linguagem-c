# Controle de fluxo
Normalmente quando executamos programas, a execução percorre linha a linha, ou sendo mais preciso, a execução percorre "statement" por "statement".

Statement, em tradução literal é uma "declaração", é simplesmente um comando finalizado por `;`, e que não necessáriamente precisa estar em uma linha.

```c
#include <stdio.h>

int main()
{
    int x, y, z;

    //Vários statements em uma única linha
    x = 10; y = 15; z = 20;

    //Um statement em múltiplas linhas
    printf(
        "x = %d\n"
        "y = %d\n"
        "z = %d\n",
        x,y,z
    );
}
```

Este fluxo delimitado por statements sequenciais pode ser desviado e modificado condicionalmente, ao utilizarmos palavras chaves para controle de fluxo.

[Laços de repetição](./5-lacos-de-repeticao.md) também são uma forma de controle de fluxo, porém serão melhor explicados na página seguinte.


# If 
A palavra chave `if`, no português "se", é utilizada para desviar o fluxo de execução caso a condição especificada não seja atendida.

Também podemos dizer isso de outra forma, que o `if` é utilizado para executar uma parcela de código quando uma condição é atendida.

A sintaxe para escrita é `if(CONDIÇÃO)`, onde `CONDIÇÃO` é uma expressão qualquer que será avaliada como uma expressão booleana, de forma que valores iguais a 0 sejam `Falso` e valores diferentes de 0 sejam `Verdadeiros`.

Exemplo usando if : 

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char linha[1024];
    
    printf("Escreva um número:");
    fgets(linha, sizeof(linha), stdin);

    int valor = (int)strtol(linha, NULL, 10);
    
    if(valor > 100)
        puts("Escreveu um valor maior que 100");
    else
        puts("Escreveu um valor que não é maior que 100");
}
```

No geral quando utilizamos `if` ou qualquer outra palavra chave de controle de fluxo, podemos colocar blocos `{}` para especificar múltiplos statements ao invés de apenas um.

Como por exemplo : 

```c
    if(temCafe) { 
        prepararCafe();
        gritarQueTemCafe();
        beberCafe();
    } else { 
        prepararParaApocalipse();
    }
```

## Else e Elseif
As palavras chaves `else` e `else if` são utilizadas em conjunto com `if` e outros `else if`, de forma que seja possível descrever um fluxo alternativo caso a condição anterior não tenha sido `Verdadeira`. 

A palavra `else` poderia ser traduzida literalmente para "se não", indicando uma outra abordagem caso a checagem anterior tenha "falhado".

A palavra chave `else` é utilizada para delimitar um código que será executado caso todas as condições anteriores em sequência tenham resultado em `Falso`.

A palavra chave `else if` é utilizada para executar condicionalmente um código assim como o `if`, mas que só será avaliada caso todas as condições anteriores em sequência tenham resultado em `Falso`. 

Exemplo utilizando `if`, `else if` e `else` : 
```c
    if(idade < 3) {
        puts("Bebê");
    } else if(idade < 12) {
        puts("Criança");
    } else if(idade < 18) {
        puts("Adolescente");
    } else if(idade < 30) {
        puts("Jovem adulto");
    } else if(idade < 65) {
        puts("Adulto");
    } else {
        puts("Idoso");
    }
```

## Switch 
A palavra chave `switch` é utilizada para realizar um controle de fluxo condicional para valores especificos de uma variável, geralmente utilizado em conjunto com constantes e enumerações.

Para cada valor que deseja-se checar, utilizamos a palavra chave `case`, escrevendo `case valor:`.

Também podemos escrever `default:` para indicar um código que será executado caso o valor da variável avaliada não seja nenhum dos `cases` listados, funcionado de maneira similar a palavra chave `else`.
 
```c
switch(valor) {
case 1:
case 2:
case 3:
    puts("1,2 ou 3");
    break;
case 4:
    puts("4");
    break;
case 5:
    puts("5");
    break;
default:
    puts("Menor que 1 e maior que 5");
    break;
}
```

Percebeu que no exemplo acima, foi usada a palavra chave `break` também?

Isso foi necessário pois cada `case` de um `switch` também apresenta um comportamento que chamamos em inglês de "fallthrough", que em português seria simplesmente "queda", pois logo após executar o código em um `case`, os códigos de outros `cases` escritos logo após ele, são executados.

Porém ao escrevermos `break` forçamos o código a sair do `switch`, evitando que isso aconteça.

Isso ocorre pois em alguns casos é útil termos código que pula para um ponto especifico, mas continua a executar a partir dele, apesar de ser incomum. 

Pode acontecer do programador simplesmente esquecer de colocar `break`, portanto a maioria dos compiladores emite um aviso quando um "fallthrough" pode acontecer em um `switch`, oferecendo geralmente como extensão de compilador, algum comando para indicar quando um fallthrough é intencional.

Desde o `C23` podemos escrever `[[falthrough]]` para indicar que um fallthrough é intencional, tornando o que antes era uma extensão especifica de compilador que deveria ser escrita de uma forma diferente para cada um, algo padronizado na linguagem.

Exemplo de `switch` utilizando fallthrough : 
```c

//A ideia aqui é listar tudo que pode ser feito em cada nível de acesso
switch(nivelDeAcesso) {
case 3: 
    puts("Você pode acessar as salas confidenciais");
case 2:
    puts("Você pode acessar as salas dos gerentes e cargos de chefia");
case 1:
    puts("Você pode acessar a sala dos funcionários");
case 0:
    puts("Você pode acessar a sala dos convidados");
    break;
default:
    puts("Nível de acesso desconhecido");
}
```

No exemplo acima, temos um caso de fallthrough intencional, onde o nível de acesso mais alto também tem os privilegios menores e portanto devem ser listados também.

O resultado do código acima ao definir `nivelDeAcesso` para 3 é : 
```
Você pode acessar as salas confidenciais
Você pode acessar as salas dos gerentes e cargos de chefia
Você pode acessar a sala dos funcionários
Você pode acessar a sala dos convidados
```

Lembrando que os valores em cada caso do `switch` precisam ser constantes de compilação, logo só podem ser definidos em enumerações, variaveis `constexpr` e definições de preprocessador (`#define`).

## Goto
Agora temos a palavra chave de controle de fluxo mais controversa de todas, o temido `goto`.

O `goto`, no português "vá para", oferece uma possibilidade de realizarmos saltos indiscriminados do fluxo de execução, se assemelhando a forma como as próprias instruções dos processadores implementam o controle de fluxo.

Para poder realizar um salto com `goto`, é necessário escrever um "label" (em português, seria rótulo) `nomeDoLabel:` em qualquer lugar do código, e ao chamar `goto nomeDoLabel`, a execução do código "salta" para o local onde o label foi escrito. 

Um exemplo simples de código utilizando `goto` :

```c
#include <stdio.h>
#include <stdlib.h>

int lerNumero(const char *mensagem)
{
    char linha[1024];
    printf(mensagem);
    fgets(linha, sizeof(linha), stdin);
    return (int) strtol(linha, NULL, 10);
}

int main(int argc, char **argv)
{
    puts("== Calculadora de multiplicação ==");

    int valor1 = lerNumero("Escreva um número maior que 0:");
    if(valor <= 0)
        goto sair;

    int valor2 = lerNumero("Escreva outro número maior que 0:");
    if(valor2 <= 0)
        goto sair;
    
    printf("%d x %d = %d\n", valor1, valor2, valor1 * valor2);
sair:
    puts("Estou saindo");
}
```

No exemplo acima, o `goto` foi utilizado para executar um código em comum na finalização do programa, apesar de existirem outras formas de realizar a mesma coisa, neste caso, utilizar `goto` é uma das formas mais simples.

## Sobre o uso de goto

O `goto` é ideal nos casos onde há tratamento de erros, necessidade de executar um código em comum na finalização, sair de vários loops aninhados e talvez em alguns outros usos especificos que não saberei citar aqui.

Porém, muitos professores de faculdade ou de cursos técnicos tem uma tendência a dizer fortemente "Não usem goto", como se isso fosse uma regra que deve ser seguida sempre e por todos.

A motivação é clara, normalmente ao analisar um programa, basta ver as chaves, identação, e facilmente podemos visualizar o fluxo de um programa, porém a possibilidade de utilizar `goto` pode contrariar isso, ainda mais se o usarmos de forma indiscriminada e colocarmos `goto`s que voltam linhas de código, pulam inicialização de variaveis, etc.

Porém um uso consciente de `goto` não prejudica a legibilidade e pode ser uma solução muito mais simples para vários problemas, considere-o como apenas mais uma ferramenta.

Minha dica é sempre pense com cuidado as possibilidades ao escrever um código e evite cair em falácias como "nunca use isso" , "nunca faça aquilo" sem nem mesmo pensar sobre.

> Ao escrever labels de `goto`, busque escrever nomes simples e fáceis de entender
> que exemplifiquem a motivação do pulo ou a ocasião em que ele ocorre.
