@echo off

setlocal
set BuildDirectory="Build"
if exist %BuildDirectory% (
	echo "%BuildDirectory% directory already exists. Deleting %BuildDirectory% directory."
	rmdir /S /Q %BuildDirectory%
)

mkdir %BuildDirectory%
cd %BuildDirectory%

cmake -G "Visual Studio 17 2022" -A x64 ..\Source -DGRAPHICS_API:STRING=OpenGL -DUSE_SSE:BOOL=FALSE
endlocal
