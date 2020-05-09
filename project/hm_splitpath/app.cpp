#include"pch.h"

/* ドライブ・ディレクトリ・ファイル名・拡張子の間に入る文字。
*/
#define SEPARATOR  L"\t"


struct HIDEMARUFILTERINFO aFilterInfo[] = {
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

static void Splitpath(std::wstring &result_string, WCHAR* pwszIn) {
	std::wistringstream stream(pwszIn);
	std::wstring		line;
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	while (std::getline(stream, line)) {
		if (line.size() == 0) {
			result_string.append(L"\n");
			continue;
		}

		auto err = _wsplitpath_s(
			line.c_str(),
			drive, _countof(drive),
			dir, _countof(dir),
			fname, _countof(fname),
			ext, _countof(ext));
		if (err == 0) {
			//success
			result_string.append(drive);
			result_string.append(SEPARATOR);
			result_string.append(dir);
			result_string.append(SEPARATOR);
			result_string.append(fname);
			result_string.append(SEPARATOR);
			result_string.append(ext);
			result_string.append(L"\n");
		}
		else {
			//error
			result_string.append(line);
			result_string.append(L"\n");
		}
	}
}

extern "C" HGLOBAL _cdecl hm_splitpath(HWND hwndHidemaru, WCHAR* pwszIn, char* /*pszParam*/, int /*cbParamBuffer*/) {
	std::wstring result_string;

	Splitpath(result_string, pwszIn);
	if (result_string.size() == 0) {
		return NULL;
	}

	auto result_byte_size = result_string.size() * sizeof(*result_string.c_str());
	auto mem = AllocMemSub(result_byte_size,hwndHidemaru);
	if (!mem) {
		return NULL;
	}

	errno_t err = -1;
	{
		auto dst = (WCHAR*)GlobalLock(mem);
		if (dst) {
			err = memcpy_s(	dst, GlobalSize(mem), result_string.c_str(), result_byte_size);
			GlobalUnlock(mem);
			dst = NULL;
		}
	}
	if (err == 0) {
		//success
		return mem;
	}
		
	//error
	GlobalFree(mem);
	mem = NULL;

	return NULL;
}