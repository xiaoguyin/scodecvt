#include "simple_codecvt.h"

#include <stdexcept>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <cstring>

#include <iconv.h>

//#undef _MSC_VER
#ifdef _MSC_VER

#include <Windows.h>

namespace simple_codecvt
{
	template <typename OutputCharType, typename InputCharType>
	static std::basic_string<OutputCharType> to_multistring(const InputCharType *text, std::size_t size)
	{
		using input_type = InputCharType;
		using output_type = OutputCharType;
		using output_string_type = std::basic_string<output_type>;
		static_assert(std::is_same_v<input_type, wchar_t> || std::is_same_v<input_type, char16_t>, "InputCharType must be wchar_t or char16_t.");

		int codepage = 0;
		if constexpr (std::is_same_v<output_type, char>)
		{
			codepage = CP_ACP;
		}
		else if constexpr (std::is_same_v<output_type, char8_t>)
		{
			codepage = CP_UTF8;
		}
		else
		{
			static_assert(false, "OutputCharType must be char or char8_t.");
		}

		auto pinput = reinterpret_cast<const wchar_t *>(text);
		auto insize = static_cast<int>(size);
		int length = ::WideCharToMultiByte(codepage, 0, pinput, insize, nullptr, 0, nullptr, nullptr);
		if (length == 0)
		{
			throw std::invalid_argument("Invalid Unicode was found in a string.");
		}
		output_string_type rst(length, 0);
		::WideCharToMultiByte(codepage, 0, pinput, insize, reinterpret_cast<char *>(rst.data()), static_cast<int>(rst.size()), nullptr, nullptr);

		return rst;
	}

	template <typename OutputCharType, typename InputCharType>
	static std::basic_string<OutputCharType> to_widestring(const InputCharType *text, std::size_t size)
	{
		using input_type = InputCharType;
		using output_type = OutputCharType;
		using output_string_type = std::basic_string<output_type>;
		static_assert(std::is_same_v<output_type, wchar_t> || std::is_same_v<output_type, char16_t>, "OutputCharType must be wchar_t or char16_t.");

		int codepage = 0;
		if constexpr (std::is_same_v<input_type, char>)
		{
			codepage = CP_ACP;
		}
		else if constexpr (std::is_same_v<input_type, char8_t>)
		{
			codepage = CP_UTF8;
		}
		else
		{
			static_assert(false, "InputCharType must be char or char8_t.");
		}

		auto pinput = reinterpret_cast<const char *>(text);
		auto insize = static_cast<int>(size);
		int length = ::MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED, pinput, insize, nullptr, 0);
		if (length == 0)
		{
			throw std::invalid_argument("The function encounters an invalid input character.");
		}
		output_string_type rst(length, 0);
		::MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED, pinput, insize, reinterpret_cast<wchar_t *>(rst.data()), static_cast<int>(rst.size()));

		return rst;
	}

	template <typename OutputCharType>
	static std::basic_string<OutputCharType> to_utf16string(const char32_t *s, std::size_t n)
	{
		using output_type = OutputCharType;
		using output_string_type = std::basic_string<output_type>;
		static_assert(std::is_same_v<output_type, wchar_t> || std::is_same_v<output_type, char16_t>, "OutputCharType must be wchar_t or char16_t.");

		output_string_type container;
		container.reserve(n * 2);

		for (std::size_t i = 0; i < n; ++i)
		{
			auto c = s[i];
			if (c < 0x10000)
			{
				container.push_back(static_cast<output_type>(c));
				continue;
			}
			auto t = c - 0x10000;
			auto h = (((t << 12) >> 22) + 0xD800);
			auto l = (((t << 22) >> 22) + 0xDC00);
			container.push_back(static_cast<output_type>(h));
			container.push_back(static_cast<output_type>(l));
		}

		return container;
	}

	template <typename InputCharType>
	static std::u32string to_utf32string(const InputCharType *s, std::size_t n)
	{
		using input_type = InputCharType;
		static_assert(std::is_same_v<input_type, wchar_t> || std::is_same_v<input_type, char16_t>, "InputCharType must be wchar_t or char16_t.");

		std::u32string container;
		container.reserve(n);

		for (auto iter = s, last = s + n; iter != last;)
		{
			const char16_t prev = *iter;
			++iter;
			if (static_cast<std::size_t>(prev - 0xd800) < 2048)
			{
				if (iter != last)
				{
					const char16_t curr = *iter;
					auto is_high_surrogate = (prev & 0xfffffc00) == 0xd800;
					auto is_low_surrogate = (curr & 0xfffffc00) == 0xdc00;
					if (is_high_surrogate && is_low_surrogate)
					{
						container.push_back((prev << 10) + curr - 0x35fdc00);
						++iter;
						continue;
					}
				}
				throw std::invalid_argument("The function encounters an invalid input character.");
			}
			else
			{
				container.push_back(prev);
			}
		}

		return container;
	}
}
#endif // _MSC_VER

