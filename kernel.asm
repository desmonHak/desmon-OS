;kernel.asm
bits 32                             ;directiva nasm
;[EXTERN end]
section .text
                                    ;Especificacion multiboot para que GRUB cargue el kernel
	align 4
	dd 0x1BADB002                   ;Campo magico que identifica la cabecera
	dd 0x00                         ;Campo flags en cero
	dd - (0x1BADB002 + 0x00)        ;Campo checksum. m+f+c deben ser cero
	;dd  end




global start
	extern init                    ;La funcion init() esta definida en el archivo boot.c
	extern apm_power_off

	start:
		cli                        ;block interrupts
	
		mov BYTE[0x0B8000], '!'
		mov eax, 1Bh
		mov [0x0B8001], eax
		times 510-($-$$) db 0
   		dw 0x55AA
		call init                  ;llama a init
		in eax, 0x60
		mov [0x0B8000], eax
		mov eax, 1Bh
		mov [0x0B8001], eax
		;call apm_power_off
		hlt                        ;detiene el CPU
		
		
;global end
;	end:
;		;Connect to APM API
;		MOV     AX,0x5301
;		XOR     BX,BX
;		INT     15
;
;		;Try to set APM version (to 1.2)
;		MOV     AX,0x530E
;		XOR     BX,BX
;		MOV     CX,0x0102
;		INT     15
;;
;		;Turn off the system
;		MOV     AX,0x5307
;		MOV     BX,0x0001
;		MOV     CX,0x0003
;		INT     15

		;Exit (for good measure and in case of failure)
;		RET
    
section .bss
	stack_space: resb 8192         ; Pila de 8KB
