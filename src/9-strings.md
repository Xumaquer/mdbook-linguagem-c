# Strings
Strings, no português chamadas de "cadeias de caracteres", são variáveis que guardam textos, estes que podem ser utilizados para praticamente qualquer coisa, desde nomes, comandos, expressões matemáticas, configurações, etc.

Em muitas linguagens de programação de alto nível, strings são consideradas como um tipo "separado" de dados, geralmente são imutáveis (modificar uma string significa gerar uma nova string, permitindo que strings sejam repassadas por referência de forma "segura").

Na linguagem C, não existe especificamente um "tipo string", strings são arrays de caracteres, geralmente repassada via ponteiro como `const char*` para strings não modificáveis, ou `char*` para strings que podem ser modificadas.

Mas ai fica a pergunta, se passarmos um único `const char*` como vamos saber o tamanho do array ? 

A resposta é que, geralmente, utiliza-se o caractere nulo `'\0'` que na prática, tem o valor `0`. Consideramos que uma string "acabou" quando, ao ler um caractere, ela tem valor igual a `\0`.

Logo uma string declarada como : 
```c
"Hello World\n"
```

Será na prática, um array que contenha : 
```c
'H' 'e' 'l' 'l' 'o' ' ' 'W' 'o' 'r' 'l' 'd' '\n' '\0'
```

Dessa forma, poderiamos fazer um código que "descobre" o tamanho de uma string, lembrando que na prática utiliizar a função `strlen` da biblioteca `string.h` é absurdamente mais otimizado: 
```c
#include <stdlib.h>
#include <stdio.h>

int tamanhoString(const char *str)
{
    const char *c = str;
    while(*c != '\0')
        c++;
    
    return (c - str);
}

int main()
{
    int tam = tamanhoString("abcdefghij");
    printf("A string tem tamanho %d\n", tam);
}
```


## Literais de strings
Quando escrevemos uma string diretamente como no exemplo abaixo : 
```c
char *teste1       = "Ola, mundo!";
const char *teste2 = "Ola, como vão?";

char *teste3       = "Ola, mundo!";
const char *teste4 = "Ola, como vão?";
```

Temos uma string "literal", que inicialmente é um array com o tamanho da string (lembrando que há uma reserva de 1 byte para o `\0`) mas que pode ser implicitamente convertido para um ponteiro e será trocado para um array constante ou não constante dependendo da variável que o recebe.

Em qualquer um dos casos o compilador provavelmente terá a string inteira já guardada dentro do executável, para que ela seja alocada junto da inicialização do programa, tornando tudo mais eficiente.

No caso do `teste1`, como não temos um ponteiro constante, o compilador provavelmente colocará a string numa região com permissão de escrita e será obrigado a reservar uma cópia separada para `teste3` caso não consiga garantir que nenhum dos dois modificou a string.

No caso do `teste2`, como o ponteiro aponta para um dado constante, o compilador provavelmente colocará a string numa região de apenas leitura, de forma que o programa seja finalizado caso alguém tente modificar a string em `teste2`, a variável `teste4` receberá uma referência para a mesma string, pois como ela é imutável, podendo ser reutilizada, diminuindo o uso de memória.


Sobre strings literais, também é interessante mencionar que strings separadas apenas por espaços/nova linha, são consideradas como parte da mesma string : 
```c
#define PASTA_RECURSOS "/home/pi/resources"

PASTA_RECURSOS "/imagem1.png"    //Este texto
"/home/pi/resources/imagem1.png" //É igual a este

//Este texto
"como vai\n"
"você?\n"
//É igual a este
"como vai\nvocê?\n"
```

