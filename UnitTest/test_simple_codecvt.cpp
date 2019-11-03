#define BOOST_TEST_MODULE simple_codecvt

#ifdef _MSC_VER
#pragma warning(disable: 4189)
#pragma warning(disable: 4702)
#endif

#include <boost/test/included/unit_test.hpp>

#include <scodecvt/simple_codecvt.h>

#include <chrono>

using namespace std::string_literals;

const std::string multistr2 = "你a好bc";
const std::wstring widestr1 = L"你a好b😀c";
const std::wstring widestr2 = L"你a好bc";
const std::u8string u8str1 = u8"你a好b😀c";
const std::u8string u8str2 = u8"你a好bc";
const std::u16string u16str1 = u"你a好b😀c";
const std::u16string u16str2 = u"你a好bc";
const std::u32string u32str1 = U"你a好b😀c";
const std::u32string u32str2 = U"你a好bc";

BOOST_AUTO_TEST_CASE(to_utf8_from_string)
{
	auto str = simple_codecvt::to_utf8(multistr2);
	BOOST_TEST((str == u8str2));
}

#if defined _MSC_VER && !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf8_from_string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	const auto cvtstr = simple_codecvt::to_string(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_utf8(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_utf8_from_string_speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_utf8_from_wstring)
{
	auto str = simple_codecvt::to_utf8(widestr1);
	BOOST_TEST((str == u8str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf8_from_wstring_speed)
{
	const std::u32string u32str4(10000 * 5000, U'😀');
	const auto cvtstr = simple_codecvt::to_wstring(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_utf8(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_utf8_from_wstring_speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_utf8_from_u16string)
{
	auto str = simple_codecvt::to_utf8(u16str1);
	BOOST_TEST((str == u8str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf8_from_u16string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'😀');
	auto cvtstr = simple_codecvt::to_utf16(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf8(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf8_from_u16string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf8(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf8_from_u16string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf8_from_u32string)
{
	auto str = simple_codecvt::to_utf8(u32str1);
	BOOST_TEST((str == u8str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf8_from_u32string_speed)
{
	std::u32string cvtstr(10000 * 5000, U'😀');

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf8(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf8_from_u32string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf8(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf8_from_u32string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf16_from_string)
{
	auto str = simple_codecvt::to_utf16(multistr2);
	BOOST_TEST((str == u16str2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf16_from_string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_string(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf16_from_wstring)
{
	auto str = simple_codecvt::to_utf16(widestr1);
	BOOST_TEST((str == u16str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf16_from_wstring_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_wstring(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_wstring_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_wstring_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf16_from_u8string)
{
	auto str = simple_codecvt::to_utf16(u8str1);
	BOOST_TEST((str == u16str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf16_from_u8string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf8(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u8string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u8string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf16_from_u32string)
{
	auto str = simple_codecvt::to_utf16(u32str1);
	BOOST_TEST((str == u16str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf16_from_u32string_speed)
{
	std::u32string cvtstr(10000 * 5000, U'😀');

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u32string__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, simple_codecvt::is_big_endian(), !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u32string__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	simple_codecvt::change_endian(cvtstr);
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u32string__3: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf16(cvtstr, !simple_codecvt::is_big_endian(), !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf16_from_u32string__4: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf32_from_string)
{
	auto str = simple_codecvt::to_utf32(multistr2);
	BOOST_TEST((str == u32str2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf32_from_string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_string(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_string__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_string__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf32_from_wstring)
{
	auto str = simple_codecvt::to_utf32(widestr1);
	BOOST_TEST((str == u32str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf32_from_wstring_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_wstring(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_wstring_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_wstring_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf32_from_u8string)
{
	auto str = simple_codecvt::to_utf32(u8str1);
	BOOST_TEST((str == u32str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf32_from_u8string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf8(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u8string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u8string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_utf32_from_u16string)
{
	auto str = simple_codecvt::to_utf32(u16str1);
	BOOST_TEST((str == u32str1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_utf32_from_u16string_speed)
{
	std::u32string u32str4(10000 * 5000, U'😀');
	auto cvtstr = simple_codecvt::to_utf16(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u16string__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, simple_codecvt::is_big_endian(), !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u16string__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	simple_codecvt::change_endian(cvtstr);
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u16string__3: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_utf32(cvtstr, !simple_codecvt::is_big_endian(), !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_utf32_from_u16string__4: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_string_from_wstring)
{
	auto str = simple_codecvt::to_string(widestr2);
	BOOST_TEST((str == multistr2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_string_from_wstring_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_wstring(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_string(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_string_from_wstring: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_string_from_u8string)
{
	auto str = simple_codecvt::to_string(u8str2);
	BOOST_TEST((str == multistr2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_string_from_u8string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf8(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_string(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_string_from_u8string_speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_string_from_u16string)
{
	auto str = simple_codecvt::to_string(u16str2);
	BOOST_TEST((str == multistr2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_string_from_u16string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf16(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_string(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_string_from_u16string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_string(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_string_from_u16string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_string_from_u32string)
{
	auto str = simple_codecvt::to_string(u32str2);
	BOOST_TEST((str == multistr2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_string_from_u32string_speed)
{
	std::u32string cvtstr(10000 * 5000, U'叼');

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_string(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_string_from_u32string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_string(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_string_from_u32string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_wstring_from_string)
{
	auto str = simple_codecvt::to_wstring(multistr2);
	BOOST_TEST((str == widestr2));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_wstring_from_string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_string(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_wstring(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_wstring_from_string_speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_wstring_from_u8string)
{
	auto str = simple_codecvt::to_wstring(u8str1);
	BOOST_TEST((str == widestr1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_wstring_from_u8string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf8(u32str4);

	auto begin = std::chrono::system_clock::now();
	auto str = simple_codecvt::to_wstring(cvtstr);
	auto end = std::chrono::system_clock::now();
	std::cout << "to_wstring_from_u8string_speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
#endif

BOOST_AUTO_TEST_CASE(to_wstring_from_u16string)
{
	auto str = simple_codecvt::to_wstring(u16str1);
	BOOST_TEST((str == widestr1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_wstring_from_u16string_speed)
{
	const std::u32string u32str4(10000 * 5000, U'叼');
	auto cvtstr = simple_codecvt::to_utf16(u32str4);

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_wstring(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_wstring_from_u16string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_wstring(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_wstring_from_u16string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif

BOOST_AUTO_TEST_CASE(to_wstring_from_u32string)
{
	auto str = simple_codecvt::to_wstring(u32str1);
	BOOST_TEST((str == widestr1));
}

#if defined _MSC_VER &&  !defined _DEBUG
BOOST_AUTO_TEST_CASE(to_wstring_from_u32string_speed)
{
	std::u32string cvtstr(10000 * 5000, U'叼');

	{
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_wstring(cvtstr);
		auto end = std::chrono::system_clock::now();
		std::cout << "to_wstring_from_u32string_speed__1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
	{
		simple_codecvt::change_endian(cvtstr);
		auto begin = std::chrono::system_clock::now();
		auto str = simple_codecvt::to_wstring(cvtstr, !simple_codecvt::is_big_endian());
		auto end = std::chrono::system_clock::now();
		std::cout << "to_wstring_from_u32string_speed__2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	}
}
#endif
