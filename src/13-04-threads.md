# Threads
Um *thread* descreve um fluxo de execução gerenciado pelo sistema operacional.

## Threads e Núcleos do processador
O que torna "possível" a execução de *threads* são os núcleos do processador, quando falamos que um processador tem 4 núcleos, é como se existissem 4 cópias do mesmo processador que conseguem executar 4 tarefas simultâneas.

A analogia mais simples para entender a relação do sistema operacional, com os núcleos do processador, processos e *threads* é a seguinte :
- Os núcleos do processador são "trabalhadores polímatas", que sabem fazer de tudo e executam tarefas
- Processos são conjuntos de uma ou mais tarefas relacionadas, toda tarefa está associada a um processo
- *threads* são tarefas que podem ser realizadas por qualquer trabalhador, mas todas exigem progresso constante e não podem ser deixadas de lado
- O sistema operacional é o gerente, que tenta garantir que todas tarefas estão progredindo e constantemente pede para os trabalhadores pararem o que estão fazendo para que outra tarefa não seja deixada de lado

Assim como na realidade, pedir para que trabalhadores parem o que estão fazendo, para realizar outra coisa, gera um atraso a mais, essa ação no contexto da computação é chamada de "troca de contexto" ou no inglês *context switch* e podemos pensar nela como o tempo perdido para que o trabalhador limpe sua mesa, anote e guarde os papéis da tarefa atual em uma pasta, para que no futuro possa continuar a tarefa.

Logo, se tivermos poucos trabalhadores e MUITAS tarefas, podemos acabar perdendo muito tempo com as constantes trocas de contexto. Um mundo ideal seria aquele onde cada trabalhador tem apenas uma tarefa ativa.

Diferente da realidade, no contexto da computação nenhuma tarefa pode ser realizada por mais de um trabalhador, se quisermos dividir uma ação, devemos dividí-la em tarefas menores e pensar em como isso deve ser feito de forma coordenada com o outro trabalhador que está fazendo algo similar.

Outra analogia que funciona bem, é que algumas tarefas envolvem esperar que algo fique pronto para prosseguir, nesse caso a tarefa fica suspensa e só deve continuar quando este "algo" estiver pronto, na computação, essas ações que necessitam de espera são chamadas de "bloqueantes" e colocam o *thread* para dormir, até que a ação seja concluída.

A existência de ações que podem ser suspensas faz com que seja ideal ter um pouco mais de tarefas do que trabalhadores, pois se tivermos o mesmo número de tarefas e trabalhadores e uma tarefa ficar suspensa, um trabalhador pode ficar sem ter o que fazer.

Em processadores modernos, temos o chamado "hyper-threading" onde um núcleo consegue executar 2 *threads* simultâneos, podemos pensar nisso como trabalhadores modernos que são tão eficientes que conseguem realizar duas tarefas ao mesmo tempo ou simplesmente como "isso significa que temos 2 vezes mais trabalhadores".

## Concorrência vs Paralelismo
Existe uma diferença entre concorrência e paralelismo, a concorrência se baseia na alternância de tarefas (ex: 2 *threads* executando em 1 núcleo) enquanto o paralelismo na execução simultânea delas (ex: 2 *threads* executando em 2 núcleos).

Gerenciar concorrência é mais fácil pois não precisamos nos atentar a conflitos por dois lugares diferentes executarem ao mesmo tempo, porém usar concorrência jamais tornará uma tarefa "mais rápida", pois estamos essencialmente alternando entre tarefas e introduzindo atrasos por conta da troca de contexto.

## Thread principal
Podemos pensar em um programa em C comum, como iniciando um único *thread* que executa a função `main`, por ser o primeiro a iniciar, ele normalmente é mencionado como o "*thread* principal" que finaliza quando a função `main` acaba ou quando `exit` ou outra função específica do sistema finaliza a execução do programa (ou uma finalização forçada por um erro crítico).

O *thread* principal é, obviamente, mais simples de lidar pois ele é iniciado de forma automática, quando o programa inicia. Já novos *threads* podem ser iniciados ao utilizar funções específicas de cada sistema operacional, ou bibliotecas e funções multiplataforma que o fazem internamente.

Para iniciar um novo *thread* geralmente precisamos das seguintes informações na função de criação: 
- Função onde o *thread* inicia
- Parâmetro inicial que será repassado a função inicial do *thread* novo
- Informações extras como tamanho da stack, prioridade,etc (que muitas vezes são opcionais)

## Threads em C
Agora vamos falar especificamente da situação do uso de threads em C. Desde o `C11` foram introduzidos threads a biblioteca padrão do C, através do header `threads.h`, porém, o suporte dos sistemas operacionais em fornecer essa implementação ainda é relativamente escasso. Além disso, algumas especificações falhas dos *threads*  do `C11` foram corrigidas somente no `C17`, contribuindo para o atraso.

Na prática, a biblioteca de *thread* mais bem suportada pela maioria dos sistemas é a [`pthread`](https://man7.org/linux/man-pages/man7/pthreads.7.html), que é a interface padronizada pelo POSIX para uso de *threads*. Parte da motivação vem que `pthread` já era o padrão da industria há mais de 10 anos antes da introdução dos threads do `C11`, além de ser mais completo em funcionalidades.

Ao utilizar threads recomenda-se que o usuário escolha entre:
- Use `pthread`, usando alguma biblioteca de compatibilidade onde não houver suporte nativo (como `winpthread`)
- Escrever sua própria biblioteca que usa diretamente as funções do sistema alvo
- Usar outras bibliotecas como [`tinycthread`](https://github.com/tinycthread/tinycthread), [`gthread`](https://docs.gtk.org/glib/struct.Thread.html) da GLIB,etc.

Neste guia vamos assumir o uso da `pthread` que é mais comum.

## pthread

## pthread - Gerenciamento de Threads

## pthread - Mutex
Mutex é uma sigla para "Mutual exclusion", no português, exclusão mútua. Ele é usado para proteger dados de modificações concorrentes

## pthread - Variáveis de Condição
O termo `cond` é uma abreviação para "condition variable", no português, variável de condição. Basicamente a variável de condição simboliza uma condição onde outros *threads*  podem esperar, até que ela seja cumprida

## pthread - Armazenamento local de Threads