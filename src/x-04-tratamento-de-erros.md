# Tratamento de erros
Este capítulo busca ensinar as diferentes formas como podemos tratar erros, identificar que erros estão sendo propriamente tratados, bem como diferentes estratégias utilizadas para representar erros em projetos de renome.

Erros são geralmente representados por um valor que o identifica unicamente (geralmente um número inteiro similar a um `enum` ou um endereço único) opcionalmente associado a uma string que detalha o erro.

As formas como erros são repassados podem ser separadas em 4 categorias : 
- Estado global
- Retorno dedicado
- Valor sentinela
- Exceções

## Erros com Estado global
Neste padrão, utiliza-se uma variável externa, que representa o último erro ocorrido.

Inicialmente este padrão era implementado utilizando uma variável global, porém, devido a presença de sistemas com suporte a múltiplos threads, as implementações modernas desse modelo utilizam variáveis "thread local", onde existe uma cópia diferente da mesma variável para cada thread.

Este padrão é utilizado pela própria biblioteca padrão do C e especificação POSIX (através da variável `errno`), assim como também é usada pelas funções da API Win32 do Windows (utilizando as funções`GetLastError` e `SetLastError`), ou seja, ao tratar de funções de baixo nível do sistema ou a própria biblioteca padrão do C, é comum que tenhamos de lidar com esse tipo de tratamento de erro.

O comportamento mais comum é que a variável externa só seja modificada quando um erro ocorre, portanto, é geralmente útil ter algum valor indicando que um erro ocorreu, como é o caso da função `fopen` do C (onde ela retorna `NULL` em caso de erro), porém, em alguns casos, acontece um comportamento não ideal onde qualquer retorno é válido, como ocorre com a função `strtol`, nesses casos, é necessário "zerar o erro" antes de chamar a função.

```c
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    /*Neste caso, basta checar por "NULL" e temos certeza que há um erro */
    FILE *arquivo = fopen("test.txt", "rb");
    if(arquivo == NULL)
        printf("fopen falhou com erro %d\n", errno);
    
    /* strtol retorna 0 quando acontece um erro, mas 0 também é um retorno 
       válido  e quando strtol falha, a função não mexe no valor de "errno", 
       sendo necessário  zerar errno, antes de checar o valor para saber com 
       certeza se um erro ocorreu */
    errno = 0;
    long valor = strtol("abacate", NULL, 10);
    if(valor == 0 && errno != 0) 
        printf("strtol falhou com erro %d\n", errno);
}
```

Lembrando que essa checagem só funciona pois internamente `fopen` e `strtol` estão escrevendo em `errno`, portanto recomenda-se SEMPRE ler a documentação da função em específico e checar como erros são reportados antes de assumir que um jeito ou outro é utilizado.

O comportamento mais comum e o usado pela maioria das implementações modernas, é ter pelo menos um valor reservado para indicar sucesso/falha, para evitar o tipo de tratamento que precisamos fazer com `strtol` conforme o exemplo acima.

A biblioteca SDL tem uma variação interessante desse tipo de tratamento, todas as funções sinalizam de alguma forma um sucesso/falha, porém os erros são reportados através da função `SDL_SetError`, que tem sintaxe similar a `printf` (ou seja, prepara uma mensagem formatada) e a função `SDL_GetError` que recupera a última string definida.

O código abaixo demontra uma implementação "teórica" e "simplificada" que não reflete com fidelidade a implementação real dessas funções, mas que exemplifica como poderiamos construir algo similar a `SDL_SetError` e `SDL_GetError`:

```c
#include <stdarg.h>
#include <stdio.h>

static _Thread_local char SDL_errbuf[4096];

const char *SDL_GetError(void)
{
    return SDL_errbuf;
}

int SDL_SetError(const char *format, ...)
{
    va_list params;
    va_start(params, format);
    (void)vsnprintf(SDL_errbuf, sizeof(SDL_errbuf), format, params);
    va_end(params);
    return -1;
}
```

Também é comum que esses erros sejam associados a strings, a função `strerror` do C por exemplo, retorna uma string que descreve o erro em `errno`, assim como nas funções do Win32, exista a função `FormatMessageW`, que monta uma mensagem descrevendo o código de erro obtido por `GetLastError`.

Vantagens:
- Simplifica os argumentos das funções que usam esse tipo de tratamento de erro
- Permite uma sinalização detalhada do erro mesmo com apenas 1 valor reservado como "retorno inválido"
- Permite que o detalhamento do erro seja gerado dinâmicamente e reutilize buffers estáticos sem alocar memória, ao usar um padrão similar a `SDL_SetError`

