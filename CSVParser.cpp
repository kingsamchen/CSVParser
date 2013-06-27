/************************************
** Edition:	Demo
** Author:	Kingsley Chen	
** Date:	2013/02/04
** Purpose:	A self-implemented CSV parser
************************************/

#include "StdAfx.h"
#include "CSVParser.h"
#include <algorithm>


CSVParser::CSVParser(istream& in, const string& delim /* = "," */)
			:_inStream(in), _delim(delim), _numField(0)
{

}


CSVParser::~CSVParser()
{

}


/*
	Description:
		Reads a line from the source then parse it
	Parameters:
		line[out] - Content of the line the stream reads, leaving blank if the
		stream has reached the end of the source
	Return Value:
		returns 0 if end-of-file has been reached, otherwise, nonezero
*/
int CSVParser::GetLineContent(string& line)
{
	using std::getline;
	
	_line = "";
	while (_line.empty() && _inStream)
	{
		getline(_inStream, _line);
	}

	line = _line;

	Split();

	/* need to be revised */
	return _inStream ? 1 : 0;
}


int CSVParser::Split()
{
	if (_line.empty())
	{
		return 0;
	}

	_numField = 0;
	string field;
	int i = 0;
	int lineLen = _line.length();

	do 
	{
		if ('"' == _line[i])
		{
			i = ExtractFromQuote(_line, field, i);
		} 
		else
		{
			i = ExtractFromPlain(_line, field, i);
		}

		if (!field.empty())
		{
			if (_numField < static_cast<int>(_field.size()))
			{
				_field[_numField] = field;
			} 
			else
			{
				_field.push_back(field);
			}
			
			++_numField;
		}

		// move to the beginning pos of the next field
		// assuming that field exsits
		++i;
	} while (i < lineLen);

	return _numField;
}


// ONLY FOR TEST INTERFACES
//int CSVParser::TestDrive()
//{
//	//string line[] = {"\"abc,\"\",\",def", "\"a b c d\""};
//	string line = "\"\"";
//	_line = line;
//	string fld;
//
//	int num = Split();
//	printf_s("extracted %d items.\n", num);
//	for (int i = 0; i < (int)_field.size(); ++i)
//	{
//		printf_s("%d. %s\n", i, _field[i].c_str());
//	}
//
//	return 0;
//}


/*
	Description:
		Extracts a field from a specified line and skips empty fields
	Parameters:
		line[in] - line need currently being extracted
		field[out] - field content extracted
		begPos[in] - pos from which to extract
	Return Value:
		pos of the next delimiter in the line or the end
*/
int CSVParser::ExtractFromPlain(const string& line, string& field, int begPos) const
{
	int j = line.find_first_of(_delim, begPos);
	if (j == string::npos)
	{
		j = line.length();
	}

	// may contain useless blanks
	field = line.substr(begPos, j - begPos);

	string tmp;
	std::for_each(field.cbegin(), field.cend(), [&](char ch)
	{
		if (ch != ' ')
		{
			tmp += ch;
		}
	});
	field = tmp;

	return j;	
}


/*
	same as the ExtraFromPlain
*/
int CSVParser::ExtractFromQuote(const string& line, string& field, int begPos) const
{
	int j = begPos + 1;

	// there must be a matched end-quote in correct format
	do 
	{
		// condition statements order cannot be any
		j = line.find_first_of('"', j);
		if (string::npos == j)
		{
			throw std::invalid_argument("format error!");
		}
		else if (j == line.size() - 1)
		{
			break;
		} 
		else if ('"' == line[j+1])
		{
			// across two quotes
			j += 2;
		}
		else if (',' == line[j+1])
		{
			break;
		}
		else
		{
			throw std::invalid_argument("format error!");
		}
	} while (true);

	/* j is at the pos of the matching quote */
	field = line.substr(begPos + 1, j - begPos - 1);
	
	// remove redundant quotes within the field since after
	// extraction, only quotes in pairs alive.
	string tmp;
	int len = static_cast<int>(field.length());
	for (int i = 0; i < len; ++i)
	{
		if ('"' == field[i] && '"' == field[i+1])
		{
			++i;
		}

		tmp += field[i];
	}
	field = tmp;

	return j + 1;
}


int CSVParser::GetNumOfField() const
{
	return _numField;
}


string CSVParser::GetField(int index) const
{
	// why not just _field.at(index)?
	if (index < 0 || index >= _numField)
	{
		throw std::out_of_range("Wrong field index!");
	}

	return _field[index];
}


string CSVParser::operator [](int index) const
{
	return _field[index];
}


typedef CSVParser::field_iterator field_iterator;

field_iterator CSVParser::cbegin()
{
    return field_iterator(_field.cbegin(), this);
}


field_iterator CSVParser::cend()
{
    return field_iterator(_field.cend(), this);
}