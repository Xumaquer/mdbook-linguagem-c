# Inclusão de arquivos
Existem algumas diretivas de compilação utilizadas para incluir arquivos externos dentro do código fonte.

Estas diretivas são utilizadas para reutilizar e/ou minimizar a quantidade de código que devemos escrever, separando parte do conteúdo em arquivos externos.

As diretivas existentes são :
- `#include` : Utilizado para incluir o conteúdo de arquivos como código fonte.
- `#embed`   : Utilizado para incluir o conteúdo de arquivos como arrays de bytes acessíveis (`C23`).

## include
A diretiva `#include` é utilizada para adicionar o conteúdo de um arquivo ao código fonte.

O comportamento efetivo de `#include` é equivalente a abrir o arquivo, copiar e colar seu conteúdo no ponto em que ele foi incluido.

A sintaxe para uso de `#include` é: 
```c
//Inclusão de arquivos externos (1)
#include <stdio.h>

//Inclusão de arquivos do usuário (2)
#include "api/web.h"

//Inclusão utilizando definições (3)
#define LIB_PADRAO <stdlib.h>
#include LIB_PADRAO
```
1. A inclusão neste estilo é direcionada para arquivos sob o controle da implementação (compilador), que deve incluir as bibliotecas padrão do C e tipicamente os arquivos presentes pastas definidas como pastas adicionais de inclusão utilizando opções do compilador.
2. A inclusão de arquivos neste estilo é direcionada para arquivos do usuário, tipicamente os arquivos relativos ao arquivo que utilizou `#include`, a maioria das implementações busca a pasta atual e caso não encontre, realiza as buscas nas pastas de inclusão conforme `1.`. 
3. Caso o texto de inclusão não contenha um texto nos padrões `1.` e `2.` (com `<>` ou aspas), realiza a substituição de macro e a substituição resultante deverá se encaixar no padrão `1.` ou `2.`.

Num texto de inclusão os seguintes caracteres não podem ser utilizados no nome do arquivo : 
- `'`, `\`, `//`, `/*`
- Caracteres utilizado para iniciar/finalizar a inclusão (`<` e `>` no padrão `1.` ou `"` no padrão `2.`)

Além disso, desde o `C23` a palavra chave `__has_include`, como já explicado no capítulo sobre [compilação condicional](./11-02-compilacao-condicional.md), pode ser utilizada em `#if` ou `#elif` para checar se um arquivo existe e é um alvo válido para `#include` (mas não checa se o código contido no arquivo é válido).

### Arquivos de Cabeçário
É muito comum que o alvo de `#include` seja sempre algum arquivo que com extensão `.h`, estes arquivos são arquivos de cabeçário ou em inglês "header" (por isso `.h` de header).

Diferente da crença de muitos iniciantes, não existe uma diferença efetiva na forma como arquivos `.c` e `.h` são interpretados, ambos são código de C válido, e não há nada que realmente impeça o uso de `#include` com um arquivo `.c`.

A diferença na verdade, é simplesmente uma convenção, geralmente se espera que arquivos com extensão `.h` sejam utilizados para incluir bibliotecas, enquanto arquivos `.c` sejam utilizados para implementar essas bibliotecas, formando uma "separação" que é bastante natural para o programador de C.

Portanto, se utilizarmos um arquivo `.h` para definições, é natural que ele inclua apenas : 
- Macros
- Declarações de funções (sem definir)
- Declarações de tipos (`typedef`, `enum`, `struct`, `union`)
- Importação de variáveis globais (sem definir, utilizando `extern`)

Também é natural que exista um arquivo de mesmo nome com a extensão `.c` que : 
- Inclua o próprio `.h`, reutilizando o código de definição de tipos e macros
- Defina e implemente todas funções declaradas no `.h`
- Defina, caso haja alguma, as variáveis globais importadas no `.h`

