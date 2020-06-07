#include"pch.h"

/* ドライブ・ディレクトリ・ファイル名・拡張子の間に入る文字。
*/
#define SEPARATOR		L"\t"
#define SEPARATOR_CHAR  L'\t'

//error C4996 : 'wcscpy' : This function or variable may be unsafe.Consider using wcscpy_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))




static struct HIDEMARUFILTERINFO aFilterInfo[] = {
	{ sizeof(HIDEMARUFILTERINFO), "hm_splitpath", "ファイルパス分解", "Split file path.", 'S', 1, 0, FILTERFLAG_RETURN_LFONLY },
	{ 0, NULL, NULL, NULL, NULL, 0, 0, 0 }
};

extern "C" HIDEMARUFILTERINFO * _cdecl EnumHidemaruFilter() {
	return aFilterInfo;
}

extern "C" DWORD _cdecl HidemaruFilterGetVersion() {
	//1.0.0
	DWORD major = 1;
	DWORD minor = 0;
	DWORD revision = 0;
	return (major << 16) + (minor * 10) + revision;
}

static HGLOBAL AllocMemSub(SIZE_T size_byte, HWND hwndParent) {
	HGLOBAL	mem = GlobalAlloc(LMEM_MOVEABLE, size_byte);
	if (mem) {
		return mem;
	}
	TCHAR	sz[256];	
	wsprintf(sz, TEXT("HmfSort.hmf: Not enough memory, size=%zu error-code=%u"), size_byte, GetLastError());
	MessageBox(hwndParent, sz, NULL, MB_ICONHAND | MB_OK);
	return NULL;	
}

static inline size_t CopyString(wchar_t * __restrict  dst, const wchar_t * __restrict  src) {
	size_t length = 0;
	while ((*src) != L'\0') {
		*dst = *src;

		++dst;
		++src;
		++length;
	}
	return length;
}

static size_t Pass1(std::vector<WCHAR*>	&line_tops, WCHAR* pwszInOut) {
	size_t	result_char_count = 0;

	{
		bool	finish = false;
		auto	current = pwszInOut;
		while (!finish) {
			auto next = wcschr(current, L'\n');
			if (next == nullptr) {
				//最終行
				const auto len = wcslen(current);
				next = current + len;
				result_char_count += len;
				finish = true;
			}
			else {
				result_char_count += next - current;
			}

			//(memo)	SEPARATOR x 3
			//			'\n'      x 1
			result_char_count += 4;

			//'\n' -> '\0'
			next[0] = L'\0';

			if (next[1] == L'\0') {
				finish = true;
			}

			line_tops.push_back(current);
			current = next + 1;
		}
	}

	return result_char_count;
}

static void Pass2(WCHAR* dst, const std::vector<WCHAR*>&line_tops) {	
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	for (auto ptr : line_tops) {
		auto err = _wsplitpath_s(
						ptr,
						drive,	_countof(drive),
						dir,	_countof(dir),
						fname,	_countof(fname),
						ext,	_countof(ext));

DISABLE_C4996

		if (err == 0) {
			//success
			const auto drive_len = CopyString(dst, drive);
			dst += drive_len;

			*dst = SEPARATOR_CHAR;
			++dst;

			const auto dir_len = CopyString(dst, dir);
			dst += dir_len;

			*dst = SEPARATOR_CHAR;
			++dst;

			const auto fname_len = CopyString(dst, fname);
			dst += fname_len;

			*dst = SEPARATOR_CHAR;
			++dst;

			const auto ext_len = CopyString(dst, ext);
			dst += ext_len;

			*dst = L'\n';
			++dst;
		}
		else {
			//error
			const auto ptr_len = CopyString(dst, ptr);
			dst += ptr_len;

			*dst = L'\n';
			++dst;
		}

ENABLE_C4996

	}

	*dst = L'\0';
}

extern "C" HGLOBAL _cdecl hm_splitpath(HWND hwndHidemaru, WCHAR* pwszIn, char* /*pszParam*/, int /*cbParamBuffer*/) {
	HGLOBAL memory_handle = NULL;

	{
		std::vector<WCHAR*>	line_tops;
		line_tops.reserve(1024 * 1024);

		{
			const size_t result_char_count = Pass1(line_tops, pwszIn);
			//(memo) +1 == '\0'のぶん。
			memory_handle = AllocMemSub((result_char_count + 1) * sizeof(WCHAR), hwndHidemaru);
			if (memory_handle == NULL) {
				return NULL;
			}
		}

		{
			auto dst = (WCHAR*)GlobalLock(memory_handle);
			if (dst == NULL) {
				GlobalFree(memory_handle);
				return NULL;
			}

			Pass2(dst, line_tops);
		}

		GlobalUnlock(memory_handle);
	}
	return memory_handle;
}