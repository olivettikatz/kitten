/*!extract.h is the header file for the kitten extraction library.*/

#ifndef __EXTRACT_H
#define __EXTRACT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <streambuf>

namespace extract
{
	using namespace std;

	/*+*/
	class Extractor
	{
	public:
		typedef enum
		{
			extraction,
			attached
		} segmentType;

	private:
		vector<pair<string, string> > ignores;
		string extractStart;
		string extractEnd;
		string enableAttachment;
		string disableAttachment;

		string trimWhitespace(string s);

	public:
		Extractor() {}
		Extractor(string s, string e) : extractStart(s), extractEnd(e) {}
		Extractor(string s, string e, string ea, string da) : extractStart(s), extractEnd(e), enableAttachment(ea), disableAttachment(da) {}
		void ignore(string from, string to);
		void extract(string from, string to);
		void attach(string enable, string disable);
		vector<pair<segmentType, string> > parse(string page);
	};

	string readFile(string path);
	/*-*/
}

#endif
