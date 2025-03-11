# Operadores
Os operadores são simbolos utilizados para operar com valores e variaveis.

Os usos dos operadores são diversos e os mesmos são separados em diferentes categorias.

## Operadores de Atribuição
No geral os operadores de atribuição, são em sua maioria junções do operador `=` com outros operadores já existentes, portanto vou apenas listar os que existem (aconselho que veja o resto do texto logo a seguir, que explica cada categoria de operadores com mais detalhes)

Para que um valor possa ficar no lado esquerdo de uma atribuição (o lado que recebe), ele deve ao menos indicar um local no qual o valor que está sendo recebido será guardado (seja uma variavel, ou o conteúdo em um endereço de memória).

No geral é comum vermos menções disso nos seguintes termos em inglês: 
- `lvalue` : Valor que pode sofrer uma atribuição, vem de `Left Value` (valor a esquerda)
- `rvalue` : Valor que só pode ser usado para leitura, vem de `Right Value` (Valor a direita)

| Operador | Descrição                                      |
| -------- | ---------------------------------------------- |
| `=`      | Atribuição                                     |
| `+=`     | Soma e atribuição                              |
| `-=`     | Subtração e atribuição                         |
| `*=`     | Multiplicação e atribuição                     |
| `/=`     | Divisão e atribuição                           |
| `%=`     | Resto da divisão e atribuição                  |
| `<<=`    | Deslocamento de bit para esquerda e atribuição |
| `>>=`    | Deslocamento de bit para direita e atribuição  |
| `&=`     | AND bit a bit e atribuição                     |
| `^=`     | XOR e atribuição                               |
| `⏐=`     | OR bit a bit e atribuição                      |

## Operadores Aritméticos
Os operadores aritméticos são feitos para efetuar as quatro operações matemáticas básicas, junto das 
| Operador | Descrição        |
| -------- | ---------------- |
| `+`      | Soma             |
| `-`      | Subtração        |
| `*`      | Multiplicação    |
| `/`      | Divisão          |
| `++`     | Incremento       |
| `--`     | Decremento       |
| `%`      | Resto da divisão |

## Operadores Booleanos
Todas as operações booleanas resultam apenas em `Verdadeiro` ou `Falso`, dizer que um valor é "Booleano" significa que ele só pode ter um desses dois valores.

As definições de `Verdadeiro` e `Falso` : 
- `Falso` : Valor igual a zero
- `Verdadeiro` : Qualquer valor diferente de zero

Essas regras são válidas para qualquer lugar que deseja ler um valor qualquer como booleano.

Porém, para que seja possível atribuir `Verdadeiro` como um valor único, o valor utilizado para atribuir `Verdadeiro` na linguagem C é `1`.
Portanto todos os operadores booleanos sempre resultam em 0 (`Falso`) ou 1 (`Verdadeiro`).


| Operador | Descrição        |
| -------- | ---------------- |
| `==`     | Igual a          |
| `!=`     | Diferente de     |
| `>`      | Maior que        |
| `>=`     | Maior ou igual a |
| `<`      | Menor que        |
| `<=`     | Menor ou igual a |
| `!`      | NOT lógico       |
| `&&`     | AND lógico       |
| `⏐⏐`     | OR lógico        |

A maioria dos operadores são auto explicativos ao considerarmos a descrição juntamente ao fato de que todos operadores só retornam `Verdadeiro` ou `Falso`.

Com exceção, claro, dos três últimos operadores da tabela:

- `Not lógico` : Opera com apenas um valor, invertendo seu estado de `Verdadeiro`/`Falso`, de forma que `0` vire `1` e valores diferentes de zero, virem `0`, sendo literalmente a lógica ao usar `Não`. 
- `AND lógico`: Resulta em `Verdadeiro` quando ambos valores sejam `Verdadeiro`, sendo literalmente a lógica ao usar `E`. 
- `OR lógico`: Resulta em `Verdadeiro` quando um dos valores é `Verdadeiro`, sendo literalmente a lógica ao usar `Ou`.

> Agora, para exemplificar, vamos pegar de exemplo a seguinte frase :
>
> "João é inteligente, mas não passou na prova"
>
> Logo, "João é inteligente" = `Verdadeiro` e "João passou na prova" = `Falso`
>
> Ao usar `Not lógico`, "João `não` é inteligente", uma preposição que era `Verdadeira` se tornou `Falsa` pois invertemos o sentido dela.
>
> Ao usar `AND lógico`, "João é inteligente `e` passou na prova", vemos que a frase se torna `Falsa` pois João não passou na prova.
>
> Ao usar `OR lógico`, "João é inteligente `ou` passou na prova", a frase se torna `Verdadeira`, pois mesmo que não tenha passado, acertamos que João é inteligente.

### Avaliação de curto circuito 
Os operadores `AND lógico` e `OR lógico` apresentam um comportamento extremamente único, a chamada "avaliação de curto circuito", com ela, os valores só são avaliados quando isso é necessário.

Como assim quando isso é "necessário" ? bem se eu sei que um `AND lógico` só será `Verdadeiro` se ambos os valores forem `Verdadeiros`, ao saber que o primeiro dos valores é `Falso`, não há necessidade de avaliar o segundo valor para saber o resultado da expressão.

O mesmo se aplica para o `OR lógico`, se o primeiro valor é `Verdadeiro`, eu não preciso saber o resultado do segundo.

Logo caso o primeiro valor já seja o suficiente para estabelecer o resultado da expressão, a avaliação do segundo valor é ignorada, isso se torna ainda mais poderoso quando juntamos isso com [Funções](./6-funcoes.md).

