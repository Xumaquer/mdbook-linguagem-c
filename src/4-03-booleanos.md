# Booleanos
Booleanos são valores que indicam um estado de `Verdadeiro` ou `Falso`.

No geral ao ler valores booleanos : 
- `Falso` indica um valor igual a zero
- `Verdadeiro` indica um valor diferente de zero

Porém como já foi descrito nos [operadores booleanos](./3-operadores.md#operadores-booleanos) :
- Expressões booleanas que resultam em `Falso` tem o valor 0
- Expressões booleanas que resultam em `Verdadeiro` tem o valor 1

Logo, é comum termos em programas `TRUE` (`Verdadeiro` em inglês) como `1` ou `FALSE` (`Falso` em inglês) como `0`.

Porém isso nos leva a um dilema, antes do `C99`, não existia nenhum tipo booleano, logo booleanos eram representados com tipos númericos como `int` ou `unsigned char`.

Ocasionando em problemas como o deste exemplo :  
```c
#include <stdio.h>

#define TRUE  1 //Verdadeiro em inglês
#define FALSE 0 //Falso em inglês

void testaValor(unsigned char booleana)
{
    if(booleana == TRUE)
        puts("O valor é verdadeiro");
    else
        puts("O valor é falso");
}

void testaValor2(unsigned char booleana)
{
    if(booleana)
        puts("O valor é verdadeiro");
    else
        puts("O valor é falso");
}

int main() {
    unsigned char valor = 5;

    testaValor(valor);  //Imprime "O valor é falso"
    testaValor2(valor); //Imprime "O valor é verdadeiro"
}
```

Perceba que neste exemplo, duas checagens diferentes reportaram um estado diferente, pois ao checar sem usar comparadores, tivemos o resultado de que `valor` era `Verdadeiro`, o que está correto.

Porém ao utilizar a clássica comparação `valor == TRUE`, como `TRUE` é apenas uma definição para `1`, e `valor` é 5, por serem números diferentes, o resultado é `Falso`.

Com este dilema, percebemos que algo que seria extremamente natural em linguagens de alto nível, se tornou um problema sutil, que exigiria cuidados extras ao manusear valores booleanos e que possivelmente seria de difícil detecção.

## O tipo booleano

Para resolver este problema o `C99` definiu o tipo `_Bool` e a biblioteca `stdbool.h`, o conteúdo da `stdbool.h` é tão pequeno que pode ser escrito aqui : 
```c
#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L //Checagem para C23
    /* bool, true e false são palavras chave da linguagem.  */
#else
    #define bool	_Bool
    #define true	1
    #define false	0
#endif
```

O tipo `_Bool` pode receber qualquer valor, porém ao guardar o valor, ele é automáticamente convertido para `0` ou `1` seguindo a regra de leitura de valores booleanos, permitindo comparações como `valor == true` sejam verdadeiras mesmo que o valor tenha sido obtido ao executar `valor = 5`.

Além disso, algumas conversões implicitas para o tipo `bool` ou `_Bool` são diferentes das conversões utilizadas para o tipo `int` : 
```c
bool b1 = 0.3;              // b1 == 1 (0.3 convertido para int é 0)
bool b2 = 2.0*_Imaginary_I; // b2 == 1 (mas convertido para int é 0)
bool b3 = 0.0 + 3.0*I;      // b3 == 1 (mas convertido para int é 0)
```

O ideal é sempre acessar o tipo utilizando o `stdbool.h` e as macros de `true` e `false` definidas nele, visto que no `C23`, `bool` se tornou um tipo único da linguagem, enquanto `true` e `false` se tornaram palavras chaves nativas da linguagem.

