# Variáveis
Variáveis são, efetivamente, lugares onde valores podem ser lidos ou escritos. 

No geral são identificadas unicamente por nomes que definimos no nosso programa, junto com uma segunda descrição que chamamos de "tipo".

Os tipos de variaveis são nomes predefinidos, que formam uma descrição "do que está sendo guardado", que ajuda a descrever que tipo de operação deve ser feita para poder ler e escrever na variável que é de um determinado tipo.

Variáveis podem ser utilizadas para descrever todo tipo de dado, e idealmente, devem ter um nome que simplifica o entendimento do seu propósito e uso de forma a facilitar a leitura do código.

A linguagem C diferencia entre nomes em letra maiúscula e letra minúscula, logo `variavel`, `VARIAVEL` e `VaRiAvEl` são considerados nomes diferentes.

> Quanto a variáveis, podemos fazer uma analogia simples mas efetiva :
>
> Considere agora, que "variáveis" são caixas, geralmente usamos caixas para guardar coisas.
>
> Porém, nem toda caixa tem o mesmo tamanho ou formato, existem vários `tipos` de caixas, para propósitos
> diferentes.
> 
> Se tivermos muitas coisas em caixas, fica difícil saber onde fica cada coisa, para isso, geralmente colocamos
> etiquetas com `nomes`, pois isso facilita a organização e o entendimento do propósito de cada caixa.
>
> De certa forma, `ler` de uma variável é similar ao processo de retirar algo de uma caixa
> da mesma forma que `escrever` em uma variável, é similar ao processo de colocar algo em
> uma caixa.

## Modificadores de tipo
Ao declarar variáveis podemos adicionar modificadores, que servem para especificar detalhes adicionais sobre a variável e seu tipo.

| Modificador | Descrição                                                       |
| ----------- | --------------------------------------------------------------- | 
| `_Atomic`   | Define que as operações de leitura/escrita são atômicas (`C11`) |
| `const`     | Define que a variável não pode mudar seu valor                  | 
| `constexpr` | Define que a variável é uma constante de compilação (`C23`)     |
| `restrict`  | Indica que o ponteiro não sofre de aliasing                     |               
| `volatile`  | Impede reordenamento e remoção de leitura e escrita             |                
| `signed`    | Usado para definir um inteiro com sinal                         |
| `unsigned`  | Usado para definir um inteiro sem sinal                         |

### _Atomic
Introduzido no `C11`, o modificador `_Atomic` indica que as operações de leitura e escrita com um valor são atômicas.

Mas o que seria uma operação atômica? É uma operação que evita o que chamamos de "Condição de corrida" ou no inglês "data race".

Uma condição de corrida é um problema que pode acontecer quando há mais de um thread (mais de um fluxo de execução) simultâneamente escrevendo/lendo de uma mesma variável ou local na memória.

Em caso do conflito descrito acima, um programa tem uma condição de corrida a menos que uma das condições abaixo seja cumprida : 
- Ambas operações de leitura/escrita sejam atômicas
- Uma das operações acontece antes da outra (ou seja, elas não acontecem ao mesmo tempo)

Caso uma condição de corrida ocorra, o comportamento do programa é indefinido, mas na prática o comportamento mais comum é que um valor intermediário antes da atualização completa por uma das partes seja lido pela outra, de forma que ela opere com um valor inválido e os cálculos envolvendo essa variável também tenham resultados "incorretos".

Ao mesmo tempo que operações atômicas evitam condições de corrida, elas também são consideravelmente mais lentas, por isso só devem ser utilizadas se realmente há um risco de uma condição de corrida ocorrer.

Qualquer compilador pode definir a macro `__STDC_NO_ATOMICS__` para indicar que `_Atomic` não é suportado.

