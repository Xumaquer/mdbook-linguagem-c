# Números Binários
Toda informação num computador é composta por bits, a palavra bit é na verdade uma sigla para o termo em inglês BInary DigiT, que em português seria "digito binário".

Binário vem do número dois, "bi", indicando uma existência de apenas dois algarismos diferentes `0` e `1`.

É importante lembrar que é comum utilizarmos os prefixos conforme listado no capítulo [sobre literais](./10-literais.md) para diferenciar os sistemas númericos :
- `0b` para binário
- `0`  para octal
- Nenhum para decimal
- `0x` para hexadecimal

O sistema binário compartilha algumas características em comum com os sistemas númerico octal, decimal e hexadecimal:

1. Existe um número limitado de símbolos usados para representar um algarismo
2. Ao incrementar o valor de um algarismo, quando não houver símbolos para representá-lo como um algarismo de valor maior, podemos voltar aquele algarismo para `0` e incrementar o algarismo a esquerda (ou adicionar um algarismo `1` caso não haja algarismo a esquerda)
3. O símbolo `0` é usado para indicar um algarismo de valor nulo

Quanto a declaração `1.` é fácil de percebê-la ao analisarmos os algarismos presentes nos diferentes sistemas: 
- Binário : `0` e `1`
- Octal: `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7`
- Decimal: `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`
- Hexadecimal: `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`, `A`, `B`, `C`, `D`, `E`, `F`

Já quanto a declaração `2.`, podemos ver por exemplo ao analisar a tabela abaixo:

| Decimal | Binário  | Octal  | Hexadecimal  |
| :-----: | :------: | :----: | :----------: |
| 0       | 0        | 0      | 0            |
| 1       | 1        | 1      | 1            |
| 2       | 10       | 2      | 2            |
| 3       | 11       | 3      | 3            |
| 4       | 100      | 4      | 4            |
| 5       | 101      | 5      | 5            |
| 6       | 110      | 6      | 6            |
| 7       | 111      | 7      | 7            |
| 8       | 1000     | 10     | 8            |
| 9       | 1001     | 11     | 9            |
| 10      | 1010     | 12     | A            |
| 11      | 1011     | 13     | B            |
| 12      | 1100     | 14     | C            |
| 13      | 1101     | 15     | D            |
| 14      | 1110     | 16     | E            |
| 15      | 1111     | 17     | F            |
| 16      | 10000    | 20     | 10           |

É fácil ver como os números "voltam" para `0` e incrementam o algarismo a esquerda quando não há mais símbolos para representar o próximo, e o nosso sistema decimal com o qual estamos tão acostumados faz exatamente a mesma coisa.

Quanto a `3.`, em todos os sistemas númericos, o número `0` indica um valor nulo e adicionar `0` a qualquer número não modifica seu valor, isso é um ponto importante que todos sistemas númericos citados compartilham.

Uma forma interessante de pensar para lermos o valor de qualquer número, é que cada algarismo tem um valor igual a 
    
\\[Valor*Tamanho^{Posição}\\]

- `Valor` é o valor individual do algarismo
- `Tamanho` é a quantidade de símbolos que o sistema númerico tem (`2` para binário, `8` para octal, `10` para decimal, etc)
- `Posição` é posição do algarismo da direita para esquerda, iniciando em `0`.

Logo podemos decompor o número binário `0b101`na expressão `1*2^2 + 0*2^1 + 1*2^1` que usa números decimais para calcular seu valor.

Chamamos essa forma de `notação posicional`.

## Conversão binário para decimal
Como números binários só tem 2 algarismos, podemos simplificar um pouco o pensamento na hora de realizar o cálculo para converter números binários para decimal.

A técnica utilizada continua sendo a de pegar cada algarismo e avaliar seu valor usando a `notação posicional`, porém como só temos dois valores possíveis (`0` ou `1`), podemos facilitar a conversão escrevendo previamente o resultado das potências de dois.

