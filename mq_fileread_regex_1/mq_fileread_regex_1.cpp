// author ������ �.� leonidblohin@gmail.com

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
        //testfile.txt **���?�**
        int filename_len  = _tcslen( argv[1] );
        if(filename_len > 100)
        {
            printf( "\n��� ����� ������� �������\n");
            return 1;
        }
        szFileName = argv[1];
        int filter_len  = _tcslen( argv[2] );
        if(filter_len > 100)
        {
            printf( "\n���������� ��������� ������� �������\n");
            return 1;
        }
        filter = new char[filter_len+1];
        WideCharToMultiByte( CP_ACP, 0, argv[2], -1, filter, filter_len, NULL, NULL );
        filter[filter_len] = '\0';
    }
    else
    {
        printf( "\n������: .exe filename.txt regexp\n" );
        printf( "\n.* - ������������������ ����� �������� �������������� �����\n" );
        printf( "\n. - ���� ����� ������\n" );
        printf( "\n? - ���� ��� ����	{0,1}	colou?r	color, colour\n" );
        printf( "\n* - ���� ��� �����	{0,}	colou*r	color, colour, colouur � �. �.\n" );
        printf( "\n+ - ���� ��� �����	{1,}	colou+r	colour, colouur � �. �. (�� �� color)\n" );
        printf( "\n���������� �������� �� ���������\n" );
        szFileName = L"testfile.txt";
        
        filter = new char[100];
        //filter = "**���?�**";
        // filter = "*�����*";
        //filter = ".*������";
         //filter = ".*����������";
        //filter = "(���|���)���*����";
        //filter = ".*��������";
        //filter = "�*�������";
       // filter = "*���*�*�*";
        //filter = "����*����";
         //filter = ".*����+����";
        //filter = "�?�+�(�?|�+)��*��";
        //filter = "�?�����*��";
        // filter = "������*��";
         //filter = "�?�������";
         //filter = "����?����";
        //filter = "�?��*��**��";
        //filter = "�?��+�+���";
        //filter = "�?��+�*���";
         // filter = "��*������";
        filter = "��+���?��.*��*�";
         //filter = "�����?����";
        //filter = "�����.����";
         //filter = "���������*�";
        //filter = "�������.*���";
        //filter = ".*��*�����.*���";
        // filter = ".*.*����������";
    }
    printf("����� ���������� ����� %s � ����� ", filter);
    _tcprintf( szFileName );
    printf(". ��� ����������� ������� ����� �������\n");
    _getch();
    int buf_result_size = 150;
    {
        CLogReader regexp_reader;
        DWORD tick_start_open = GetTickCount();
        if(!regexp_reader.Open(szFileName))
        {
            std::cerr << "�� ������� ������� ���� " << szFileName;
            return 1;
        }
        DWORD tick_end_open = GetTickCount();
        DWORD tick_start_regex = GetTickCount();

        if( !regexp_reader.SetFilter(filter) )
        {
            std::cerr << "�� ������� ���������� ������ " << filter;
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
    
        printf( "\n���� ������ �� %d ��\n", tick_end_open-tick_start_open );
        printf( "\n����� ���������� �� %d ��\n", tick_end-tick_start_regex );
        printf( "\n���������� ��������� %s \n", filter );
        printf( "\n������� %d �������\n", find_counter );
        printf( "\n������� ����� �������, ��� ���������� ���������\n");
    }
    

    _CrtMemDumpAllObjectsSince(&_ms);
    _CrtDumpMemoryLeaks();
    _getch();
	 
    return 0;
   
}
