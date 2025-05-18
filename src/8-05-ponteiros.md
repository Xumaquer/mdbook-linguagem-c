# Ponteiros
Um ponteiro é um tipo de variavel utilizado para se referir a uma variável de outro tipo ou função. 
Ponteiros também podem referênciar o "nada", indicado pelo valor especial de ponteiro nulo.

Ponteiros são classificados em duas categorias distintas, ponteiros para dados que referenciam outras variáveis e ponteiros para funções, que permitem referenciar e chamar funções via referência.

## Declaração de ponteiros
Para declarar um ponteiro utilizamos a seguinte sintaxe : 
```c
* atributos modificadores nome

//exemplo
int *a;        //Ponteiro para int 
float **b;     //Ponteiro de ponteiro para float
const char *c; //Ponteiro para caractere constante
char *const d; //Ponteiro constante para caractere
```
- O asterisco é obrigatório e indica a declaração do ponteiro
- `atributos` são opcionais e indicam os atributos adicionados na linguagem no `C23`, eles devem sempre aparecer após o asterico mas antes dos `modificadores` e do `nome`.
-  `modificadores` são opcionais e indicam modificadores como `const`, `restrict`, `volatile`, etc.
- `nome` indica o nome da variavel criada ou outro `*`.

## Explicação 
Um ponteiro atua guardando um "endereço de memória", que pode ser utilizado para ler e escrever em outras variaveis ou no caso de ponteiros de função, executá-las.

Ponteiros podem ser utilizados para implementar uma série de funcionalidades e ao entender cada uma delas, teremos um entendimento mais completo do conceito de ponteiros.

Normalmente chamamos de "indireção" a necessidade de ler e acessar um endereço de memória para efetivamente ter acesso a uma variável, ponteiros podem ter vários "níveis de indireção", sinalizados pela quantidade de asteriscos usados na declaração da variável.

Algumas das funcionalidades que podem ser implementadas com ponteiros : 
- Passagem por referência 
- Acesso a objetos alocados dinamicamente
- Parâmetros "opcionais" 
- Relacionar estruturas
- Callbacks
- Interfaces genéricas

Algumas dessas funcionalidades serão explicadas e discutidas ao longo do texto.

## Como interpretar a sintaxe de ponteiros
Cada asterisco utilizado para declarar um ponteiro indica um nível de indireção necessário para "chegar efetivamente" no tipo apontado, essa é a maneira pretendida pelos autores da linguagem para interpretarmos a sintaxe de ponteiros.

Por exemplo, é comum ver as duas interpretações diferentes, geralmente associadas até mesmo a forma como os programadores formatam a declaração da variável : 
```c
const char **a; //Interpretação "a"
const char** b; //Interpretação "b"
```
- `Interpretação a` : Após duas indireções, teremos o tipo `const char`.
- `Interpretação b` : Temos um ponteiro de ponteiro para `const char`.

A diferença, apesar de sutil, é que na `interpretação a` estamos preocupados com o "tipo efetivo após indireções", enquanto na `interpretação b` com o "tipo final gerado na declaração".

Ambas as interpretações são válidas, porém infelizmente a linguagem favorece a `interpretação a`, apresentando casos onde a `interpretação b` pode levar o programador ao erro ou confusão com a sintaxe para declarar ponteiros mais complexos: 
```c
//Neste caso : 
//a é        "const char**"
//b é apenas "const char"
const char** a, b; 

//Neste caso, agora correto : 
//Ambos tem o tipo "const char**"
const char  **c, **d; 

//Se removermos o asterisco e o parenteses
//teremos exatamente a sintaxe de função!              
void funcao();       //Declaração de função
void (*funcaoptr)(); //Ponteiro de função

//Se utilizarmos *arr, ele se torna um array de 10 elementos
//Logo, *arrptr também se torna um array de 10 elementos após a indireção
int arr[20][10];   //Matriz
int (*arrptr)[10]; //Ponteiro para matriz
```  