> Para entender de forma mais simples como uma condição de corrida ocorre
> imagine dois fluxos de execução que querem incrementar em 1 um contador.
>
> Imagine que o contador inicia em 0 e a seguinte sequência de operações ocorra : 
>
> - Fluxo 1 lê o valor do contador (0)
> - Fluxo 1 incrementa o valor do contador no seu registrador (1)
> - Fluxo 2 lê o valor do contador (0)
> - Fluxo 1 guarda o valor atualizado (1)
> - Fluxo 2 incrementa o valor do contador no seu registrador (1)
> - Fluxo 2 guarda o valor atualizado (1)
> 
> Percebe como um contador que deveria ter 2, acabou resultando no valor 1 ?
>
> É isso que normalmente acontece com uma condição de corrida, valores são atualizados
> sem levar em consideração modificações do outro fluxo.

### Const
O modificador `const` faz com que o valor não possa ser modificado após sua definição.

Utilizar `const` ao definir variáveis ajuda a delimitar seu uso e evitar modificações desnecessárias e dependendo do caso pode permitir ou facilitar mais otimizações por parte do compilador ao delimitar melhor como valores serão utilizados.

Outro uso extremamente importante do `const` é ao utilizar ele com ponteiros em parâmetros de função, pois um `const` indica que o parâmetro não será modificado pela função, sem a necessidade de explicações adicionais.

```c
//Como demonstra a função abaixo
//Os valores utilizados para calcular um "const" não precisam
//ser constantes
double pythagoras(double a, double b)
{
    const double a2 = a * a;
    const double b2 = b * b;
    const double c  = sqrt(a2 + b2);

    return c;
}

void test() 
{
    int v1 = 50;
    const int v2 = v1 + 10; //60
    v2 = 70; //Erro, não pode modificar constante!
}

//"const" também 
void copiaMemoria(const void *fonte, void *destino, size_t tamanho)
{
    const char *bfonte = fonte;
    char *bdestino = destino;

    while(tamanho--)
        *bdestino++ = *bfonte++;
}
```

Como constantes podem ser inicializadas com valores não constantes, elas não podem ser utilizadas como labels para o operador `switch`, usada para definir `enum`, inicializar valores definidos como `static` ou `thread_local`, bit fields em estruturas e quando utilizados como tamanho de arrays, os arrays resultantes são arrays de tamanho variável.

### Constexpr
O modificador `constexpr` foi adicionado apenas no `C23` e indica que a variável é uma constante de compilação.

Uma constante de compilação, é um valor que nunca muda e tem um valor que já é conhecido desde antes do programa iniciar (ou seja, seu valor foi adicionado diretamente no executável, sem necessidade de ser calculado durante a execução).

Dessa forma, um valor `constexpr` não tem as limitações do modificador `const` que não pode ser usado em lugares que exigem uma constante de compilação.

```c
    //Valor constexpr
    constexpr int RODAS_CARRO = 4;

    //Pode ser utilizado para inicializar enums
    enum RodasDeVeiculos { 
        RODAS_MOTO = RODAS_CARRO / 2,
        RODAS_CAMINHAO_GRANDE = RODAS_CARRO * 2,
    };

    //Pode ser utilizado como tamanho de array
    int tipoRoda[RODAS_CARRO]; 

    //Pode ser utilizado num switch, assim como o enum
    switch(quantidadeDeRodas) {
        case RODAS_MOTO:
            puts("É uma moto");
            break;
        case RODAS_CARRO:
            puts("É um carro");
            break;
        case RODAS_CAMINHAO_GRANDE:
            puts("É um caminhão grande!");
            break;
    }

    //Erro, não pode ser modificado!
    RODAS_CARRO = 10; 
``` 

### Restrict
O modificador `restrict` indica que um ponteiro não sofre de "aliasing".

O termo "aliasing" neste caso se refere a possibilidade de dois ponteiros distintos apontarem para o mesmo objeto, ou parcialmente, de forma que a modificação de um deles, afete o valor do outro.

Caso haja, por exemplo, uma função que recebe dois ponteiros de mesmo tipo, a cada escrita em um dos ponteiros, pode ser necessário ler novamente do outro ponteiro para que o C garanta o comportamento correto para qualquer entrada.