Exemplo deste padrão (arquivos são delimitados pelos comentários) : 
```c
//test.h
#ifndef TEST_H
#define TEST_H
struct VersaoSoft {
    int maior;
    int menor;
    int revisao;
};

void printaVersao(struct VersaoSoft *versao);

#endif 

//test.c
#include <stdio.h>
#include "test.h"

void printaVersao(struct VersaoSoft *versao)
{
    printf("Versao %u.%u.%u\n", versao->maior, versao->menor, versao->revisao);
}
```

Podemos entender um pouco melhor a necessidade dessa separação ao entendermos os dois problemas que aconteceriam se decidissemos "incluir arquivos com implementações completas" em vários lugares : 
- Toda função ou variável pública sem `static` é exportável, portanto se duas ou mais unidades de tradução diferentes incluirem uma implementação da mesma função, ocorrerá um conflito durante a vinculação, ocasionando em um erro de compilação.
- Aumento no tempo de compilação, devido a necessidade de recompilar o código adicionado uma vez para cada unidade de tradução que o inclua.

Apesar de um pouco menos comum, também há casos onde utiliza-se um arquivo `.h` para fazer uma biblioteca "facilmente embarcável", nestes casos costuma-se utilizar `static` em todas funções e variáveis, de forma que mesmo que dois arquivos diferentes incluam a mesma biblioteca, não ocorra nenhum conflito na etapa de vinculação.

### Include Guard
Alguns padrões específicos normalmente são utilizados para impedir a inclusão recursiva (um arquivo incluir ele mesmo diretamente 
ou indiretamente) ou múltiplas inclusões do mesmo arquivo.

Esses padrões são comumente chamados de "include guards", que no português seria algo como "proteção de inclusão".

Vale notar que incluir múltiplas vezes o mesmo arquivo poderia causar um erro de compilação se o arquivo incluir, por exemplo, definições de tipos (pois não podemos redefinir um mesmo tipo), além de ser um desperdício de tempo para o compilador.

Geralmente utilizamos o seguinte padrão para realizar um include guard : 
```c
#ifndef NOME_DO_ARQUIVO_H
#define NOME_DO_ARQUIVO_H

//Conteúdo do arquivo

#endif 
```
Logo no ínicio do arquivo, utilizamos um `#ifndef` indicando que o código abaixo só é válido se `NOME_DO_ARQUIVO_H` não estiver definido, porém, dentro do próprio arquivo, definimos a macro `NOME_DO_ARQUIVO_H`, de forma que numa segunda passada, o código seja ignorado.

Essa técnica é muito comum, e é a que você provavelmente encontrará em qualquer um dos headers das bibliotecas padrão do C do seu compilador, a maioria dos compiladores realizam otimizações específicas que permitem que o compilador possa pular abrir e checar um arquivo caso ele seja incluído novamente.

A maioria dos compiladores modernos permitem que você use `#pragma once` ou `_Pragma("once")` para sinalizar que um arquivo só pode ser incluído uma vez, sendo uma alternativa possível aos include guards.

Para garantir que o compilador possa realizar a otimização mencionada, é recomendado que o include guard seja escrito na sequência : 
- Comentários e espaço/nova linha apenas (ou vazio)
- `#ifndef NOME_dA_MACRO`
- Seu código
- `#endif`
- Comentários e espaço/nova linha apenas (ou vazio)

Apesar de `#ifndef X` ser equivalente a `#if ! defined(X)`, alguns compiladores só realizam a otimização citada caso utilizemos `#ifndef`.

### Otimizando tempo de Compilação
A inclusão de arquivos de bibliotecas pode tomar um tempo considerável da compilação, para isso existem algumas soluções que podem ajudar a diminuir este tempo.

Para diagnosticar e melhorar o tempo de compilação, é recomendado que o usuário : 
- Utilize uma ferramenta de build que permita compilar vários arquivos em paralelo.
- Evite utilizar cabeçários enormes, pode ser mais fácil só declarar o que vai usar.
- Pre-compilar cabeçários mais pesados, formando um formato que pode ser processado rapidamente.
- Evite recompilar bibliotecas grandes, compilando ou utilizando-as como bibliotecas compartilhadas (`.so`/`.dll`/`.dylib`).

