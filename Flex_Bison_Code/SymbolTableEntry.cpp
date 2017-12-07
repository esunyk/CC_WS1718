#include "SymbolTableEntry.h"
#include <string>

SymbolTableEntry::~SymbolTableEntry()
{
}

SymbolTableEntry::SymbolTableEntry(){

}
SymbolTableEntry::SymbolTableEntry(std::string type, int decLine, int decPos){
	this->type = type;
	this->decLine = decLine;
	this->decPos = decPos;

}

SymbolTableEntry::SymbolTableEntry(std::string type){
	this->type = type;
}

SymbolTableEntry::SymbolTableEntry(std::string type, int decLine, int decPos, SymbolTableEntry* entry){
	this->type = type;
	this->decLine = decLine;
	this->decPos = decPos;
	this->typeDecl = entry;
}
int SymbolTableEntry::getDecLine(){
	return this->decLine;
}

int SymbolTableEntry::getDecPos(){
	return this->decPos;
}

std::string SymbolTableEntry::getType(){
	return this->type;
}

void SymbolTableEntry::setDecLine(int decLine){
	this->decLine = decLine;
}

void SymbolTableEntry::setDecPos(int decPos){
	this->decPos = decPos;
}

void SymbolTableEntry::setType(std::string type){
	this->type = type;
}