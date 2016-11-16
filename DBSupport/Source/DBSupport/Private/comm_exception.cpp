
#include "DBSupportPrivatePCH.h"
#include  "comm_exception.h"
#include  < sstream >
using   namespace  std;


ostream & operator <<   (ostream   &  out  ,   const   comm_exception   &     e)
{
	out <<   " what:     "   << e.what() << endl
		<<   " code:      "   << e.get_error_code()  << endl
		<<   " throw exception at:----------------- "   <<  endl
		<<   " file:     "   <<  e.get_throw_file()  << endl
		<<   " function: "   <<  e.get_throw_function()  << endl
		<<   " line:     "   << e.get_throw_line() << endl
		<<   " catch excetion at:------------------ "   <<  endl
		<<   " file:     "   <<  e.get_catch_file() << endl
		<<   " function: "   <<  e.get_catch_function() << endl
		<<   " line:     "   << e.get_catch_line() << endl;
	return   out ;
}

string   comm_exception::str()   const     
{
	stringstream ss;
	ss  <<   * this ;
	return  ss.str();
}

