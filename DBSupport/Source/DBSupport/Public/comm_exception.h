
#pragma  once

#ifndef    IBMS_STDLIB_COMM_EXCEPTION
#define    IBMS_STDLIB_COMM_EXCEPTION

#include  <exception>
#include  <string>
#include  <ostream>
#include  <iomanip>

using namespace std;

#ifndef __FUNCTION__
#define  __FUNCTION__ "Unkown"
#endif 

class comm_exception : public  exception
{
public:
    comm_exception(  string   what,   string   throw_file  =  ""    , 
		string  throw_function = "" ,  int  throw_line =- 1 )
        : m_what(what),m_throw_file(throw_file),
        m_throw_function(throw_function),m_throw_line(throw_line),m_catch_line( - 1 ),
        m_error_code( - 1 )
	{

	}

	comm_exception(  int   code,  string   what  =  ""  ,   string   throw_file  =  ""    , 
		string  throw_function = "" ,  int  throw_line =- 1 )
		: m_what(what),m_throw_file(throw_file),
		m_throw_function(throw_function),m_throw_line(throw_line),m_catch_line( - 1 ),
            m_error_code(code)
	{

	}

	virtual ~ comm_exception(void) {}
	
	virtual const char * what()  const throw  () {  return  m_what.c_str(); }
	
    inline  const   char   * get_throw_file()  const  
    {
		return  m_throw_file.c_str();
    }

    inline  const   char   * get_throw_function()  const
    {
		return  m_throw_function.c_str();
    }

    inline  int   get_throw_line()  const
    {
		return  m_throw_line;
    }

    inline  void  set_catch_file( const   char   * catch_file)
    {
        m_catch_file  =  catch_file;
    }

    inline  void  set_catch_function( const   char   * catch_function)
    {
        m_catch_function  =  catch_function;
    }

    inline  void  set_catch_line( int  catch_line)
    {
        m_catch_line  =  catch_line;
    }

    inline  const   char   * get_catch_file()  const  
    {
		return  m_catch_file.c_str();
    }

    inline  const   char   * get_catch_function()  const  
    {
		return  m_catch_function.c_str();
    }

    inline  int  get_catch_line()  const  
    {
		return  m_catch_line;
    }

    inline  int  get_error_code()  const
    {
		return  m_error_code;
    }
	
    string  str()  const ;

protected    :
	string  m_what;
	string  m_throw_file;
	string  m_throw_function;
	int  m_throw_line;
	
	string  m_catch_file;
	string  m_catch_function;
	int  m_catch_line;
    int  m_error_code;

};

//  override  operator << for class type comm_exception
ostream  &   operator   <<  (ostream  & out ,  const  comm_exception  &  e);

//  throw a comm_exception that contains throw position information
#define  THROW_EX(what) \
throw  comm_exception(what, __FILE__,__FUNCTION__,__LINE__);

// throw a comm_exception that contians a error code
#define  THROW_MY_CODE(errorCode) \
     throw  comm_exception((errorCode),  "" , __FILE__, __FUNCTION__, __LINE__ );

//  throw a comm_exception that conts a error code and what 
#define  THROW_MY_CODE0(errorCode,what) \
throw  comm_exception((errorCode), what,__FILE__, __FUNCTION__, __LINE__ );

// throw a exception that derived from comm_exception
#define  THROW_EX_TYPE(type, what) \
     throw  type(what, __FILE__,__FUNCTION__,__LINE__);

//  throw a comm_exception that contians a error code
#define  THROW_MY_CODE_TYPE(type,errorCode) \
throw  type((errorCode),  "" , __FILE__, __FUNCTION__, __LINE__ );

// throw a exception that derived from comm_exception
#define  THROW_MY_CODE0_TYPE(type,errorCode,what) \
throw  type((errorCode), what,__FILE__, __FUNCTION__, __LINE__ );

// set the position information of catching the comm_exception 
#define  SET_CATCH_POS(comm_ex) \
    comm_ex.set_catch_file(__FILE__);\
    comm_ex.set_catch_function(__FUNCTION__);\
comm_ex.set_catch_line(__LINE__);

#ifdef _DEBUG
#define  DEBUG_ONLY(f)      (f)
#else  
#define  DEBUG_ONLY(f)      ((void)0)
#endif 


#endif 