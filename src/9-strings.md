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

Dessa forma, poderiamos fazer um código que "descobre" o tamanho de uma string, lembrando que na prática a função `strlen` da biblioteca `string.h` é muito mais otimizada: 
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

Como strings são apenas "ponteiros para arrays" existem várias operações que podem ser realizadas com elas : 
```c
"25" + 1    // "5" (avança um caractere)
"comida"[3] // 'm' (pega o 4° caractere)
*"abc"      // 'a' (pega o 1° caractere)
[1]"BIFE"   // 'I' (pega o 2° caractere)

//Descontamos 1 por causa do \0 em "Hello "
//"World" (pula a frase "Hello ")
"Hello World" + sizeof("Hello ") - 1 
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
Antes de falar sobre os tipos de strings literais, precisamos falar sobre encodings.

Encoding, no contexto de strings (que seria "codificação" em português), é o padrão no qual decidimos quais códigos númericos utilizamos para indicar cada caractere, no geral todo encoding moderno extende o padrão ASCII, de forma que os códigos númericos 0 a 127 definidos no padrão ASCII ainda representem os mesmos caracteres.

Antes de contiinuar, é importante também entender o termo `code unit`  que simboliza um dos números utilizados para representar um caractere, em alguns casos, mais de um número é utilizado para representar um único caractere, houvendo uma distinção entre `code unit` vs caracteres (em aalguns encodings, um caractere pode ser formado por mais de um `code unit`).

#### Um resumo da história dos Encodings

O padrão ASCII era relativamente limitado, pois não suportava acentuações, caracteres japoneses, chineses, etc. Foi ai que surgiram diferentes codificações que buscavam adicionar mais caracteres ao padrão ASCII.

Vários padrões surgiram extendendo ASCII, como `Latin-1`, `Shift-JIS`, entre outros. O problema é que essas novas codificações não conversavam entre sí, de forma que não fosse possível codificar um caractere japonês em `Latin-1` e não haja acentuação em `Shift-JIS` e um mesmo valor númerico representasse coisas diferentes em cada codificação.

Para solucionar o problema dos formatos incompatíveis, surgiu o padrão conhecido como [`Unicode`](https://home.unicode.org/), que buscava uma representação universal para todo e qualquer caractere.

Foi então que, em 1993, surgiram as codificações `UCS-2` e `UCS-4`, usando respectivamente 16bits e 32bits para representar caracteres e sem nenhum mecanismo para usar mais de um `code unit` por caractere, muitos acreditaram que 16bits seria o suficiente para representar todos caracteres, levando a grande adoção do padrão `UCS-2`, inclusive de várias gigantes da tecnologia como Apple (MacOs, iOs), Sun (Java), Microsoft (Windows) que participavam do consórcio Unicode que definia os padrões Unicode.

- `UTF-8`, surgiu em 1993, é um formato compatível com ASCII puro que usa 8bits por `code unit`, podendo ter de 1 a 4 `code units` por caractere
- `UTF-16`, surgiu em 1996 como uma solução para aqueles que adotaram `UCS-2`, é um formato que usa 16bits por `code unit`, podendo ter de 1 a 2 `code units` por caractere
- `UTF-32`, é um sinônimo do formato `UCS-4`, onde cada `code unit` ocupa 32bits e representa qualquer caractere 

É por estes motivos históricos, a grande adoção de `UCS-2`, que ainda hoje, muitos softwares e linguagens de programação utilizam `UTF-16`, mesmo que o formato `UTF-8` seja mais compacto e eficiente. Agravado pelo fato que muitos sistemas e linguagens novas escolhem usar `UTF-16` ainda hoje, pelo simples fato que sistemas com os quais eles desejam comunicar ou operar ainda estão usando `UTF-16`.

### Unicode
Os caracteres do padrão Unicode são dividos em "planos" e "blocos", cada plano é uma divisão de 65536 valores divididos em vários "blocos" que servem como categorias.

O valor númerico de caracteres Unicode geralmente são expressados com o prefixo `U+` seguido de um número em hexadecimal, logo `U+00A0` representa o caractere Unicode cujo código númerico é o número hexadecimal `A0`, esses valores são denominados `code points`.

A faixa de valores do Unicode é definido como de `U+0000` até `U+10FFFF`, utilizando efetivamente 21 bits. Um site bom para consultar os caracteres disponíveis no Unicode é o [Compart](https://www.compart.com/en/unicode/).

Inclusive, por questões de compatibilidade, os valores na faixa de `U+0000` até `U+00FF` representam os mesmos caracteres do encoding `Latin-1`.

Todos os encodings que respeitam o padrão Unicode, tentam representar os mesmos valores númericos. Logo, o que muda entre os diferentes encodings é a forma de extrair o valor númerico ou como ocorre no caso do `UCS-2` (que é considerado obsoleto), uma impossibilidade de representar todos caracteres.

#### UTF-8

O formato `UTF-8` codifica os valores dos `codepoints` Unicode da seguinte forma (considerando `U+uvwxyz`) : 

| Faixa de codepoints   | Byte 1   | Byte 2   | Byte 3   | Byte 4   |
| --------------------- | -------- | -------- | -------- | -------- |
| U+0000 até U+007F     | 0yyyzzzz |          |          |          |
| U+0080 até U+07FF     | 110xxxyy | 10yyzzzz |          |          |
| U+0800 até U+FFFF     | 1110wwww | 10xxxxyy | 10yyzzzz |          |
| U+010000 até U+10FFFF | 11110uvv | 10vvwwww | 10xxxxyy | 10yyzzzz |


Nem todos os bits de cada byte são utilizados como valor efetivo, isso permite uma retrocompatibilidade com ASCII. Permitindo que você busque por caracteres ASCII como `\n` usando funções como `strchr` dentro de uma string `UTF-8`, sem medo de cair num pedaço que é usado para representar um caractere que precisa de 2,3 ou 4 bytes.

Para iterar "por caracteres" em uma string `UTF-8`, devemos extrair os `codepoints` da string, o código abaixo implementa isso : 
```c
#include <stdio.h>
#include <uchar.h>
#ifdef _WIN32
    #include <Windows.h>
