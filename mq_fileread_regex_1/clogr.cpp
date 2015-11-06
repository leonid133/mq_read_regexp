#include "stdafx.h"
#include "clogr.h"

CLogReader::CLogReader()
{
   
    m_virt_buff_size= 4096;
    m_filter = NULL;
    m_file_size = 0;
    

    //Sedgewick RegExp 
    m_line_counter = 0;
    automaton[0].next1 = 0;
	automaton[0].next2 = 0;
}

CLogReader::~CLogReader()
{
    VirtualFree(m_virt_work_,0,MEM_RELEASE);
}

bool CLogReader::Open( const TCHAR szFileName[100] )
{
    
    m_h_file_ = CreateFile(szFileName, GENERIC_READ, 0 ,NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL);
    if(m_h_file_ == INVALID_HANDLE_VALUE)
            return false; 
    
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
    
    //Sedgewick RegExp 
    compile(m_filter);
    
    return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufsize)
{
   
   // CopyMemory(buf , buf_work_text, bufsize);
    char* buf_null = new char[bufsize];
    for(int it_buf = 0; it_buf < bufsize; ++it_buf)
            buf_null[it_buf] = '\0';
    
    OVERLAPPED ov = {0,0,0,0,NULL};

    LPVOID virt_mem_buf = VirtualAlloc(NULL,m_virt_buff_size,MEM_COMMIT,PAGE_READWRITE);
    if(virt_mem_buf == NULL)
        return false;
    
    m_file_size = GetFileSize(m_h_file_, NULL);
   
    m_virt_work_ = VirtualAlloc(NULL,m_file_size,MEM_COMMIT,PAGE_READWRITE);
    if(m_virt_work_ == NULL)
        return false;

    char* buf_work_text =  reinterpret_cast<char*>(m_virt_work_);
    
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

	while(m_line_counter < m_file_size) 
    {
        
        int bufsize_ln = 0;
		for( bufsize_ln = 1; bufsize_ln < (bufsize-1); ++bufsize_ln)
		{
            if(buf_work_text[bufsize_ln + m_line_counter] == '\n')
                break;
		}
        CopyMemory(buf , buf_null, bufsize);
        CopyMemory(buf , buf_work_text + m_line_counter, bufsize_ln);
        m_line_counter +=bufsize_ln;	
        /*
        CopyMemory(buf , buf_null, bufsize);
        CopyMemory(buf , buf_work_text + m_line_counter, bufsize);
        m_line_counter +=bufsize;
        */
		int n = searchLen(buf);
	    if(n >= 0)
		    return true;
    }
    return false;
}

  //Sedgewick RegExp 
/*inline int CLogReader::search(const char *str, unsigned start)
{
	unsigned dummy;
	return searchLen(str, dummy, start);
}*/

// RegExpr::Deque
CLogReader::Deque::Deque()
{
	head = 0;
	tail = 0;
}

CLogReader::Deque::~Deque()
{
	while(head) {
		tail = head;
		head = head->next;
		delete tail;
	}
}

void CLogReader::Deque::push(int n)
{
	Element *ptr = new Element;
	ptr->n = n;
	ptr->next = head;
	if(!tail)
		tail = ptr;
	head = ptr;
}

void CLogReader::Deque::put(int n)
{
	Element *ptr = new Element;
	ptr->n = n;
	ptr->next = 0;
	if(!head)
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
	if(!head)
		tail = 0;
	delete ptr;
	return ret;
}

inline int CLogReader::Deque::isEmpty()
{
	return head ? 0 : 1;
}

// RegExpr


void CLogReader::clear_automaton()
{
	for(int n=0;n<MAXSTATES;n++) {
		automaton[n].the_char = '\0';
		automaton[n].next1 = 0;
		automaton[n].next2 = 0;
	}
}


// compiler
inline int CLogReader::isLetter(char c)
{
	return (c != '|' && c != '(' && c != ')' && c != '*' && c != '\\' && c != '\0' ? 1 : 0);
}

void CLogReader::compile(const char *pattern)
{
   	m_p = pattern;

	m_state = 0;
	m_j = 0;

	clear_automaton();
	int n = list();
	if(automaton[0].next1 == 0)
		automaton[0].next1 = automaton[0].next2 = n;
	automaton[m_state].next1 = automaton[m_state].next2 = 0;

}

unsigned CLogReader::list()
{
	unsigned s1 = m_state++;
    unsigned s2 = element();
    unsigned s3;
	if(m_p[m_j] == '|')
    {
		m_j++;
		s3 = ++m_state;
		automaton[s3].next1 = s2;
		automaton[s3].next2 = list();
		automaton[s3-1].next1 = automaton[s3-1].next2 = m_state;
		if(automaton[s1].next1 == s2 || automaton[s1].next1 == 0)
			automaton[s1].next1 = s3;
		if(automaton[s1].next2 == s2 || automaton[s1].next2 == 0)
			automaton[s1].next2 = s3;
		return s3;
	}
	return s2;
}

