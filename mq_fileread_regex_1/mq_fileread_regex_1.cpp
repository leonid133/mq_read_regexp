// author ������ �.� leonidblohin@gmail.com

#include "stdafx.h"
#include "clogr.h"

int _tmain( int argc, _TCHAR* argv[] )
{
    setlocale( LC_CTYPE, "Russian" );

    char *filter;
    TCHAR *szFileName;
    szFileName = new TCHAR[100];

    if( 3 == argc ) 
    {
        //testfile.txt **���?�**
        szFileName = argv[1];
        int filter_len  = _tcslen( argv[2] );
        filter = new char[filter_len+1];
        WideCharToMultiByte( CP_ACP, 0, argv[2], -1, filter, filter_len, NULL, NULL );
        filter[filter_len] = '\0';
    }
    else
    {
        szFileName = L"testfile.txt";
        
        filter = new char[100];
        //filter = "**���?�**";
        // filter = "*�����*";
        // filter = " ?????? ";
        //filter = "������";
        // filter = "����������";
        //filter = "(���|���)���*����";
        //filter = "���*����";
        //filter = "�*�������";
        //filter = "*���*�*�*";
        //filter = "����*����";
        // filter = "����+����";
        //filter = "�?�+�(�?|�+)��*��";
        //filter = "�?�����*��";
        // filter = "������*��";
         //filter = "�?�������";
         //filter = "����?����";
        //filter = "�?��*��**��";
        //filter = "�?��+�+���";
        //filter = "�?��+�+���";
         // filter = "��*������";
        filter = "��+���?����*�";
    }
    printf("����� ���������� ����� %s � ����� ", filter);
    _tcprintf( szFileName );
    printf(". ��� ����������� ������� ����� �������\n");
    _getch();
    int buf_result_size = 150;
   
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

    printf( "\n���� ������ �� %d ��\n", tick_end_open-tick_start_open );
    printf( "\n����� ���������� �� %d ��\n", tick_end-tick_start_regex );
    printf( "\n���������� ��������� %s \n", filter );
    printf( "\n������� %d �������\n", find_counter );

    _getch();
	
    return 0;
}
