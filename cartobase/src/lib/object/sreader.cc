/* Copyright (c) 1995-2005 CEA
 *
 *  This software and supporting documentation were developed by
 *      CEA/DSV/SHFJ
 *      4 place du General Leclerc
 *      91401 Orsay cedex
 *      France
 *
 * This software is governed by the CeCILL license version 2 under 
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the 
 * terms of the CeCILL license version 2 as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license version 2 and that you accept its terms.
 */

//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/object/sreader.h>
#include <cartobase/object/lexicon.h>
#include <cartobase/exception/file.h>
#include <cartobase/exception/parse.h>

using namespace carto;
using namespace std;

//=============================================================================
//	M E T H O D S
//=============================================================================

SyntaxReader::SyntaxReader(const string& filename)
	: _filename(filename), _stream(filename.c_str())
{
	// are files opened?
	if (!_stream) 
		throw file_error(_filename);
}


SyntaxReader::~SyntaxReader()
{
}


void
SyntaxReader::skipWhile(const string& s)
{
	int c = _stream.get();
	while (c != EOF && s.find_first_of(c) != string::npos)
	{
		c = _stream.get();
	}
	_stream.unget();
}


string
SyntaxReader::readUntil(const string& s)
{
	string token;

	int c = _stream.get();
	while (c != EOF && s.find_first_of(c) == string::npos)
	{
		token += static_cast<char>(c);
		c = _stream.get();
	}
	_stream.unget();

	return token;	
}


string
SyntaxReader::nextToken()
{
	skipWhile(" \t\n");
	return readUntil(" \t\n");
}


void
SyntaxReader::read(SyntaxSet& rules)
{
	string token;

	while (1)
	{
		token = nextToken();
		if (_stream.eof() && token == "")
			break;
		if (token != Lexicon::begin())
			throw parse_error(Lexicon::begin(), token, \
				_filename, _stream.line());

		token = nextToken();
		if (_stream.eof() || token != Lexicon::syntax())
			throw parse_error(Lexicon::syntax(), token, \
				_filename, _stream.line());

		token = nextToken();
		if (_stream.eof() || rules.find(token) != rules.end())
			throw parse_error("{new syntactic attribute}", \
				token, _filename, _stream.line());

		read(rules[token]);
	}
}


void
SyntaxReader::read(Syntax& syntax)
{
	string token;

	while (1)
	{
		token = nextToken();
		if (_stream.eof())
			throw parse_error(Lexicon::end(), token, \
				_filename, _stream.line());
		if (token == Lexicon::end())
			break;
		string name = token;

		token = nextToken();
		if (_stream.eof())
			throw parse_error("{semantic attribute type}", token, \
				_filename, _stream.line());
		string type = token;

		skipWhile(" \t");
		token = readUntil(" \t\n");
		if (_stream.eof())
			throw parse_error("!", token, \
				_filename, _stream.line());
		bool needed = (token == "") ? false : true;

		Semantic semantic;
		semantic.type = type;
		semantic.needed = needed;
		syntax[name] = semantic;
	}
}


//=============================================================================
//	F U N C T I O N
//=============================================================================

SyntaxReader&
operator>>(SyntaxReader& reader, SyntaxSet& rules)
{
	reader.read(rules);
	return reader;
}