Considere a conversão do número binário `0b101101` para decimal :  
```c
4096 2048 1024 512 256 128 64 32 16 8 4 2 1 //Potências de dois em decimal
                               1  0 1 1 0 1 //Número binário
```

Todos os algarismos em `1` representam números decimais que serão adicionados no cálculo, logo `0b101101` é `32 + 8 + 4 + 1` que resulta em `45`.

Podemos aplicar a mesma lógica para números maiores como `0b11010101` : 
```c
4096 2048 1024 512 256 128 64 32 16 8 4 2 1 //Potências de dois em decimal
                         1  1  0  1 0 1 0 1 //Número binário
```
Com isso teremos `128 + 64 + 16 + 4 + 1` que resulta em `213`.


## Conversão decimal para binário
Existem duas formas de converter:
- Utilizando a técnica da `notação posicional` (fácil para números menores)
- Utilizando divisão (ideal para convertermos números grandes ou para quem prefere um método que serve bem para qualquer caso)

### Conversão com notação posicional
Para realizar a conversão, podemos escrever as potências de 2 e ir diminuindo a maior potência de dois que seja maior ou igual ao número, anotando um `1` em cada potência usada e `0` nas que não foram utilizadas.

Repetimos o passo até que o número resultante das subtrações seja `0`.

Por exemplo, para converter `55` podemos fazer : 
```c
64 32 16 8 4 2 1
 0  1  1 0 1 1 1

//De forma simplificada
55 - 32 = 23 - 16 = 7 - 4 = 3 - 2 = 1 - 1 = 0

//Passo a passo
55 >= 64 ? 0
55 >= 32 ? 1 (55-32 = 23)
23 >= 16 ? 1 (23-16 = 7)
7  >= 8  ? 0
7  >= 4  ? 1 (7-4 = 3)
3  >= 2  ? 1 (4-2 = 1)
1  >= 1  ? 1 (1-1 = 0)
```

Agora vamos converter `5000` utilizando o mesmo método : 
```c 
4096 2048 1024 512 256 128 64 32 16 8 4 2 1
   1    0    0   1   1   1  0  0  0 1 0 0 0

5000 - 4096 = 904 - 512 = 392 - 256 = 136 - 128 = 8 - 8 = 0
```

### Conversão com divisão
Outra forma de implementar consiste em realizar divisões por 2 e utilizar os restos da divisão como digitos, o único porém é que os digitos estarão em ordem inversa.

Por exemplo, para convertermos o número `56` para binário (usaremos `%` como operador de módulo): 
```c
//Operações iniciais (resto e divisão)
56 % 2 = 0 (56/2 = 28)
28 % 2 = 0 (28/2 = 14)
14 % 2 = 0 (14/2 = 7)
7  % 2 = 1 (7/2  = 3)
3  % 2 = 1 (3/2  = 1)
1  % 2 = 1 (1/2  = 0)

//Agora precisamos ler os resultados dos restos de baixo para cima
//resultando em 111000

56       //Decimal
0b111000 //Binário
```

Agora vamos converter um número maior, `9872` : 
```c
//Operações iniciais (resto e divisão)
9872 % 2 = 0 (9872/2 = 4936)
4936 % 2 = 0 (4936/2 = 2468)
2468 % 2 = 0 (2468/2 = 1234)
1234 % 2 = 0 (1234/2 = 617)
617  % 2 = 1 (617/2  = 308)
308  % 2 = 0 (308/2  = 154)
154  % 2 = 0 (154/2  = 77)
77   % 2 = 1 (77/2   = 38)
38   % 2 = 0 (38/2   = 19)
19   % 2 = 1 (19/2   = 9)
9    % 2 = 1 (9/2    = 4)
4    % 2 = 0 (4/2    = 2)
2    % 2 = 0 (2/2    = 1)
1    % 2 = 1 (1/2    = 0)

//De forma que 
9872             //Decimal
0b10011010010000 //Binário
```