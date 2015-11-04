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
   
  void    Close();                         // �������� �����

public:
           CLogReader();
          ~CLogReader();
   bool    Open( const TCHAR szFileName[100] );   // �������� �����, false - ������
   
   bool    SetFilter(const char *filter);   // ��������� ������� �����, false - ������
   bool    GetNextLine(char *buf,           // ������ ��������� ��������� ������, 
                       const int bufsize);  // buf - �����, bufsize - ������������ �����
                                            // false - ����� ����� ��� ������
  };