namespace simple_codecvt
{
	template <typename OutputCharType, typename InputCharType>
	static std::basic_string<OutputCharType> iconvert(std::basic_string_view<InputCharType> text, const char *tocode, const char *fromcode)
	{
		using input_type = InputCharType;
		using output_type = OutputCharType;
		using input_string_type = std::basic_string<input_type>;
		using output_string_type = std::basic_string<output_type>;

		typename input_string_type::size_type outlength = text.size();
		if constexpr (std::is_same_v<output_type, char> || std::is_same_v<output_type, char8_t>)
		{
			outlength *= 4;
		}
		else if constexpr (std::is_same_v<output_type, wchar_t> || std::is_same_v<output_type, char16_t>)
		{
			outlength *= 2;
		}
		else if constexpr (!std::is_same_v<output_type, char32_t>)
		{
			static_assert(false, "Character type is illegal.");
		}

		auto cvt = iconv_open(tocode, fromcode);
		if (cvt == reinterpret_cast<iconv_t>(-1))
		{
			throw std::invalid_argument(std::strerror(errno));
		}
		std::shared_ptr<void> ptr(cvt, iconv_close);

		std::basic_string<output_type> ans(outlength, 0);
		auto pinbuf = reinterpret_cast<const char *>(text.data());
		auto poutbuf = reinterpret_cast<char *>(ans.data());
		auto insize = static_cast<size_t>(sizeof(input_type) * text.size());
		auto outsize = static_cast<size_t>(sizeof(output_type) * outlength);
		if (iconv(cvt, &pinbuf, &insize, &poutbuf, &outsize) == static_cast<std::size_t>(-1))
		{
			throw std::invalid_argument(std::strerror(errno));
		}
		ans.resize(ans.size() - outsize / sizeof(output_type));
		ans.shrink_to_fit();

		return ans;
	}
}

