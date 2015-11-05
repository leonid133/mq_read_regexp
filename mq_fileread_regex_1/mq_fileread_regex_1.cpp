// mq_fileread_regex_1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "clogr.h"


//#define TEST_BYTE 52428800//50 Мб файл 

//#define m_max_byte_file_size 1048576
 /*

void WriteF();
void ReadFSynh();
void ReadFAsynh();
static int Count;
 
HANDLE hFile;
*/

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_CTYPE, "Russian");
    /*
    if(argc != 2 && argc != 3) 
    {
        std::cerr << "Пример использования параметров: " << argv[0] << " РегулярноеВыражение [Файл]" << "\r\n";
		return 1;
	}

    if(argc == 3) 
    {
		std::cerr << argv[0] << ": Can't open " << argv[2] << "\r\n";
		return 1;
    }
    else
        std::cerr << argv[0] << ": " << argv[1]<< ": " << argv[2] << "\r\n";
     _getch();*/
    
     size_t buf_result_size = 150;
    const char filter[] = "БОЛ*АР";
    const TCHAR szFileName[100] = {L"testfile.txt"};

    CLogReader regexp_reader;
    DWORD tick1_ = GetTickCount();
    regexp_reader.Open(szFileName);
    DWORD tick2_ = GetTickCount();

    regexp_reader.SetFilter(filter);
    char* buf = new char[buf_result_size];
    /*for(int it_buf = 0; it_buf < buf_result_size; ++it_buf)
        buf[it_buf] = '\n';
    */
    int find_counter = 0;
    while(regexp_reader.GetNextLine(buf, buf_result_size-1))
    {
        DWORD tick_print_start_ = GetTickCount();
        printf( "%s \n", buf );
        DWORD tick_print_end_ = GetTickCount();
        find_counter++;
        tick2_ = tick2_ + (tick_print_end_ - tick_print_start_);
    }
    DWORD tick3_ = GetTickCount();
    printf("\nФайл считан за %d мс\n",tick2_-tick1_);
    printf("\nПоиск произведен за %d мс\n",tick3_-tick2_);
    printf( "\nФильтр поиска %s \n", filter );
    printf("\nНайдено %d строчек\n", find_counter);

    _getch();
	
    return 0;
}
/*
void ReadFAsynh()
{       
    DWORD maxBuff=0;
    DWORD error1,error;
    LPVOID VirtMem = VirtualAlloc(NULL,512,MEM_COMMIT,PAGE_READWRITE);
 
    OVERLAPPED ov = {0,0,0,0,NULL};//структура для задания режима чтения
    hFile = CreateFile(szFileName, GENERIC_READ, 0 ,NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
            return; 
   DWORD beg1 = GetTickCount();
   while(ov.Offset<TEST_BYTE)
{
   ReadFile(hFile,(LPVOID)VirtMem,512,NULL, &ov);//sizeof(DWORD)
    //error1 = GetLastError();      
    WaitForSingleObject(hFile,INFINITE);    
    ov.Offset += 512;
}   
 //GetOverlappedResult(m_h_file_, &ov,&maxBuff,TRUE);        
 //error = GetLastError();                        
    CloseHandle(hFile);
    //определяем время считывания файла
    DWORD beg2 = GetTickCount()-beg1; 
    //if(error1 == 997)
    //      printf("ERROR_IO_PENDIN - Асинхронная операция ввода / вывода в процессе.\n");
    //  else
    //      printf("Не все данные считаны. Ошибка - %d\n",error1);
    //if(error = 38)
    //          printf("ERROR_HANDLE_EOF - достигнут конец файла\n",error);
    printf("\nФайл в асинхронном режиме считан за %d мс\n",beg2);
    VirtualFree(VirtMem,0,MEM_RELEASE);
    
_getch();
}
 
//считывание в синхронном режиме
void ReadFSynh()
{
    //DWORD lpBuff=NULL;
    DWORD maxBuff=1;
    BOOL read = true;
    DWORD error1;
 
    LPVOID VirtMem = VirtualAlloc(NULL,512,MEM_COMMIT,PAGE_READWRITE);
 
    hFile = CreateFile(szFileName, GENERIC_READ, 0 ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
            return;
DWORD beg1 = GetTickCount(); 
 
while(read&&maxBuff!=0)
    {
        read = ReadFile(hFile,VirtMem,512,&maxBuff, NULL);
       //error1 = GetLastError(); 
    }
CloseHandle(hFile); 
DWORD beg2 = GetTickCount()-beg1; //определяем время считывания файла

printf("\nФайл в синхронном режиме считан за %d мс\n",beg2);
VirtualFree(VirtMem,0,MEM_RELEASE);
_getch();
}

void WriteF()
{
    DWORD dwTemp;
    DWORD Rand;
    
    Count = 0;
        printf("Записываем файл!");
        hFile = CreateFile(szFileName,GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
            return;
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        DWORD beg1 = GetTickCount(); 
        while(dwFileSize<TEST_BYTE)
        {
            Rand = rand();
            WriteFile(hFile, &Rand,sizeof(Rand),&dwTemp, NULL);
            dwFileSize = GetFileSize(hFile, NULL);
        }
        CloseHandle(hFile);
    DWORD beg2 = GetTickCount()-beg1; //определяем время считывания файла
  
printf("\nФайл в синхронном режиме записан за %d мс\n",beg2);
}*/