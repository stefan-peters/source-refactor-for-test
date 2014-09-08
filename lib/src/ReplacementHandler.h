#ifndef REPLACEMENT_HANDLER_H
#define REPLACEMENT_HANDLER_H

#include "clang/Tooling/Tooling.h"

using namespace clang::tooling;


class ReplacementHandler {
public:
	TranslationUnitReplacements TUReplacement;

public:

	void add(const Replacement& replacement) {
		TUReplacement.Replacements.push_back(replacement);
	}

};


#endif