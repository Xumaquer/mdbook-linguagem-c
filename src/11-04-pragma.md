# Pragma
O comando `#pragma` é utilizado para realizar um controle de comportamentos definidos pela implementação, é uma maneira padronizada com que o programador possa "comunicar-se" com detalhes específicos do compilador.

Pode ser escrito de duas formas : 
```c
//Primeira forma
#pragma identificador parametros   

//Segunda forma (C99)
_Pragma("identificador parametros")  
```
- `#pragma`: se comporta de uma maneira definida pela implementação, a menos que seja um dos pragmas padrão especificados depois.
- `_Pragma`: Disponível desde o `C99`, pode ser utilizado ao expandir uma macro, algo que é impossível com o `#pragma`, seu conteúdo precisa estar no formato de string, e o conteúdo da string é lido como se fosse a entrada de um `#pragma`.

## Pragmas padrão
Desde o `C99` foram adicionadas três pragmas padrão da linguagem, onde `arg` pode ser `ON` (ativo), `OFF` (desativo) , `DEFAULT` (padrão):
- `#pragma STDC FENV_ACCESS arg`: Indica que o usuário vai modificar o estado das flags de ponto flutuante do processador, proibindo algumas otimizações, geralmente o padrão é desabilitado.
- `#pragma STDC FP_CONTRACT arg`: Permite otimizações de ponto flutuante que omitem erros de arredondamento e exceções de ponto flutuante que seriam observadas se a expressão fosse usada diretamente (por exemplo permite que `(x*y)+z` seja implementado por uma instrução otimizada específica do processador ao invés de realizar a multiplicação e soma separadamente), geralmente o padrão é habilitado.
- `#pragma STDC CX_LIMITED_RANGE arg`: Indica que multiplicação, divisão e o valor absoluto de números complexos podem usar formulas matemáticas simplificadas apesar da possibilidade de overflow intermediário, em outras palavras, o programador garante que a faixa de valores passadas a essas funções será limitada, desabilitado por padrão.

Alguns compiladores podem não suportar esses pragmas, mas devem ao menos ter as mesmas opções como opções de linha de comando do compilador.

## Pragmas conhecidos
Todos os outros pragmas são definidos pela implementação, porém existem dois pragmas que são muito comuns : `#pragma once` e `#pragma pack`.

### Pragma once
Uma forma mais simplificada de escrita que tem efeito similar a um `include guard` conforme descrito no capítulo sobre [inclusão de arquivos](./11-03-inclusao-de-arquivos.md).

Basicamente ao utilizar `#pragma once`, o arquivo será ignorado se for incluido novamente, de forma que possamos ter um efeito similar a um `include guard`, porém sem a necessidade de criar uma macro única para o arquivo.

### Pragma pack
Utilizado para controlar o alinhamento de uma `struct` ou `union`, pode ser utilizado de 5 formas, onde `arg` é uma potência de 2 indicando o alinhamento:
- `#pragma pack(arg)`: Define o alinhamento como o valor de `arg`.
- `#pragma pack()`: Define o alinhamento como o valor padrão (especificado por um argumento da linha de comando).
- `#pragma pack(push)`: Guarda o valor do alinhamento atual numa "stack" interna.
- `#pragma pack(push, arg)`: Guarda o valor do alinhamento atual na "stack" interna, e define o alinhamento atual para `arg`.
- `#pragma pack(pop)`: Remove o último valor de alinhamento da "stack" interna e define o alinhamento para aquele valor.