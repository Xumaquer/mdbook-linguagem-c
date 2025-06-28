# Atributos 
Desde o `C23`, foram adicionados os "atributos" a linguagem, que são uma forma de sinalizar detalhes adicionais ao compilador, uma funcionalidade que até então só existia no `C++` (adicionada no `C++11`).

Os atributos servem, similar ao `#pragma`, como uma forma padronizada e unificada de indicar detalhes ao compilador, que normalmente eram efetuados utilizando extensões de compilador (como `__attribute__` do GCC e IBM, ou `__declspec` do MSVC).

Declaração de atributos : 
```c
[[atributo-padrao]]               //1° forma
[[prefixo::atributo]]             //2° forma
[[atributo-padrao(argumentos)]]   //3° forma
[[prefixo::atributo(argumentos)]] //4° forma
```
Todos atributos específicos de compilador tem um `prefixo` e `::` antes do nome efetivo do atributo, já os atributos padrão da linguagem são escritos sem prefixo.

Alguns atributos podem ter um ou mais argumentos, que ficam entre parênteses, caso haja mais de um, eles são separados por virgula.

Exemplo do uso de atributos:
```c
float f1 = 5.4;

//Atributo padrão para inibir
//avisos do compilador sobre variável não utilizada
[[maybe_unused]]
float f2 = 9.2;

//Usando atributo para especificar
//exceção a regra de strict aliasing ao GCC (C23)
[[gnu::may_alias]]
int *i1 = (int*)&f1;

//Utilizando extensão de compilador (antes do C23)
__attribute((__may_alias__)) int *i2 = (int*)&f1;
```

## Atributos padrão
Os seguintes atributos são considerados padrão da linguagem:

- `[[deprecated]]`: Indica que a função ou variável associada é depreciada e não é recomendada, gerando um aviso do compilador.
- `[[deprecated("motivo")]]`: Versão do atributo acima, com um parâmetro indicando a mensagem a ser exibida ao gerar o aviso.
- `[[fallthrough]]`: Indica que continuar a execução para o próximo `case` por conta da ausência da palavra chave `break` numa estrutura de controle de fluxo `switch` é intencional.
- `[[nodiscard]]`: Faz com que o compilador gere um aviso se o valor de retorno for ignorado.
- `[[nodiscard("motivo")]]`: Versão do atributo acima, com um parâmetro indicando a mensagem a ser exibida ao gerar o aviso.
- `[[maybe_unused]]`: Impede o compilador de gerar avisos se a variável/função associada não for utilizada.
- `[[noreturn]]`: Indica que a função não retorna.
- `[[reproducible]]`: Indica que a função é `sem efeito` e `independente`, isso permite que o compilador trate múltiplas chamadas em sequência como uma única chamada.
- `[[unsequenced]]`: Indica que a função é `sem efeito`, `idempotente`, `sem estado` e `independente`, permite que o compilador trate múltiplas chamadas em sequência como uma única chamada e também que elas sejam paralelizadas e reordenadas de forma arbitrária.

Para entender melhor os atributos `unsequenced` e `reproducible`, as funções podem ou não ser classificadas como : 
- `Sem efeito`: Uma função é `sem efeito` se ela modifica apenas variáveis locais, ou valores através de um único ponteiro repassado como parâmetro a função.
- `Idempotente`: Uma função é `idempotente` quando outra chamada dela pode ser executada logo em sequência sem alterar o resultado ou o estado observável de execução.
- `Sem Estado`: Uma função é considerada `sem estado`, se qualquer definição de variável de duração `estática` ou de `thread` nela ou em alguma função que ela chame seja `const` e não tenha o especificador `volatile`.
- `Independente`: Uma função é `independente` quando ela não modifica nenhum parâmetro via ponteiro, não modifica o estado de variáveis globais nem o estado global do programa ou sistema operacional.

Para compreender de maneira prática (quais funções poderiam ter os atributos `unsequenced` ou `reproducible`): 
- `printf` não poderia ter nenhum
- `strlen` e `memcmp` podem ser `[[unsequenced]]`
- `memcpy` pode ser `[[reproducible]]`
- `memmove` não pode ser nenhum dos dois, por que ele não é `idempotente` para regiões de memória que se sobrepõem
- `fabs` pode ser `[[unsequenced]]`
- `sqrt` não pode ser nenhum dos dois, pois ele modifica o ambiente de ponto flutuante e pode modificar `errno`

Os exemplos acima foram retirados [dessa resposta do StackOverflow](https://stackoverflow.com/questions/76847905/what-are-the-reproducible-and-unsequenced-attributes-in-c23-and-when-sh).

## Checar atributo
Podemos checar se um atributo existe utilizando `__has_c_attribute` dentro de um `#if` ou `#elif`.

O valor resultante da expressão `__has_c_attribute` difere entre atributos padrão e atributos específicos de compiladores : 
- Atributos padrão tem tipo `long` e valor indicando o ano e mês em que o atributo foi adicionado aos documentos de especificação da linguagem, ex: um atributo adicionado em Abril de 2019 terá o valor `201904L`. 
- Atributos específicos de compilador são considerados como existentes caso tenham valor diferente de zero.
- Atributos inexistentes terão o seu valor zerado.
