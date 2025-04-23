#include "Utils.hpp"
#include <iostream>

void trySizeTConversion(const std::string &str)
{
	size_t num = 0;
	try
	{
		num = toSizeT(str);
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}
	std::cout << "Converted string to size_t: " << num << std::endl;
}

int main()
{
	trySizeTConversion("123");
	trySizeTConversion("123abc");
	trySizeTConversion("abc123");
	trySizeTConversion("123.45");
	trySizeTConversion("0");
	trySizeTConversion("-123");
	trySizeTConversion("-1-123");
	trySizeTConversion("++123");
	trySizeTConversion("+123");
	trySizeTConversion("123+");
	trySizeTConversion("18446744073709551615"); // max size_t
	trySizeTConversion("18446744073709551616"); // overflow
	trySizeTConversion("18446744073709551616abc"); // overflow with trailing characters
	trySizeTConversion("18446744073709551615abc"); // overflow with leading characters
	trySizeTConversion("18446744073709551615.0"); // overflow with decimal point
	trySizeTConversion("18446744073709551615abc123"); // overflow with leading and trailing characters
	trySizeTConversion("18446744073709551615.0abc"); // overflow with decimal point and trailing characters
	trySizeTConversion("18446744073709551615abc123.0"); // overflow with leading, trailing characters and decimal point
}