#endif

/* Extrai um codepoint e avança a string para o próximo caractere */
static char32_t utf8_next_codepoint(const char **text)
{
    const char* ptr    = *text;
    char32_t codepoint = '\0';
    int size;

    if ((ptr[0]&0xF8) == 0xF0) {
        if (((ptr[1]&0xC0)^0x80) || ((ptr[2]&0xC0)^0x80) || ((ptr[3]&0xC0)^0x80))
            return codepoint;
        codepoint = ((ptr[0]&0x07) << 18) | ((ptr[1]&0x3F) << 12) | 
                    ((ptr[2]&0x3F) << 6) | (ptr[3]&0x3F);
        size = 4;
    } else if ((ptr[0]&0xF0) == 0xE0) {
        if (((ptr[1]&0xC0)^0x80) || ((ptr[2]&0xC0)^0x80))
            return codepoint;
        codepoint = ((ptr[0]&0x0F) << 12) | ((ptr[1]&0x3F) << 6) | (ptr[2]&0x3F);
        size = 3;
    } else if ((ptr[0]&0xE0) == 0xC0) {
        if ((ptr[1]&0xC0)^0x80)
            return codepoint;
        codepoint = ((ptr[0]&0x1F) << 6) | (ptr[1]&0x3F);
        size = 2;
    } else {
        codepoint = ptr[0];
        size = 1;
    }
    *text += size;
    return codepoint;
}