## Ponteiros para dados
Ponteiros para dados podem ser inicializados utilizando o resultado do operador de endereço `&` aplicado a uma variavel.

```c
int v1;
int *p1 = &v1;       //p1 agora tem o endereço de "v1"
*p1 = 10;            //Escreve em v1

int v2[10];          //Array de 10 elementos
int (*p2)[10] = &v2; //Ponteiro para array de 10 elementos
(*p2)[0] = 10;       //Escreve em v2[0]

int v3[20];          //Array de 20 elementos
int *p3 = v3;        //Aponta para o array
*p3 = 20;            //Escreve em v3[0]

struct{
    int n;
}v4;                //Estrutura com um inteiro
int *p4 = &v4.n;    //Aponta para o membro
*p4 = 30;           //Escreve em v4.n 
```

A sintaxe para uso de ponteiros pode ser um tanto confusa de inicio, mas na verdade ela é bem similar ao uso de arrays, o examplo abaixo demonstra isso : 
```c
int array[10];
int *ptr = array;

//O operador de dereferenciar ponteiros funciona igual ao [0]
//e funciona tanto para arrays, quanto para ponteiros
*array = 10; //Escreve 10 em array[0]
*ptr   = 20; //Escreve 20 em array[0]

//Mesmo comportamento
array[0] = 30;
ptr[0]   = 40;

//Diferente
sizeof(array); //Igual a sizeof(int) * 10
sizeof(ptr);   //Igual a sizeof(int*) 

array++; //Proibido, não podemos mudar o inicio do array!
ptr++;   //Permitido, agora ptr[0] modificará array[1]

ptr[0] = 30; //Modifica array[1] devido ao incremento do ponteiro
ptr--;       //Permitido, agora ptr[0] novamente modificará array[0]
```

Basicamente, ponteiros tem um comportamento similar a arrays, porém o operador `sizeof` relatará o tamanho de um ponteiro e podemos somar/subtrair da variável do ponteiro para avançar/retroceder elementos.

Neste caso, podemos pensar num ponteiro como uma junção de "array" + "indice" em um único valor.

Comparações de ponteiros de dados também funcionam, comparações de igualdade podem checar se ponteiros apontam para o mesmo endereço, já comparações de maior/menor funcionam apenas com ponteiros que apontam para um mesmo array, ou para diferentes membros de uma mesma estrutura.

Quando utilizamos ponteiros para estruturas, o operador `->` pode ser utilizado para acessar os membros de uma estrutura via ponteiro, facilitando a sintaxe : 

```c
int main()
{
    struct Pessoa{
        const char *nome;
        int idade;
    }pessoa;

    struct Pessoa *joao = &pessoa;
    
    //Com o operador "->"
    joao->nome  = "Joao da Silva";
    joao->idade = 25;

    //Sem o operador "->"
    (*joao).nome  = "Joao da Silva";
    (*joao).idade = 25;
}
```

Quando somamos ou subtraimos um ponteiro, obtemos um ponteiro avançando/retrocedendo elementos equivalente a soma/subtração, logo `ptr+1` é equivalente a `&ptr[1]`.

Esse comportamento também demonstra a importância do tipo do ponteiro, pois digamos que `ptr` represente um ponteiro para um tipo que tem 8 bytes e esteja no endereço `0x200`, `ptr+1` estará no endereço `0x208`, logo somar 1 no ponteiro, na verdade está somando 8 internamente.

Além disso, um fato curioso sobre o operador `[]` pode ser visto no exemplo abaixo : 
```c
int x = 0;
int array[10];

//Isso aqui
array[5] = 10;

//É equivalente a 
*(array + 5) = 10;

//Logo ambos são equivalentes : 
array[x]; 
*(array + x);

//Devido a forma como o C implementa o operador []
//Isso aqui funciona : 
x[array]; 
//Pois será o equivalente a
*(x + array);

//A ordem da soma não importa, portanto, funciona
```  
## Ponteiros para funções 
Ponteiros de função guardam referências para funções e podem ser utilizados para chamá-las.

