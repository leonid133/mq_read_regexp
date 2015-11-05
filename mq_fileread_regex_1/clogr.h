/*
�������� �������. ������ ����� � ���������. ������ ������
�������������� ����������:
WinApi 
ReadFile
1. http://www.cyberforum.ru/win-api/thread1361909.html

2. http://zetblog.ru/programming/200902/winapi-%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0-%D1%81-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0%D0%BC%D0%B8-%D0%BE%D1%81%D0%BD%D0%BE%D0%B2%D0%BD%D1%8B%D0%B5-%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D0%B8/

���������� ������ �������� �� �����, �����, ��� ������������ ������ �������, ������ ���. ������� ������ ������ � 100�� �� ������������, � ������� ��� � ������. 
����� ������ ������� �� ����� ��������� ���������, ������� �������� ��������, ���� ����� ��� ����� ������� �������� ������ ����� �� ����������. 

Regexp 
http://www.codeproject.com/Articles/798/Fast-regular-expressions

2. http://rus-linux.net/nlib.php?name=/MyLDP/algol/cpattern/Regular_Expressions_in_C_ru.html

3. http://rutracker.org/forum/viewtopic.php?t=3539489

4. http://www.softpanorama.org/Algorithms/pattern_matching.shtml

5. http://rutracker.org/forum/viewtopic.php?t=4294269 ������ �������

6. http://www.tldp.org/LDP/LG/issue27/mueller.html

�� ���������� Sedgewick ��������� ����� ������� ��������, �� ��� ����� ����� ���������, ��� ����������, � ��� ������. 
�������� ���������� ������� ���, �� �� �������� ��������, ����� ���, � ����� �� ������������.

*/

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

unsigned int    m_line_counter;
  //Sedgewick RegExp 
        

public:
           CLogReader();
          ~CLogReader();
   bool    Open( const TCHAR szFileName[100] );   // �������� �����, false - ������
   void    Close();                         // �������� �����
   bool    SetFilter(const char *filter);   // ��������� ������� �����, false - ������
   bool    GetNextLine(char *buf,           // ������ ��������� ��������� ������, 
                       const int bufsize);  // buf - �����, bufsize - ������������ �����
                                            // false - ����� ����� ��� ������
  };
