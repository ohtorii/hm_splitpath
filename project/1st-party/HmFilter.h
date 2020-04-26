// 2004.12.15 ���n�߁B
// �u�ҏW�E�ϊ��v�z���̃R�}���h��DLL�ɒǂ��o���B���łɁA�����I�ɑ����̕ϊ��֐����v���O�C���I�ɃT�|�[�g����B
// 2008.2.25
//  HIDEMARUFILTERFUNCA�͎g���Ȃ��̂ō폜�B
// 2016.10.31
//  HIDEMARUINFO_GETMANUALTAB��HIDEMARUINFO_GETFILEFULLPATH�ǋL�B
// 2016.11.25
//  WM_HIDEMARUINFO���̓}�N���w���v�ɋL�ڂ��āA��������͍폜�B
// 2018.8.29
//  abReserve[2]��bReserve,bFlags�ɕύX�BFILTERFLAG_LFONLY��ǉ��B
// 2019.7.22
//  FILTERFLAG_RETURN_*��ǉ�

typedef HGLOBAL (_cdecl* HIDEMARUFILTERFUNC)( HWND hwndHidemaru, WCHAR* pwszIn, char* pszParam, int cbParamBuffer );

struct HIDEMARUFILTERINFO {
    UINT                cbStructSize;
    const char*         pszExportName;
    const char*         pszNameJapan;
    const char*         pszNameUs;
    BYTE                chAccel;        // �A�N�Z�����[�^�L�[
    BYTE                fMustLineUnit;  // �s�P�ʂłȂ��Ǝ��s�ł��Ȃ��t���O
    BYTE                bReserve;
    BYTE                bFlags;         // FILTERFLAG_*
};

#define FILTERFLAG_RETURN_MASK		0x03
#define FILTERFLAG_RETURN_CRLF		0x00	//
#define FILTERFLAG_RETURN_LFONLY	0x01	//FILTERFLAG_LFONLY�Ɠ���
#define FILTERFLAG_RETURN_RESERVED	0x02	//���g�p
#define FILTERFLAG_RETURN_RAW_CR_LF	0x03	//���s�R�[�h�̋�ʂ����̂܂ܓn���iV8.89��16�ȍ~�j

#define FILTERFLAG_LFONLY			0x01	//���s��0x0A�����ɂ���iV8.85��3�ȍ~�j//FILTERFLAG_RETURN_LFONLY�Ɠ���

// extern "C" HIDEMARUFILTERINFO* _cdecl EnumHidemaruFilter();
// extern "C" DWORD _cdecl HidemaruFilterGetVersion();

