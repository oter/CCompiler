CC=myc-d
ASMBIN=as
LINKBIN=ld

all : cc asm link
	
cc :
	$(CC) --c main.c
asm :
	$(ASMBIN) main.asm_ -o main.obj
link :
	$(LINKBIN) -s -o main.exe main.obj
clean :
	rm *.obj
	rm *.asm_
	# rm errors.txt
	# rm func.lst



# ml /c /coff /Cp stdlib\std_init.asm
# ml /c /coff /Cp stdlib\std_print.asm

# ml /c /coff /Cp main.asm
# link /subsystem:console main.obj std_print.obj std_init.obj