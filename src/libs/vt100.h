#ifdef _WIN32
    #include <windows.h>
#endif 

#define VT100_ESC "\x1b"

//Posicionamento do cursor
#define VT100_CURSOR_UP1     VT100_ESC"M" //Sobe o cursor em 1 posição
#define VT100_CURSOR_SAVE    VT100_ESC"7" //Salva a posição do cursor
#define VT100_CURSOR_RESTORE VT100_ESC"8" //Restaura a posição salva do cursor
 
#define VT100_CURSOR_UP(X)        VT100_ESC"["#X"A"      //Movimenta o cursor `X` posições para cima
#define VT100_CURSOR_DOWN(X)      VT100_ESC"["#X"B"      //Movimenta o cursor `X` posições para baixo
#define VT100_CURSOR_BACK(X)      VT100_ESC"["#X"C"      //Retrocede o cursor `X` posições
#define VT100_CURSOR_NEXT(X)      VT100_ESC"["#X"D"      //Avança o cursor `X` posições
#define VT100_CURSOR_LINE_DOWN(X) VT100_ESC"["#X"E"      //Desce `X` linhas com o cursor
#define VT100_CURSOR_LINE_UP(X)   VT100_ESC"["#X"F"      //Sobe `X` linhas com o cursor
#define VT100_CURSOR_ABS_HORZ(X)  VT100_ESC"["#X"G"      //Move o cursor para a posição `X` absoluta horizontalmente
#define VT100_CURSOR_ABS_VERT(X)  VT100_ESC"["#X"d"      //Move o cursor para a posição `X` absoluta verticalmente
#define VT100_CURSOR_MOVE(X,Y)    VT100_ESC"["#Y";"#X"H" //Move o cursor para a posição `X`,`Y` especificada

//Efeitos do cursor
#define VT100_CURSOR_BLINK    VT100_ESC"[?12h" //Habilita a piscada periódica do cursor
#define VT100_CURSOR_NO_BLINK VT100_ESC"[?12l" //Desabilita a piscada periódica do cursor
#define VT100_CURSOR_SHOW     VT100_ESC"[?25h" //Deixa o cursor visível
#define VT100_CURSOR_HIDE     VT100_ESC"[?25l" //Deixa o cursor invisível

//Formato do cursor
#define VT100_CURSOR_DEFAULT          VT100_ESC"[0 q" //Padrão
#define VT100_CURSOR_BLINK_BLOCK      VT100_ESC"[1 q" //Bloco piscante
#define VT100_CURSOR_STEADY_BLOCK     VT100_ESC"[2 q" //Bloco não piscante
#define VT100_CURSOR_BLINK_UNDERLINE  VT100_ESC"[3 q" //Underline piscante
#define VT100_CURSOR_STEADY_UNDERLINE VT100_ESC"[4 q" //Underline não piscante
#define VT100_CURSOR_BLINK_BAR        VT100_ESC"[5 q" //Barra piscante
#define VT100_CURSOR_STEADY_BAR       VT100_ESC"[6 q" //Barra não piscante

//Scroll
#define VT100_SCROLL_UP(X)   VT100_ESC"["#X"S" //Movimenta o texto para cima
#define VT100_SCROLL_DOWN(X) VT100_ESC"["#X"T" //Movimenta o texto para baixo

//Modificação de texto
#define VT100_INSERT_SPACES(X) VT100_ESC"["#X"@" //Insere `X` espaços partindo da posição atual (empurrando o texto após o cursor para direita)
#define VT100_DELETE_CHARS(X)  VT100_ESC"["#X"P" //Apaga `X` caracteres, adicionando espaços pelo canto direito da tela
#define VT100_ERASE_CHARS(X)   VT100_ESC"["#X"X" //Apaga `X` caracteres, sobrescrevendo caracteres com espaço
#define VT100_INSERT_LINES(X)  VT100_ESC"["#X"L" //Insere `X` linhas, deslocando a linha que o cursor está e todas as posteriores
#define VT100_DELETE_LINES(X)  VT100_ESC"["#X"M" //Apaga `X` linhas, começando pela linha em que o cursor está
#define VT100_CLEAR            VT100_ESC"[2J"    //Limpa a tela
#define VT100_CLEAR_SCROLL     VT100_ESC"[3J"    //Limpa a scroll

//Volta pro início, limpa tela e scroll
#define VT100_CLEAR_SEQUENCE   VT100_INPUT_HOME VT100_CLEAR VT100_CLEAR_SCROLL 

#define VT100_ERASE_LINE(X)    VT100_ESC"["#X"M" //Substitui todo texto na linha especificada por espaço

//Cores (genérico)
#define VT100_DEFAULT_COLOR VT100_ESC"[0m" //Retorna a cor padrão
#define VT100_SWAP_COLOR    VT100_ESC"[7m" //Troca a cor de fundo com a cor do texto

