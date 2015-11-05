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
		unsigned    m_j;
        unsigned    m_state;
		const char *m_p;

		unsigned    list();
		unsigned    element();
		unsigned    v();

		     int    isLetter(char c);

		// automaton simulation
		int simulate(const char *str, int j);

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
/*
class NFA
{
private:
    char* re; // match transitions
	Digraph G; // epsilon transition digraph
	int M; // number of states
public:	
    NFA(const char* regexp, const int lenght)
	{
		M = lenght;
		re = regexp;
		G = buildEpsilonTransitionsDigraph();
	}
	bool recognizes(const char* txt)
	{ 
		Bag<Integer> pc = new Bag<Integer>();
		DirectedDFS dfs = new DirectedDFS(G, 0);
		for (int v = 0; v < G.V(); v++)
		if (dfs.marked(v)) pc.add(v);
			for (int i = 0; i < txt.length(); i++)
			{
				Bag<Integer> match = new Bag<Integer>();
				for (int v : pc)
				{
					if (v == M) continue;
						if ((re[v] == txt.charAt(i)) || re[v] == '.')
							match.add(v+1);
				}
				dfs = new DirectedDFS(G, match);
				pc = new Bag<Integer>();
				for (int v = 0; v < G.V(); v++)
					if (dfs.marked(v)) pc.add(v);
			}
			for (int v : pc)
				if (v == M) return true;
			return false; 
	}
	Digraph buildEpsilonTransitionDigraph()
	{ 
		Digraph G = new Digraph(M+1);
		Stack<Integer> ops = new Stack<Integer>();
		for (int i = 0; i < M; i++) 
		{
			int lp = i;
			if (re[i] == '(' || re[i] == '|') 
				ops.push(i);
			else if (re[i] == ')')
			{
				int or = ops.pop();
				if (re[or] == '|') 
				{
					lp = ops.pop();
					G.addEdge(lp, or+1);
					G.addEdge(or, i);
				}
				else lp = or;
			}
			if (i < M-1 && re[i+1] == '*') 
			{
				G.addEdge(lp, i+1);
				G.addEdge(i+1, lp);
			}
			if (re[i] == '(' || re[i] == '*' || re[i] == ')')
			G.addEdge(i, i+1);
		}
		return G;
	}
};*/