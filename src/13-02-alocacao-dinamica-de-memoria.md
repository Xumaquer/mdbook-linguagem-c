# Alocação Dinâmica de Memória
Como exemplificado no capítulo sobre [layout de memória](./13-01-layout-de-memoria.md), um programa pode alocar memória dinâmicamente, e geralmente chamamos toda região de memória alocada dessa forma de "heap".

A linguagem C fornece, através da sua biblioteca padrão e expõe pelo header `stdlib.h` funções para realizar o gerencimento manual dessa memória, fornecendo funções que alocam, realocam e liberam memória dinamicamente.

Quando alocamos memória, estamos efetivamente reservando uma quantidade específica de bytes para algum uso específico.

## Funções para alocação
Para realizar a alocação de memória, existem 3 funções da biblioteca padrão.

### malloc
`malloc` é a principal função para alocação de memória, e tem a seguinte sintaxe : 
```c
void *malloc(size_t tamanho);
```

Onde `tamanho` é a quantidade de bytes que devem ser alocados e o retorno é um `void*` indicando o endereço da memória alocado, ou `NULL` caso a alocação falhe.

A memória alocada com `malloc` não tem nenhuma garantia quanto ao seu conteúdo inicial, é comum que seu conteúdo seja similar ao de uma variável não inicializada da stack.

O alinhamento da memória alocada é o maior alinhamento que qualquer tipo possa ter, equivalente ao alinhamento do tipo `max_align_t` do `C11`.

Essa memória posteriormente pode ser liberada utilizando `free` ou alguma das outras funções de liberar memória da `stdlib.h` que serão mencionadas mais a frente.

Utilizar `0` como `tamanho` pode, dependendo da implementação, retornar `NULL` ou um ponteiro válido que não pode ser acessado mas ainda deve ser liberado.

Acessar memória além do que foi alocado gera um comportamento indefinido, que pode sobrescrever/ler outras variáveis ou causar uma finalização forçada do programa pelo sistema operacional.

Exemplo de uso da função malloc:
```c
int main()
{
    //Aloca memória para 10 inteiros
    int *quadradosPerfeitos = malloc(sizeof(int) * 10);

    for(int i = 0; i < 10; i++)
        quadradosPerfeitos[i] = (i+1) * (i+1);
    
    //Libera a memória utilizada
    free(quadradosPerfeitos);
}
```

É comum que ao alocar memória, o sistema operacional simplesmente "marque" que uma região do endereço virtual de um processo é válido, sem efetivamente reservar memória física para ele.

Quando o usuário acessa (tenta ler/escrever no endereço alocado) ocorre um erro interno no sistema denominado de "falha de página", isso faz com que uma função do kernel seja chamada para lidar com o erro, essa função efetivamente prepara essa página de memória, e retorna o fluxo de execução ao programa do usuário.

Esse mecanismo faz com que a memória só seja efetivamente consumida, após ser utilizada pela primeira vez.

### calloc
Assim como a função `malloc` existe a função `calloc` que pode ser utilizada para alocar memória onde todos os bytes são zero.

A sintaxe para uso de `calloc` é : 
```c
void *calloc(size_t num, size_t tamanho);
```

Apesar de ter 2 parâmetros, o tamanho final da alocação é efetivamente a multiplicação `num * tamanho` e o comportamento é exatamente o mesmo de `malloc`, exceto pela memória alocada ser inicializada com `0`.

Lembrando que ponteiros nulos não necessariamente são compostos por bytes zerados, apesar desse ser o comportamento padrão em boa parte se não de todos sistemas modernos.

A motivação pelo uso de dois parâmetros é histórica e não tem mais um propósito efetivo nos dias de hoje.

Alguns guias e autores erroneamente falam que utilizar `calloc` é efetivamente igual a chamar `malloc` + `memset` para zerar a memória, mas essa constatação está errada.

Devido ao mecanismo de muitos sistemas operacionais de esperar o primeiro acesso para realmente alocar, uma alocação realizada com `calloc` pode tirar proveito disso e ser muito mais rápida sem consumir memória até seu primeiro uso.

### aligned_alloc
A função `aligned_alloc` foi introduzida no `C11`, seu propósito é realizar uma alocação similar a `malloc` porém controlando o alinhamento de memória.

```c
void *aligned_alloc(size_t alinhamento, size_t tamanho);
```

A função tem comportamento extremamente similar a `malloc`, porém utilizando o alinhamento definido pela função.