Para diagnosticar os maiores gargalos relacionados a inclusão de arquivos, existe um programa gratuito e open source chamado [`IncludeGuardian`](https://includeguardian.io/) que também ajuda a diagnosticar.

Também é interessante citar o linker [`mold`](https://github.com/rui314/mold) que é um projeto de um linker moderno muito mais rápido e eficiente, que atualmente só funciona no linux.

## embed
A diretiva de compilação `#embed` é utilizada para embarcar os bytes de qualquer arquivo no código fonte.

Existem três padrões de escrita, da mesma forma que `#include`: 
- `#embed <arquivo>` : Inclusões de arquivos externos
- `#embed "arquivo"` : Inclusões de arquivos locais
- `#embed MACRO`: Inclusões com macros

A diretiva `#embed` deve ser utilizada para inicializar um array do tipo `char` ou `unsigned char`, sendo equivalente a escrever cada um dos bytes do arquivo como literais do tipo inteiro.

Por conta disso, é necessário colocar uma virgula se desejarmos adicionar dados além dos incluidos pelo `#embed`.

Exemplos : 
```c
const unsigned char dadosImagem[] = {
#embed "foto.png"
};

//Como o #embed funciona "injetando" inteiros literais
//podemos colocar sufixos ou prefixos
const char mensagem[] = {
'M', 'e', 'n', 's','a','g','e','m',':','\n',
#embed "mensagem.txt"
,'\0' 
};

//Ou até mesmo embarcar vários arquivos
//em um único array
const char documento[] = {
#embed "header.txt"
'\n',
#embed "body.txt"
,'\n',
#embed "footer.txt"
};
```

Além disso, existem 4 opções extras que podem ser utilizados no máximo 1 vez cada em conjunto com a diretiva `#embed`, todos seguindo a sintaxe `opcao(parametro)`: 
- `limit`: O parâmetro deve ser um número inteiro não negativo, limita a quantidade de bytes máxima que podem ser incluídas do arquivo.
- `prefix`: Se o arquivo utilizado no `#embed` existir e não for vazio, o parâmetro do `prefix` é colocado logo antes da expansão do `#embed` como prefixo
- `sufix`: Se o arquivo utilizado no `#embed` existir e não for vazio, o parâmetro do `sufix` é colocado logo após a expansão do `#embed` como sufixo
- `if_empty`: Se o arquivo utilizado no `#embed` não existir ou for vazio, o parâmetro do `if_empty` é será colocado no array, sendo omitido caso contrário.

Exemplo : 

```c
const char mensagem[] = {
#embed "mensagem.txt" if_empty('V','a','z','i','o')
,'\0'
};

#define CONTENT_TYPE     'C','o','n','t','e','n','t','-','T','y','p','e',':'
#define MIME_TYPE_JPEG   'i','m','a','g','e','/','j','p','e','g'
#define VERSAO_HTTP      'H','T','T','P','/','1','.','1',' '
#define STATUS_OK        '2','0','0',' ','O','K','\r','\n'
#define STATUS_NOT_FOUND '4','0','4',' ','N','O','T',' ','F','O','U','N','D','\r','\n'

//Embarca o arquivo com a resposta HTTP
const char respostaHttp[] = {
    VERSAO_HTTP, 
#embed "imagem.jpg" \
prefix(STATUS_OK, \
       CONTENT_TYPE, MIME_TYPE_JPEG,'\r','\n' \
       '\r','\n') \   //200 OK, Content-Type:image/jpeg
if_empty(STATUS_NOT_FOUND) //404 NOT FOUND
}
```

Além disso, similar ao `__has_include`, também podemos utilizar `__has_embed` em diretivas `#if` ou `#elif` para verificar se um recurso é embarcável com `#embed`, a diferença é que ap invés de `0` ou `1`, o `#embed` resulta em um entre três valores : 
- `__STDC_EMBED_NOT_FOUND__`: Arquivo não encontrado
- `__STDC_EMBED_EMPTY__`: Arquivo vazio
- `__STDC_EMBED_FOUND__`: Arquivo encontrado e com conteúdo