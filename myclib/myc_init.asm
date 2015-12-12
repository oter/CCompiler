.386
.model flat, c
option casemap:none

include \masm32\include\msvcrt.inc
includelib  \masm32\lib\msvcrt.lib
include \masm32\include\masm32rt.inc

include myc_print.inc

.code

myc_init_all proc
	call myc_print_init
	ret
myc_init_all endp

end