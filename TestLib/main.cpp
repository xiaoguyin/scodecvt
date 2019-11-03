#include <iostream>
#include <scodecvt/simple_codecvt.h>
#include <clocale>

int main(void)
{
	auto str = U"你a好b😀c";
	auto out = simple_codecvt::to_wstring(str);
	std::setlocale(LC_ALL, "");
	std::wcout << out << std::endl;
	return 0;
}