Desvantagens:
- Menos eficiente do que o método de Valor Sentinela ou Retorno Dedicado
- Fácil de usar errado, pois chamar outras funções pode sobrescrever o último erro antes dele ser lido

## Erros com Retorno dedicado
Nesse caso, as funções tem um valor dedicado para o código de erro, que pode ser o retorno, ou um valor repassado via ponteiro (seguindo o padrão de usar ponteiros para múltiplos retornos em C).

É comum que o tipo usado para erros seja definido com `typedef` para simplificar a detecção pelo programador do parâmetro de erro.

O Windows usa esse padrão internamente em seu kernel, todas as funções `Nt` (que são funções que chamam diretamente o kernel) retornam um valor do tipo `NTSTATUS`, que é definido como :
```c
typedef long NTSTATUS; //No Windows, este "long" é sempre 32bits


/* Essa segunda definição de NTSTATUS não é "oficial", mas indica a representação interna
   usada nos códigos de erro com NSTATUS (conforme documentado pela Microsoft):
*/
struct NTSTATUS {
    unsigned int severity : 2;   /* 0 -> Sucesso, 1 -> Informação, 2 -> Aviso, 3 -> Erro */
    unsigned int c : 1;          /* 0 -> Microsoft, 1 -> Erro do usuário */
    unsigned int n : 1;          /* 0 -> Mapeável para HRESULT, 1 -> Não pode ser convertido */
    unsigned int facility : 12;  /* Especifica de onde o código se originou (USB, HID, IO, Hypervisor, etc) */
    unsigned int code : 16;      /* Código de erro efetivo */
};
```
Como o código de erro é dedicado, essa separação do código de erro em categorias e subcategorias é possível pois temos muito espaço para definir diferentes códigos de erro.

Exemplo simples utilizando o padrão para sinalizar divisão por zero : 
```c
#include <errno.h> //Utilizado pela constante "EDOM"

typedef errno_err int; 

/* Neste caso, utiliza-se o retorno para o código de erro 
   e o resultado é preenchido via ponteiro */
errno_err divide1(int a, int b, int *result) 
{
    if(b == 0)
        return EDOM;

    *result = a / b;
    return 0;
}

/* Contrário de "divide1", utiliza-se o retorno para resultado 
   e o erro é preenchido via ponteiro */
int divide2(int a, int b, errno_err *err) 
{
    if(b == 0) {
        *err = EDOM;
        return 0;
    } 
    *err = 0;
    return a/b;
}
```

As únicas funções que usam esse tipo de tratamento na biblioteca padrão do C, são as funções do Annex K com sufixo `_s` adicionadas no `C11` pela Microsoft (concidência? acho que não).

Vantagens:
- Eficiente pois passa o valor de erro diretamente
- Flexível, pois permite que o código de erro seja qualquer coisa (ponteiro para string ou struct, número)
- Diminui a chance do usuário ignorar o erro, pois o expõem como um parâmetro ou retorno explicito
- Também permite que funções onde qualquer resultado é válido a exporem erros
Desvantagens:
- Aumenta a complexidade das funções, exigindo adição de parâmetros extras

## Erros com Valores sentinelas
Este termo é mais comum no inglês (Sentinel Value), um valor "sentinela", é um valor especial usado para sinalizar um estado específico ao invés de dados efetivos. Neste caso estamos falando de reservar valores específicos do resultado para indicar erros.

Um dos maiores exemplos de valores sentinelas, são os próprios `NaN`, `+Infinity` e `-Infinity` que são valores especiais de ponto flutuante usados para representar situações erroneas.

Outro exemplo interessante, é que o próprio kernel do Linux utiliza valores sentinelas para representar erros, no Linux nenhuma chamada de sistema usa valores negativos como retorno válido, portanto, todas funções reportam erros retornando os mesmos códigos de erro usados em `errno` com sinal negativo (ex: retorna `-EIO` para `EIO`).

Mas você deve estar pensando "tá, mas e quando o retorno é um ponteiro?", "Para um ponteiro, apenas `NULL` é um valor inválido". Infelizmente, o kernel do Linux discorda de você, enquanto `NULL` pode até ser o único valor inválido em arquiteturas de 16bits ou 8bits, é comum que em arquiteturas de 32bits e 64bits, os maiores endereços (beirando ao limite do endereçamento virtual) não sejam acessíveis.

