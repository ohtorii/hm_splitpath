@echo off
REM 
REM Coverity�����ɕK�v�ȃt�@�C���𐶐�����
REM 

setlocal

set COV_BUILD=C:\cov-analysis-win64-2021.12.1\bin\cov-build.exe

REM �t�H���_���͕ς��Ȃ�����(Coverity�̃}�j���A���ɐ�������)
set COV_DIR=cov-int
set CURRENT=%~dp0

set SOLUTION=%CURRENT%\..\hm_splitpath.sln
set BUILD_COMMAND=msbuild %SOLUTION% /t:clean;rebuild /p:Configuration=Release;Platform=x64;PostBuildEventUseInBuild=false

if not exist %COV_BUILD% (
    echo cov-build.exe ��������܂���
    exit /b 1
)

call :CreateCleanIntermediateDir
if %errorlevel% neq 0 (
	exit /b 1
)

pushd %~dp0
call :Main
set RESULT=%errorlevel%
popd
exit /b %RESULT%

:Main
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
	if %errorlevel% neq 0 (
		echo VsDevCmd.bat�̎��s�Ɏ��s���܂���
	)
	%COV_BUILD% --dir %COV_DIR% %BUILD_COMMAND%
	if %errorlevel% neq 0 (
		echo cov-build�����s���܂���
		exit /b 1
	)
	REM �������ʂ�WEB�փA�b�v���邽�߂�tar�Ōł߂�
	tar czvf ohtorii_tools.tgz %COV_DIR%
	exit /b 0


:CreateCleanIntermediateDir
    if exist %CURRENT%\%COV_DIR% (
    	rmdir /s /q %CURRENT%\%COV_DIR%
    	if %errorlevel% neq 0 (
    	    echo ���ԃf�B���N�g�����N���A�ł��܂���ł���
    		exit /b %errorlevel%
    	)
    )
    mkdir %CURRENT%\%COV_DIR%
    if not exist %CURRENT%\%COV_DIR% (
    	echo ���ԃf�B���N�g�����쐬�ł��܂���ł����B
    	exit /b 1
    )
    exit /b 0