//Cor/estilo do texto (Modificar a cor e estilo do texto não altera o que já foi desenhado)
#define VT100_TEXT_DEFAULT      VT100_ESC"[39m" //Volta a cor do texto para o padrão
#define VT100_TEXT_BOLD         VT100_ESC"[1m"  //Deixa o texto em negrito
#define VT100_TEXT_NO_BOLD      VT100_ESC"[22m" //Desabilita o texto em negrito
#define VT100_TEXT_UNDERLINE    VT100_ESC"[4m"  //Texto com underline
#define VT100_TEXT_NO_UNDERLINE VT100_ESC"[24m" //Texto sem underline
#define VT100_TEXT_BLACK        VT100_ESC"[30m" //Muda a cor do texto para preto
#define VT100_TEXT_RED          VT100_ESC"[31m" //Muda a cor do texto para vermelho
#define VT100_TEXT_GREEN        VT100_ESC"[32m" //Muda a cor do texto para verde
#define VT100_TEXT_YELLOW       VT100_ESC"[33m" //Muda a cor do texto para amarelo
#define VT100_TEXT_BLUE         VT100_ESC"[34m" //Muda a cor do texto para azul
#define VT100_TEXT_MAGENTA      VT100_ESC"[35m" //Muda a cor do texto para magenta
#define VT100_TEXT_CYAN         VT100_ESC"[36m" //Muda a cor do texto para ciano
#define VT100_TEXT_WHITE        VT100_ESC"[37m" //Muda a cor do texto para branco
#define VT100_TEXT_BOLD_BLACK   VT100_ESC"[90m" //Muda a cor do texto para preto em negrito
#define VT100_TEXT_BOLD_RED     VT100_ESC"[91m" //Muda a cor do texto para vermelho em negrito
#define VT100_TEXT_BOLD_GREEN   VT100_ESC"[92m" //Muda a cor do texto para verde em negrito
#define VT100_TEXT_BOLD_YELLOW  VT100_ESC"[93m" //Muda a cor do texto para amarelo em negrito
#define VT100_TEXT_BOLD_BLUE    VT100_ESC"[94m" //Muda a cor do texto para azul em negrito
#define VT100_TEXT_BOLD_MAGENTA VT100_ESC"[95m" //Muda a cor do texto para magenta em negrito
#define VT100_TEXT_BOLD_CYAN    VT100_ESC"[96m" //Muda a cor do texto para ciano em negrito
#define VT100_TEXT_BOLD_WHITE   VT100_ESC"[97m" //Muda a cor do texto para branco em negrito

//Cor do fundo
#define VT100_BKGND_DEFAULT      VT100_ESC"[49m"  //Volta a cor do fundo para o padrão
#define VT100_BKGND_BLACK        VT100_ESC"[40m"  //Muda a cor do fundo para preto
#define VT100_BKGND_RED          VT100_ESC"[41m"  //Muda a cor do fundo para vermelho
#define VT100_BKGND_GREEN        VT100_ESC"[42m"  //Muda a cor do fundo para verde
#define VT100_BKGND_YELLOW       VT100_ESC"[43m"  //Muda a cor do fundo para amarelo
#define VT100_BKGND_BLUE         VT100_ESC"[44m"  //Muda a cor do fundo para azul 
#define VT100_BKGND_MAGENTA      VT100_ESC"[45m"  //Muda a cor do fundo para magenta
#define VT100_BKGND_CYAN         VT100_ESC"[46m"  //Muda a cor do fundo para ciano
#define VT100_BKGND_WHITE        VT100_ESC"[47m"  //Muda a cor do fundo para branco
#define VT100_BKGND_BOLD_BLACK   VT100_ESC"[100m" //Muda a cor do fundo para preto em negrito
#define VT100_BKGND_BOLD_RED     VT100_ESC"[101m" //Muda a cor do fundo para vermelho em negrito
#define VT100_BKGND_BOLD_GREEN   VT100_ESC"[102m" //Muda a cor do fundo para verde em negrito
#define VT100_BKGND_BOLD_YELLOW  VT100_ESC"[103m" //Muda a cor do fundo para amarelo em negrito
#define VT100_BKGND_BOLD_BLUE    VT100_ESC"[104m" //Muda a cor do fundo para azul em negrito
#define VT100_BKGND_BOLD_MAGENTA VT100_ESC"[105m" //Muda a cor do fundo para magenta em negrito
#define VT100_BKGND_BOLD_CYAN    VT100_ESC"[106m" //Muda a cor do fundo para ciano em negrito 
#define VT100_BKGND_BOLD_WHITE   VT100_ESC"[107m" //Muda a cor do fundo para branco em negrito

#define VT100_TEXT_RGB(RR,GG,BB)  VT100_ESC"[38;2;"#RR";"#GG";"#BB"m" //Muda a cor do texto para qualquer cor RGB (os valores devem estar em decimal) 
#define VT100_BKGND_RGB(RR,GG,BB) VT100_ESC"[48;2;"#RR";"#GG";"#BB"m" //Muda a cor do fundo para qualquer cor RGB (os valores devem estar em decimal)


