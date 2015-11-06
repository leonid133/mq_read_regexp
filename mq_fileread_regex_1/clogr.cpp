// author Блохин Л.Н leonidblohin@gmail.com
#include "stdafx.h"
#include "clogr.h"

CLogReader::CLogReader()
{
    m_virt_buff_size= 4096;
    m_filter = NULL;
    m_file_size = 0;
    
    m_line_counter = 0;
    m_buf_null = new char[( m_virt_buff_size*2 )+1];
    for( int it_buf = 0; it_buf < ( m_virt_buff_size*2 ); ++it_buf )
    {
        m_buf_null[it_buf] = '\0';  
    }
    
    //Sedgewick RegExp 
    aut_state[0].next1 = 0;
	aut_state[0].next2 = 0;
}

CLogReader::~CLogReader()
{
   VirtualFree( m_read_mem_buf, 0, MEM_RELEASE );
   VirtualFree( m_work_mem_buf, 0, MEM_RELEASE );
}

bool CLogReader::Open( const TCHAR file_name[100] )
{
    m_h_file_ = CreateFile( file_name, GENERIC_READ, 0 , NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL );
    if( m_h_file_ == INVALID_HANDLE_VALUE )
            return false; 

    m_file_size = GetFileSize( m_h_file_, NULL );
    m_line_counter = 0;
    m_it_read_counter = 0;

    m_read_mem_buf = VirtualAlloc( NULL, (m_virt_buff_size + 16), MEM_COMMIT, PAGE_READWRITE );
    if(m_read_mem_buf == NULL)
        return false;
    
    m_work_mem_buf = VirtualAlloc( NULL, ( m_virt_buff_size*2 + 32 ), MEM_COMMIT, PAGE_READWRITE );
    if( m_work_mem_buf == NULL )
        return false;

    char* buf_work_text = reinterpret_cast<char*>( m_work_mem_buf );
    CopyMemory(buf_work_text , m_buf_null, ( m_virt_buff_size*2 ) );

    return true;
}

void CLogReader::Close()
{
     CloseHandle( m_h_file_ );
}

bool CLogReader::SetFilter( const char* filter )
{
    if( sizeof( filter[0] ) == 0 )
        return false;
    
    m_size_filter = sizeof( filter ) / sizeof( filter[0] );
    m_filter = new char[m_size_filter+1];
    m_filter = filter;
    
    //Sedgewick RegExp 
    compile( m_filter );
    
    return true;
}

bool CLogReader::GetNextLine( char* buf, const int bufsize )
{
    OVERLAPPED ov = { 0, 0, 0, 0, NULL};

    char* buf_work_text = reinterpret_cast<char*>( m_work_mem_buf );
    
    ov.Offset = m_line_counter;

    while( ov.Offset < m_file_size 
        || m_it_read_counter < ( m_virt_buff_size*2 ) )
    {
        if( ( m_it_read_counter >= m_virt_buff_size 
           || 0 == m_it_read_counter )
           && ov.Offset < m_file_size )
        {
            ReadFile( m_h_file_, (LPVOID)m_read_mem_buf, m_virt_buff_size, NULL, &ov);
            DWORD error = GetLastError();      
            WaitForSingleObject( m_h_file_, INFINITE );   
        
            if( error == ERROR_HANDLE_EOF )
                break;
            else if( error == ERROR_IO_PENDING )
                ;
            else if( error == ERROR_ACCESS_DENIED 
                  || error == ERROR_FILE_CORRUPT 
                  || error == ERROR_FILE_INVALID 
                  || error == ERROR_FILE_NOT_FOUND )
                return false;

             CopyMemory( ( buf_work_text + m_virt_buff_size ), m_read_mem_buf, m_virt_buff_size );
             ov.Offset += m_virt_buff_size;
             m_line_counter = ov.Offset;
             m_it_read_counter = 0;
        }
        
        int bufsize_line = 0;
        bool not_ended_line = false;
        
        while( m_it_read_counter < m_virt_buff_size 
           ||( m_it_read_counter < ( m_virt_buff_size*2 ) && not_ended_line )
           ||((m_line_counter > m_file_size)&&(m_it_read_counter < ( m_virt_buff_size*2 ))) )  
		{
            
            for( bufsize_line = 0; ( bufsize_line < (bufsize-1) ) && ( bufsize_line < ( m_virt_buff_size*2 ) ); bufsize_line )
		    {
                if( '\n' == buf_work_text[bufsize_line + m_it_read_counter]
                 || '\0' == buf_work_text[bufsize_line + m_it_read_counter] )
                {
                    not_ended_line = false;
                    if( bufsize_line==0 )
                        ++bufsize_line;
                    break;
                }
                else
                   not_ended_line = true; 
                ++bufsize_line;
		    }
/*        if(bufsize_line>1)
                printf("debug");*/
            CopyMemory( buf , m_buf_null, bufsize );
            CopyMemory( buf , ( buf_work_text + m_it_read_counter ), bufsize_line );
        
            m_it_read_counter += bufsize_line;

		    int n = SearchInLine( buf );
	        if( n >= 0 )
		        return true;
        }
        CopyMemory( buf_work_text, m_read_mem_buf, m_virt_buff_size );
        if( m_line_counter < m_file_size )
            m_it_read_counter = 0;
    }   
     
    return false;
}

