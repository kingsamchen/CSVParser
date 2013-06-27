/************************************
** Edition:	Demo
** Author:	Kingsley Chen	
** Date:	2013/02/04
** Purpose:	class declaration
************************************/

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _CSVPARSER_53B3DF25_CEB2_4433_A900_943796CD0519_
#define _CSVPARSER_53B3DF25_CEB2_4433_A900_943796CD0519_

#include <iosfwd>
#include <vector>
#include <iterator>

using std::istream;
using std::vector;
using std::string;

class CSVParser
{
    private:
        class CSVParserIterator;

    public:
	    typedef CSVParserIterator field_iterator;

    public:
        CSVParser(istream& in, const string& delim = ",");
        ~CSVParser();

    private:
        CSVParser(const CSVParser& csv);
        CSVParser& operator =(const CSVParser& rhs);

    public:
	    int GetLineContent(string& line);
	    string GetField(int index) const;
	    string operator [](int index) const;
	    int GetNumOfField() const;
        field_iterator cbegin();
        field_iterator cend();
	    int TestDrive();

    private:
	    int Split();
	    int ExtractFromQuote(const string& line, string& field, int begPos) const;
	    int ExtractFromPlain(const string& line, string& field, int begPos) const;
	
    private:
	    istream&        _inStream;
	    string          _line;
	    vector<string>  _field;
	    int             _numField;
	    string          _delim;

    // inner iterator class
    private:
        class CSVParserIterator : public std::iterator<std::random_access_iterator_tag, 
                                                        string>
        {
            public:
                typedef vector<string>::const_iterator PosType;

            public:
                CSVParserIterator() : _container(nullptr)
                {}


                CSVParserIterator(const PosType& pos, CSVParser* container)
                                    : _pos(pos), _container(container)
                {}

                CSVParserIterator(const CSVParserIterator& it) : _pos(it._pos),
                                                                 _container(it._container)
                {}


                ~CSVParserIterator(){}

            public:
                CSVParserIterator& operator =(const CSVParserIterator& rhs)
                {
                    _pos = rhs._pos;
                    _container = rhs._container;

                    return *this;
                }


                string operator *()
                {
                    // random_access_iterator only
                    if (0 == _container->_numField || _pos < _container->_field.begin()
                        || _pos >= _container->_field.end())
                    {
                        throw std::out_of_range("The iterator points to a wrong index");
                    }

                    return *_pos;
                }


                CSVParserIterator& operator ++()
                {
                    ++_pos;
                    return *this;
                }


                // dump action
                CSVParserIterator& operator ++(int)
                {
                    ++_pos;
                    return *this;
                }


                bool operator ==(const CSVParserIterator& rhs)
                {
                    return _pos == rhs._pos;
                }


                bool operator !=(const CSVParserIterator& rhs)
                {
                    return _pos != rhs._pos;
                }

                // other overloaded operators omitted

            private:
                CSVParser*  _container;
                PosType     _pos; 
        };
};

#endif