//Buffer de tela (permite mudar para uma tela alternativa, sem afetar a principal)
#define VT100_SCREEN_BUFFER_ALT  VT100_ESC"[?1049h" //Muda para a tela alternativa
#define VT100_SCREEN_BUFFER_MAIN VT100_ESC"[?1049l" //Muda para a tela principal

//Customização da janela
#define VT100_WND_TITLE(TITLE) VT100_ESC"]2;"TITLE"\x1b\x5c"

//Sequências de entrada (podem ser lidas pela aplicação para ler teclas adicionais)
#define VT100_INPUT_ARROW_UP    VT100_ESC"[A"    //Lido quando o usuário pressiona a seta para cima
#define VT100_INPUT_ARROW_DOWN  VT100_ESC"[B"    //Lido quando o usuário pressiona a seta para baixo
#define VT100_INPUT_ARROW_RIGHT VT100_ESC"[C"    //Lido quando o usuário pressiona a seta para direita
#define VT100_INPUT_ARROW_LEFT  VT100_ESC"[D"    //Lido quando o usuário pressiona a seta para esquerda
#define VT100_INPUT_HOME        VT100_ESC"[H"    //Lido quando o usuário pressiona a tecla "home"
#define VT100_INPUT_END         VT100_ESC"[F"    //Lido quando o usuário pressiona a tecla "end"
#define VT100_INPUT_CTRL_UP     VT100_ESC"[1;5A" //Lido quando o usuário pressiona CTRL + seta para cima
#define VT100_INPUT_CTRL_DOWN   VT100_ESC"[1;5B" //Lido quando o usuário pressiona CTRL + seta para baixo
#define VT100_INPUT_CTRL_RIGHT  VT100_ESC"[1;5C" //Lido quando o usuário pressiona CTRL + seta para direita
#define VT100_INPUT_CTRL_LEFT   VT100_ESC"[1;5D" //Lido quando o usuário pressiona CTRL + seta para esquerda
#define VT100_INPUT_BACKSPACE   "\x7f"           //Lido quando o usuário pressiona Backspace
#define VT100_INPUT_PAUSE       "\x1a"           //Lido quando o usuário pressiona Pause
#define VT100_INPUT_ESCAPE      "\x1B"           //Lido quando o usuário pressiona ESC
#define VT100_INPUT_INSERT      VT100_ESC"[2~"   //Lido quando o usuário pressiona Insert
#define VT100_INPUT_DELETE      VT100_ESC"[3~"   //Lido quando o usuário pressiona Delete
#define VT100_INPUT_PAGE_UP     VT100_ESC"[5~"   //Lido quando o usuário pressiona page up
#define VT100_INPUT_PAGE_DOWN   VT100_ESC"[6~"   //Lido quando o usuário pressiona page down
#define VT100_INPUT_F1          VT100_ESC"OP"    //Lido quando o usuário pressiona F1
#define VT100_INPUT_F2          VT100_ESC"OQ"    //Lido quando o usuário pressiona F2
#define VT100_INPUT_F3          VT100_ESC"OR"    //Lido quando o usuário pressiona F3
#define VT100_INPUT_F4          VT100_ESC"OS"    //Lido quando o usuário pressiona F4
#define VT100_INPUT_F5          VT100_ESC"O[15~" //Lido quando o usuário pressiona F5
#define VT100_INPUT_F6          VT100_ESC"O[17~" //Lido quando o usuário pressiona F6
#define VT100_INPUT_F7          VT100_ESC"O[18~" //Lido quando o usuário pressiona F7
#define VT100_INPUT_F8          VT100_ESC"O[19~" //Lido quando o usuário pressiona F8
#define VT100_INPUT_F9          VT100_ESC"O[20~" //Lido quando o usuário pressiona F9
#define VT100_INPUT_F10         VT100_ESC"O[21~" //Lido quando o usuário pressiona F10
#define VT100_INPUT_F11         VT100_ESC"O[23~" //Lido quando o usuário pressiona F11
#define VT100_INPUT_F12         VT100_ESC"O[24~" //Lido quando o usuário pressiona F12

/**
 * @brief Realiza a preparação para uso de UTF-8 + VT100 no windows 
 * Não é necessário fazer nada no linux e macOs
 */
static void vt100_prepare()
{
    #ifdef _WIN32
    HANDLE hStdin  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    DWORD mode;

    if(GetFileType(hStdin) == FILE_TYPE_CHAR) {
        SetConsoleCP(CP_UTF8);
    }
    if(GetFileType(hStdout) == FILE_TYPE_CHAR) {
        if(GetConsoleMode(hStdout, &mode))
            SetConsoleMode(hStdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleOutputCP(CP_UTF8);
    }
    #endif 
}
