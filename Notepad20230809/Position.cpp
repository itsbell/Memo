//Position.cpp
#include "Position.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Position::Position() {
	this->row = -1;
	this->column = -1;
}

Position::Position(Long row, Long column) {
	this->row = row;
	this->column = column;
}

Position::Position(const Position& source) {
	this->row = source.row;
	this->column = source.column;
}

Position::~Position() {

}

void Position::SetPosition(Long row, Long column) {
	this->row = row;
	this->column = column;
}

bool Position::operator<(const Position& other)
{
	bool ret = false;
	
	if (this->row < other.row) {
		ret = true;
	}
	else if (this->row == other.row && this->column < other.column) {	
		ret = true;
	}

	return ret;
}

bool Position::operator<=(const Position& other)
{
	bool ret = false;

	if (this->row < other.row) {
		ret = true;
	}
	else if (this->row == other.row && this->column <= other.column) {
		ret = true;
	}

	return ret;
}

bool Position::operator>(const Position& other)
{
	bool ret = false;

	if (this->row > other.row) {
		ret = true;
	}
	else if (this->row == other.row && this->column > other.column) {
		ret = true;
	}

	return ret;
}

bool Position::operator>=(const Position& other)
{
	bool ret = false;

	if (this->row > other.row) {
		ret = true;
	}
	else if (this->row == other.row && this->column >= other.column) {
		ret = true;
	}

	return ret;
}

bool Position::operator==(const Position& other)
{
	bool ret = false;

	if (this->row == other.row && this->column == other.column) {
		ret = true;
	}

	return ret;
}

bool Position::operator!=(const Position& other)
{
	bool ret = false;

	if (this->row != other.row || this->column != other.column) {
		ret = true;
	}

	return ret;
}

Position& Position::operator=(const Position& source) {
	this->row = source.row;
	this->column = source.column;

	return *this;
}