Os detalhes específicos de qual faixa especificamente pode ser usada é um detalhe específico de cada arquitetura mas é fato que isso acontece na maioria das arquiteturas e o Linux justamente se aproveita desse detalhe codificando erros dentro de ponteiros, caso tenha interesse em pesquisar, procure pelas macros `IS_ERR`, `PTR_ERR`, `IS_PTR_ERR` no [código fonte do kernel do Linux](https://github.com/torvalds/linux).

Vantagens:
- É a forma mais eficiente de representar erros
- Evita poluir ou aumentar complexidade de funções que retornam erros

Desvantagens:
- Não pode ser usado em casos onde há poucos ou nenhum resultado que pode ser considerado "inválido"

## Erros com Exceções
Exceções são mecanismos especiais que forçam o fluxo de execução a pular para um ponto onde o erro possa ser devidamente tratado. Se você realmente conhece a linguagem C, deve saber que ela não tem uma implementação "nativa" ou direta de exceções (mas fique tranquilo, pois também vamos falar de implementações que são possíveis em C).

Geralmente linguagens de alto nível, como o próprio C++, fornecem a funcionalidade de exceções usando as palavras chave `try` e `catch`, conforme exemplo: 
```cpp
//Lembre-se ESSE CÓDIGO não funciona em C!!!
try {
    //Código que será executado
} catch(const std::exception& erro) {
    //Código vai PULAR para cá, se ocorrer algum erro
    printf("Erro : %s\n", erro);
}
```

Exceções são uma funcionalidade presente na maioria das linguagens modernas, elas eliminam a necessidade de checar e repassar erros em todos os usos e afetam minimamente a performance nos casos onde erros "não acontecem", porém são extremamente ineficientes nos casos onde erros acontecem.

Recomenda-se que erros "esperados" ou "comuns" não sejam implementados como exceções devido ao alto custo de performance de lançar uma exceção. 

Porém fluxos que normalmente envolveriam muitas checagens de erros que são acontecem muito raramente provavelmente se beneficiariam do uso de exceções.

### setjmp e longjmp
A implementação mais "comum" em C que se assemelha ao tratamento usado em exceções é utilizando as funções `setjmp` e `longjmp` presentes na biblioteca `setjmp.h`.

A função `setjmp` salva o contexto atual do processador em uma variável do tipo `jmp_buf`, que pode posteriormente ser recuperada com a função `longjmp`, tendo a seguinte sintaxe:
```c
int setjmp(jmp_buf env);
```

O contexto do processador contêm informações sobre o estado de execução atual, incluindo endereço da instrução atual, posição na stack, valores em registradores, etc. Ou seja, restaurar o contexto significa voltar para o momento da execução de `setjmp`.

Variáveis locais não constantes que estão no escopo de `setjmp` devem ser marcadas como `volatile`, isso ocorre pois é necessário impedir que o compilador use um registrador para representar a variável (conflitando com o mecanismo de restaurar contexto).

O retorno de `setjmp` não pode ser colocado em uma variável, ele deve estar na expressão de condição em um `if`, `switch`, `while`, `do-while`,`for` ou chamado sem usar o retorno. O valor desse retorno é `0` na primeira chamada, e quando o contexto é restaurado via `longjmp`, o retorno tem valor igual ao parâmetro `status` passado a `longjmp`.

O exemplo abaixo mostra alguns usos válidos com `setjmp` :
```c
jmp_buf jmpbuf;

//1. Chamada separada que descarta o retorno
(void)setjmp(jmpbuf); 

//2. Com IF
if(setjmp(jmpbuf) == 0) {
    //Código
}

//3. Em um switch
switch(setjmp(jmpbuf)) {
case 0:
    //Código
    break;
case 1:
    //Código
    break;
}
```

A função `longjmp` é usada para voltar ao contexto de execução do parâmetro `env`, efetivamente voltando ao ponto onde `setjmp` foi chamado, onde `status` será o valor de retorno de `setjmp`, a sintaxe da função é a seguinte:
```c
//Até C11
void longjmp(jmp_buf env, int status);
//Até C23
_Noreturn void longjmp(jmp_buf env, int status);
//C23
[[noreturn]] void longjmp(jmp_buf env, int status);
```

Vale lembrar que quando passamos `0` para `status`, o valor se torna `1`, isso acontece pois o valor `0` é utilizado para diferenciar quando `setjmp` foi executada pela primeira vez, sem isso perderiamos uma garantia importante.

Exemplo do uso de `setjmp` e `longjmp` utilizando uma implementação própria de quicksort: 
```c
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

/* Usa a respectiva função do sistema para obtenção de bytes aleatórios */
#ifdef _WIN32
    #define RtlGenRandom SystemFunction036
    extern int __stdcall RtlGenRandom(void *buffer, unsigned long len);	

    static void fill_random(void *buf, size_t len) {
        RtlGenRandom(buf, (unsigned long)len);
    }
#else
    #include <sys/random.h>
    static void fill_random(void *buf, size_t len) {
        getrandom(buf, len, 0);
    }
#endif 

#define ARRAY_SIZE(X) (sizeof(X)/sizeof(*X))

#define SETJMP_TRY 0
#define XOR_SWAP(V1, V2) \
    do { \
        V1 ^= V2; \
        V2 ^= V1; \
        V1 ^= V2; \
    } while(0)

static jmp_buf jmpbuf;

int quicksort_partition(int arr[], int low, int high) {
    int p = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= p && i <= high - 1) 
            i++;
        
        while (arr[j] > p && j >= low + 1) 
            j--;
        
        if (i < j)
            XOR_SWAP(arr[i], arr[j]);
    }
    XOR_SWAP(arr[low], arr[j]);
    return j;
}

void quicksort(int *arr, int low, int high, int count)
{
    if(count > 65)
        longjmp(jmpbuf, 1); //Escapa, impedindo stack overflow
    if(low >= high)
        return;

    int pi = quicksort_partition(arr, low, high);

    quicksort(arr, low, pi - 1, count+1);
    quicksort(arr, pi + 1, high, count+1);
}
    
int main()
{
    static int rnd_nums[256ULL * 1024ULL * 1024ULL / sizeof(int)]; //256MB

    puts("Preenchendo array de 256MB com valores aleatorios!");
    fill_random(rnd_nums, sizeof(rnd_nums));

    switch(setjmp(jmpbuf)) {
    case SETJMP_TRY:
        puts("Array preenchido, ordenando!");
        quicksort(rnd_nums, 0, ARRAY_SIZE(rnd_nums)-1, 0);
        puts("Array ordenado com sucesso!");
        break;
    case 1:
        puts("Call stack muito grande!");
        break;
    }
}
```

No exemplo acima, os valores foram "exagerados", tanto a checagem de `count` (valor muito abaixo do que realmente causaria um stack overflow) quanto o tamanho do array de entrada que é bem grande, mas ele demonstra um caso real da aplicação, tratamento de erro em funções recursivas (onde seria necessário repassar o erro e possivelmente introduzir checagens que degradariam a performance)

Vale lembrar que tem várias situações onde usar o mecanismo de `setjmp` e `longjmp` é problemático:
- É comportamento indefinido quando um thread chama `longjmp` para restaurar um contexto salvo por outro thread em `setjmp`
- Quando há alocação de recursos entre a chamada de `setjmp` e `longjmp`, facilmente levando a vazamento de memória
- Ao usar `longjmp`, arrays de tamanho variável não são limpos, levando a um possível vazamento de memória

O mecanismo de `setjmp` e `longjmp` é diferente do utilizado por linguagens alto nível para exceções. O uso de `setjmp` é mais custoso do que o equivalente do bloco `try` (pois é necessário salvar todo contexto de execução), porém, o uso de `longjmp` é muito mais eficiente comparado aos casos onde a exceção é lançada.

### Exceções baseadas em meta-dados
Essa é a forma utilizada por linguagens de alto nível, mas também é um tipo de tratamento que é muito difícil ou impossível de realizar somente em C, sem depender de extensões de compiladores, assembly inline e/ou ferramentas externas por conta da falta de um mecanismo que forcem o compilador a gerar esses metadados.

Basicamente, quando escrevemos um bloco `try`, em uma linguagem como C++, estamos especificando que todas as operações de lançar exceções (`throw`) dentro daquele bloco, devem pular para o endereço especificado em `catch` e para que isso funcione, metadados adicionais são colocados no executável, para que seja possível voltar na call stack até o `catch`.

Para realizar essa tarefa, compiladores de C++ utilizam a bibliotecas como [`libunwind`](https://github.com/libunwind/libunwind), para implementar a funcionalidade de voltar pela pilha de chamadas (call stack), segundo a [ABI de exceções do C++](https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#base-abi).


### Vantagens e desvantagens de exceções

Vantagens:
- Evita poluir ou compilar funções com códigos de erro ou usar estado global
- Evita a necessidade de ficar repassando um código de erro entre múltiplas chamadas de função
- Custo mínimo quando uma exceção não é lançada (exceto nos casos onde usamos `setjmp`/`longjmp`)

Desvantagens:
- A mera possibilidade de presença de exceções aumenta muito a imprevisibilidade do código
- [É dificil de identificar propriamente um código que usa exceções de forma incorreta](https://devblogs.microsoft.com/oldnewthing/20050114-00/?p=36693)
- Lançar uma exceção é geralmente muito ineficiente (não tão ineficiente quando usamos `setjmp`/`longjmp`)