Podemos dizer que `malloc` é efetivamente equivalente a uma chamada de `aligned_alloc` da seguinte forma:
```c
void *ptr = aligned_alloc(_Alignof(max_align_t), tamanho);
```

## Realocação de memória
Quando há necessidade de expandir ou diminuir o uso de memória de uma alocação feita anteriormente, utilizamos a função `realloc`, que tem a seguinte sintaxe:

```c
void *realloc(void *ptr, size_t tamanho);
```
`ptr` é o ponteiro para a memória previamente alocada com `malloc`, `calloc` ou `aligned_alloc` ou `NULL`.

`tamanho` é o novo tamanho da memória realocada, não recomenda-se passar `0` como tamanho pois o comportamento para tal é definido pela implementação e desde o `C23` se tornou indefinido (permitindo que implementações tratem esse caso como bug e façam qualquer coisa).

A função retorna o novo bloco realocado, ou `NULL` caso haja uma falha em realocar, nesse caso o bloco `ptr` ainda é válido e não é liberado por `realloc`.

A ideia por trás do `realloc` é que existem dois comportamentos possíveis ao realizar uma realocação :
- A região já alocada por `ptr` é expandida ou contraida, se possível. os conteúdos da região antiga que estará contida na região nova são mantidos. 
- Uma nova região é alocada, o conteúdo da região antiga é copiado para ela e o bloco antigo é liberado.

Lembrando que caso o segundo comportamento seja escolhido devido a uma falha em expandir/contrair a região atual, será necessário ter, mesmo que temporariamente, memória o suficiente para a existências das duas regiões de memória (a antiga e a nova) para que seja possível copiar os conteúdos da região antiga para a nova.

Também é comum que algumas implementações utilizem o seguinte código ao realizar uma realocação : 
```c
ptr = realloc(ptr, tamanho);
``` 

Isso não é exatamente ideal, pois no caso da alocação falhar, teremos efetivamente perdido o valor do endereço da região original, chamamos essa "perda de referência" de vazamento de memória.

Uma forma ideal de utilizar `realloc` lidando com erros corretamente seria : 
```c
void *novoPtr = realloc(ptr, novoTamanho);
if(novoPtr != NULL) {
    ptr = novoPtr;
    tamanho = novoTamanho;
} else {
    fputs(stderr, "Erro ao realocar memória no arquivo %s e próximo a linha %d\n",
            __FILE__, __LINE__);
}
```

## Funções para liberar memória
Para liberar o uso de memória, existem 3 funções diferentes.

### free
`free` é a principal função para liberar o uso de memória pelas funções de alocação e realocação, até o `C23` era a única função para tal.

A sintaxe para uso da função é extremamente simples : 
```c
void free(void *ptr);
```

`ptr` é o ponteiro para o ínicio do bloco alocado com qualquer uma das funções de alocação/realocação da biblioteca padrão do C.

Se `ptr` for um ponteiro nulo, a função `free` não faz nada.

Ao utilizar `free`, a memória alocada pelas funções é marcada como inultilizável, permitindo que ela seja devolvida ao sistema operacional ou reutilizada em futuras chamadas para as funções de alocação.

O comportamento de `free` é indefinido se a memória de `ptr` não foi alocada com alguma das funções de alocação da biblioteca padrão do C ou já foi inultilizada por uma chamada anterior a `free`.

Os erros causados por chamar `free` duas vezes são considerados erros comuns de programação em C.

Muitos tendem a anular o ponteiro após uma chamada a `free`, impossibilitando que esse erro aconteça por descuido:
```c
free(ptr);
ptr = NULL;
```

Também é comum que programadores reforçem a necessidade de chamar `free` com toda memória alocada pelas funções da biblioteca do C, isso será discutido com detalhes mais frente.

### free_sized
No `C23`, a função `free_sized` foi adicionada como uma forma adicional de liberar a memória e funciona com todas funções de alocação/realocações exceto `aligned_alloc`.

A função apresenta um parâmetro extra especificando o tamanho da alocação original : 
```c
    void free_sized(void *ptr, size_t tamanho);
```

Onde `tamanho` seria o tamanho utilizado para previamente realizar a alocação.

