# scodescv

Simple code conversion library for C++.

## Sample

```C++
#include <scodecvt/simple_codecvt.h>

namespace sc = simple_codecvt;

int main(void)
{
	auto str = U"你a好b😀c";
	auto widestring = sc::to_wstring(str); // to wide character string
	auto utf8string = sc::to_utf8(str); // to UTF-8 string
	auto utf16string = sc::to_utf16(str); // to UTF-16 string
	auto utf32string = sc::to_utf32(utf16string); // to UTF-32 string

	return 0;
}
```

## Windows

### Visual Studio 2019

1. Right click project → **Manage NuGet packages...** → **Browse** → search `libscodecvt`，install package.

2. Project configuration: **C/C++** → **Langage** → **C++ langage standard** → **std:c++lastest**

