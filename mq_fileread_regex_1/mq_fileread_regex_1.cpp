// author Блохин Л.Н leonidblohin@gmail.com

#include "stdafx.h"
#include "clogr.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)

#endif

int _tmain( int argc, _TCHAR* argv[] )
{
     _CrtMemState _ms;
    _CrtMemCheckpoint(&_ms);
   // _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );

    setlocale( LC_CTYPE, "Russian" );


    char *filter;
    TCHAR *szFileName;
    szFileName = new TCHAR[100];

    if( 3 == argc ) 
    {
        //testfile.txt **ОЛГ?Р**
        int filename_len  = _tcslen( argv[1] );
        if(filename_len > 100)
        {
            printf( "\nИмя файла слишком большое\n");
            return 1;
        }
        szFileName = argv[1];
        int filter_len  = _tcslen( argv[2] );
        if(filter_len > 100)
        {
            printf( "\nРегулярное выражение слишком большое\n");
            return 1;
        }
        filter = new char[filter_len+1];
        WideCharToMultiByte( CP_ACP, 0, argv[2], -1, filter, filter_len, NULL, NULL );
        filter[filter_len] = '\0';
    }
    else
    {
        printf( "\nПример: .exe filename.txt regexp\n" );
        printf( "\n.* - последовательность любых символов неограниченной длины\n" );
        printf( "\n. - один любой символ\n" );
        printf( "\n? - Ноль или одно	{0,1}	colou?r	color, colour\n" );
        printf( "\n* - Ноль или более	{0,}	colou*r	color, colour, colouur и т. д.\n" );
        printf( "\n+ - Одно или более	{1,}	colou+r	colour, colouur и т. д. (но не color)\n" );
        printf( "\nИспользуем значения по умолчанию\n" );
        szFileName = L"testfile.txt";
        
        filter = new char[100];
        //filter = "**ОЛГ?Р**";
        // filter = "*ОЛГАР*";
        //filter = ".*БОЛГАР";
         //filter = ".*ЗАКРЫВАШКИ";
        //filter = "(ОТК|ЗАК)РЫВ*АШКИ";
        //filter = ".*ПЕЧЕНЬКА";
        //filter = "П*ЕЧЕНЬКА";
       // filter = "*ЕЧЕ*Н*К*";
        //filter = "ПЕЧЕ*НЬКА";
         //filter = ".*ПЕЧЕ+НЬКА";
        //filter = "П?Е+Ч(Е?|Е+)НЬ*КА";
        //filter = "П?ЕЧЕНЬ*КА";
        // filter = "ПЕЧЕНЬ*КА";
         //filter = "П?ЕЧЕНЬКА";
         //filter = "ПЕЧЕ?НЬКА";
        //filter = "П?ЧЕ*НЬ**КА";
        //filter = "П?ЧЕ+Н+ЬКА";
        //filter = "П?ЧЕ+Н*ЬКА";
         // filter = "ПЕ*ЧЕНЬКА";
        filter = "КА+ЗАБ?УЛ.*ЬК*А";
         //filter = "КАЗАБ?ЛЬКА";
        //filter = "КАЗАБ.ЛЬКА";
         //filter = "КАЗАБУЛЬК*А";
        //filter = "КАЗАБУЛ.*ЬКА";
        //filter = ".*КА*ЗАБУЛ.*ЬКА";
        // filter = ".*.*КАЗАБУЛЬКА";
    }
    printf("Будет произведен поиск %s в файле ", filter);
    _tcprintf( szFileName );
    printf(". Для продолжения нажмите любую клавишу\n");
    _getch();
    int buf_result_size = 150;
    {
        CLogReader regexp_reader;
        DWORD tick_start_open = GetTickCount();
        if(!regexp_reader.Open(szFileName))
        {
            std::cerr << "Не удалось открыть файл " << szFileName;
            return 1;
        }
        DWORD tick_end_open = GetTickCount();
        DWORD tick_start_regex = GetTickCount();

        if( !regexp_reader.SetFilter(filter) )
        {
            std::cerr << "Не удалось установить фильтр " << filter;
            return 1;
        }
        char* buf = new char[buf_result_size+1];
 
        int find_counter = 0;
        while(regexp_reader.GetNextLine(buf, buf_result_size))
        {
            DWORD tick_print_start_ = GetTickCount();
            printf( "\n %s \n ", buf );
            DWORD tick_print_end_ = GetTickCount();
            find_counter++;
            tick_start_regex = tick_start_regex + (tick_print_end_ - tick_print_start_);
        
        }
        DWORD tick_end = GetTickCount();
        regexp_reader.Close();
        delete buf;
    
        printf( "\nФайл открыт за %d мс\n", tick_end_open-tick_start_open );
        printf( "\nПоиск произведен за %d мс\n", tick_end-tick_start_regex );
        printf( "\nРегулярное выражение %s \n", filter );
        printf( "\nНайдено %d строчек\n", find_counter );
        printf( "\nНажмите любую клавишу, для завершения программы\n");
    }
    

    _CrtMemDumpAllObjectsSince(&_ms);
    _CrtDumpMemoryLeaks();
    _getch();
	 
    return 0;
   
}
