#if defined(_UCRT) && defined(_WIN32)
    #define fclose_unlocked _fclose_nolock
    #define fflush_unlocked _fflush_nolock
    #define fgetc_unlocked  _fgetc_nolock
    #define fgetwc_unlocked _fgetwc_nolock
    #define fread_unlocked  _fread_nolock
    #define fseek_unlocked  _fseek_nolock
    #define fseek64_unlocked _fseeki64_nolock
    #define ftell_unlocked   _ftell_nolock
    #define ftell64_unlocked _ftelli64_nolock
    #define fwrite_unlocked  _fwrite_nolock
    #define getc_unlocked   _getc_nolock
    #define getwc_unlocked  _getwc_nolock
    #define putc_unlocked   _putc_nolock
    #define putwc_unlocked  _putwc_nolock
    #define putch_unlocked  _putch_nolock
    #define putwch_unlocked _putwch_nolock
    #define putchar_unlocked _putchar_nolock
    #define putwchar_unlocked _putwchar_nolock
    #define ungetc_unlocked   _ungetc_nolock
    #define ungetwc_unlocked  _ungetwc_nolock
    #define ungetch_unlocked  _ungetch_nolock
    #define ungetwch_unlocked _ungetwch_nolock

    //Funções indisponíveis
    #define feof_unlocked feof
    #define ferror_unlocked ferror
    #define fgets_unlocked fgets
    #define fgetws_unlocked fgetws
    #define fputs_unlocked fputs
    #define fputws_unlocked fputws
#elif defined(__linux__)
    #define fclose_unlocked fclose
    #define fseek_unlocked fseek
    #define fseek64_unlocked fseek
    #define ftell_unlocked ftell
    #define ftell64_unlocked ftell64

    #define ungetc_unlocked   ungetc
    #define ungetwc_unlocked  ungetwc
    #define ungetch_unlocked  ungetch
    #define ungetwch_unlocked ungetwch
#else 
    #define fclose_unlocked fclose
    #define fflush_unlocked fflush
    #define fgetc_unlocked  fgetc
    #define fgetwc_unlocked fgetwc
    #define fread_unlocked  fread
    #define fseek_unlocked  fseek
    #define ftell_unlocked   ftell
    #define fwrite_unlocked  fwrite
    #define getc_unlocked   getc
    #define getwc_unlocked  getwc
    #define putc_unlocked   putc
    #define putwc_unlocked  putwc
    #define putch_unlocked  putch
    #define putwch_unlocked putwch
    #define putchar_unlocked putchar
    #define putwchar_unlocked putwchar
    #define ungetc_unlocked   ungetc
    #define ungetwc_unlocked  ungetwc
    #define ungetch_unlocked  ungetch
    #define ungetwch_unlocked ungetwch
    #define feof_unlocked feof
    #define ferror_unlocked ferror
    #define fgets_unlocked fgets
    #define fgetws_unlocked fgetws
    #define fputs_unlocked fputs
    #define fputws_unlocked fputws

    #ifdef _WIN32
        #define fseek64_unlocked _fseeki64
        #define ftell64_unlocked _ftelli64
    #else
        #define fseek64_unlocked seek
        #define ftell64_unlocked tell
    #endif 
#endif