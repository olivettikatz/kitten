#include <iostream>
#include "extract.h"

using namespace std;
using namespace extract;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "usage: " << argv[0] << " <INPUT FILE>\n";
		return 1;
	}

	Extractor e = Extractor("/*!", "*/", "/*+*/", "/*-*/");
	e.ignore("\"", "\"");
	e.ignore("'", "'");

	vector<pair<Extractor::segmentType, string> > tmp = e.parse(readFile(argv[1]));

	for (vector<pair<Extractor::segmentType, string> >::iterator i = tmp.begin(); i != tmp.end(); i++)
	{
		if (i->first == Extractor::extraction)
			cout << "Extracted: '" << i->second << "'\n";
		else if (i->first == Extractor::attached)
			cout << "Attached: '" << i->second << "'\n";
	}

	return 0;
}
