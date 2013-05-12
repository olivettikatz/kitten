#ifndef __KTP_LEXICAL_PATTERN_H
#define __KTP_LEXICAL_PATTERN_H

#include <iostream>
#include <string>
#include <vector>

namespace ktp
{
	using namespace std;

	class Pattern
	{
	public:
		typedef enum
		{
			isEqualTo,
			isNotEqualTo,
			onlyContains,
			doesNotContain,
			startsWith,
			endsWith,
			alternate
		} algorithmType;

		typedef enum
		{
			attachAnd,
			attachOr,
			attachAndNot,
			attachOrNot,
		} attachmentType;

		typedef bool (*callback)(string s);

	private:
		algorithmType algorithm;
		string argument;
		callback alternateCallback;

		vector<Pattern> andPatterns;
		vector<Pattern> orPatterns;

		vector<pair<attachmentType, Pattern> > attachments;

	public:
		Pattern() : algorithm(alternate), alternateCallback(NULL) {}
		Pattern(algorithmType a, string s) : algorithm(a), argument(s), alternateCallback(NULL) {}
		Pattern(callback a) : alternateCallback(a) {}
		Pattern &setAlgorithm(algorithmType a);
		Pattern &setArgument(string a);
		Pattern &setAlternate(callback a);

		Pattern &attach(attachmentType t, Pattern p);
		
		Pattern &operator == (string s);
		Pattern &operator != (string s);
		Pattern &operator += (string s);
		Pattern &operator -= (string s);
		Pattern &operator < (string s);
		Pattern &operator > (string s);
		Pattern &operator () (callback a);

		Pattern &operator && (Pattern other);
		Pattern &operator || (Pattern other);

		bool compare(Pattern other);


		unsigned int getLengthRequest();

		bool match(string s);
	};

	extern string digits;
	extern string lettersLower;
	extern string lettersUpper;
}

#endif
