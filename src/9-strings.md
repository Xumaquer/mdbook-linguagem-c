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
    char *c = str;
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

Houve uma época onde existiam vários encodings diferentes que codificavam vários caracteres diferentes como `Latin-1` e `Shift-JIS`, entre outros. O problema dessas codificações é que elas não conversam entre sí, não podemos codificar um caractere japonês no `Latin-1` e não temos acesso a acentuação em `Shift-JIS`.

É importante também entender o termo `code unit`  que simboliza um dos números utilizados para representar um caractere, em alguns casos, mais de um número é utilizado para representar um único caractere, portanto é importante entender a distinção entre `code unit` vs caracteres.

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
strcpy(str3, "abcdef"); //Essa é uma das formas de copiar a string (usando a função strcpy)

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