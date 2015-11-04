#pragma once
#include <tchar.h>
#include <windef.h>

class CLogReader
  {
private:
      HANDLE    m_h_file_;
      LPVOID    m_virt_mem_;
      LPVOID     m_virt_work_;
      DWORD     m_virt_buff_;
      
unsigned int    m_max_byte_file_size;
 const char*    m_filter;
   
  void    Close();                         // закрытие файла

public:
           CLogReader();
          ~CLogReader();
   bool    Open( const TCHAR szFileName[100] );   // открытие файла, false - ошибка
   
   bool    SetFilter(const char *filter);   // установка фильтра строк, false - ошибка
   bool    GetNextLine(char *buf,           // запрос очередной найденной строки, 
                       const int bufsize);  // buf - буфер, bufsize - максимальная длина
                                            // false - конец файла или ошибка
  };