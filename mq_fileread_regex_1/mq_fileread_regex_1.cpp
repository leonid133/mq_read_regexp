// mq_fileread_regex_1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "clogr.h"

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_CTYPE, "Russian");

    char *filter;
    filter = new char[100];

    TCHAR *szFileName;
    szFileName = new TCHAR[100];

    if(argc == 3) 
    {
        szFileName = argv[1];
        
        int filter_len  = _tcslen( argv[2]);
        filter = new char[filter_len+1];
        WideCharToMultiByte(CP_ACP, 0, argv[2], -1, filter, filter_len, NULL, NULL);
        filter[filter_len] = '\0';
    }
    else
    {
        szFileName = L"testfile.txt";
        //const char filter[] = "*ОЛГАР*";
        
        filter = "**ОЛГ?Р**";
        //const char filter[] = " ?????? ";
        // const char filter[] = "БОЛГАР";
    }
    printf("Будет произведен поиск %s в файле ", filter);
    _tcprintf( szFileName );
    printf(". Для продолжения нажмите любую клавишу\n");
    _getch();
    int buf_result_size = 150;
   
    CLogReader regexp_reader;
    DWORD tick1_ = GetTickCount();
    if(!regexp_reader.Open(szFileName))
    {
        std::cerr << "Не удалось открыть файл " << szFileName;
        return 1;
    }
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
    regexp_reader.Close();

    printf("\nФайл открыт за %d мс\n",tick2_-tick1_);
    printf("\nПоиск произведен за %d мс\n",tick3_-tick2_);
    printf( "\nФильтр поиска %s \n", filter );
    printf("\nНайдено %d строчек\n", find_counter);

    _getch();
	
    return 0;
}