unsigned CLogReader::element()
{
	unsigned s1 = m_state;
    unsigned s2 = 0;

	    if(m_p[m_j] == '('){
		    m_j++;
		    s2 = list();
		    if(m_p[m_j] == ')') {
			    automaton[m_state].next1 = automaton[m_state].next2 = m_state+1;
			    m_state++;
			    m_j++;
		    } else{}
	    } 
        else
        {
		    s2 = v();
        }

	    if(m_p[m_j] == '*') 
        {
            //automaton[m_state].the_char = '*';
		    automaton[m_state].next1 = s2;
       	    automaton[m_state].next2 = m_state+1;
		    s1 = m_state;
            
		    if(automaton[s2].next1 == s2 || automaton[s2].next1 == 0)
			    automaton[s2].next1 = m_state;
       	    if(automaton[s2].next2 == s2 || automaton[s2].next2 == 0)
	       	    automaton[s2].next2 = m_state;
		    m_state++;
   		    m_j++;
	    } 
        else 
        {
       	    if(automaton[s1-1].next1 == 0)
	       	    automaton[s1-1].next1 = s2;
       	    if(automaton[s1-1].next2 == 0)
	       	    automaton[s1-1].next2 = s2;
	    }

	    if(m_p[m_j] != '|' && m_p[m_j] != ')' && m_p[m_j] != '*' && m_p[m_j] != '\0')
		    s1 = element();

	return s1;
}

unsigned CLogReader::v()
{
	unsigned s1 = m_state;
	char *s;

	if(m_p[m_j] == '\\' )
		m_j++;
	else if(!isLetter(m_p[m_j]))
    {
        //clear_automaton();
        while(m_p[m_j] && !isLetter(m_p[m_j]))
        {
            ++m_j;
        }
        //s1 = element();
        //return s1;
    }

	automaton[m_state].the_char = m_p[m_j++];
	automaton[m_state].next1 = automaton[m_state].next2 = m_state+1;
	m_state++;
	return s1;
}

// automaton simulation
int CLogReader::searchLen(const char *str, unsigned start)
{
	if(automaton[0].next1 == 0 && automaton[0].next2 == 0)
		return REGEXPR_NOT_COMPILED;

	int n;
    int m;
    int slen;
    n = m = slen = strlen(str);
   
    

	for(n = start;n < slen;n++)
    {
		m_state = automaton[0].next1;
	    if(automaton[0].next1 != automaton[0].next2)
		    deque.push(automaton[0].next2);
	    deque.put(-1);
        bool match_b;
        m = simulate(str, n, match_b);
		//if(m < (n-1) || match_b) 
        if(match_b) 
        {
			return n;
		}
        else if(m == -2)
            return REGEXPR_NOT_FOUND;
	}
    
	return REGEXPR_NOT_FOUND;
}

#define next_char	-1
int CLogReader::simulate(const char *str, int j, bool &match_b )
{
	int last_match = j - 1;
	size_t len = strlen(str);
    match_b = false;
	do {
		if(m_state == next_char) 
        {
			if(str[j])
				j++;
			deque.put(next_char);
		} 
        else if(automaton[m_state].the_char == str[j] || automaton[m_state].the_char == '?') 
        {
			deque.put(automaton[m_state].next1);
			if(automaton[m_state].next1 != automaton[m_state].next2)
				deque.put(automaton[m_state].next2);
		} 
        else if(!automaton[m_state].the_char) 
        {
			deque.push(automaton[m_state].next1);
			if(automaton[m_state].next1 != automaton[m_state].next2)
				deque.push(automaton[m_state].next2);
		}
       /* else if(automaton[m_state].the_char == '*')
        {
            //deque.push(automaton[m_state].next1);
			if(automaton[m_state].next1 != automaton[m_state].next2)
			{
                int saved_state = automaton[m_state].next2;;
                for(int n = j; n < (len-j);n++)
                {
                    m_state = saved_state;
                    deque.put(automaton[saved_state].next2);
                    deque.put(automaton[saved_state].next2);
                    deque.put(-1);
                    int m = simulate(str, n);
                    if(m < (n-1)) 
                    {
			            return n;
		            }
                }
                return -2;
            }
        
        }
        else if(automaton[m_state].the_char == '?')
        {
            deque.put(automaton[m_state].next1);
			if(automaton[m_state].next1 != automaton[m_state].next2)
				deque.put(automaton[m_state].next2);
        }*/
		m_state = deque.pop();
		if(m_state == 0) {
			last_match = j - 1;
			m_state = deque.pop();
            last_match = last_match - m_state;
            match_b = true;
		}
	} while(j <= len && !deque.isEmpty());

	return last_match;
}
#undef next_char