// ROBERT SEDGEWICK | KEVIN WAYNE REGEX 

CLogReader::Deque::Deque()
{
	head = 0;
	tail = 0;
}

CLogReader::Deque::~Deque()
{
	while( head ) 
    {
		tail = head;
		head = head->next;
		delete tail;
	}
}

void CLogReader::Deque::push( int int_n )
{
	Element *ptr = new Element;
	ptr->n = int_n;
	ptr->next = head;
	if( !tail )
		tail = ptr;
	head = ptr;
}

void CLogReader::Deque::put( int n )
{
	Element *ptr = new Element;
	ptr->n = n;
	ptr->next = 0;
	if( !head )
		head = ptr;
	else
		tail->next = ptr;
	tail = ptr;
}

int CLogReader::Deque::pop()
{
	int ret = head->n;
	Element *ptr = head;
	head = head->next;
	if( !head )
		tail = 0;
	delete ptr;
	return ret;
}

inline int CLogReader::Deque::isEmpty()
{
	return head ? 0 : 1;
}

// RegExpr


void CLogReader::clear_aut_state()
{
	for( int it_states=0; it_states < MAXSTATES; it_states++ ) 
    {
		aut_state[it_states].char_state = '\0';
		aut_state[it_states].next1 = 0;
		aut_state[it_states].next2 = 0;
	}
}


// compiler
inline int CLogReader::isLetter( char input_char )
{
	return ( input_char != '|' 
          && input_char != '(' 
          && input_char != ')' 
          && input_char != '*' 
          && input_char != '\\' 
          && input_char != '\0' 
          ? 1 : 0 );
}

void CLogReader::compile( const char *regex )
{
   	m_regex_pattenn = regex;

	m_state_counter = 0;
	m_regex_char_counter = 0;

	clear_aut_state();
	int n = list();
	if( aut_state[0].next1 == 0 )
    {
		aut_state[0].next1 = aut_state[0].next2 = n;
    }

	aut_state[m_state_counter].next1 = aut_state[m_state_counter].next2 = 0;

}

unsigned CLogReader::list()
{
	unsigned state_1 = m_state_counter++;
    unsigned state_2 = element();
    unsigned state_3;
	if( m_regex_pattenn[m_regex_char_counter] == '|' )
    {
		m_regex_char_counter++;
		state_3 = ++m_state_counter;
		aut_state[state_3].next1 = state_2;
		aut_state[state_3].next2 = list();
		aut_state[state_3-1].next1 = aut_state[state_3-1].next2 = m_state_counter;
		
        if( aut_state[state_1].next1 == state_2 || aut_state[state_1].next1 == 0 )
        {
            aut_state[state_1].next1 = state_3;
        }
		if( aut_state[state_1].next2 == state_2 || aut_state[state_1].next2 == 0 )
        { 
                aut_state[state_1].next2 = state_3;
        }
		return state_3;
	}
	return state_2;
}

