#include "stdafx.h"
#include "clogr.h"

CLogReader::CLogReader()
{
    setlocale(LC_CTYPE, "Russian");
    m_virt_buff_size= 4096;
    m_filter = NULL;
    m_file_size = 0;
}

CLogReader::~CLogReader()
{
    VirtualFree(m_virt_work_,0,MEM_RELEASE);
}

bool CLogReader::Open( const TCHAR szFileName[100] )
{
    OVERLAPPED ov = {0,0,0,0,NULL};
    m_h_file_ = CreateFile(szFileName, GENERIC_READ, 0 ,NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL);
    if(m_h_file_ == INVALID_HANDLE_VALUE)
            return false; 
    m_file_size = GetFileSize(m_h_file_, NULL);
   
    m_virt_work_ = VirtualAlloc(NULL,m_file_size,MEM_COMMIT,PAGE_READWRITE);
    if(m_virt_work_ == NULL)
        return false;
        
    LPVOID virt_mem_buf = VirtualAlloc(NULL,m_virt_buff_size,MEM_COMMIT,PAGE_READWRITE);
    if(virt_mem_buf == NULL)
        return false;
    
    while(ov.Offset < m_file_size)
    {
        ReadFile(m_h_file_,(LPVOID)virt_mem_buf, m_virt_buff_size,NULL, &ov);
        DWORD error = GetLastError();      
        WaitForSingleObject(m_h_file_,INFINITE);   
        
        char* charBuffer = reinterpret_cast<char*>(m_virt_work_);
        CopyMemory(charBuffer + ov.Offset , virt_mem_buf, m_virt_buff_size);

        ov.Offset += m_virt_buff_size;
        if(error == ERROR_HANDLE_EOF)
            break;
        else if(error == ERROR_IO_PENDING)
            ;
        else if(error == ERROR_ACCESS_DENIED || error == ERROR_FILE_CORRUPT || error == ERROR_FILE_INVALID || error == ERROR_FILE_NOT_FOUND)
            return false;
    }   
    VirtualFree(virt_mem_buf,0,MEM_RELEASE);
    
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
    
    m_size_filter = sizeof(filter) / sizeof(filter[0]);
    m_filter = new char[m_size_filter];
    m_filter = filter;
    return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufsize)
{
    char* buf_work_text =  reinterpret_cast<char*>(m_virt_work_);
    
    CopyMemory(buf , buf_work_text, bufsize);
    return true;
}
