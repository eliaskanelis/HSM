@echo off
Break ON
echo This batch file runs the Doxygen program using the Doxyfile configuration
echo file.
echo ===================================================

MKDIR "../../doc/"

doxygen.exe "doxyfile" 1>NUL 2>"../../doc/doxygen_stderr.txt"

echo ===================================================
echo Doxygen process completed.
pause