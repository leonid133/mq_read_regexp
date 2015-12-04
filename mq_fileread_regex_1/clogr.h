// author Блохин Л.Н leonidblohin@gmail.com
#pragma once
#include <tchar.h>
#include <windef.h>

#define MAXSTATES	100
// error codes of RegExpr::search and RegExpr::SearchInLine
#define REGEXPR_NOT_FOUND		-1
#define REGEXPR_NOT_COMPILED	-2

class CLogReader
  {
private:
      HANDLE    m_h_file_;
      LPVOID    m_read_mem_buf;
      LPVOID    m_work_mem_buf;
      
       DWORD    m_virt_buff_size;
       DWORD    m_file_size;
unsigned int    m_it_read_counter;
       
       char*    m_buf_null;      
unsigned int    m_max_byte_file_size;

       char*    m_filter;
      size_t    m_size_filter;

        bool    ReadFileNextBuf( OVERLAPPED* ov);

         int    readed_data;
        bool    buffer_empty;
  //Sedgewick RegExp 
        bool    any_occurrence;
         int    m_line_counter;
         
         struct m_state_struct
         {
             char char_state;
             unsigned next1;
             unsigned next2;
         } aut_state[MAXSTATES];

        void    clear_aut_state();
        void    compile(const char * pattern);
         int    SearchInLine( const char * str, unsigned start = 0); 
         //const char *operator = (const char *pattern);

        class   Deque
        {
            public:
                Deque();
                ~Deque();

                void push(int n);
                void put(int n);
                int pop();

                int isEmpty();
                void Clean();

            protected:
                struct Element 
                {
                    int n;
                    Element *next;
                } *head, *tail;
        } m_deque;

      // compiler
         int    n_match;
unsigned int    m_regex_char_counter;
         int    m_state_counter;
  const char    *m_regex_pattenn;

    unsigned    list();
    unsigned    element();
    unsigned    v();

         int    isLetter(char c);

        // aut_state simulation
         int    simulate( const char const *& str, int j );

public:
                CLogReader();
               ~CLogReader();
        bool    Open( const TCHAR szFileName[100] );   // открытие файла, false - ошибка
        void    Close();                         // закрытие файла
        bool    SetFilter( char * filter);   // установка фильтра строк, false - ошибка
        bool    GetNextLine( char *& buf,           // запрос очередной найденной строки, 
                           const int bufsize);  // buf - буфер, bufsize - максимальная длина
                                                // false - конец файла или ошибка
  };
/*
Представление	Число повторений	Эквивалент	Пример	Соответствие
.* - последовательность любых символов неограниченной длины;
. - один любой символ;
?	Ноль или одно	{0,1}	colou?r	color, colour
*	Ноль или более	{0,}	colou*r	color, colour, colouur и т. д.
+	Одно или более	{1,}	colou+r	colour, colouur и т. д. (но не color)
(ОТК|ЗАК)РЫВАШКИ
*/