Existem duas possíveis vantagens/motivações ao utilizar `free_sized` que podem ser flexibilizadas dependendo da implementação efetiva:
- Prover uma segurança maior, onde uma diferença entre o tamanho esperado/real permita que a implementação encerre o programa, ajudando a identificar o problema (a função não tem retorno, portanto essa é a única alternativa)
- Melhorar a performance, permitindo que a memória seja liberada sem necessidade de buscar pelo seu tamanho, estima-se que a performance das implementações pode subir em até 30% eliminando essa etapa

É importante lembrar que a decisão final do que efetivamente será feito com essa nova função depende das implementações da biblioteca do C, algumas podem decidir focar mais no aspecto de segurança, outras no de performance.

Inclusive nenhuma implementação é obrigada a se beneficiar das vantagens dessa nova função, sendo ainda válido implementar `free_sized` como : 
```c
void free_sized(void *ptr, size_t /*tamanho*/)
{
    free(ptr);
}
```

### free_aligned_sized
Similar a função `free_sized`, também introduzida no `C23`, porém preparada para lidar com `alloc_aligned`.

A sintaxe para função é muito similar a `free_sized`, sendo necessário também informar o alinhamento utilizado em `alloc_aligned` :
```c
void free_aligned_sized(void* ptr, size_t alinhamento, size_t tamanho);
```

Todas as vantagens e explicações a cerca de `free_sized` também se aplicam a função `free_aligned_sized`.


## Fragmentação de memória
Um problema que pode ocorrer ao realizar muitas alocações é a fragmentação da memória, dizemos que a memória está fragmentada quando os blocos de memória livre estão muito separados.

Em alguns casos mesmo que o sistema tenha memória disponível para realizar uma alocação, ela ainda pode falhar devido a fragmentação de memória.

A fragmentação de memória percebida nos processos geralmente não é uma fragmentação da memória física, mas sim uma fragmentação da memória virtual.

É possível por exemplo, que um programa 32bits falhe em alocar memória devido a fragmentação de seu endereçamento virtual, mesmo que ele esteja executando em um sistema operacional 64bits com memória RAM sobrando.

Algumas linguagens que apresentam gerenciamento automático de memória geralmente tem suas variáveis alocadas na heap reorganizadas na memória para evitar a fragmentação, isso fica mais evidente ainda quando linguagens como C# tem palavras chave como [`fixed`](https://learn.microsoft.com/pt-br/dotnet/csharp/language-reference/statements/fixed) que impedem o runtime da linguagem de mudar o endereço de uma variável.

## Quanto a necessidade de liberar memória
É necessário ter muito cuidado quando alocamos memória, é sempre bom pensar muito sobre o periodo em que aquela memória precisa se manter alocada e quais as condições para que ela deixe de ser utilizada e seja liberada.

Recomenda-se sempre liberar a memória alocada nos casos onde ela não será utilizada durante toda execução do programa, principalmente quando o programa tem uma vida útil longa.

Dessa forma, é comum que muitos autores, guias e programadores em C relembrem constantemente a necessidade de sempre chamar a função `free` e liberar toda memória em uso antes da finalização do programa.

Porém existem casos onde liberar a memória não é exatamente necessário, apenas considerada como uma boa prática.

Nos sistemas operacionais modernos, quando alocamos memória estamos efetivamente criando novas páginas na tabela de páginas do sistema, quando um processo finaliza, é responsabilidade do kernel recuperar toda memória alocada pelo processo e isso ocorre de maneira automática.

A recuperação da memória virtual pelo kernel ocorre de página em página de memória, que geralmente tem tamanho de 4KB, essa liberação é muito mais simples e eficiente do que o trabalho necessário para liberar memória pela função `free`.

Porém em sistemas RTOS, bare metal ou sistemas operacionais antigos e primitivos pode não existir efetivamente uma memória virtual ou um sistema que libere a memória do processo automáticamente na finalização.