Portanto, uma função que obviamente não foi construida com este intuito, pode especificar `restrict` nos ponteiros, indicando que são valores que apontam para objetos distintos, ou mesmo que apontem para um mesmo objeto, indicando que isso não deve ser levado em consideração.

Indicar que ponteiros não sofrem de aliasing permite que o compilador gere um programa mais eficiente, que pode manter em cache/registradores os valores lidos ou executar várias operações simultâneas neles sem se preocupar em tratar uma possível modificação por meio de outros ponteiros.

```c
    //Por exemplo, se o compilador assumir aliasing, ele 
    //precisa fazer uma multiplicação por vez
    void multiplicarArray(double *a, double *b, double *resultado, size_t tam)
    {
        for(size_t i = 0; i < tam; i++)
            resultado[i] = a[i] * b[i];
    }

    /* Porém ao definir que as variaveis não dão "aliasing", o
       compilador é livre para utilizar instruções que carregam
       e multiplicam multiplos valores de uma vez, assumindo 
       que uma operação não afeta a outra                   */
    void multiplicarArrayEx(double *restrict a, double *restrict b, 
                            double *restrict resultado, size_t tam)
    {
        for(size_t i = 0; i < tam; i++)
            resultado[i] = a[i] * b[i];
    }

```
### Volatile
O modificador `volatile` indica que leituras e escritas a variável não podem ser reordenadas, removidas por otimização ou lidas de cache.

A necessidade de uso desse modificador geralmente se da quando um valor pode ser modificado de forma imprevisível, ou serve para interfacear ou controlar hardware (controlar saídas, comunicar com HD, placa de rede, tela, GPU, etc).

