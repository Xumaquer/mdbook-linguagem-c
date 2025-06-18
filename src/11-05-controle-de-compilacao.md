# Controle de compilação
Algumas macros podem ser utilizadas para gerar avisos ou abortar a compilação.

Elas são : 
- `#error mensagem`: Gera um erro de compilação, fazendo com que o compilador mostre a `mensagem` especificada.
- `#warning mensagem`: Adicionado no `C23`, gera um aviso do compilador, mostrando `mensagem` mas não interrompe a compilação.

`mensagem` pode ser um conjunto de várias palavras não necessariamente entre aspas (apesar do mais comum ser utilizar aspas).

Antes de ser padronizado, `#warning` era suportado por vários compiladores como uma extensão.

Exemplo do uso de `#error` : 
```c
#include <limits.h>

#if (CHAR_BIT * sizeof(void*)) < 32
    #error "Este programa não funciona em arquiteturas menores que 32bits"
#endif
```