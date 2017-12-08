#pragma once
#include <exception>
#include <string>
class ParseException :
	public std::exception
{
public:
	std::string message; //custom mesage to avoid making more exception classes
	ParseException(std::string msg) : message(msg){} // decl : init
	~ParseException() throw(){} //destructor is needed to avoid error 
	const char* what() const throw(){ return message.c_str(); } //const so that message is not accidentally modified in catch
};

