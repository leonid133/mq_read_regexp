// mq_fileread_regex_1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "clogr.h"

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_CTYPE, "Russian");
    /*
    if(argc != 2 && argc != 3) 
    {
        std::cerr << "������ ������������� ����������: " << argv[0] << " ������������������� [����]" << "\r\n";
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
    
    int buf_result_size = 150;
    //const char filter[] = "*�����*";
     const char filter[] = "**���?�**";
      //const char filter[] = " ?????? ";
    // const char filter[] = "������";
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
    regexp_reader.Close();

    printf("\n���� ������ �� %d ��\n",tick2_-tick1_);
    printf("\n����� ���������� �� %d ��\n",tick3_-tick2_);
    printf( "\n������ ������ %s \n", filter );
    printf("\n������� %d �������\n", find_counter);

    _getch();
	
    return 0;
}