Situações e pontos a favor de liberar sempre a memória:
- Para iniciantes, obter a prática de sempre liberar memória é extremamente benéfica
- Em praticamente todos os casos onde a memória não vai mais ser utilizada, liberar a memória é a coisa certa a se fazer, pois estamos permitindo que outras alocações e outros programas utilizem essa memória.
- Lida melhor com sistemas antigos ou com recursos limitados, que podem não desalocar a memória após a finalização do programa
- Não liberar memória pode gerar "falsos positivos" em ferramentas como [`valgrind`](https://valgrind.org/) que detectam vazamentos de memória, visto que não liberar é um "vazamento intencional"

Considerações extras (considerando um sistema operacional moderno):
- Liberar a memória com `free` é mais lento e necessita de mais trabalho do que a finalização automática realizada pelo sistema
- Não há uma necessidade real de liberar memória logo antes de finalizar um programa
- Podemos chamar a função `exit` ou similares e finalizar previamente um programa sem medo de "vazar memória do sistema"

## Erros comuns e causas ao gerenciar memória
Gerenciamento de memória é um assunto relativamente simples, mas que continua sendo uma das maiores causas de vulnerabilidades e defeitos em programas.

Por isso, é bom evidenciarmos os problemas que podem acontecer, as possíveis causas e como podemos evitar esses problemas.

### Vazamento de memória
Um vazamento de memória ocorre quando perdemos o valor do ponteiro de uma região de memória previamente alocada, de forma que não seja mais possível liberar a memória.

Possíveis causas : 
- Alocar uma `struct` que guarda ponteiros para outras alocações e esquecer de desalocar a memória deles antes de desalocar a estrutura
- Sobrescrever um ponteiro de uma alocação antiga com um ponteiro de uma alocação nova, sem que a região antiga tenha sido liberada.
- Uso incorreto de `realloc` (esquecendo de manter o ponteiro em outra variável, como listado anteriormente)
- Utilizar funções de bibliotecas externas de maneira incorreta, sem respeitar as recomendações da documentação
- Funções ou bibliotecas externas que tem um vazamento internamente

Um dos sintomas que podem indicar um vazamento de memória, é que o uso de memória sempre cresça quando realizamos uma sequência de etapas que deveria alocar e desalocar um ou mais recursos. (ex: abrir uma janela, realizar uma mesma ação e fechar ela, depois repetir o processo).

Podemos evitar vazamentos de memória de várias formas : 
- Utilizando ferramentas de detecção especializadas como [`valgrind`](https://valgrind.org/) (o valgrind especificamente só funciona no linux, alguns projetos usam wine para testar aplicações de windows no linux com valgrind)
- Criar um header que substitui as chamadas de `malloc`, `free` e afins por macros que formam uma lista com `endereço alocado`, `nome do arquivo` e `número da linha`, removendo-a da lista na chamada de `free` e reportando todos os vazamentos na finalização utilizando a função `atexit`, como faz o projeto [`Leaky`](https://github.com/itsYakub/Leaky/?tab=readme-ov-file) no Github.

### Liberação Dupla
Após liberar a memória, chamar outra função de liberar memória causa comportamento indefinido, como já foi descrito na explicação sobre `free`.

Possíveis causas : 
- O ponteiro é compartilhado e usado em mais de uma variável, portanto uma parcela do código não está ciente que a memória foi liberada
- O código não lida ou não marca corretamente que a variável foi desalocada e acaba tentando desalocar ela novamente
- Uso incorreto de bibliotecas externas, apesar de um pouco mais incomum

Podemos resolver ou detectar problemas de liberação dupla :
- Simplesmente utilizando um debugger, pois esses problemas geralmente finalizam o programa de forma inesperada e podem ser diagnosticado, analisando o valores das variáveis e a pilha de chamadas durante o acontecido
- Utilizando uma implementação similar ao [`Leaky`](https://github.com/itsYakub/Leaky/?tab=readme-ov-file) mencionado anteriormente, porém checando se os blocos alocados já foram liberados e reportando esse erro, o que permitiria um diagnóstico
- Utilizando [`valgrind`](https://valgrind.org/)
- Anulando as variáveis que carregam o ponteiro após liberar a memória pela primeira vez (que de preferência, seja apenas uma)
- No caso de múltiplas variáveis referenciando o mesmo ponteiro, podemos utilizar um contador de referência e liberar a memória quando a contagem chegar a `0`

### Uso após free
Após liberarmos uma região alocada, utilizá-la novamente é efetivamente um erro e causa comportamento indefinido.

As causas do uso após `free` são extremamente similares as causas da `Liberação Dupla` pois ambos remetem a um desconhecimento quanto a primeira liberação da região alocada.

A diferença maior é que o erro não acontece em uma chamada de função que poderia ser subtituida por macros e sim num acesso genérico direto ou indireto de uma região de memória, dificultando o diagnóstico.

Podemos resolver ou detectar problemas de uso após `free`:
- Combinando a prática de zerar os ponteiros após `free` junto de checagens constantes de ponteiro nulo em pontos chave do código
- Utilizando debuggers para detectar a causa do erro e resolver
- Utilizando contadores de referência e checando se a contagem é zero
- Utilizando [`valgrind`](https://valgrind.org/)

## Sobre uso de alocadores no windows
É comum que no windows, alguns projetos se recusem a usar `malloc`/`free`, pois preferem utilizar as funções `HeapAlloc` e `HeapFree`, entre eles a linguagem Rust, pelos seguintes motivos : 

- A implementação do `malloc` da biblioteca padrão do C utiliza `HeapAlloc`, então ela é apenas uma camada adicional em cima dessas mesmas funções
- O runtime padrão do C usa sua própria heap (`__crtheap`) no lugar da heap do processo obtida com `GetProcessHeap`, aumentando a fragmentação de memória pela mistura dos dois tipos de alocação visto que a heap do processo é usada por funções do sistema.
- Utilizar essas funções permite que um ponteiro em uma DLL seja liberado em outra, usar `malloc`/`free` nesse caso leva a comportamento indefinido

## Como outras linguagens gerenciam memória
Outro ponto que ajuda a compreeender melhor, é entender como outras implementações de linguagens efetivamente gerenciam a memória ou mesmo quais ferramentas elas oferecem para ajudar nisso.

- `Python`, `Javascript`, `Java`, `C#`: Utilizam o que chamamos de Garbage Collector (no português, coletor de lixo), que periodicamente pausa a execução e realiza uma análise verificando as variáveis da heap em uso, liberando o que não é mais utilizado, efetivamente removendo a necessidade do programador de se preocupar com isso.
- `C++`: Muitas classes utilizam o conceito de RAII (Resource Acquisition Is Initialization, no português "aquisição de recurso é inicialização"), onde a criação de uma variável aloca memória e a destruição dela desaloca a própria memória, isso também pode ser feito de maneira individual com os ponteiros inteligentes `std::unique_ptr` e `std::shared_ptr` (`shared_ptr` usa um contador de referência e `unique_ptr` libera a memória ao sair do escopo).
- `Rust`: No Rust o comportamento padrão é que alocações de memória da heap tem sempre uma variável que é "dona" da memória e somente quando ela deixa o escopo, a memória é liberada, o Rust tem suas próprias regras que mantêm esse comportamento sempre consistente e gera erros quando usado de forma incorreta, além de ter seus próprios tipos de ponteiros inteligentes como `Box` e `Rc` (equivalentes a `unique_ptr` e `shared_ptr` do C++ respectivamente).
- `Swift`: A linguagem Swift utiliza contadores de referência para todas as variáveis alocadas na heap, isso garante que ela libere a memória apenas quando não houver mais nenhuma variável referenciando o valor.

Quanto as implicações da estratégias utilizadas pelas linguagens:
- Utilizar garbage collector é mais custoso em relação a performance, porém elimina totalmente as preocupações quanto ao gerenciamento de memória.
- Utilizar contagem de referência para todas variáveis alocadas dinâmicamente é geralmente mais eficiente do que utilizar um garbage collector,
- O método do Rust e do C++ são as formas ideais, onde temos abstrações de "custo zero" para desalocar memória, o único problema é que elas introduzem complexidade adicional na escrita do código.

## Uso de memória dinâmica em sistemas embarcados
Em sistemas embarcados, é normalmente desaconselhado o uso de memória dinâmica por vários motivos : 
- Mal uso de alocação dinâmica é uma grande causa de vários erros, erros que muitas vezes não deveriam ocorrer nunca em sistemas embarcados que desempenham tarefas críticas
- O tempo para execução de `malloc`, `free` e similares não é deterministico, que podem ter um impacto significativo na performance e afetar o tempo de tarefas críticas
- É comum que apenas um programa seja executado num sistema embarcado, não existe outros programas competindo pela memória disponível
- Usar apenas memória estática (com variáveis globais ou `static` em funções), simplifica a detecção e análise de problemas relacionados ao uso de memória

Algumas dessas regras e outras relacionadas ao desenvolvimento de código crítico para embarcados em C pode ser visto nos documentos da [`NASA`](https://www.cs.otago.ac.nz/cosc345/resources/nasa-10-rules.pdf) e [`MISRA C`](https://www.mathworks.com/help/bugfinder/misra-c-2023-reference.html?s_tid=CRUX_lftnav).

## Memória dinâmica vs Memória estática
Apesar dos problemas mencionados no uso de memória dinâmica em sistemas embarcados, existem ainda diversas vantagens no uso de memória dinâmica e alguns outros pontos negativos não mencionados.

Vantagens ao usar memória dinâmica : 
- Utilizar memória dinâmica permite que a memória seja utilizada sob demanda, apenas quando é realmente necessária, sendo ideal no caso onde múltiplos programas precisam compartilhar o uso da memória física, como ocorre na maioria dos sistemas operacionais
- É muito mais fácil separar e implementar código de bibliotecas para que a memória seja utilizada e preparada apenas quando solicitada, inclusive repassando a responsabilidade de gerenciar a memória ao chamador da função
- Permite também o uso da heap como memória temporária, quando a memória da stack não é grande o suficiente
- Não há necessidade de definir limites fixos para tudo, listas e arrays podem crescer desde que haja memória para tal

Desvantagens ao usar memória dinâmica:
- Tempo de execução aumentado para alocar e liberar memória
- Mal uso de alocação de memória é fonte de diversos bugs e vulnerabilidades
- Memória obrigatóriamente precisa ser acessada de forma indireta via ponteiro, introduzindo um pequeno custo de performance no acesso

## Alocadores Arena
O alocador arena é um alocador extremamente simples, que usa a mesma estratégia da memória stack para alocações.

A vantagem do alocador arena é que ele é muito mais eficiente e a alocação de memória/desalocação é extremamente rápida e trivial (visto que basta avançar/recuar um ponteiro).

Porém, o alocador arena é obrigado a desalocar começando pelos últimos elementos alocados, seguindo a ordem de LIFO exatamente igual a uma pilha.

Utilizar um alocador arena também pode ser benéfico para diminuir a complexidade ao gerenciar memória manualmente, ao invés de gerenciar múltiplas alocações individuais, é mais simples realizar múltiplas deslocações de uma vez em um único ponto.

É possível implementar um alocador arena utilizando `malloc` e `free`, mas é muito mais eficiente utilizar [`mmap`](https://man7.org/linux/man-pages/man2/mmap.2.html) no linux/macOs, [`VirtualAlloc`](https://learn.microsoft.com/pt-br/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc) no windows ou até mesmo uma função própria de alocação de algum outro sistema.

Existe também um truque que pode ser feito ao combinarmos a capacidade de funções como [`mmap`](https://man7.org/linux/man-pages/man2/mmap.2.html) e [`VirtualAlloc`](https://learn.microsoft.com/pt-br/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc) de reservar endereçamento de memória sem alocar junta da disponibilidade de endereçamento de memória presente em aplicações 64bits (onde um ponteiro permitiria até 16 exabytes de endereçamento) para reservar espaços enormes e permitir realocações que expandam/contraiam o bloco de memória virtual de forma garantida.

<details>
  <summary>Exemplo de implementação do alocador arena em 64bits no linux</summary>

```c
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096 //4kb
#define RESERVED_SIZE (1 << 35) //32GB

struct Arena {
    size_t commited;
    size_t size;
    void *base;
};

static inline size_t round_to_page(size_t value)
{
    size_t round_value = (~value + 1) & (PAGE_SIZE - 1);
    return value + round_value;
}

static bool arena_commit(struct Arena *arena, size_t size)
{
    size = round_to_page(size);
    if(arena->commited + size > RESERVED_SIZE)
        return false;

    void *addr = (char*)arena->base + arena->size;
    int result = mprotect(addr, size, PROT_READ | PROT_WRITE);
    if(result != -1)
        arena->commited += size;

    return (result != -1);
}

struct Arena arena_create(size_t commit_size)
{
    struct Arena arena;
    arena.size = 0;
    arena.commited = 0;
    commit_size = round_to_page(commit_size);

    arena.base = mmap(NULL, RESERVED_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, 0);
    if(arena.base == MAP_FAILED)
        arena.base = NULL;
    else if(commit_size > 0)
        arena_commit(&arena, commit_size);
    return arena;
}

void *arena_push(struct Arena *arena, size_t size)
{
    size_t rem_size = arena->commited - arena->size;
    if(rem_size < size && !arena_commit(arena, size - rem_size))
        return NULL; //Commit failed 
    
    void *addr = (char*)arena->base + arena->size;
    arena->size += size;
    return addr;
}

void arena_free(struct Arena *arena)
{
    if(arena->base != NULL) {
        munmap(arena->base);
        arena->base = NULL;
    }
}
```
</details>


