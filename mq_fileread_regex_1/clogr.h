#pragma once
#include <tchar.h>
#include <windef.h>

#define MAXSTATES	100
// error codes of RegExpr::search and RegExpr::searchLen
#define REGEXPR_NOT_FOUND		-1
#define REGEXPR_NOT_COMPILED	-2

class CLogReader
  {
private:
      HANDLE    m_h_file_;
      
      LPVOID    m_virt_work_;
       DWORD    m_virt_buff_size;
       DWORD    m_file_size;
      
unsigned int    m_max_byte_file_size;
 const char*    m_filter;
      size_t    m_size_filter;

  //Sedgewick RegExp 
         int    m_line_counter;
         
         struct State
         {
	         char the_char;
	         unsigned next1;
             unsigned next2;
         } automaton[MAXSTATES];

        void    clear_automaton();
        void    compile(const char *pattern);
	     //int    search(const char *str, unsigned start = 0);
	     int    searchLen(const char *str, unsigned start = 0); 
         //const char *operator = (const char *pattern);

                class Deque
                {
	                public:
		                Deque();
		                ~Deque();

		                void push(int n);
		                void put(int n);
		                int pop();

		                int isEmpty();

	                protected:
		                struct Element 
                        {
			                int n;
			                Element *next;
		                } *head, *tail;
                } deque;

      // compiler
	unsigned int    m_j;
             int    m_state;
	  const char *m_p;

		unsigned    list();
		unsigned    element();
		unsigned    v();

		     int    isLetter(char c);

		// automaton simulation
		int simulate(const char *str, int j, bool &match_b);

public:
           CLogReader();
          ~CLogReader();
   bool    Open( const TCHAR szFileName[100] );   // открытие файла, false - ошибка
   void    Close();                         // закрытие файла
   bool    SetFilter(const char *filter);   // установка фильтра строк, false - ошибка
   bool    GetNextLine(char *buf,           // запрос очередной найденной строки, 
                       const int bufsize);  // buf - буфер, bufsize - максимальная длина
                                            // false - конец файла или ошибка
  };
