#include <iostream>
#include <sstream>
#include <args/args.h>
#include "extract.h"

using namespace std;
using namespace args;
using namespace extract;

string replaceAll(string src, string pat, string rep)
{
	while(src.find(pat) != string::npos)
		src.replace(src.find(pat), src.find(pat)+pat.size(), rep);
	return src;
}

int main(int argc, char *argv[])
{
	ArgumentParser ap = ArgumentParser();
	ap.setName("kitten-extract-doc");
	ap.setAuthor("Oliver Katz");
	ap.setVersion("0.01 alpha");
	ap.setUsage("kitten-extract-doc [OPTIONS] <INPUT FILES>");
	ap.setDescription("Extracts documentation from source code files.");
	ap << Argument('s', "css", "the path of the stylesheet to use");
	ap << Argument('n', "name", "the name of the document");
	ap << Argument('d', "debug", "enables debugging messages");

	ap.parse(argc, argv);

	Extractor e = Extractor("/*!", "*/", "/*+*/", "/*-*/");
	e.ignore("\"", "\"");
	e.ignore("'", "'");
	
	stringstream prefix;

	prefix << "<html><head>";
	if (ap.getFlag("name"))
		prefix << "<title>" << ap.getValue("name") << "</title>";
	if (ap.getFlag("css"))
		prefix << "<link rel='stylesheet' type='text/css' href='" << ap.getValue("css") << "'>";
	prefix << "</head><body>";

	stringstream links;
	links << "files:<br>";

	stringstream content;

	for (unsigned int i = 0; i < ap.getFreeValues().size(); i++)
	{
		string anchor = replaceAll(ap.getFreeValues()[i], "/", "-");
		content << "<a name='" << anchor << "'></a><div id='file-" << anchor << "'><div id='file-title'>" << ap.getFreeValues()[i] << "</div>";
		links << "<a href='#" << anchor << "'>" << ap.getFreeValues()[i] << "</a><br>";
		vector<pair<Extractor::segmentType, string> > tmp = e.parse(readFile(ap.getFreeValues()[i]));

		for (vector<pair<Extractor::segmentType, string> >::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			string tmp;
			for (string::iterator k = j->second.begin(); k != j->second.end(); k++)
			{
				if (*k == '<')
					tmp += "&lt;";
				else if (*k == '>')
					tmp += "&gt;";
				else
					tmp += *k;
			}

			if (j->first == Extractor::extraction)
			{
				if (ap.getFlag("debug"))
					cerr << "[kitten-extract-doc] Extracted '" << j->second << "'\n";
				content << "<div id='segment-extracted'>" << tmp << "</div>";
			}
			else
			{
				if (ap.getFlag("debug"))
					cerr << "[kitten-extract-doc] Attached '" << j->second << "'\n";
				content << "<div id='segment-attached'><pre><code>" << tmp << "</code></pre></div>";
			}
		}

		content << "</div>";
	}

	links << "<p>";
	content << "</body></html>";
	cout << prefix.str() << links.str() << content.str();

	return 0;
}

