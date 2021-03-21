[BITS 16]
global apm_power_off
	apm_power_off:
    ;mov ax,5301h ; connect real mode interface
    ;sub bx,bx
    ;int 15h
    sti
    mov ax, 0x1000
    mov ax, ss
    mov sp, 0xf000
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15