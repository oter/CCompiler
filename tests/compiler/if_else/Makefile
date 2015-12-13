CC=myc
ASMBIN=JWasm
LINKBIN=link

all : asm link
	cc asm link
 cc :
	$(CC) main.c
asm :
	$(ASMBIN) -4 -coff out.s
link :
	$(LINKBIN) /subsystem:console out.obj
clean :
	rm *.obj
	rm out.s
	# rm errors.txt
	# rm func.lst



# ml /c /coff /Cp stdlib\std_init.asm
# ml /c /coff /Cp stdlib\std_print.asm

# ml /c /coff /Cp main.asm
# link /subsystem:console main.obj std_print.obj std_init.obj