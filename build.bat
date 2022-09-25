@echo off
setlocal

set BuildDirectory="build"
set OutputFilename=NonEuclidian_debug.exe
set FilesToCompile=../main.c

if not exist "build" (
  mkdir "build"
)

pushd %BuildDirectory%

where /q cl || (
  echo MSVC not found.
  exit /b 1
)

set CompilerFlags=/nologo /EHsc /Z7 /W3 /Fe%OutputFilename%
set LinkerFlags=/opt:ref /incremental:no /opt:icf
call cl %FilesToCompile% %CompilerFlags% /link %LinkerFlags%

popd