```c
    /*
        Neste caso, não faria sentido se conectar ao servidor 
        se ele não estivesse funcionando, a ordem das checagens importa
        e a avaliação de curto circuito impede o programa de perder tempo
        tentando conectar num servidor que não está de pé
    */
    if(servidorEstaVivo() && conectarServidor())
        printf("Conectei!\n");
    
```

## Operadores Bit a Bit 
Antes de olhar esta parte sobre operadores bit a bit, aconselho fortemente que veja a seção sobre [Números Binários](#x-01-numeros-binarios.md), ou deixe este pedaço para depois.

Assume-se ao menos um conhecimento acerca de números binários e que o leitor saiba o que são BITS.

| Operador | Descrição                         |
| -------- | --------------------------------- |
| `~`      | NOT bit a bit                     |
| `&`      | AND bit a bit                     |
| `⏐`      | OR bit a bit                      |
| `^`      | XOR bit a bit                     |
| `<<`     | Deslocamento de bit para esquerda |
| `>>`     | Deslocamento de bit para direita  |

- `NOT bit a bit` : Realiza uma inversão de cada bit do valor individualmente.
- `AND bit a bit` : Realiza uma operação similar ao `AND lógico` em cada bit, gerando um resultado onde apenas os bits em comuns que os dois valores compartilham estão ativos.
- `OR bit a bit` : Realiza uma operação similar ao `OR lógico` em cada BIT, gerando um resultado onde os bits estão ativos a não ser que ambos os valores não tenham aquele bit ativo.
- `XOR bit a bit` : Traduzido, é um `OU Não Exclusivo`, seu papel é similar ao `OR lógico` porém se ambos os valores forem `Verdadeiros`, o resultado é `Falso`, porém o `XOR` em C não tem versão lógica, portanto ele é sempre bit a bit.
- `Deslocamento de bit para esquerda`: Desloca os bits para esquerda, resultando geralmente, numa multiplicação por 2 elevado na `X`, sendo `X` o número do deslocamento (claro que para valores com sinal, isso muda pois podemos acabar gerando um número negativo ao alcançar o bit do sinal)
- `Deslocamento de bit para direita`: Desloca os bits para direita, resultando geralmente, numa divisão por 2 elevado na `X`, sendo `X` o número do deslocamento.

Operações comuns utilizando os operadores bit a bit: 
```c
    //Ativar um bit
    valor |= bit; 

    //Remover um bit
    valor &= ~bit;

    //Checar se um bit está ativo
    if(valor & bit)
        printf("Está ativo\n");

    //Inverter o estado do bit (com XOR)
    valor ^= bit; 
```

## Ordem de precedência

A ordem de precedência é a prioridade com a qual os operadores são aplicados.

Os itens mais no topo da tabela a seguir, são os itens de maior prioridade (ordem decrescente): 

| Operador(es)      | Descrição                                        | Associatividade       |
| ----------------- | ------------------------------------------------ | --------------------- |
| `++`/`--`         | Incremento/Decremento (sufixo)                   | Esquerda para direita |
| `()`              | Chamada de função                                | Esquerda para direita |
| `[]`              | Subscrição de array/matriz                       | Esquerda para direita |
| `.`               | Acesso de membro de estrutura/união              | Esquerda para direita |
| `->`              | Acesso de membro de estrutura/união via ponteiro | Esquerda para direita |
| `(tipo){lista}`   | Literal composto (`C99`)                         | Esquerda para direita |
| `++`/`--`         | Incremento/Decremento (prefixo)                  | Direita para esquerda |
| `+`/`-`           | Sinal de valor (ex: +5 ou -5)                    | Direita para esquerda |
| `!`/`~`           | NOT lógico e bit a bit                           | Direita para esquerda |
| `(tipo)`          | Conversão de tipo                                | Direita para esquerda |
| `*`               | Indireção (desreferenciar)                       | Direita para esquerda |
| `&`               | Endereço de                                      | Direita para esquerda |
| `sizeof`          | Tamanho de                                       | Direita para esquerda |
| `_AlignOf`        | Alinhamento de (`C11`)                           | Direita para esquerda |
| `*`/`/`/`%`       | Multiplicação, divisão e resto                   | Esquerda para direita |
| `+`/`-`           | Soma e subtração                                 | Esquerda para direita |
| `<<`/`>>`         | Deslocamento de bit                              | Esquerda para direita |
| `<`/`<=`/`>`/`>=` | Comparações de maior/menor                       | Esquerda para direita |
| `==`/`!=`         | Igual a ou diferente de                          | Esquerda para direita |
| `&`               | AND bit a bit                                    | Esquerda para direita |
| `^`               | XOR bit a bit                                    | Esquerda para direita |
| `⏐`               | OR bit a bit                                     | Esquerda para direita |
| `&&`              | AND lógico                                       | Esquerda para direita |
| `⏐⏐`              | OR lógico                                        | Esquerda para direita |
| `?:`              | Condição ternária                                | Direita para esquerda |
| `=`               | Atribuição                                       | Direita para esquerda |
| `+=`/`-=`         | Soma/subtração e atribuição                      | Direita para esquerda |
| `*=`/`/=`/`%=`    | Soma/subtração e atribuição                      | Direita para esquerda |
| `<<=`/`>>=`       | Soma/subtração e atribuição                      | Direita para esquerda |
| `&=`/`^=`/`⏐=`    | AND/XOR/OR bit a bit e atribuição                | Direita para esquerda |





> No geral muitos programadores utilizam parenteses para forçar uma certa ordem de precedência
> Isso diminui a necessidade de um estudo mais minucioso dessa tabela, ao mesmo tempo que facilita a leitura do código
> para pessoas que não a decoraram, ou que tem pouca experiência