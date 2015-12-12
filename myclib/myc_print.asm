.386
.model flat, c
option casemap:none
include \masm32\include\msvcrt.inc
includelib  \masm32\lib\msvcrt.lib
include \masm32\include\masm32rt.inc

externdef h_msvcrt:DWORD

.data
	  h_msvcrt dd 0
.code

myc_print_init proc
	invoke LoadLibrary, chr$("msvcrt.dll")
	mov h_msvcrt, eax
	invoke GetProcAddress, h_msvcrt, chr$("printf")
	ret
myc_print_init endp

myc_print proc
	mov ebx, h_msvcrt
	call ebx
	ret
myc_print endp

end