#ifdef _MSC_VER
std::u8string simple_codecvt::to_utf8(std::string_view text)
{
	auto tmp = to_widestring<wchar_t>(text.data(), text.size());
	return to_multistring<char8_t>(tmp.data(), tmp.size());
}
#else // _MSC_VER
std::u8string simple_codecvt::to_utf8(std::string_view text)
{
	return iconvert<char8_t, char>(text, "UTF-8", "char");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u8string simple_codecvt::to_utf8(std::wstring_view text)
{
	return to_multistring<char8_t>(text.data(), text.size());
}
#else // _MSC_VER
std::u8string simple_codecvt::to_utf8(std::wstring_view text)
{
	return iconvert<char8_t, wchar_t>(text, "UTF-8", "wchar_t");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u8string simple_codecvt::to_utf8(std::u16string_view text, bool is_utf16be)
{
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return to_multistring<char8_t>(text.data(), text.size());
	}
	auto str = change_endian_copy(text);
	return to_multistring<char8_t>(str.data(), str.size());
}
#else // _MSC_VER
std::u8string simple_codecvt::to_utf8(std::u16string_view text, bool is_utf16be)
{
	return iconvert<char8_t, char16_t>(text, "UTF-8", is_utf16be ? "UTF-16BE" : "UTF-16LE");
}
#endif // _MSC_VER

std::u8string simple_codecvt::to_utf8(std::u32string_view text, bool is_utf32be)
{
	return iconvert<char8_t, char32_t>(text, "UTF-8", is_utf32be ? "UTF-32BE" : "UTF-32LE");
}

#ifdef _MSC_VER
std::u16string simple_codecvt::to_utf16(std::string_view text, bool is_utf16be)
{
	auto str = to_widestring<char16_t>(text.data(), text.size());
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u16string simple_codecvt::to_utf16(std::string_view text, bool is_utf16be)
{
	return iconvert<char16_t, char>(text, is_utf16be ? "UTF-16BE" : "UTF-16LE", "char");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u16string simple_codecvt::to_utf16(std::wstring_view text, bool is_utf16be)
{
	auto str = std::u16string(text.begin(), text.end());
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u16string simple_codecvt::to_utf16(std::wstring_view text, bool is_utf16be)
{
	return iconvert<char16_t, wchar_t>(text, is_utf16be ? "UTF-16BE" : "UTF-16LE", "wchar_t");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u16string simple_codecvt::to_utf16(std::u8string_view text, bool is_utf16be)
{
	auto str = to_widestring<char16_t>(text.data(), text.size());
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u16string simple_codecvt::to_utf16(std::u8string_view text, bool is_utf16be)
{
	return iconvert<char16_t, char8_t>(text, is_utf16be ? "UTF-16BE" : "UTF-16LE", "UTF-8");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u16string simple_codecvt::to_utf16(std::u32string_view text, bool is_utf32be, bool is_utf16be)
{
	std::u16string str;
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		str = to_utf16string<char16_t>(text.data(), text.size());
		if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
		{
			return str;
		}
		change_endian(str);
		return str;
	}
	return iconvert<char16_t, char32_t>(text, is_utf16be ? "UTF-16BE" : "UTF-16LE", is_utf32be ? "UTF-32BE" : "UTF-32LE");
}
#else // _MSC_VER
std::u16string simple_codecvt::to_utf16(std::u32string_view text, bool is_utf32be, bool is_utf16be)
{
	return iconvert<char16_t, char32_t>(text, is_utf16be ? "UTF-16BE" : "UTF-16LE", is_utf32be ? "UTF-32BE" : "UTF-32LE");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u32string simple_codecvt::to_utf32(std::string_view text, bool is_utf32be)
{
	auto tmp = to_widestring<char16_t>(text.data(), text.size());
	auto str = to_utf32string(tmp.data(), tmp.size());
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u32string simple_codecvt::to_utf32(std::string_view text, bool is_utf32be)
{
	return iconvert<char32_t, char>(text, is_utf32be ? "UTF-32BE" : "UTF-32LE", "char");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u32string simple_codecvt::to_utf32(std::wstring_view text, bool is_utf32be)
{
	auto str = to_utf32string(text.data(), text.size());
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u32string simple_codecvt::to_utf32(std::wstring_view text, bool is_utf32be)
{
	return iconvert<char32_t, wchar_t>(text, is_utf32be ? "UTF-32BE" : "UTF-32LE", "wchar_t");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u32string simple_codecvt::to_utf32(std::u8string_view text, bool is_utf32be)
{
	auto tmp = to_widestring<wchar_t>(text.data(), text.size());
	auto str = to_utf32string(tmp.data(), tmp.size());
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u32string simple_codecvt::to_utf32(std::u8string_view text, bool is_utf32be)
{
	return iconvert<char32_t, char8_t>(text, is_utf32be ? "UTF-32BE" : "UTF-32LE", "UTF-8");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::u32string simple_codecvt::to_utf32(std::u16string_view text, bool is_utf16be, bool is_utf32be)
{
	std::u32string str;
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		str = to_utf32string(text.data(), text.size());
	}
	else
	{
		auto tmp = change_endian_copy(text);
		str = to_utf32string(tmp.data(), tmp.size());
	}
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		return str;
	}
	change_endian(str);
	return str;
}
#else // _MSC_VER
std::u32string simple_codecvt::to_utf32(std::u16string_view text, bool is_utf16be, bool is_utf32be)
{
	return iconvert<char32_t, char16_t>(text, is_utf32be ? "UTF-32BE" : "UTF-32LE", is_utf16be ? "UTF-16BE" : "UTF-16LE");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::string simple_codecvt::to_string(std::wstring_view text)
{
	return to_multistring<char>(text.data(), text.size());
}
#else // _MSC_VER
std::string simple_codecvt::to_string(std::wstring_view text)
{
	return iconvert<char, wchar_t>(text, "char", "wchar_t");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::string simple_codecvt::to_string(std::u8string_view text)
{
	auto str = to_widestring<wchar_t>(text.data(), text.size());
	return to_multistring<char>(str.data(), str.size());
}
#else // _MSC_VER
std::string simple_codecvt::to_string(std::u8string_view text)
{
	return iconvert<char, char8_t>(text, "char", "UTF-8");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::string simple_codecvt::to_string(std::u16string_view text, bool is_utf16be)
{
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return to_multistring<char>(text.data(), text.size());
	}
	auto str = change_endian_copy(text);
	return to_multistring<char>(str.data(), str.size());
}
#else // _MSC_VER
std::string simple_codecvt::to_string(std::u16string_view text, bool is_utf16be)
{
	return iconvert<char, char16_t>(text, "char", is_utf16be ? "UTF-16BE" : "UTF-16LE");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::string simple_codecvt::to_string(std::u32string_view text, bool is_utf32be)
{
	std::wstring str;
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		str = to_utf16string<wchar_t>(text.data(), text.size());
	}
	else
	{
		auto tmp = change_endian_copy(text);
		str = to_utf16string<wchar_t>(tmp.data(), tmp.size());
	}
	return to_multistring<char>(str.data(), str.size());
}
#else // _MSC_VER
std::string simple_codecvt::to_string(std::u32string_view text, bool is_utf32be)
{
	return iconvert<char, char32_t>(text, "char", is_utf32be ? "UTF-32BE" : "UTF-32LE");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::wstring simple_codecvt::to_wstring(std::string_view text)
{
	return to_widestring<wchar_t>(text.data(), text.size());
}
#else // _MSC_VER
std::wstring simple_codecvt::to_wstring(std::string_view text)
{
	return iconvert<wchar_t, char>(text, "wchar_t", "char");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u8string_view text)
{
	return to_widestring<wchar_t>(text.data(), text.size());
}
#else // _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u8string_view text)
{
	return iconvert<wchar_t, char8_t>(text, "wchar_t", "UTF-8");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u16string_view text, bool is_utf16be)
{
	if ((is_big_endian() && is_utf16be) || (!is_big_endian() && !is_utf16be))
	{
		return std::wstring(text.begin(), text.end());
	}
	auto str = change_endian_copy(text);
	return std::wstring(str.begin(), str.end());
}
#else // _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u16string_view text, bool is_utf16be)
{
	return iconvert<wchar_t, char16_t>(text, "wchar_t", is_utf16be ? "UTF-16BE" : "UTF-16LE");
}
#endif // _MSC_VER

#ifdef _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u32string_view text, bool is_utf32be)
{
	if ((is_big_endian() && is_utf32be) || (!is_big_endian() && !is_utf32be))
	{
		return to_utf16string<wchar_t>(text.data(), text.size());
	}
	auto str = change_endian_copy(text);
	return to_utf16string<wchar_t>(str.data(), str.size());
}
#else // _MSC_VER
std::wstring simple_codecvt::to_wstring(std::u32string_view text, bool is_utf32be)
{
	return iconvert<wchar_t, char32_t>(text, "wchar_t", is_utf32be ? "UTF-32BE" : "UTF-32LE");
}
#endif // _MSC_VER

std::u16string simple_codecvt::change_endian_copy(std::u16string_view text)
{
	std::u16string str;
	str.reserve(text.size());
	for (auto ch : text)
	{
		str.push_back(static_cast<char16_t>((ch << 8) + (ch >> 8)));
	}
	return str;
}

std::u32string simple_codecvt::change_endian_copy(std::u32string_view text)
{
	std::u32string str;
	str.reserve(text.size());
	for (auto ch : text)
	{
		char32_t c1 = (ch & 0x000000FF) << 24;
		char32_t c2 = (ch & 0x0000FF00) << 8;
		char32_t c3 = (ch & 0x00FF0000) >> 8;
		char32_t c4 = (ch & 0xFF000000) >> 24;
		str.push_back(c1 + c2 + c3 + c4);
	}
	return str;
}

void simple_codecvt::change_endian(std::u16string &text) noexcept
{
	for (auto &ch : text)
	{
		ch = static_cast<char16_t>((ch << 8) + (ch >> 8));
	}
}

void simple_codecvt::change_endian(std::u32string &text) noexcept
{
	for (auto &ch : text)
	{
		char32_t c1 = (ch & 0x000000FF) << 24;
		char32_t c2 = (ch & 0x0000FF00) << 8;
		char32_t c3 = (ch & 0x00FF0000) >> 8;
		char32_t c4 = (ch & 0xFF000000) >> 24;
		ch = c1 + c2 + c3 + c4;
	}
}

void simple_codecvt::change_endian(char16_t *text) noexcept
{
	for (; *text != u'\0'; ++text)
	{
		const auto ch = *text;
		*text = static_cast<char16_t>((ch << 8) + (ch >> 8));
	}
}

void simple_codecvt::change_endian(char32_t *text) noexcept
{
	for (; *text != U'\0'; ++text)
	{
		const auto ch = *text;
		char32_t c1 = (ch & 0x000000FF) << 24;
		char32_t c2 = (ch & 0x0000FF00) << 8;
		char32_t c3 = (ch & 0x00FF0000) >> 8;
		char32_t c4 = (ch & 0xFF000000) >> 24;
		*text = c1 + c2 + c3 + c4;
	}
}

void simple_codecvt::change_endian(char16_t *text, std::size_t n) noexcept
{
	for (std::size_t i = 0; i < n; ++i)
	{
		text[i] = static_cast<char16_t>((text[i] << 8) + (text[i] >> 8));
	}
}

void simple_codecvt::change_endian(char32_t *text, std::size_t n) noexcept
{
	for (std::size_t i = 0; i < n; ++i)
	{
		char32_t c1 = (text[i] & 0x000000FF) << 24;
		char32_t c2 = (text[i] & 0x0000FF00) << 8;
		char32_t c3 = (text[i] & 0x00FF0000) >> 8;
		char32_t c4 = (text[i] & 0xFF000000) >> 24;
		text[i] = c1 + c2 + c3 + c4;
	}
}

std::u16string simple_codecvt::to_big_endian_copy(std::u16string_view text)
{
	if (is_big_endian())
	{
		return std::u16string(text.begin(), text.end());
	}
	return change_endian_copy(text);
}

std::u32string simple_codecvt::to_big_endian_copy(std::u32string_view text)
{
	if (is_big_endian())
	{
		return std::u32string(text.begin(), text.end());
	}
	return change_endian_copy(text);
}

void simple_codecvt::to_big_endian(std::u16string &text) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_big_endian(std::u32string &text) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_big_endian(char16_t *text) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_big_endian(char32_t *text) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_big_endian(char16_t *text, std::size_t n) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text, n);
	}
}

simple_codecvt_api void simple_codecvt::to_big_endian(char32_t *text, std::size_t n) noexcept
{
	if (!is_big_endian())
	{
		change_endian(text, n);
	}
}

simple_codecvt_api std::u16string simple_codecvt::to_little_endian(std::u16string_view text)
{
	if (!is_big_endian())
	{
		return std::u16string(text.begin(), text.end());
	}
	return change_endian_copy(text);
}

simple_codecvt_api std::u32string simple_codecvt::to_little_endian(std::u32string_view text)
{
	if (!is_big_endian())
	{
		return std::u32string(text.begin(), text.end());
	}
	return change_endian_copy(text);
}

simple_codecvt_api void simple_codecvt::to_little_endian(std::u16string &text) noexcept
{
	if (is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_little_endian(std::u32string &text) noexcept
{
	if (is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_little_endian(char16_t *text) noexcept
{
	if (is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_little_endian(char32_t *text) noexcept
{
	if (is_big_endian())
	{
		change_endian(text);
	}
}

simple_codecvt_api void simple_codecvt::to_little_endian(char16_t *text, std::size_t n) noexcept
{
	if (is_big_endian())
	{
		change_endian(text, n);
	}
}

simple_codecvt_api void simple_codecvt::to_little_endian(char32_t *text, std::size_t n) noexcept
{
	if (is_big_endian())
	{
		change_endian(text, n);
	}
}

bool simple_codecvt::is_big_endian(void) noexcept
{
	static union
	{
		unsigned char bytes[4];
		std::uint32_t value;
	} o32_host_order = { { 0, 1, 2, 3 } };
	static auto ans = o32_host_order.value != 0x03020100ul;
	return ans;
}