unsigned CLogReader::element()
{
	unsigned state_1 = m_state_counter;
    unsigned state_2 = 0;

	    if( m_regex_pattenn[m_regex_char_counter] == '(' )
        {
		    m_regex_char_counter++;
		    state_2 = list();
		    if( m_regex_pattenn[m_regex_char_counter] == ')' ) 
            {
			    aut_state[m_state_counter].next1 = aut_state[m_state_counter].next2 = m_state_counter+1;
			    m_state_counter++;
			    m_regex_char_counter++;
		    } 
            else{/*ожидаемого закрытия скобочки нет*/}
	    } 
        else
        {
		    state_2 = v();
        }
        
	    if( m_regex_pattenn[m_regex_char_counter] == '*' ) 
        {
            aut_state[m_state_counter].next1 = state_2;
       	    aut_state[m_state_counter].next2 = m_state_counter+1;
		    state_1 = m_state_counter;
            
		    if( aut_state[state_2].next1 == state_2 || aut_state[state_2].next1 == 0 )
			{
                aut_state[state_2].next1 = m_state_counter;
            }
       	    if( aut_state[state_2].next2 == state_2 || aut_state[state_2].next2 == 0 )
	       	{
                 aut_state[state_2].next2 = m_state_counter;
            }
		    m_state_counter++;
   		    m_regex_char_counter++;
	    } 
        else 
        {
       	    if( aut_state[state_1-1].next1 == 0 )
	       	    aut_state[state_1-1].next1 = state_2;
       	    if( aut_state[state_1-1].next2 == 0 )
	       	    aut_state[state_1-1].next2 = state_2;
	    }

	    if( m_regex_pattenn[m_regex_char_counter] != '|' 
            &&  m_regex_pattenn[m_regex_char_counter] != ')' 
            && m_regex_pattenn[m_regex_char_counter] != '*' 
            && m_regex_pattenn[m_regex_char_counter] != '\0' )
		    state_1 = element();

	return state_1;
}

unsigned CLogReader::v()
{
	unsigned state_1 = m_state_counter;

	if( m_regex_pattenn[m_regex_char_counter] == '\\' )
		m_regex_char_counter++;
	else if( !isLetter( m_regex_pattenn[m_regex_char_counter] ) )
    {
        //clear_aut_state();
        while( m_regex_pattenn[m_regex_char_counter] && !isLetter( m_regex_pattenn[m_regex_char_counter] ) )
        {
            ++m_regex_char_counter;
        }
        //s1 = element();
        //return s1;
    }

	aut_state[m_state_counter].char_state = m_regex_pattenn[m_regex_char_counter++];
	aut_state[m_state_counter].next1 = aut_state[m_state_counter].next2 = m_state_counter+1;
	m_state_counter++;
	return state_1;
}

int CLogReader::SearchInLine( const char *text_line, unsigned start )
{
	if( aut_state[0].next1 == 0 && aut_state[0].next2 == 0 )
		return REGEXPR_NOT_COMPILED;

    int text_char_size;
    text_char_size = strlen( text_line );
   
    for( int n = start; n < text_char_size; n++ )
    {
		m_state_counter = aut_state[0].next1;
	    if( aut_state[0].next1 != aut_state[0].next2 )
		    deque.push( aut_state[0].next2 );
	    deque.put(-1);
        bool match_b;
        int m = simulate( text_line, n, match_b );
        if( match_b ) 
        {
			return n;
		}
        //else if( m == -2 )
          //  return REGEXPR_NOT_FOUND;
	}
    
	return REGEXPR_NOT_FOUND;
}

#define next_char	-1
int CLogReader::simulate( const char *str, int j, bool &match_b )
{
	int last_match = j - 1;
	size_t len = strlen( str );
    match_b = false;
	do 
    {
		if( m_state_counter == next_char ) 
        {
			if(str[j])
				j++;
			deque.put(next_char);
		} 
        else if( aut_state[m_state_counter].char_state == str[j] || aut_state[m_state_counter].char_state == '?' ) 
        {
			deque.put( aut_state[m_state_counter].next1 );
			if( aut_state[m_state_counter].next1 != aut_state[m_state_counter].next2 )
				deque.put( aut_state[m_state_counter].next2 );
		} 
        else if( !aut_state[m_state_counter].char_state ) 
        {
			deque.push( aut_state[m_state_counter].next1 );
			if( aut_state[m_state_counter].next1 != aut_state[m_state_counter].next2 )
				deque.push( aut_state[m_state_counter].next2 );
		}
       
		m_state_counter = deque.pop();
		if( m_state_counter == 0 ) 
        {
			last_match = j - 1;
			m_state_counter = deque.pop();
            last_match = last_match - m_state_counter;
            match_b = true;
		}
	} 
    while( j <= len && !deque.isEmpty() );

	return last_match;
}
#undef next_char