Existem alguns motivos pelo qual esse modificador pode ser útil: 
- Quando a variável pode ser diretamente modificada pelo hardware, ou o valor dela tem influência sobre o hardware
- Quando a variável faz parte de uma memória compartilhada e pode ser modificada por outros processos
- Quando há outros threads que podem modificar a variável
- Quando existe alguma função que modifica a variável e pode ser chamada externamente (como chamadas de procedimento remoto, ou os [signals](https://man7.org/linux/man-pages/man7/signal.7.html) do POSIX que são geralmente chamados pelo próprio kernel)


### Signed
Usado como modificador para declarar variáveis de tipos inteiros com sinal (ou seja, que podem ter valores negativos).

Na maioria dos casos, não há necessidade de declarar `signed` explicitamente, pois o padrão da maioria dos tipos inteiros já são inteiros com sinais, com uma única exceção do tipo `char`, que por padrão pode ser `unsigned`ou `signed`, uma escolha que é definida pela implementação da linguagem pelos compiladores.

> A única necessidade real de escrever `signed` é para utilização do tipo `signed char`.

### Unsigned
Usada como modificador para declarar variáveis de tipos inteiros sem sinal (ou seja, que não podem ter valores negativos).

Inteiros sem sinal tem certas vantagens em relação aos tipos com sinal, no geral por não incluir valores negativos, os tipos sem sinal geralmente conseguem guardar valores até duas vezes maiores do que o valor máximo de uma variável com sinal de tamanho equivalente.

No geral os tipos sem sinal garantem um comportamento circular em caso de overflow (ultrapassar o limite da variável), onde elas "dão a volta" e retornam ao valor 0, da mesma forma que subtrair 1 de 0, faz com que um número sem sinal chegue ao seu valor máximo (este caso é geralmente chamado de "underflow" e é um dos principais motivos para evitar números sem sinal).

Além disso o comportamento para deslocamento é sempre definido, enquanto que para valores com sinal, deslocar para esquerda os bits de um valor negativo é indefinido.

```c
signed char a;   //char com sinal (geralmente -128 a 127)
unsigned char b; //char sem sinal (geralmente 0 a 255)

int c;          //int com sinal
unsigned int d; //int sem sinal

long long e;          //long long com sinal
unsigned long long f; //long long sem sinal


//Valor máximo de um "unsigned char"
unsigned char teste = UCHAR_MAX;

//Neste caso, da overflow e resulta em 0 (isso é garantido pela linguagem)
teste = teste + 1;

//Valor mínimo de um "unsigned char"
unsigned char teste2 = 0;

//Neste caso, ocorre um underflow e o valor resulta em UCHAR_MAX
teste2 -= 1;

//Uma das armadilhas do C que muitos acabam caindo
//Alguns assumiriam que "v2" resultaria em "-2"
//PORÉM, a resposta certa é "2147483646" devido ao
//underflow em (v1-8)
unsigned int v1 = 4;
int v2 = (v1-8)/2; 
```

## Modificadores de armazenamento
Os modificadores de armazenamento podem ser utilizados para modificar variáveis ou funções e definem tanto a duração (apenas para variáveis) e vinculação (para ambos).

A duração, neste caso, é o escopo no qual a variável vai continuar existindo, os tipos de duração existentes são : 
- `automática`: A variável é armazenada quando o bloco inicia e desalocada quando o bloco é encerrado, isso se aplica a variáveis locais e parâmetros de funções.
- `estática`: A variável é armazenada durante toda a execução do programa, e o valor guardado no objeto é inicializado apenas uma vez, antes da execução da função `main`, isso se aplica a variáveis globais ou variáveis com `static` dentro de funções.
- `thread`: A duração da variável é igual a duração do thread na qual ela foi criada e seu valor é inicializado quando o thread é inicializado.
- `alocada`: A variável foi alocada dinâmicamente usando [alocação de memória dinâmica]() e pode ser desalocada a qualquer momento pelo próprio programa.

A vinculação, indica os locais onde a função ou variável é acessível e pode ser usada, para facilitar, é importante conhecermos o termo "unidade de tradução" do C.

Uma unidade de tradução é geralmente cada arquivo `.c` separado junto dos arquivos `.h` incluidos por ele, cada unidade de tradução é um dos arquivos que você pede para serem compilados pelo compilador.

Dito isso, os tipos de vinculação existentes são : 
- `Sem vinculação` : A variável ou função só pode ser referida dentro do mesmo bloco.
- `Interna`: A variável só pode ser referida dentro da mesma unidade de tradução.
- `Externa`: A variável pode ser referida por qualquer unidade de tradução, ou seja, pode ser referida por outros arquivos.

Os modificadores existentes são : 

| Modificador    | Descrição                                                   |
| -------------- | ----------------------------------------------------------- | 
| `auto`         | Duração `automática` e `sem vinculação`                     | 
| `register`     | Dica para que o compilador guarde a variável em registrador |
| `static`       | Duração `estática` e vinculação `interna`                   |               
| `extern`       | Duração `estática` e vinculação `externa`                   |  
| `thread_local` | Duração de `thread` (`C11`)                                 |               

O padrão de cada tipo é : 
- variável local : duração `automática` e `sem vinculação`
- variáveis globais : vinculação `externa`
- funções : vinculação `externa`

### auto 

O modificador `auto` é considerado inútil, pois só pode ser aplicado a variáveis locais e já é o comportamento padrão para elas.

### register

O modificador `register` é utilizado para indicar que uma variável deve ser diretamente guardada apenas em registrador, isto é, nas "mãos" do processador, utilizadas para operar com valores, e portanto, o endereço da variável não pode ser acessado.

`register` implica duração `automática` e `sem vinculação` assim como a palavra chave `auto`.

Antigamente o modificador `register` era útil, ajudando nas otimizações ao fornecer dicas ao compilador, mas hoje em dia, com a presença de compiladores super inteligentes, eles acabam tendo muito mais informação do que nós sobre uma variedade de parâmetros ao decidir sobre otimizações, diminuindo ou eliminando vantagens que normalmente seriam oferecidas pela palavra chave `register`.

Apesar disso, vale relembrar que a palavra chave `register` ainda é a única forma de "proibir" o uso do endereço de uma variável.

### static
O modificador `static` tem dois usos, ele pode ser usado dentro de blocos para definir variáveis que só podem ser acessadas no bloco mas tem duração `estática`, ou seja, mantêm o valor entre execuções da função e são efetivamente guardadas no mesmo lugar que variáveis globais seriam.

Seu segundo uso é para definir que funções e variáveis globais não afetem outras unidades de tradução, evitando conflitos de nomes e facilitando algumas otimizações (ao garantir que algo nunca vai ser usado externamente, algumas otimizações adicionais se tornam possíveis).

### extern
O modificador `extern` normalmente não precisa ser usado em funções, pois já é o padrão.

Já em variáveis, o modificador funciona como uma "importação" de uma variável já existente, permitindo que ela seja usada no código, mesmo se ela estiver definida em outra unidade de tradução, mas ocasionando em erro caso não nenhuma unidade de tradução tenha a implementação da variável global.

```c
    //Caso 1, erro ao compilar, pois ninguém definiu 
    //a variável sem "extern"
        //arquivo teste1.c
        extern int a;

        //arquivo teste2.c
        extern int a;

    //Caso 2, definição correta
        //arquivo teste1.c
        int a;

        //arquivo teste2.c
        extern int a;

    //Nesse caso, teste2.c pode utilizar a variável
    //que foi definida em teste1.c 
```
## thread_local
De forma similar ao `_AlignOf` descrito nos operadores, o modificador `thread_local` foi introduzido no `C11` como `_Thread_local` junto da macro `thread_local` presente em `threads.h`, porém no `C23`, `thread_local` se tornou efetivamente um modificador válido no C.

O modificador `thread_local` simplesmente indica que a variável é local do thread, e é criada justamente quando um thread é criado. De forma que cada thread tenha sua própria cópia da variável, sendo ainda possível utilizar `static`/`extern` para ajustar o tipo de vinculação.

## Palavra chave typedef
A palavra chave `typedef` é utilizada para definir um novo tipo de variável que é efetivamente um apelido para um tipo já existente.

O `typedef` também pode ser utilizado para gerar um apelido simplificado para um tipo complexo que tenha modificadores como `const`, `volatile`, `_Atomic` e/ou seja um ponteiro, ponteiro de função, etc.

A sintaxe para uso do `typedef` é igual a sintaxe de declaração de uma variável, no qual ao invés de criarmos uma variável, o nome que essa variável teria se torna o nome do novo tipo definido.

Essa similaridade se torna ainda mais evidente quando percebemos que de forma similar a declaração de uma variável, podemos declarar vários apelidos com um único `typedef` : 

```c
//Ao criar uma variável :

//Cria um "char" chamado "a"
//Um "ponteiro para char" chamado "b"
//E um "array fixo de char" chamado "c"
char a, *b, c[10];

//De forma similar, ao usar typedef : 

//Cria um apelido para "char" chamado "ac"
//Cria um apelido para "ponteiro para char" chamado "bc"
//Cria um apelido para "array fixo de 10 chars" chamado "cc"
typedef char ac, *bc, cc[10];

//Agora é possível criar variáveis usando os apelidos!
ac v1; //char     chamado v1
bc v2; //char*    chamado v2
cc v3; //char[10] chamado v3
```

Ao perceber a similaridade entre declarações de variáveis e `typedef`, é dispensável explicações adicionais de como utilizar `typedef` com qualquer outra declaração mais complexa de tipos, pois o comportamento é exatamente o mesmo da declaração de uma variável.

Existe apenas uma única exceção desse comportamento do `typedef` onde é possível criar um apelido para um [array](./8-02-arrays.md) de tipo incompleto, que pode ser posteriormente completado : 
```c
typedef char caracteres[];

//usa um typedef de um tipo incompleto, que foi completado na declaração
caracteres vogais = {'a','e','i','o','u'};
```  

## Variáveis primitivas
Variáveis primitivas são todas variáveis que utilizam tipos que existem inerentemente na linguagem e não dependem da existência de outros tipos.

Pode-se dizer que as variáveis primitivas são os principais blocos para construção de variáveis com tipos compostos, que dependem da existência de outros tipos.

Logo entender bem elas é crucial para entender bem os outros tipos mais complexos.

Os subtópicos nas páginas seguintes detalharão cada tipo de variável primitiva.
