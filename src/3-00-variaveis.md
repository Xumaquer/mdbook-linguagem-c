# Variaveis
Variaveis são, efetivamente, lugares onde valores podem ser lidos ou escritos. 

No geral são identificadas unicamente por nomes que definimos no nosso programa, junto com uma segunda descrição que chamamos de "tipo".

Os tipos de variaveis são nomes predefinidos, que formam uma descrição "do que está sendo guardado", que ajuda a descrever que tipo de operação deve ser feita para poder ler e escrever na variavel que é de um determinado tipo.

Variaveis podem ser utilizadas para descrever todo tipo de dado, e idealmente, devem ter um nome que simplifica o entendimento do seu propósito e uso de forma a facilitar a leitura do código.

A linguagem C diferencia entre nomes em letra maiúscula e letra minúscula, logo `variavel`, `VARIAVEL` e `varIAVEL` são considerados nomes diferentes.

> Quanto a variaveis, podemos fazer uma analogia simples mas efetiva :
>
> Considere agora, que "variaveis" são caixas, geralmente usamos caixas para guardar coisas.
>
> Porém, nem toda caixa tem o mesmo tamanho ou formato, existem vários `tipos` de caixas, para propósitos
> diferentes.
> 
> Se tivermos muitas coisas em caixas, fica difícil saber onde fica cada coisa, para isso, geralmente colocamos
> etiquetas com `nomes`, pois isso facilita a organização e o entendimento do propósito de cada caixa.
>
> De certa forma, `ler` de uma variavel é similar ao processo de retirar algo de uma caixa
> da mesma forma que `escrever` em uma variavel, é similar ao processo de colocar algo em
> uma caixa.

## Modificadores de tipo
Ao declarar variaveis podemos adicionar modificadores, que servem para especificar detalhes adicionais sobre a variavel e seu tipo.

| Modificador | Descrição                                                   |
| ----------- | ----------------------------------------------------------- | 
| `const`     | Define que a variavel não pode mudar seu valor              | 
| `constexpr` | Define que a variavel é uma constante de compilação (`C23`) |
| `restrict`  | Indica que o ponteiro não sofre de aliasing                 |               
| `volatile`  | Impede reordenamento e remoção de leitura e escrita         |                
| `signed`    | Usado para definir um inteiro com sinal                     |
| `unsigned`  | Usado para definir um inteiro sem sinal                     |


### Const
O modificador `const` faz com que o valor não possa ser modificado após sua definição.

Utilizar `const` ao definir variaveis ajuda a delimitar seu uso e evitar modificações desnecessárias e dependendo do caso pode permitir ou facilitar mais otimizações por parte do compilador ao delimitar melhor como valores serão utilizados.

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
O modificador `constexpr` foi adicionado apenas no `C23` e indica que a variavel é uma constante de compilação.

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
O modificador `volatile` indica que leituras e escritas a variavel não podem ser reordenadas, removidas por otimização ou lidas de cache.

A necessidade de uso desse modificador geralmente se da quando um valor pode ser modificado de forma imprevisível, ou serve para interfacear ou controlar hardware (controlar saídas, comunicar com HD, placa de rede, tela, GPU, etc).