Podemos dizer que toda função já é um ponteiro de função, pois quando escrevemos uma função sem colocar parenteses, temos efetivamente o endereço dela.

De forma que assim como ponteiros de dados tem uma sintaxe muito similar a arrays, ponteiros de função tem uma sintaxe muito similar a funções.

Para declarar um ponteiro de função, temos uma sintaxe muito similar a utilizada ao declarar funções : 
```c
//Declaração de função
tipoRetorno NomeFunção(parametros);

//Declaração de ponteiro de função
tipoRetorno (*NomeVariavel)(parametros);
```
Basicamente a mesma sintaxe, porém com o nome entre parenteses com um asterisco, indicando que há uma indireção para acessar efetivamente a função.

Para chamar uma função via ponteiro, a sintaxe é exatamente a mesma utilizada para chamar funções : 
```c
#include <stdio.h>

int main()
{
    //Similar a declaração de função, mas com um parenteses a mais
    int (*puts2)(const char*) = puts;
    
    puts("Testando puts");
    puts2("Testando outro puts"); //Igual a chamada de função normal
}
```  

Para ponteiros de função, apenas a comparações de igualdade são aceitas pelo padrão da linguagem, já todas as outras operações como soma, subtração, maior/menor não estão definidas.

## Ponteiro Nulo
O ponteiro nulo é um valor especial que indica que a variavel atualmente representa uma referência vazia, ou seja, "nada".

É um valor constantemente utilizado para indicar um erro, ausência de valor, entre outras situações excepcionais.

No geral a linguagem C tem um tratamento um tanto "especial" para indicar um ponteiro nulo, algo que ainda hoje causa confusão e desentendimentos.

Toda vez que atribuimos um valor que é "uma constante de compilação inteira de valor `0`" a um ponteiro, ele magicamente se transforma no "valor de ponteiro nulo", que pode ou não ser representado internamente como `0` (todos bits em 0). 

Da mesma forma, a comparação de igualdade de um ponteiro nulo com `0` é sempre `Verdadeira`, mesmo que o hardware use um número diferente internamente.

Logo isso indica que "para linguagem C" o ponteiro nulo é sempre 0, porém para o hardware isso pode ser diferente, as arquiteturas modernas resolveram esse problema definindo em hardware o ponteiro nulo como 0, assim como o padrão POSIX exige que ponteiros nulos sejam representados em hardware com todos bits em 0.

Um exemplo demonstrando este comportamento, assumindo uma arquitetura onde o ponteiro nulo não é `0`: 
```c
enum {
    NULO_1 = 0,
};

#define NULO_2 0

int main()
{
    const int nulo3 = 0;

    int arr[5];
    void *p = arr; 

    memset(p, 0, sizeof(p)); //Define como 0, e não ponteiro nulo
    p = NULO_1;              //Define como ponteiro nulo
    p = NULO_2;              //Define como ponteiro nulo
    p = nulo3;               //Define como 0, pois nulo3 não é constante de compilação
}
```

No geral é muito comum programas utilizarem a função `memset` com o valor `0` para inicializar uma estrutura, isso faz com que o código não seja portável para arquiteturas antigas ou de embarcados onde o valor em hardware para o ponteiro nulo não seja `0`, mas não há problema algum em escrever código assim, desde que não seja utilizado em tais arquiteturas.

### Macro NULL
Geralmente quando queremos atribuir ou comparar um ponteiro nulo, utilizamos a macro `NULL` definida em `stdlib.h`, `stdio.h` e vários outros headers padrões do C, usada para gerar a constante de ponteiro nulo.

Sobre ela, qualquer implementação pode escolher uma entre as seguintes opções para implementar a macro : 

