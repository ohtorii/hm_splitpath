// test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include<vector>
#include <io.h>
#include <chrono>
#include"hm_splitpath.h"

namespace {
	bool LoadFile(std::vector<wchar_t>& out_fileimage, const wchar_t* filename) {
		FILE* fp = 0;
		errno_t  error = _wfopen_s(&fp, filename, L"rb");
		if (error != 0) {
			return false;
		}
		if (fp == 0) {
			return false;
		}

		{
			auto byte_size = _filelengthi64(_fileno(fp));
			if (byte_size == 0) {
				return true;
			}
			auto num = byte_size / sizeof(wchar_t);
			out_fileimage.reserve(num + 1);
			out_fileimage.resize(num);
		}
		size_t elementSize = 1;
		size_t count = out_fileimage.size() * sizeof(wchar_t);
		fread_s(
			&out_fileimage.at(0), 
			out_fileimage.size()*sizeof(wchar_t),
			elementSize,
			count, 
			fp);
		fclose(fp);

		out_fileimage.push_back(L'\0');

		return true;
	}

	bool LoadFileUTF8(std::vector<wchar_t>& out_fileimage, const wchar_t* filename) {
		FILE* fp = 0;
		errno_t  error = _wfopen_s(&fp, filename, L"rt,ccs=UTF-8");
		if (error != 0) {
			return false;
		}
		if (fp == 0) {
			return false;
		}
		
		{
			auto byte_size = _filelengthi64(_fileno(fp));
			out_fileimage.reserve(byte_size / sizeof(wchar_t) + sizeof(L'\0'));
		}

		wchar_t buffer[1024];
		while (std::fgetws(buffer, _countof(buffer), fp) != NULL) {
			auto len = wcsnlen_s(buffer, _countof(buffer));
			out_fileimage.insert(out_fileimage.end(), &buffer[0], &buffer[len]);
			
		}
		out_fileimage.push_back(L'\0');
		fclose(fp);
		return true;
	}

} /*namespace*/

int main()
{
    std::vector<wchar_t> filelist;
	std::chrono::steady_clock clock;	
	auto current_clock = clock.now();

	printf("File load start\n");

	auto filename = L"D:\\temp\\giga_byte_file_list_mini.txt";
	//auto filename = L"D:\\temp\\giga_byte_file_list_utf16.txt";
	if (! LoadFile(filelist,filename)) {
		printf("[error]File load\n");
		return 1;
	}
	printf("File load finish.(%f seconds)\n", static_cast<std::chrono::duration<double>>(clock.now() - current_clock).count());
	current_clock = clock.now();

	printf("hm_splitpath start.\n");
    HGLOBAL memory_handle = hm_splitpath(NULL, &filelist.at(0), NULL, 0);
	if (memory_handle == NULL) {
		printf("[error]hm_splitpath\n");
		return 1;
	}	
	printf("hm_splitpath finish.(%f seconds)\n", static_cast<std::chrono::duration<double>>(clock.now() - current_clock).count());

	if (true) {
		//デバッグ用途
		//結果をコマンドプロンプトへ出力する。
		auto string_top = static_cast<wchar_t*>(GlobalLock(memory_handle));
		wprintf_s(L"%s\n", L"==== begin ====");
		wprintf_s(L"%s", string_top);
		wprintf_s(L"%s\n", L"==== end ====");
		GlobalUnlock(memory_handle);
	}
	GlobalFree(memory_handle);
	return 0;
}