Existem alguns motivos pelo qual esse modificador pode ser útil: 
- Quando a variavel pode ser diretamente modificada pelo hardware, ou o valor dela tem influência sobre o hardware
- Quando a variável faz parte de uma memória compartilhada e pode ser modificada por outros processos
- Quando há outros threads que podem modificar a variável
- Quando existe alguma função que modifica a variavel e pode ser chamada externamente (como chamadas de procedimento remoto, ou os [signals](https://man7.org/linux/man-pages/man7/signal.7.html) do POSIX que são geralmente chamados pelo próprio kernel)


### Signed
Usado como modificador para declarar variáveis de tipos inteiros com sinal (ou seja, que podem ter valores negativos).

Na maioria dos casos, não há necessidade de declarar `signed` explicitamente, pois o padrão da maioria dos tipos inteiros já são inteiros com sinais, com uma única exceção do tipo `char`, que por padrão pode ser `unsigned`ou `signed`, uma escolha que é definida pela implementação da linguagem pelos compiladores.

> A única necessidade real de escrever `signed` é para utilização do tipo `signed char`.

### Unsigned
Usada como modificador para declarar variáveis de tipos inteiros com sinal (ou seja, que não podem ter valores negativos).

Inteiros sem sinal tem certas vantagens em relação aos tipos com sinal, no geral por não incluir valores negativos, os tipos sem sinal geralmente conseguem guardar valores até duas vezes maiores do que o valor máximo de uma variável com sinal de tamanho equivalente.

No geral os tipos sem sinal garantem um comportamento circular em caso de overflow (ultrapassar o limite da variavel), onde elas "dão a volta" e retornam ao valor 0, além disso o comportamento para deslocamento é sempre definido, enquanto que para valores com sinal, deslocar para esquerda os bits de um valor negativo é indefinido.

```c
signed char a;   //char com sinal (geralmente -128 a 127)
unsigned char b; //char sem sinal (geralmente 0 a 255)

int c;          //int com sinal
unsigned int d; //int sem sinal

long long e;          //long long com sinal
unsigned long long f; //long long sem sinal


//Valor máximo de um "unsigned char"
unsigned char teste = UCHAR_MAX;

//Nesse caso, da overflow e resulta em 0 (isso é garantido pela linguagem)
teste = teste + 1;
```

## Modificadores de armazenamento
Os modificadores de armazenamento podem ser utilizados para modificar variáveis ou funções e definem tanto a duração (apenas para variáveis) e vinculação (para ambos).

A duração, neste caso, é o escopo no qual a variável vai continuar existindo, os tipos de duração existentes são : 
- `automática`: A variável é armazenada quando o bloco inicia e desalocada quando o bloco é encerrado, isso se aplica a variaveis locais e parâmetros de funções.
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
| `auto`         | Duração `estática` e `sem vinculação`                       | 
| `register`     | Dica para que o compilador guarde a variavel em registrador |
| `static`       | Duração `estática` e vinculação `interna`                   |               
| `extern`       | Duração `estática` e vinculação `externa`                   |  
| `thread_local` | Duração de `thread` (`C11`)                                 |               

O padrão de cada tipo é : 
- variavel local : duração `automática` e `sem vinculação`
- variavei globais : vinculação `externa`
- funções : vinculação `externa`

### auto 

O modificador `auto` é considerado inútil, pois só pode ser aplicado a variaveis locais e já é o comportamento padrão para elas.

### register

O modificador `register` é utilizado para indicar que uma variavel deve ser diretamente guardada apenas em registrador, isto é, nas "mãos" do processador, utilizadas para operar com valores, e portanto, o endereço da variável não pode ser acessado.

Antigamente o modificador `register` era útil, ajudando nas otimizações ao fornecer dicas ao compilador, mas hoje em dia, com o compiladores super inteligentes, eles acabam tendo muito mais informação do que nós sobre uma variedade de parâmetros ao decidir sobre otimizações.

### static
O modificador `static` tem dois usos, ele pode ser usado dentro de blocos para definir variaveis que só podem ser acessadas no bloco mas tem duração `estática`, ou seja, mantêm o valor entre execuções da função e são efetivamente guardadas no mesmo lugar que variáveis globais seriam.

Seu segundo uso é para definir que funções e variáveis globais não afetem outras unidades de tradução, evitando conflitos de nomes e facilitando algumas otimizações (ao garantir que algo nunca vai ser usado externamente, algumas otimizações adicionais se tornam possíveis).

### extern
O modificador `extern` normalmente não precisa ser usado em funções, pois já é o padrão.

Já em variáveis, o modificador funciona como uma "importação" de uma variável já existente, permitindo que ela seja usada no código, mesmo se ela estiver definida em outra unidade de tradução, mas ocasionando em erro caso não nenhuma unidade de tradução tenha a implementação da variável global.

```c
    //Caso 1, erro ao compilar, pois ninguém definiu 
    //a variavel sem "extern"
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

## Variaveis primitivas
Variaveis primitivas são todas variaveis que utilizam tipos que existem inerentemente na linguagem e não dependem da existência de outros tipos.

Pode-se dizer que as variaveis primitivas são os principais blocos para construção de variaveis com tipos derivados, que dependem da existência de outros tipos.

Logo entender bem elas é crucial para entender bem os outros tipos mais complexos.

Os subtópicos nas páginas seguintes detalharão cada tipo de variavel primitiva.
