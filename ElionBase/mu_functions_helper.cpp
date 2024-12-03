#include "stdafx.h"
#include "mu_functions_helper.h"

mu_unicodestr AsciiToUnicode(const mu_asciistr& s)
{
	return boost::locale::conv::utf_to_utf<mu_unicode>(s);
}

mu_asciistr UnicodeToAscii(const mu_unicodestr& s)
{
	return boost::locale::conv::utf_to_utf<mu_char>(s);
}