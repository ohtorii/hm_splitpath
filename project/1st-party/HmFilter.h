// 2004.12.15 作り始め。
// 「編集・変換」配下のコマンドをDLLに追い出す。ついでに、将来的に多くの変換関数をプラグイン的にサポートする。
// 2008.2.25
//  HIDEMARUFILTERFUNCAは使われないので削除。
// 2016.10.31
//  HIDEMARUINFO_GETMANUALTABとHIDEMARUINFO_GETFILEFULLPATH追記。
// 2016.11.25
//  WM_HIDEMARUINFO等はマクロヘルプに記載して、ここからは削除。
// 2018.8.29
//  abReserve[2]をbReserve,bFlagsに変更。FILTERFLAG_LFONLYを追加。
// 2019.7.22
//  FILTERFLAG_RETURN_*を追加

typedef HGLOBAL (_cdecl* HIDEMARUFILTERFUNC)( HWND hwndHidemaru, WCHAR* pwszIn, char* pszParam, int cbParamBuffer );

struct HIDEMARUFILTERINFO {
    UINT                cbStructSize;
    const char*         pszExportName;
    const char*         pszNameJapan;
    const char*         pszNameUs;
    BYTE                chAccel;        // アクセラレータキー
    BYTE                fMustLineUnit;  // 行単位でないと実行できないフラグ
    BYTE                bReserve;
    BYTE                bFlags;         // FILTERFLAG_*
};

#define FILTERFLAG_RETURN_MASK		0x03
#define FILTERFLAG_RETURN_CRLF		0x00	//
#define FILTERFLAG_RETURN_LFONLY	0x01	//FILTERFLAG_LFONLYと同じ
#define FILTERFLAG_RETURN_RESERVED	0x02	//未使用
#define FILTERFLAG_RETURN_RAW_CR_LF	0x03	//改行コードの区別をそのまま渡す（V8.89β16以降）

#define FILTERFLAG_LFONLY			0x01	//改行を0x0Aだけにする（V8.85β3以降）//FILTERFLAG_RETURN_LFONLYと同じ

// extern "C" HIDEMARUFILTERINFO* _cdecl EnumHidemaruFilter();
// extern "C" DWORD _cdecl HidemaruFilterGetVersion();

