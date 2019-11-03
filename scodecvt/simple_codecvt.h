#ifndef __SIMPLE_CODECVT_H__
#define __SIMPLE_CODECVT_H__

#ifdef _MSC_VER
#ifdef USING_STATIC_SIMPLE_CODECVT
#define simple_codecvt_api
#elif defined BUILDING_SIMPLE_CODECVT
#define simple_codecvt_api __declspec(dllexport)
#else
#define simple_codecvt_api __declspec(dllimport)
#endif // _WINDLL
#else // _MSC_VER
#define simple_codecvt_api
#endif // _MSC_VER

#include <string>
#include <string_view>
#include <climits>
#include <cstddef>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

namespace simple_codecvt
{
	simple_codecvt_api bool is_big_endian(void) noexcept;

	simple_codecvt_api [[nodiscard]] std::u8string to_utf8(std::string_view text);
	simple_codecvt_api [[nodiscard]] std::u8string to_utf8(std::wstring_view text);
	simple_codecvt_api [[nodiscard]] std::u8string to_utf8(std::u16string_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u8string to_utf8(std::u32string_view text, bool is_utf32be = is_big_endian());

	simple_codecvt_api [[nodiscard]] std::u16string to_utf16(std::string_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u16string to_utf16(std::wstring_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u16string to_utf16(std::u8string_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u16string to_utf16(std::u32string_view text, bool is_utf32be = is_big_endian(), bool is_utf16be = is_big_endian());

	simple_codecvt_api [[nodiscard]] std::u32string to_utf32(std::string_view text, bool is_utf32be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u32string to_utf32(std::wstring_view text, bool is_utf32be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u32string to_utf32(std::u8string_view text, bool is_utf32be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::u32string to_utf32(std::u16string_view text, bool is_utf16be = is_big_endian(), bool is_utf32be = is_big_endian());

	simple_codecvt_api [[nodiscard]] std::string to_string(std::wstring_view text);
	simple_codecvt_api [[nodiscard]] std::string to_string(std::u8string_view text);
	simple_codecvt_api [[nodiscard]] std::string to_string(std::u16string_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::string to_string(std::u32string_view text, bool is_utf32be = is_big_endian());

	simple_codecvt_api [[nodiscard]] std::wstring to_wstring(std::string_view text);
	simple_codecvt_api [[nodiscard]] std::wstring to_wstring(std::u8string_view text);
	simple_codecvt_api [[nodiscard]] std::wstring to_wstring(std::u16string_view text, bool is_utf16be = is_big_endian());
	simple_codecvt_api [[nodiscard]] std::wstring to_wstring(std::u32string_view text, bool is_utf32be = is_big_endian());

	simple_codecvt_api [[nodiscard]] std::u16string change_endian_copy(std::u16string_view text);
	simple_codecvt_api [[nodiscard]] std::u32string change_endian_copy(std::u32string_view text);
	simple_codecvt_api void change_endian(std::u16string &text) noexcept;
	simple_codecvt_api void change_endian(std::u32string &text) noexcept;
	simple_codecvt_api void change_endian(char16_t *text) noexcept;
	simple_codecvt_api void change_endian(char32_t *text) noexcept;
	simple_codecvt_api void change_endian(char16_t *text, std::size_t n) noexcept;
	simple_codecvt_api void change_endian(char32_t *text, std::size_t n) noexcept;

	simple_codecvt_api [[nodiscard]] std::u16string to_big_endian_copy(std::u16string_view text);
	simple_codecvt_api [[nodiscard]] std::u32string to_big_endian_copy(std::u32string_view text);
	simple_codecvt_api void to_big_endian(std::u16string &text) noexcept;
	simple_codecvt_api void to_big_endian(std::u32string &text) noexcept;
	simple_codecvt_api void to_big_endian(char16_t *text) noexcept;
	simple_codecvt_api void to_big_endian(char32_t *text) noexcept;
	simple_codecvt_api void to_big_endian(char16_t *text, std::size_t n) noexcept;
	simple_codecvt_api void to_big_endian(char32_t *text, std::size_t n) noexcept;

	simple_codecvt_api [[nodiscard]] std::u16string to_little_endian(std::u16string_view text);
	simple_codecvt_api [[nodiscard]] std::u32string to_little_endian(std::u32string_view text);
	simple_codecvt_api void to_little_endian(std::u16string &text) noexcept;
	simple_codecvt_api void to_little_endian(std::u32string &text) noexcept;
	simple_codecvt_api void to_little_endian(char16_t *text) noexcept;
	simple_codecvt_api void to_little_endian(char32_t *text) noexcept;
	simple_codecvt_api void to_little_endian(char16_t *text, std::size_t n) noexcept;
	simple_codecvt_api void to_little_endian(char32_t *text, std::size_t n) noexcept;
}

#endif // __SIMPLE_CODECVT_H__