- `0`
- `(void*)0`
- `nullptr` (desde `C23`)

O principal problema neste caso é a possibilidade da utilização de `0`, que em alguns contextos, será tratada como `int` e não `void*`, causando problemas em interações com funções com parâmetros variádicos, operador `sizeof`, operador `_Generic` do `C11` ou `typeof` do `C23`.

O padrão POSIX determina que toda implementação de C deve utilizar `NULL` como `(void*)0`, evitando os problemas citados.

### nullptr
No `C23` foi adicionado a palavra chave `nullptr` que tem uma funcionalidade similar a `NULL`, criada para resolver problemas devido a possibilidade de `NULL` ser `0` e não `(void*)0` que tem um tipo próprio, `nullptr_t`.

Isso levou as seguintes vantagens:

- Compatibilidade maior com C++
- Dispensa necessidade de incluir as bibliotecas padrão só pela definição de `NULL`
- Tipo especifico para uso em `_Generic`
- Solução para problemas ao usar `typeof`
- Solução para problemas que ocorreriam caso `NULL` fosse `0`

## Tamanho de um ponteiro
O tamanho de um ponteiro é vinculado diretamente a arquitetura do processador, sistema operacional e compilador, então seu valor geralmente é dependente de uma variadade de fatores.

Nos sistemas modernos é comum que o tamanho dos ponteiros seja exatamente a quantidade de bits da arquitetura e o tamanho dos registradores em hardware, logo programas de 32bits tem ponteiros de 32bits (4 bytes) e programas 64bits tem ponteiros de 64bits (8 bytes).

No geral os ponteiros, tem sempre o mesmo tamanho independente do tipo que referenciam : 

`sizeof(char*)` = `sizeof(double*)` = `sizeof(void*)`

Assim como o mesmo ocorre entre ponteiros de função :

`sizeof(void (*)())` = `sizeof(int (*)(float))`

Alguns sistemas embarcados ou mais antigos tem sua memória separada em segmentos, os compiladores para essas arquiteturas geralmente suportam um modificador extra `far` como extensão de compilador, que permite criar ponteiros "maiores" que podem referenciar dados que estejam em outros segmentos.

Ponteiros para funções e ponteiros para dados não necessariamente precisam ter o mesmo tamanho, em algumas arquiteturas antigas e sistemas embarcados o `código de máquina` e dados ficam em segmentos de memória separados, possibilitando tamanhos diferentes de endereçamento.

Porém, todo sistema operacional moderno utiliza ponteiros de função e de dados com o mesmo tamanho, inclusive a especificação POSIX para sistemas UNIX portáveis exige essa igualdade.

Programa que escreve um endereço e os tamanhos de ponteiros : 
```c
#include <stdio.h>

int main()
{
    int valor;
    int *p = &valor;

    printf("%p\n"                  //%p  é o formatador para ponteiros
           "tamanho int*  = %zu\n" //%zu é o formatador para "size_t"
           "tamanho void* = %zu\n"
           "tamanho ponteiro funcao = %zu\n",
           p, sizeof(p), sizeof(void*), sizeof(void (*)()));
}
```


## Ponteiro void
No C é muito comum o uso do tipo `void*` para representar ponteiros, como todo ponteiro de dados tem o mesmo tamanho, podemos ter um parâmetro que indica um ponteiro de dados "genérico" com o `void*`.

A ideia é que não é possível acessar seu valor, pois ele aponta para um objeto de tipo inexistente (`void`), não é possível somar/subtrair o ponteiro, pois não se sabe seu tamanho.

Qualquer ponteiro de dados pode ser implicitamente convertido para `void*` (tanto no C quanto no C++), e qualquer ponteiro `void*` pode ser implicitamente convertido para qualquer outro tipo de ponteiro (apenas no C).

Na prática isso significa que podemos passar qualquer ponteiro a uma função que aceita `void*`, mas se a função deseja acessá-los, ela deve fazer um cast ou passar para uma variável que seja um ponteiro de um tipo conhecido.