/* Converte o codepoint para UTF-8 */
static int codepoint_to_utf8(char32_t codepoint, char *utf8)
{
    int size;
    if(codepoint <= 0x007F) {
        utf8[0] = (char)codepoint;
        size = 1;
    } else if(codepoint <= 0x07FF) {
        utf8[0] = 0xC0 | (codepoint >> 6);
        utf8[1] = 0x80 | (codepoint & 0x3F);
        size = 2;
    } else if(codepoint <= 0xFFFF) {
        utf8[0] = 0xE0 | (codepoint >> 12);
        utf8[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8[2] = 0x80 | (codepoint & 0x3F);
        size = 3; 
    } else if(codepoint <= 0x10FFFF) {
        utf8[0] = 0xF0 | (codepoint >> 18);
        utf8[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        utf8[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8[3] = 0x80 | (codepoint & 0x3F);
        size = 4;
    } else { 
        size = 0;
    }
    return size;
}

int main()
{
    #ifdef _WIN32 /* Necessário no Windows para configurar 
                     saída do terminal para UTF-8 */
        SetConsoleOutputCP(CP_UTF8);
    #endif

    const char *text = (u8"💻: Olá humano");
    char32_t codepoint;
    while(codepoint = utf8_next_codepoint(&text)) {
        char utf8[5] = {0}; //4 bytes no máximo por codepoint + '\0'
        codepoint_to_utf8(codepoint, utf8);

        printf("%s = U+%04X\n", utf8, codepoint);
    }
}
```

#### UTF-16

O formato `UTF-16` é similar ao `UTF-8`, porém utiliza 16bits por `code unit`. A principal vantagem para uso do `UTF-16` é que na maioria dos casos, os caracteres usados para suportar múltiplas línguas fazem parte do plano Unicode `BMP` (Basic Multilingual Plane, ou no português  Plano Multilíngue Básico) que podem ser representados por um único `code unit` em `UTF-16`, permitindo que na maioria dos casos, a quantidade de `code units` seja igual a quantidade de `code points`.

Porém, como na maioria dos casos os caracteres mais utilizados são os presentes no padrão ASCII, o formato `UTF-8` acaba sendo mais eficiente na maior parte dos casos, exceto na representação de alguns caracteres japonêses e chineses, onde o formato `UTF-8` usa 3 bytes e o `UTF-16` usa 2.

Para representação dos `codepoints` Unicode, o `UTF-16` utiliza as seguintes conversões : 

Valores na faixa `U+0000` até `U+D7FF` e na faixa `U+E000` até `U+FFFF` são representados por um único `code unit` em `UTF-16`, a faixa `U+D800` até `U+DFFF` tem um propósito especial.

Para representar valores na faixa de `U+010000` até `U+10FFFF` são utilizados o que chamamos de par substituto (ou no inglês "surrogate pair"), onde seguimos a sequência de decodificação : 
- `0x10000` é subtraido do `codepoint`, de forma que sobre um número de 20bits na faixa de `0x00000–0xFFFFF`.
- Os 10 bits mais significativos desse número (na faixa `0x000-0x3FF`) são adicionados ao número `0xD800` para formar o primeiro `code unit` na faixa `0xD800-0xDBFF`
- Os 10 bits menos significativos desse número (na faixa `0x000-0x3FF`) são adicionados ao número `0xDC00` para formar o segundo `code unit` na faixa `0xDC00-0xDFFF`

```c
#include <stdio.h>
#include <uchar.h>

static char32_t utf16_next_codepoint(const char16_t **text)
{
    const char16_t* ptr = *text;
    char32_t codepoint  = '\0';

    if(*ptr <= 0xD7FF || *ptr >= 0xE000) {
        codepoint = *ptr;
        *text += 1;
    } else if(*ptr >= 0xD800 && *ptr <= 0xDBFF) { //UTF16LE (little endian)
        codepoint = 0x10000 + ((*ptr - 0xD800) << 10) + (ptr[1] - 0xDC00);
        *text += 2;
    } else if(*ptr >= 0xDC00 && *ptr <= 0xDBFF) { //UTF16BE (big endian)
        codepoint = 0x10000 + ((*ptr - 0xDC00) << 10) + (ptr[1] - 0xD800);
        *text += 2;
    }
    return codepoint;
}

int main()
{
    const char16_t *text = u"💻: Olá humano";
    char32_t codepoint;
    while(codepoint = utf16_next_codepoint(&text))
        printf("Codepoint = U+%04X\n", codepoint);
}
```

#### UTF-32
O formato `UTF-32` é bastante simples, todos seus `code units` representam exatamente um `codepoint` Unicode, logo ele tem exatamente os mesmos valores e não há necessidade de conversão.

Podemos dizer que o `UTF-32` seria basicamente o `UTF-8` ou `UTF-16` "descomprimido", onde a única vantagem é o acesso `O(1)` a qualquer caractere, dado um array de caracteres em `UTF-32`, com a desvantagem do uso de memória adicional, que pode também prejudicar a performance.

O formato se tornou sinônimo de `UCS-4`, pois na época da formulação o `UCS-4` era definido como tendo até 31bits reservados para caracteres, enquanto o `UTF-32` foi concebido tendo a faixa de valores `U+0000` até `U+10FFFF` de 21bits, mais tarde ambos foram padronizados como sinônimos (onde `UCS-4` passou a ter a mesma definição).

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

//Ou com a própria função memcpy
memcpy(str3, "abcdef", sizeof("abcdef"));

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
Devido a falta de consideração pela segurança no design inicial da linguagem C, existem muitas funções que são consideradas "não ideais" para tratar strings pois são suscetíveis a erros, para isso alguns projetos como [`git`](https://github.com/git/git/blob/0d42fbd9a1f30c63cf0359a1c5aaa77020972f72/banned.h#L4) ou [engenheiros de segurança da Microsoft](https://github.com/x509cert/banned) mantêm um header com uma lista de funções "banidas", que causa um erro de compilação caso alguém tente usar elas.

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

## Iteração com strings
É comum que iniciantes que vieram de outras linguagens de alto nível iterem sobre strings de forma ineficiente em C.

Uma implementação que normalmente podemos ver é : 
```c
for(int i = 0; i < strlen(str); i++) {
    str[i]; //Caractere
}
```

Observe que a função `strlen` é chamada a cada iteração, isso é extremamente ineficiente, pois cada chamada de `strlen` itera a string inteira até achar o caractere `\0` que é o finalizador.

Em linguagens alto nível é comum que string seja uma classe que já contêm o tamanho como uma de suas propriedades, tornando essa operação eficiente, o que não é o caso com as strings padrão do C.

Uma forma muito mais simples e muito mais eficiente de iterar sobre strings em C : 
```c
for(const char *c = str; *c; c++) {
    *c; //Caractere
}
```

Nesse caso temos um ponteiro que acessa cada caractere, avançando uma posição a cada iteração e que para quando chega no caractere `\0` (`*c` é `Falso` quando chega no valor `\0`).

Também podemos implementar de forma similar, utilizando indices : 
```c
for(int i = 0; str[i]; i++) {
    str[i]; //Caractere
}
```

## Localidade
Na biblioteca `locale.h` temos a função `setlocale` que permite a mudança da localidade do ambiente de execução da linguagem C, essa mudança pode ser completa ou parcial.

A função `setlocale` tem a seguinte sintaxe
```c
char *setlocale(int categoria, const char *locale);
```

Onde a `categoria` indica flags que são utilizadas para selecionar a categoria que será modificada :
- `LC_ALL`: Todas as categorias são modificadas (inclusive categorias que não são padrão)
- `LC_COLLATE`: Afeta comparação e transformações de strings adaptadas aos requisitos do idioma específico (afeta `strcoll` e `strxfrm`) 
- `LC_CTYPE`: Afeta o resultado das funções da biblioteca `ctype.h` que classificam caracteres (`isalpha`, `isdigit`, etc)
- `LC_MONETARY`: Afeta a formatação de unidades monetárias (afeta apenas `localeconv`)
- `LC_NUMERIC`: Afeta a formatação númerica (afeta `printf`, `scanf`, `strtod`, `strtof`, etc)
- `LC_TIME`: Afeta a formatação de tempo (afeta `strftime`)
- `LC_MESSAGES`: Presente apenas em sistemas POSIX, afeta o idioma das mensagens de erro fornecidas por `strerror` e `perror`

`locale` é uma string que identifica as configurações de localidade com formato não definido pelo padrão do C, mas que segue o seguinte formato tanto no linux quanto no Windows:
``` 
idioma[_território][.encoding]
```

Onde (considere os campos com `[]` como opcionais) : 
- `idioma` é um dos códigos da [ISO 639](https://www.loc.gov/standards/iso639-2/php/code_list.php) no Linux e [segue uma lista específica](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-lcid/a9eac961-e77d-41a6-90a5-ce1a8b0cdb9c) no Windows.
- `território` é um dos códigos de países da [ISO 3166](https://static.developer.mastercard.com/content/cross-border-services/uploads/ISO%203166%20Country%20Codes.pdf) no Linux ou [segue uma lista específica](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-lcid/a9eac961-e77d-41a6-90a5-ce1a8b0cdb9c) no Windows.
- `encoding` é um texto específico de plataforma identificando o encoding utilizado, no geral o aconselhável é utilizar sempre `utf8` que é o modo mais portável, no Windows `encoding` pode ser o número do código de página ou `ACP` para o código de página atual do usuário e no Linux o nome do encoding .

Podemos checar a lista de opções de locale no Linux executando o comando `locale -a` no terminal.

Lembrando que o encoding `utf8` só é suportado desde o Windows 10 e para que ele funcione devemos utilizar o ambiente de execução UCRT (Universal C Runtime) que é um ambiente de execução mais moderno para C no Windows ao invés do clássico mingw que usa a biblioteca de vínculo dinâmico `msvcrt.dll`.

Ainda assim, mesmo quando utilizamos o locale de `utf8`, no Windows a conversão que ocorre é `encoding do C` -> `encoding do terminal` -> `UTF-16`, de forma que mesmo definindo corretamente o locale como `utf8`, ainda estamos limitados aos caracteres suportados pelo encoding atual do terminal, que pode ser modificado utilizando a função [`SetConsoleOutputCP`](https://learn.microsoft.com/en-us/windows/console/setconsoleoutputcp) (muda o encoding da saída do terminal) e [`SetConsoleCP`](https://learn.microsoft.com/en-us/windows/console/setconsolecp) (muda o encoding da entrada do terminal).

Por padrão, o locale inicialmente configurado é equivalente a `setlocale(LC_ALL, "C")`, ao utilizar `"C"` estamos indicando o locale padrão que tem um comportamento consistente em todos os ambientes, podemos também utilizar uma string vazia `""` para indicar que queremos utilizar o locale do sistema, que geralmente é controlado pelas configurações do sistema operacional e variáveis de ambiente. Para obter a configuração de locale atual, basta utilizar `NULL` na configuração do locale.

Além disso, há uma função chamada `localeconv` que retorna um ponteiro do tipo `struct lconv`, que contém diversas variáveis com detalhes sobre formatação de unidades monetárias e números associados a configuração da localidade atual.

## Strings de múltiplos bytes
O formato de string considerado como atual pelo ambiente de execução da linguagem C depende da configuração definida com `setlocale`, alguns encodings podem utilizar mais de um byte por caractere, neste caso, há funções da biblioteca padrão que reconhecem esse detalhe e lidam especificamente com ele.

As strings que se encaixam nessa categoria são mencionadas como strings de múltiplos bytes, ou no inglês "multi-byte strings" (identificadas pelo prefixo `mb` nas funções da biblioteca padrão), é possível obter o maior número de bytes que um caractere pode precisar no formato de string atual através da macro `MB_CUR_MAX`.

Podemos utilizar as funções iniciando em `mbrto` para converter um caractere multi byte para diversos formatos como UTF-8 (`C23`), UTF-16 (`C11`), UTF-32 (`C11`) ou `wchar_t`. 

A iteração sobre strings de múltiplos bytes normalmente envolve o tipo `mbrstate_t` que é capaz de guardar o estado atual da conversão, isso acaba sendo necessário pois alguns formatos usam códigos especiais para trocar o significado dos outros caracteres (como o encoding japonês Shift-JIS).

É recomendável que o usuário evite funções que não utilizam `mbrstate_t` mas tem uma contraparte que aceita, como por exemplo a função `mblen` que idealmente deve ser substituida por `mbrlen`, pois essas funções geralmente dependem de um estado global, o que acaba por causar conflitos ao utilizar em conjunto com múltiplos threads ou problemas ao utilizar as funções de forma incorreta.

## Diferentes representações de String
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
- Facilidade de encontrar o tamanho em caracteres e bytes de qualquer string
- Mantêm a representação mais compacta que ainda permita acesso `O(1)` para qualquer caractere
- Uso reduzido de memória em algumas representações, apesar de questionável em outras
- Relativamente trivial de embarcar strings ASCII usando o formato legado
- Localidade de cache (pois basta passar um ponteiro no novo formato)

Desvantagens:
- Header relativamente grande, com vários campos que podem não ser utilizados
- Uso de memória considerável ao representar certos caracteres, que poderiam estar em `UTF-8`
- Extremamente confuso, usar strings UTF-8 diretamente e manter um cache do tamanho em caracteres poderia ser mais eficiente
- Na maioria dos casos é necessário alocar uma string nova, pois é muito difícil ou impossível embarcar a maioria das strings
- Extremamente ineficiente em tamanho comparado com outros formatos que codificam o header na string
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