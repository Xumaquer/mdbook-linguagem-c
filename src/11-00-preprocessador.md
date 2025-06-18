# Preprocessador
Chamamos de preprocessamento uma etapa que ocorre antes da compilação, nessa etapa um arquivo é "preprocessado" e utilizado para gerar um arquivo final que será repassado ao compilador.

Chamamos de diretivas de preprocessador os comandos que atuam na etapa de preprocessamento e são executados antes da compilação.

As diretivas de preprocessador podem ser utilizadas para :
- Adicionar textos ou bytes de outros arquivos para o código fonte
- Abortar a compilação devido a algum erro
- Gerar avisos ao compilar, indicando que algo está errado ou deve ser tratado com cuidado
- Adicionar ou remover diretivas de preprocessador ou código condicionalmente
- Especificar detalhes extras que podem modificar o comportamento do compilador

A sintaxe para uma diretiva de preprocessador é : 
```c
#DIRETIVA ARGUMENTOS
```
- `#` é obrigatório e indica que é uma diretiva de preprocessador
- `DIRETIVA` é o nome da diretiva de preprocessador (`define`, `include`, `ifdef`, etc)
- `ARGUMENTOS` são os argumentos da diretiva, que dependem da diretiva utilizada e podem ou não ser opcionais

Toda diretiva de preprocessador é normalmente finalizada quando colocamos uma nova linha, porém podemos utilizar o caractere `\` para extender a diretivas em mais linhas : 
```c
//Diretiva com uma linha
#define ALFABETO "abcdefghijklmnopqrstuvwxyz"

//Extendida em mais linhas utilizando "\"
#define MENSAGEM \
 "Ola como vai\n" \
 "Você?\n"
```