Ao mesmo tempo que funções como `malloc` que retornam `void*`, podem ser diretamente atribuidas a qualquer variavel de ponteiro sem necessidade de cast (no C apenas).

## Passagem por referência
O conceito de passagem de valor por referência envolve a forma como passamos variáveis a funções.

Sempre que passamos um parâmetro a uma função, consideramos que a função recebe uma "cópia" do parâmetro original, logo qualquer modificação na cópia não altera a variável original que foi copiada, chamamos isso de "passagem por valor".

Podemos dizer que essa noção muda quando utilizamos ponteiro, pois ponteiros são capazes de modificar a variável original ao utilizarmos seu endereço.

Quando passamos um parâmetro a uma função, que permita que essa função leia ou modifique diretamente a variável original sem criar cópias, dizemos que essa é uma "passagem por referência".

Exemplo de passagem de valor vs passagem por referência : 
```c
#include <stdio.h>

void modificaPorValor(int valor, int novoValor) 
{
    valor = novoValor;
}

void modificaPorReferencia(int *valor, int novoValor)
{
    *valor = novoValor;
}

int main()
{
    int teste = 5; //O valor original é 5
    modificaPorValor(teste, 10);      //Não consegue modificar, teste ainda é 5
    printf("%d\n", teste);

    modificaPorReferencia(&teste, 10); //Consegue modificar, teste agora é 10
    printf("%d\n", teste);
}
```

> Numa analogia simples, podemos pensar na situação onde queremos mostrar com detalhes uma foto a um 
> amigo.
>
> Uma "passagem por valor" envolveria enviar a foto pelo whatsapp, discord ou qualquer outra rede social para que ele tenha sua própria cópia.
> 
> Uma "passagem por referência" seria deixarmos ele segurar seu celular para que ele mesmo possa ver e dar zoom, mas isso também possibilita que nessa situação, ele apague a foto original.

### Passagem por valor ou referência?
Entenda que, mesmo quando utilizamos uma "passagem por referência", uma cópia ainda ocorre, mas o que é efetivamente copiado é o endereço, e não o valor da variavel.

Em muitos casos o uso de ponteiros para passsagem de referência é realizado pensando em evitar cópias dos valores, mas em alguns casos a passagem de valor pode ser mais eficiente do que a de referência.

O fator determinante reside no tamanho em bytes de um ponteiro junto do leve custo adicional de acessar o valor via uma indireção.

No geral, podemos começar considerar repassar um valor por referência como mais eficiente do que um `tipo` via ponteiro quando : 
```c
sizeof(tipo) > sizeof(void*) 
```

E consideramos que é recomendado que o valor seja passado via ponteiro quando : 
```c
sizeof(tipo) > (2 * sizeof(void*))
```

Importante lembrar que é muito comum passar variáveis com tipo `struct` via ponteiro devido a possibilidade de adicionarmos novos membros no futuro. 

### Acesso a variáveis alocadas dinamicamente
Aqui vai uma leve introdução ao assunto, que será explicado com detalhes em outro capítulo.

Na biblioteca `stdlib.h` existem as funções `malloc`, `realloc` e `free` que permitem um gerenciamento manual de memória, onde podemos solicitar uma quantia de bytes a ser utilizada pelo nosso programa e posteriormente devolver a quantia solicitada, para que possa ser reutilizada em outra solicitação ou por outros programas.

Ao utilizar alocação dinamica, recebemos efetivamente um ponteiro para um bloco de memória que esteja livre e tenha pelo menos o tamanho solicitado, a questão é que o local exato desse bloco é decidido na hora e provavelmente será diferente a cada execução do programa.

Logo, é impossível utilizar essa memória dinâmica sem utilizar ponteiros, pois precisamos de uma variável que possa guardar e acessar esse local "imprevisível" que conterá nossos dados.