Para digitar aspas e outros caracteres especiais numa string literal, utilizamos a mesma sintaxe das [`sequências de escape`](./4-01-caracteres.md#sequências-de-escape) descritas no capítulo sobre caracteres.

### Encoding
Antes de falar sobre os tipos de strings literais, precisamos falar sobre encoding.

Encoding, no contexto de strings (que seria "codificação" em português), é o padrão no qual decidimos quais códigos númericos utilizamos para indicar cada caractere, no geral todo encoding moderno segue o padrão ASCII, de forma que os códigos númericos 0 a 127 representem os mesmos caracteres.

Porém, nos encodings modernos geralmente desejamos representar mais do que apenas 128 caracteres diferentes, foi ai que surgiram diferentes codificações que buscavam colocar 
diferentes caracteres codificados de formas diferentes.

É importante também entender o termo `code unit`  que simboliza um dos números utilizados para representar um caractere, em alguns casos, mais de um número é utilizado para representar um único caractere, portanto é importante entender a distinção entre `code unit` vs caracteres.

Houve uma época onde existiam vários encodings diferentes que codificavam vários caracteres diferentes como `Latin-1` e `Shift-JIS`, entre outros. O problema dessas codificações é que elas não conversam entre sí, não podemos codificar um caractere japonês no `Latin-1` e não temos acesso a acentuação em `Shift-JIS`.

Para isso foi criado um padrão conhecido como [`Unicode`](https://home.unicode.org/), que busca uma representação universal para todo e qualquer caractere. O padrão `Unicode` gerou 3 tipos de codificações, `UTF-8`, `UTF-16` e `UTF-32`, que usam, respectivamente, 1 byte, 2 bytes ou 4 bytes por código (`code unit`).

Onde : 
- `UTF-8` é compatível com ASCII e usa 1 byte por code unit, valores além dos caracteres ASCII, podendo usar de 1 a 4 `code units` por caractere.
- `UTF-16` utiliza 2 bytes por `code unit`, sendo necessário 2 `code units` para representar alguns caracteres.
- `UTF-32` utiliza sempre 4 bytes mas representa qualquer caractere em um `code unit`.

### Tipos de strings literais

Existem 5 tipos diferentes de string literais no C : 
```c
"T1"   //String literal
u8"T2" //String literal UTF-8  (C11)
u"T3"  //String literal UTF-16 (C11)
U"T4"  //String literal UTF-32 (C11)
L"T5"  //String literal "wide"
```

- `String literal` : Uma string literal padrão, utiliza um encoding definido pelo compilador, mas geralmente usa exatamente o mesmo encoding usado no arquivo de texto onde o código fonte se encontra, tem tipo `char[N]`.
- `String literal UTF-8  (C11)`: Uma string literal que é sempre em UTF-8, independente do compilador, tem tipo `char[N]` (ou `char8_t[N]` no `C23`).
- `String literal UTF-16 (C11)`: Uma string literal que é sempre em UTF-16, independente do compilador, tem tipo `char16_t[N]`.
- `String literal UTF-32 (C11)`: Uma string literal que é sempre em UTF-32, independente do compilador, tem tipo `char32_t[N]`.
- `String literal "wide"`: Uma string literal longa, com encoding definido pelo compilador, no geral é `UTF-32` em sistemas baseados em Unix e `UTF-16` no Windows, tem tipo `wchar_t[N]`.


## Inicialização de strings
Podemos também utilizar strings literais para inicializar arrays : 
```c
#include <string.h>

char arr[] = "Abc"; //Neste caso, arr tem tamanho 4, com 'A','b','c','\0'

const char str2[] = "def"; //Podemos inicializar arrays constantes também
 
char str3[10];
str3 = "abcdef"         //Errado, isso só funciona na inicialização

//Essa é uma das formas de copiar a string (usando a função strcpy)
strcpy(str3, "abcdef"); 

//Neste caso, temos 'g','h','i','\0' no conteúdo, porém temos mais espaço reservado
//para expandir e colocar mais coisas na string caso necessário...
char str4[20] = "ghi"; 

//Também podemos inicializar estruturas com strings literais...
struct {
    char *txt1;
    int  i1;
} teste = {"Ola", 0};
```

## Segurança das funções de strings
Devido a idade da linguagem C, existem muitas funções que são consideradas "não ideais" para tratar strings pois são suscetíveis a erros, para isso alguns projetos como [`git`](https://github.com/git/git/blob/0d42fbd9a1f30c63cf0359a1c5aaa77020972f72/banned.h#L4) ou [engenheiros de segurança da Microsoft](https://github.com/x509cert/banned) mantêm um header com uma lista de funções "banidas", que causa um erro de compilação caso alguém tente usar elas.

Utilizar essas funções não necessariamente significa que você tem um problema de segurança, mas basta um erro no seu uso para elas se TORNAREM um problema de segurança.

## Annex K 
Vale lembrar que no `C11` a Microsoft empurrou a criação de versões "seguras" de muitas funções no chamado Annex K (Anexo K, o nome da parte do documento do padrão do `C11` especificando sobre estas novas funções), essas funções tem o sufixo `_s` e nomes muito parecidos com as funções padrões do C.

Porém a implementações das funções adicionadas no Annex K são opcionais, e estarão apenas presentes se a macro `__STDC_LIB_EXT1__` existir e o usuário definir `__STDC_WANT_LIB_EXT1__` para `1` antes de incluir `string.h` ou outra biblioteca, no geral o Annex K é considerado um fiasco pois foi baseadas em implementações da Microsoft que foram posteriormente modificadas, de forma que nem mesmo a própria Microsoft siga o padrão e muitos compiladores simplesmente se recusaram a implementar as funções descritas pois elas são opcionais.

Algumas materias, em inglês, com explicações mais a fundo sobre o Annex K : 
- [Experiência em campo com Annex K](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1967.htm)
- [Sugestões de correções para o Annex K](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2809.pdf)

No geral muitos criticaram muitas funções do Annex K pois : 
- Já existem outras soluções feitas por usuários/bibliotecas que são mais efetivas, simples e seguras
- É muito difícil fazer testes automatizados de código utilizando as funções `_s`
- Não é fácil de integrar as funções novas com código que usa as funções antigas
- As funções inerentemente diminuem a performance ao realizar checagens extensas e possivelmente redundantes

## Funções de manipulação de strings
Vamos apresentar rapidamente uma lista com as funções de strings recomendadas para cada ação, visando usabilidade e segurança, colocando `Sim` nos casos onde a função padrão é suficiente, `Não` nos casos que demandam explicações ou o nome da uma outra função quando ela é mais recomendada : 
 
| Ação               | Padrão  | Wide   | Recomendado  |
| :----------------: | :-----: | :----: | :----------: |
| Copiar string      | strcpy  | wcscpy | strlcpy      |
| Buscar caractere   | strchr  | wcschr | Sim          |
| Concatenar strings | strcat  | wcscat | strlcat      |
| Comparar strings   | strcmp  | wcscmp | Sim          |
| Tamanho de string  | strlen  | wcslen | Sim          |
| Achar substring    | strstr  | wcsstr | Sim          |

No caso de copiar strings, a função `strcpy` geralmente é substituida pela `strlcpy`, o `git` por exemplo usa essa função dessa forma e mantêm uma implementação própria caso a função não esteja disponível (pois ela não é padrão no C): 
```c
size_t gitstrlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}
```

A função `strlcat` também não é padrão, mas existe em alguns distribuições assim como a `strlcpy`, caso ela não exista, podemos ter nossa própria implementação :
```c
size_t strlcat(char *dest, const char *src, size_t size)
{
    if(size == 0)
        return 0;
    
    size_t destsize = strlen(dest);
    size_t ret = 0;
    if(destsize < size)
        ret = strlcpy(dest + destsize, src, size - destsize);
    return ret;
}
```

Para versão wide, podemos checar se as funções `wcslcpy` ou `wcslcat` existem, caso contrário podemos utilizar as implementações sugeridas, trocando os tipos `char` por `wchar_t` e a variável `len` no `memcpy` por `len * sizeof(wchar_t)`.

### Diferentes representações de String
Neste campo vamos falar sobre algo um pouco avançado, as diferentes implementações para representar strings em várias linguagens, incluindo C e até mesmo bibliotecas renomadas que buscam algo mais eficiente, destacando os pontos positivos e negativos de cada formato.

Alguns termos, que precisam de explicação, serão reutilizados ao longo das explicações : 
- Localidade de cache : Indica que os dados estão próximos na memória, logo o acesso a string está efetivamente acessando apenas um local da memória para conseguir o tamanho e os dados da string, o que pode ajudar na performance.
- Evita realocações : Geralmente quando a string tem um campo indicando o tamanho realmente reservado para string, permitindo concatenações sem realocar a memória reservada.
- Dificuldade de gerar substrings : A facilidade de pegar um pedaço de uma string e usar ele como uma nova string, útil em diversas situações.

{{#tabs }}

{{#tab name="C"}}
{{#tabs }}

{{#tab name="Padrão"}}
Bom, como já explicado neste guia, a linguagem C utiliza strings terminadas com `\0`, sendo necessário percorrer a string para obter o tamanho.

Vantagens : 
- Localidade de cache
- Leve nos casos onde saber o tamanho antecipadamente não é necessário
- Utilizada em chamadas de sistemas
- É o formato que usa menos memória para representação de strings

Desvantagens:
- Usa `strlen` para obter tamanho, operação `O(n)`
- Dificil de obter substrings
- Pode ser necessário realocar para concatenar

{{#endtab }}

{{#tab name="Glib"}}
A biblioteca [`Glib`](https://docs.gtk.org/glib/struct.String.html), utiliza strings num formato composto por:
- `str`:  ponteiro para uma string terminada por zero, como as padrões do C
- `len`: tamanho atualmente ocupado pela string 
- `allocated_len`: tamanho em bytes da memória reservada para string, que pode ser maior do que `len`, indicando que é possível concatenar a string sem realizar uma realocação.

```c
struct GString {
  char* str;
  size_t len;
  size_t allocated_len;
};
``` 

Vantagens: 
- Evita realocações
- Fácil de embarcar strings já existentes em C
- Obtenção rápida do tamanho

Desvantagens:
- Complexidade adicional para acesso (no código fonte)
- Indireção adicional (ponteiro para `GString` para só depois acessar `str`)
- Uso adicional de memória (8 bytes em 32bits / 16 bytes em 64bits para 2 `size_t`)
- Dilema ao gerar substrings, pois ela pode não ser terminada por zero, ou necessitará de cópia

{{#endtab }}

{{#tab name="SDS"}}
A biblioteca [`SDS`](https://github.com/antirez/sds) é extremamente interessante, ela tem uma forma única de representar strings, feita pelo autor do [`Redis`](https://redis.io/). 


Lembrando que :
- `lsb` é uma sigla para `least significant bits` que significa "bits menos significativos"
- `msb` é uma sigla para `most significant bits` que significa "bits mais significativos"

A biblioteca fornece 5 representações diferentes, que são trocadas conforme a string vai crescendo : 
```c
struct __attribute__ ((__packed__)) sdshdr5 { //Para strings com até 31 bytes
    unsigned char flags; /* 3 lsb do tipo e 5 msb do tamanho da string */
    char buf[];
};
struct __attribute__ ((__packed__)) sdshdr8 { //Para strings com até 255 bytes
    uint8_t len; /* tamanho da string */
    uint8_t alloc; /* Não leva em consideração header e terminador 0 */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};
struct __attribute__ ((__packed__)) sdshdr16 { //Para strings com até 65535 bytes
    uint16_t len; /* tamanho da string  */
    uint16_t alloc; /* Não leva em consideração header e terminador 0 */
    unsigned char flags; /* 3 lsb do tipo, 5 bits não utilizados */
    char buf[];
};
struct __attribute__ ((__packed__)) sdshdr32 { //Para string com até 2^32 - 1 bytes
    uint32_t len; /* tamanho da string */
    uint32_t alloc; /* Não leva em consideração header e terminador 0 */
    unsigned char flags; /* 3 lsb do tipo, 5 bits não utilizados */
    char buf[];
};
struct __attribute__ ((__packed__)) sdshdr64 { //Para strings com até 2^64-1 bytes
    uint64_t len; /* tamanho da string */
    uint64_t alloc; /* Não leva em consideração header e terminador 0 */
    unsigned char flags; /* 3 lsb do tipo, 5 bits não utilizados */
    char buf[];
};
```

As strings são repassadas passando um ponteiro de `buf` e os headers são acessados ao acessar `buf[-1]`, verificar as flags e depois acessar os campos anteriores.
Isso permite que a string inteira seja repassada usando apenas um ponteiro.

Vantagens:
- Localidade de cache
- Obtenção rápida do tamanho
- Evita realocações
- Compatibilidade com strings em C
- Facilidade de uso (pode ser usada da mesma forma que uma string normal de C)

Desvantagens:
- Não pode embarcar strings existentes, é necessário alocar uma nova
- Impossível obter substring, é necessário fazer cópias

{{#endtab }}

{{#endtabs }}
{{#endtab }}

{{#tab name="C++"}}
{{#tabs }}

{{#tab name="std::string (GCC)"}}
No C++, assim como na biblioteca `Glibc` em C, temos 3 campos : 
- `size` : Tamanho da string
- `data` : Ponteiro para string terminada por 0
- `capacity` : Tamanho realmente alocado para a string

Porém, existe uma otimização única realizada pelas implementações do C++, onde strings menores que os tamanhos `data` + `capacity` são guardadas num `union` dentro da própria string, evitando alocações adicionais, num sistema 64bits isso significa que strings com até 16 bytes (15 caracteres + `\0`) não necessitam de uma alocação.

```cpp
class string {
private:
    size_type m_size;
    union {
        class {
            std::unique_ptr<char[]> m_data;
            size_type m_capacity;
        } m_large;
        std::array<char, sizeof(m_large)> m_small;
    };
};
``` 

Ou talvez de um jeito mais amigável para um leitor em C : 
```c
struct string {
    size_t size;
    union {
        struct {
            char *data;
            size_t capacity;
        } large;
        char small[sizeof(size_t) + sizeof(char*)];
    };
}
```

Vantagens: 
- Evita realocações
- Fácil de embarcar strings já existentes em C
- Obtenção rápida do tamanho
- Otimização adicional para strings pequenas

Desvantagens:
- Indireção adicional (ponteiro para `std::string` para só depois acessar `data`)
- Uso adicional de memória (8 bytes em 32bits / 16 bytes em 64bits para 2 `size_t`)
- Dilema ao gerar substrings, pois ela pode não ser terminada por zero, ou necessitará de cópia
- Devido as funcionalidades automáticas da `std::string`, pode ser necessário alocar cópias de strings literais.
{{#endtab }}

{{#tab name="std::string_view (GCC)"}}
O tipo `std::string_view` do C++ pode ser representado pelo seguinte código em C : 
```c
struct string_view {
    char   *str;
    size_t len;
};
```  

Ele é extremamente simples, apenas um ponteiro para os dados do caractere + tamanho, similar as strings do Rust, porém sem um campo `capacity`, o que é ideal para leitura de strings e pode ser passado diretamente por valor, permitindo que ambos valores sejam repassados via registradores a uma função (diminuindo uma indireção).

Vantagens: 
- Possível eliminar indireção ao passar por valor de forma menos custosa (Localidade de cache)
- Ideal para utilizar substrings
- Fácil de embarcar strings existentes em C
- Permite embarcar arquivo de texto mapeado em memória
- Obtenção rápida do tamanho

Desvantagens: 
- Não é ideal para concatenação
- Não pode ser repassado diretamente para funções em C, pois pode não ter o `\0`
- Mesmo passando por valor, exige o uso de 2 parâmetros

{{#endtab }}

{{#endtabs }}
{{#endtab }}

{{#tab name="Python"}}
{{#tabs }}

{{#tab name="CPython"}}
O CPython pode representar strings de várias formas diferentes : 
```c
typedef struct{
   size_t refcnt;      /* Contagem de referência */
   PyTypeObject *type; /* Ponteiro para informações do tipo */
} PyObject;

typedef struct {
    PyObject head;  //Cabeçário com informações do tipo
    ssize_t length; //Tamanho da string 
    ssize_t hash;   //-1 se não definido
    struct { 
        unsigned int interned : 2; 
        unsigned int kind:3;
        unsigned int compact:1;
        unsigned int ascii:1;
        unsigned int statically_allocated:1;
    } state;
}PyASCIIObject;

typedef struct {
  PyASCIIObject _base;
  ssize_t utf8_length;
  char *utf8;
} PyCompactUnicodeObject;

typedef struct {
  PyCompactUnicodeObject _base;
  union {
      void *any;
      Py_UCS1 *latin1;
      Py_UCS2 *ucs2;
      Py_UCS4 *ucs4;
  } data;
} PyUnicodeObject;
```

Todas as strings são terminadas por zero, assim como strings em C.

Toda string começa com o campo `PyASCIIObject`, as flags presentes em `state` simbolizam : 
- `interned`: Se a string foi colocada num cache interno, mecanismo similar ao `.intern` do Java.
- `kind`: Indica quantos bytes são utilizados por codepoint (indicando também o formato da string)
- `compact`: Se a string está num formato "compacto", se não estiver ela é representada por `PyUnicodeObject`
- `ascii`: Se a string contem apenas caracteres ASCII
- `statically_allocated`: Se a string foi alocada de forma estática

No geral, o python alterna entre os formatos (mudando encoding caso um caractere acima do range apareça):
- `ascii`: Se todos os caracteres estão no range `U+0000` até `U+007F`.
- `Latin1`: Se todos os caracteres estão no range `U+0000` até `U+00FF`.
- `UCS-2`: Se todos os caracteres estão no range `U+0000` até `U+FFFF`.
- `UCS-4`: Se todos os caracteres estão no range `U+0000` até `U+10FFFF`.

Se o formato não for `compact`, os valores são guardados nos ponteiros presentes em `data`, separadamente, esse formato é descrito como "legado" e não é aconselhável.

O novo formato sempre tem o campo `compact` como `1`, neste caso, se a codificação for `ASCII`, os dados da string se encontram logo após a estrutura `PyASCIIObject`, caso contrário, os dados da string se encontram logo após a estrutura `PyCompactUnicodeObject`.

Isso ocorre pois os campos `utf8_length` e `utf8` são utilizados apenas quando um código em C chama uma API para "pedir" a string para o Python e são preenchidos ao realizar a conversão e mantidos como cache, porém quando a codificação é `ASCII` essa conversão é desnecessária (pois podemos considerar uma string `UTF-8` como superset de `ASCII`).

Vantagens:
- Facilidade de encontrar o tamanho em caracteres de qualquer string
- Uso reduzido de memória em algumas representações, apesar de questionável em outras
- Mantêm 1 caractere por `code unit` com a representação mais leve (de forma que `length` sirva como ambos tamanhos)
- Relativamente trivial de embarcar strings ASCII usando o formato legado
- Localidade de cache (pois basta pasar um ponteiro no novo formato)

Desvantagens:
- Header relativamente grande, com vários campos que podem não ser utilizados
- Uso de memória considerável ao representar certos caracteres, que poderiam estar em `UTF-8`
- Extremamente confuso, usar strings UTF-8 diretamente e manter um cache do tamanho em caracteres poderia ser mais eficiente
- Na maioria dos casos é necessário alocar uma string nova, pois é muito difícil ou impossível embarcar a maioria das strings
- Extremamente ineficiente comparado com outros formatos que codificam o header na string
- Necessidade de ter vários códigos diferentes para ler os diferentes formatos, ou converter para um formato em comum

{{#endtab }}

{{#endtabs }}
{{#endtab }}


{{#tab name="Javascript"}}
Em construção...
{{#endtab }}

{{#tab name="Java"}}
{{#tabs }}

{{#tab name="Java 8 ou anterior"}}
No Java 8, temos os seguintes campos : 
- `value` : Um array de bytes, que terá o tamanho do array + valores, onde utilizamos `UTF-16` para representar os caracteres
- `hash`  : Um cache do HASH dessa string (inicializado como 0)
- `hashIsZero`: Indica que o hash já foi checado e é 0 (evita recalcular hash caso ele der 0)

```java
public final class String {
    private final byte[] value;
    private int hash;
    private boolean hashIsZero;
}
```

No caso do Java, assume-se que arrays guardam `pointero` + `tamanho`, mas não guardam um valor de `capacidade` principalmente devido a imutabilidade de strings do Java.

Vantagens :
- Útil nos casos onde operações de hash são usadas com frequência
- Segurança caso imutabilidade seja desejável
- Obtenção rápida do tamanho

Desvantagens:
- Uso adicional de memória para hash, que pode não ser utilizado
- Uso adicional de memória para vários campos, devido ao uso de um Objeto + Array
- Indireção adicional para acessar a classe `String` + o array interno
- Desde o Java 7, substrings geram cópias
- Não evita realocações ao concatenar strings devido a imutabilidade
- Por não ser terminada por zero, não pode ser utilizada em funções que esperam uma string terminada por zero (como funções do sistema)
- Uso adicional de memória por cada string devido ao encoding UTF-16
{{#endtab }}

{{#tab name="Java 9+"}}
No Java 9 ou superior, temos os seguintes campos : 
- `value` : Um array de bytes, que terá o tamanho do array + valores, onde utilizamos `UTF-16` ou `Latin-1` para representar os caracteres
- `coder` : Flag que indica quando a codificação é `UTF-16` ou `Latin-1`
- `hash`  : Um cache do HASH dessa string (inicializado como 0)
- `hashIsZero`: Indica que o hash já foi checado e é 0 (evita recalcular hash caso ele der 0)

```java
public final class String {
    private final byte[] value;
    private final byte coder;
    private int hash;
    private boolean hashIsZero;
}
```

Neste caso, o Java utiliza a codificação `Latin-1` quando possível, trocando para a codificação `UTF-16` quando houver algum caractere que não pode ser representado em `Latin-1`, isso diminui muito o uso de memória para strings.

No caso do Java, assume-se que arrays guardam `pointero` + `tamanho`, mas não guardam um valor de `capacidade` principalmente devido a imutabilidade de strings do Java.

Vantagens :
- Útil nos casos onde operações de hash são usadas com frequência
- Segurança caso imutabilidade seja desejável
- Obtenção rápida do tamanho
- Compactação de strings que usam caracteres presentes no encoding `Latin-1`

Desvantagens:
- Uso adicional de memória para hash, que pode não ser utilizado
- Uso adicional de memória para vários campos, devido ao uso de um Objeto + Array
- Indireção adicional para acessar a classe `String` + o array interno
- Desde o Java 7, substrings geram cópias
- Não evita realocações ao concatenar strings devido a imutabilidade
- Por não ser terminada por zero, não pode ser utilizada em funções que esperam uma string terminada por zero (como funções do sistema)
{{#endtab }}


{{#endtabs }}
{{#endtab }}

{{#tab name="Rust"}}
{{#tabs }}

{{#tab name="String"}}

O tipo `String` em Rust é representado por 3 campos : 
- `ptr` : Ponteiro para string em UTF-8 não terminado por zero
- `len` : Tamanho atual da string em bytes
- `capacity`: Tamanho alocado para a string em bytes

```c
struct RustString {
    char *ptr;
    size_t len;
    size_t capacity;
};
```

Vantagens:
- Facilidade de embarcar strings já existentes, mesmo strings em C
- Evita realocações
- Facilidade ao gerar substrings
- Permite embarcar arquivos de textos mapeados em memória

Desvantagens:
- Por não ser terminada por zero, não pode ser utilizada em funções que esperam uma string terminada por zero (como funções do sistema)
- Indireção adicional
- Uso adicional de memória (8 bytes em 32bits / 16 bytes em 64bits para 2 `size_t`)
{{#endtab }}

{{#tab name="CString"}}
Tem exatamente a mesma representação de uma string do C, sem guardar tamanho nem capacidade, pensado justamente para interoperabilidade com a linguagem.
{{#endtab }}

{{#tab name="OsString"}}
O formato `OsString` utiliza :
- Unix : `Vec<u8>` com bytes quaisquer
- Windows: Strings em [`WTF-8`](https://simonsapin.github.io/wtf-8/), que é um formato alternativo de `UTF-8` que permite representar `UTF-16` inválido

Isso ocorre pois ambos sistemas operacionais, em seus sistemas de arquivo, não validam strings unicode para checar se são `UTF-8` ou `UTF-16` válido.

Assim como a `String` do Rust, `OsString` não apresenta um terminador nulo, sendo necessário para o Rust realizar essa conversão antes de efetivamente realizar chamadas de sistema, isso também permite uma conversão trivial entre `OsString` e `String` caso `OsString` não seja `UTF-8` inválido. 
{{#endtab }}

{{#endtabs }}
{{#endtab }}


{{#endtabs }}