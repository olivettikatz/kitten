#ifndef __NMCPP_ERROR_H
#define __NMCPP_ERROR_H

#include <iostream>
#include <unistd.h>
#include "data.h"

namespace nmc
{
	using namespace std;

	extern string currentFile;
	extern string currentPage;
	extern stringstream errorBuffer;
	extern unsigned int errorCount;
	extern vector<string> lines;
	extern bool enableDeveloperInfo;

	#define NMC_ERROR(tok, msg) {errorBuffer << "- \033[0;31m" << currentFile << "\033[0;0m:\033[0;31m" << tok.getLine() << "\033[0;0m:" << tok.getCol() << "\033[0;33m " << msg << "\033[0;0m\n  \033[0;32m" << lines[tok.getLine()] << "\033[0;0m\n  " << string(tok.getCol(), ' ') << "^\n"; if(enableDeveloperInfo){errorBuffer << "  ! " << __FILE__ << ":" << __LINE__ << "\n";}errorCount++;}
	#define NMC_NOTE(msg) {errorBuffer << "\t* " << msg << "\n";}

	void initErrors(string f, string p);
	void dumpErrors();
	void dieIfErrors();
}

#endif
