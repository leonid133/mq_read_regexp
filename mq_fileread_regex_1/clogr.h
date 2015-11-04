#pragma once
#include <tchar.h>
#include <windef.h>

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