#include "stdafx.h"
#include "clogr.h"

CLogReader::CLogReader()
{
    setlocale(LC_CTYPE, "Russian");
    m_virt_buff_=4096;
   
}

CLogReader::~CLogReader()
{
    VirtualFree(m_virt_mem_,0,MEM_RELEASE);

    VirtualFree(m_virt_work_,0,MEM_RELEASE);
    
    int size_filter = sizeof(m_filter) / sizeof(m_filter[0]);
    if(size_filter>0)
        delete[] m_filter;
}

bool CLogReader::Open( const TCHAR szFileName[100] )
{
    
    DWORD error;
 
    OVERLAPPED ov = {0,0,0,0,NULL};//структура для задания режима чтения
    m_h_file_ = CreateFile(szFileName, GENERIC_READ, 0 ,NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL);
    if(m_h_file_ == INVALID_HANDLE_VALUE)
            return false; 

    DWORD file_size = GetFileSize(m_h_file_, NULL);
    m_virt_mem_ = VirtualAlloc(NULL,m_virt_buff_,MEM_COMMIT,PAGE_READWRITE);
    if(m_virt_mem_ == NULL)
        return false;
    
    m_virt_work_ = VirtualAlloc(NULL,file_size,MEM_COMMIT,PAGE_READWRITE);
    if(m_virt_work_ == NULL)
        return false;
    
    while(ov.Offset < file_size)
    {
        ReadFile(m_h_file_,(LPVOID)m_virt_mem_, m_virt_buff_,NULL, &ov);
        error = GetLastError();      
        WaitForSingleObject(m_h_file_,INFINITE);   
        
        ov.Offset += m_virt_buff_;
        if(error == ERROR_HANDLE_EOF)
            break;
        else if(error == ERROR_IO_PENDING)
            continue;
        else if(error == ERROR_ACCESS_DENIED || error == ERROR_FILE_CORRUPT || error == ERROR_FILE_INVALID || error == ERROR_FILE_NOT_FOUND)
            return false;
    }   
    
    return true;
}

void CLogReader::Close()
{
     CloseHandle(m_h_file_);
}

bool CLogReader::SetFilter(const char* filter)
{
    if(sizeof(filter[0]) == 0)
        return false;
    int size_filter = sizeof(filter) / sizeof(filter[0]);
    m_filter = new char[size_filter];
    m_filter = filter;
    return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufsize)
{
    return true;

}
