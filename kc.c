#include "_include/stdio.c"
#include "_include/include/ND_Memory.h"
#include "_include/stdint.h"
extern uint32_t end; 

#define null 0
#define NULL null
#pragma GCC diagnostic ignored "-Wcomment"

/**
 * apm_power_off - ask the BIOS to power off
 *
 * Handle the power off sequence. This is the one piece of code we
 * will execute even on SMP machines. In order to deal with BIOS
 * bugs we support real mode APM BIOS power off calls. We also make
 * the SMP call on CPU0 as some systems will only honour this call
 * on their first cpu.
 */

void apm_power_off(void);
/*{
 unsigned char po_bios_call[] = {
  0xb8, 0x00, 0x10, /* movw  $0x1000,ax  /
  0x8e, 0xd0,       /* movw  ax,ss       /
  0xbc, 0x00, 0xf0, /* movw  $0xf000,sp  /
  0xb8, 0x07, 0x53, /* movw  $0x5307,ax  /
  0xbb, 0x01, 0x00, /* movw  $0x0001,bx  /
  0xb9, 0x03, 0x00, /* movw  $0x0003,cx  /
  0xcd, 0x15        /* int   $0x15       / 
 };

 /* Some bioses don't like being called from CPU != 0 /
 if (apm_info.realmode_power_off) {
  set_cpus_allowed_ptr(current, cpumask_of(0));
  machine_real_restart(po_bios_call, sizeof(po_bios_call));
 } else {
  (void)set_system_power_state(APM_STATE_OFF);
 }
}*/
typedef uint32_t size_t;

struct GDT_Entry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));
struct GDT_Ptr{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
struct IDT_Entry{
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));
struct IDT_Ptr{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct regs{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

struct GDT_Entry gdt[3];
struct GDT_Ptr gp;
struct IDT_Entry idt[256];
struct IDT_Ptr idtptr;

void ISR1(){
    asm volatile(
        "cli \n"
        "pushl 0 \n"
        "pushl 0 \n"
        "jmp ND_ISR_Common \n"
    );
}

void ND_ISR_Common(){
    asm volatile(
        "pusha \n"
        "push %ds \n"
        "push %es \n"
        "push %fs \n"
        "push %gs \n"
        "movw $0x10, %ax \n"
        "movw %ax, %ds \n"
        "movw %ax, %es \n"
        "movw %ax, %fs \n"
        "movw %ax, %gs \n"
        "movl %esp, %eax \n"
        "push %eax \n"
        "movl $ND_ISR_Handler, %eax \n"
        "call *%eax \n"
        "popl %eax \n"
        "popl %ds \n"
        "popl %es \n"
        "popl %fs \n"
        "popl %gs \n"
        "popa \n"
        "addl 8, %esp \n"
        "iret \n"
    );
}
void Show(const char* error){
    set_color_next(_stdio.red, _stdio.white);
    clear_screen();
    print("Panic Kernel Error: \n");
    print(error);
}
const char* exception_messages[]={
	"Division by zero, ocurrio un error en el code al intear dividir un numero entre 0",
	"Debug exception",
	"Non maskable interrupt exception",
	"Breakpoint exception",
	"Into detected overflow exception",
	"Out of bounds exception",
	"Invalid opcode",
	"No coprocessor exception",
	"Double fault exception",
	"Coprocessor segment overrdide exception",
	"Bad TSS exception",
	"Segment not present exception",
	"Stack fault exception",
	"General protection fault exception",
	"Page fault exception",
	"Unknown interrupt exception",
	"Coprocessor fault exception",
	"Alignment check exception",
	"Machine check exception",
};
void ND_ISR_Handler(struct regs *r){
    if(r->int_no < 32) {Show(exception_messages[r->int_no]);
    for(;;);
}
}
void Flush(){
	asm volatile("lidtl (idtptr)");
}
void *Set(void* buf, int c, size_t len){
    unsigned char* tmp=(unsigned char*)buf;
    while(len--){
        *tmp++=c;
    }
    return buf;
}

void GDT_SetGate(int num, uint32_t base, uint32_t limit, uint8_t access,uint8_t gran){
    gdt[num].base_low=(base & 0xFFFF);
    gdt[num].base_middle=(base >> 16) & 0xFF;
    gdt[num].base_high=(base >> 24) & 0xFF;
    gdt[num].limit_low=(limit & 0xFFFF);
    gdt[num].granularity=(limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access=access;
}

void IDT_SetGate(uint8_t num,uint32_t base,uint16_t sel, uint8_t flags){
    idt[num].base_low=(base & 0xFFFF);
    idt[num].base_high=(base >> 16) & 0xFFFF;
    idt[num].sel=sel;
    idt[num].always0=0;
    idt[num].flags=flags;
}

void GDT_Install(){
    gp.limit=(sizeof(struct GDT_Entry)*3)-1;
    gp.base=(uint32_t)&gdt;
}

void IDT_Install(){
    idtptr.limit=(sizeof(struct IDT_Entry)*256)-1;
    idtptr.base=(uint32_t)&idt;
    Set(&idt,0,sizeof(struct IDT_Entry)*256);
    IDT_SetGate(0,(unsigned)ISR1,0x08,0x8E);
    Flush();
    print("Tabla IDT instalada\n");
}

char* hex(uint8_t number){
    uint8_t arr[2] = {(int)number/16, (int)number%16};
    const char *letters = "0123456789ABCDEF";
    arr[0] = letters[arr[0]];
    arr[1] = letters[arr[1]];
    return arr;
}
int inthex(char number[2]){
    uint8_t lettersAbstraction[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    const char *letters = "0123456789ABCDEF";
    uint8_t numberExit = 0;
    for(uint8_t contador = 0; contador <= 16; contador++){
        if(number[0] == lettersAbstraction[contador]){
            numberExit += lettersAbstraction[contador]*16;
        }
        if(number[1] == lettersAbstraction[contador]){
            numberExit = lettersAbstraction[contador];
        }
    }
    return numberExit;
}

uint32_t kmalloc(uint32_t sz){
    heap_t *kheap = 0;
    page_directory_t *kernel_directory = 0;
    uint32_t placement_address=(uint32_t)&end;
    uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys){
        if (kheap != 0){
            void *addr = alloc(sz, (uint8_t)align, kheap);
            if (phys != 0){
                page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
                *phys = page->frame*0x1000 + ((uint32_t)addr&0xFFF);
            }
            return (uint32_t)addr;
        } else{
            if (align == 1 && (placement_address & 0xFFFFF000) ){
                // Align the placement address;
                placement_address &= 0xFFFFF000;
                placement_address += 0x1000;
            }
            if (phys){
                *phys = placement_address;
            }
            uint32_t tmp = placement_address;
            placement_address += sz;
            return tmp;
        }
    }
    return kmalloc_int(sz, 0, 0);
}

void OutputB(uint16_t port, uint8_t value){
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}
uint8_t InputB(uint16_t _port){
    unsigned char rv;
    asm volatile("inb %1, %0" : "=a"(rv) : "dN"(_port));
    return rv;
}


void *irq[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void ND_IRQ_InstallHandler(int irq_code,void (*handler)(struct regs* r)){
	irq[irq_code]=(void *)handler;
}
void UninstallHandler(int irq_code){
	irq[irq_code]=0;
}
int ND_TIMER_TICKS=0;

void Phase(int hz)
{
	int divisor=1193180/hz;
	OutputB(0x43,0x36);
	OutputB(0x40, divisor & 0xFF);
	OutputB(0x40, divisor >> 8);
}
void Wait(uint8_t ticks){
    const char *_str = hex(01);
    for(uint32_t eticks = 0; eticks < (ticks*10000)/2; ){
        eticks++;

        const char *str = hex(eticks/60);
        char end='\0';
        int contador = 0;
        while(str[contador] != end) {
            if ((int)str[contador] == line_salto){
                ultima_direcion = 80*(1+columna);
                if ((ultima_direcion/80) != 1){
                    columna+=2;
                }
            } else{
                if(str =! _str){
                    print(str);
                    print(", \n");
                    _str = str;
                } else{
                    _str = hex(eticks/60);
                }
            }
            
            ++contador;
            
        }   
	}
}
void ND_Timer_Handler(struct regs* r)
{
	ND_TIMER_TICKS++;
	if(ND_TIMER_TICKS % 18 ==0)
	{

		 //WE SHOULD DO A REFRESH SCREEN
	}
}
void Setup()
{
	print("\nSetup time... ");
	ND_IRQ_InstallHandler(0,&ND_Timer_Handler);
	print("done\n");
}

void Remap(int pic1, int pic2){
    #define PIC1 0x20
    #define PIC2 0xA0
    #define ICW1 0x11
    #define ICW4 0x01
    /* send ICW1 */
    OutputB(PIC1, ICW1);
    OutputB(PIC2, ICW1);
    /* send ICW2 */
    OutputB(PIC1 + 1, pic1); /* remap */
    OutputB(PIC2 + 1, pic2); /* pics */
    /* send ICW3 */
    OutputB(PIC1 + 1, 4); /* IRQ2 -> connection to slave */
    OutputB(PIC2 + 1, 2);
    /* send ICW4 */
    OutputB(PIC1 + 1, ICW4);
    OutputB(PIC2 + 1, ICW4);
    /* disable all IRQs */
    OutputB(PIC1 + 1, 0xFF);
}

void IRQ1(){
    asm volatile(
    "cli \n"
    "pushl 0\n"
    "pushl 32\n"
    "jmp IRQ_Common"
    );
}

void IRQ_Install(){
    print("Instalando tabla IRQ...\n");
    Remap(0x20,0x28);
    IDT_SetGate(32,(unsigned)IRQ1,0x08,0x8E);
    /*IDT_SetGate(33,(unsigned)IRQ2,0x08,0x8E);
    IDT_SetGate(34,(unsigned)IRQ3,0x08,0x8E);
    IDT_SetGate(35,(unsigned)IRQ4,0x08,0x8E);
    IDT_SetGate(36,(unsigned)IRQ5,0x08,0x8E);
    IDT_SetGate(37,(unsigned)IRQ6,0x08,0x8E);
    IDT_SetGate(38,(unsigned)IRQ7,0x08,0x8E);
    IDT_SetGate(39,(unsigned)IRQ8,0x08,0x8E);
    IDT_SetGate(40,(unsigned)IRQ9,0x08,0x8E);
    IDT_SetGate(41,(unsigned)IRQ10,0x08,0x8E);
    IDT_SetGate(42,(unsigned)IRQ11,0x08,0x8E);
    IDT_SetGate(43,(unsigned)IRQ12,0x08,0x8E);
    IDT_SetGate(44,(unsigned)IRQ13,0x08,0x8E);
    IDT_SetGate(45,(unsigned)IRQ14,0x08,0x8E);
    IDT_SetGate(46,(unsigned)IRQ15,0x08,0x8E);
    IDT_SetGate(47,(unsigned)IRQ16,0x08,0x8E);*/
    print("tabla instalada.\n");
    asm volatile("sti");
}
void IRQ_Common(){
    asm volatile(
    "pusha \n"
    "push %ds\n"
    "push %es\n"
    "push %fs\n"
    "push %gs\n"
    "movw $0x10, %ax \n"
    "movw %ax, %ds \n"
    "movw %ax, %es \n"
    "movw %ax, %fs \n"
    "movw %ax, %gs \n"
    "movl %esp, %eax \n"
    "push %eax \n"
    "movl $ND_IRQ_Handler, %eax \n"
    "call *%eax \n"
    "popl %eax \n"
    "popl %ds \n"
    "popl %es \n"
    "popl %fs \n"
    "popl %gs \n"
    "popa \n"
    "addl 8, %esp \n"
    "iret \n"
    );
}
void ND_IRQ_Handler(struct regs* r){
    void (*handler)(struct regs *r);
    if(r->int_no >= 40){
        OutputB(0xA0,0x20);
    }
    OutputB(0x20,0x20);
}


#define ND_KEYBOARD_KEY_RELEASE 0x80
const char en_US[128]={
0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
0, /* Ctrl */
'a','s','d','f','g','h','j','k','l',';',
'\'','`',0, /* Left Shift */
'\\','z','x','c','v','b','n','m',',','.','/', 0,/* Right shift */
'*', 0, /* Alt */
' ',
0, /* Caps lock*/
0,0,0,0,0,0,0,0,0,0, /* F1-F10 keys */
0, /* Num lock */
0, /* Scroll lock */
0, /* Home key */
0, /* Up arrow */
0, /* Page up */
'-',
0, /* Left arrow */
0,
0, /* Right arrow */
'+',
0, /* End key */
0, /* Down arrow */
0, /* Page down */
0, /* Insert key */
0, /* Delete key */
0,0,0,
0, 0, /* F11-F12 Keys */
0,
81,
};
//unsigned char *en_US = "\x0c\x0b\r\n\t ~}|{`_^]\\[@?>=<;:/.-,+*)(\'&%$#\"!ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba9876543210";
//unsigned char *en_US = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n\r\x0b\x0c";
#define MAX_STRING_SIZE 1024
char stringBuffer[MAX_STRING_SIZE];
unsigned int stringPos=0;
char GetChar(){
	char scancode;
	scancode=InputB(0x60);
    print("valor de entrada en formato hex de la funcion INPUTB: ");
    print(hex(scancode));
    print("\n");
    print("valor hexadecimal del valor obtenido del array mediante el valor de la func: ");
    print(hex(en_US[scancode]));
    print("\n");
    print("valor obtenido del array crudo: ");
    const char *salida = en_US[scancode];
    print(salida);
    print("\n");
	if(scancode & ND_KEYBOARD_KEY_RELEASE){
		return 255;
	}else{
        return en_US[scancode];
	}
}
void ND_Keyboard_Handler(void){
    unsigned char scancode = GetChar();
    if(scancode!=255)
    {
		print(scancode);
		stringBuffer[stringPos]=scancode;
		stringPos++;
	}
}
char* GetString()
{
	while(stringBuffer[stringPos-1]!='\n')
	{
		
	}
	stringPos=0;
	
	return stringBuffer;
}


void init(void) {
    clear_screen();
    Phase(30);
    print(hex(ND_TIMER_TICKS));
    print(hex(ND_TIMER_TICKS));
    print(hex(ND_TIMER_TICKS));
   
    set_color_next(_stdio.black, _stdio.yellow);

    // definir tabla GDT:
    GDT_Install();
    print("Instalando tablas GDT: \n");
    GDT_SetGate(0,0,0,0,0); /* NULL segmente entry */
    print("Segmento NULL establecido\n");
    GDT_SetGate(1,0,0xFFFFFFFF,0x9A,0xCF); /* 4 GiB for Code Segment */
    print("Segmento code establecido\n");
    GDT_SetGate(2,0,0xFFFFFFFF,0x92,0xCF); /* 4 GiB for Data segment */
    print("segmento de datos establecido\n");
    asm volatile("lgdtl (gp)");
    asm volatile(
    "movw $0x10, %ax \n"
    "movw %ax, %ds \n"
    "movw %ax, %es \n"
    "movw %ax, %fs \n"
    "movw %ax, %gs \n"
    "movw %ax, %ss \n"
    "ljmp $0x08, $next \n"
    "next: \n"
    );
    print("tabla GDT fue instalada correctamente.\n");
    // -------------------
    IDT_Install();
    IRQ_Install();

    Setup();

    const char *str = "Bienvenidos a desmon OS:\n";
    uint32_t **pointer = kmalloc(sizeof(char));
    
    pointer = "esto es un texto esta almacenado en un puntero reservado en memoria\n";
    set_color_next(_stdio.black, _stdio.LightCyan);
    print("[*] kernel cagado\n");
    Wait(4000);
    print("modulo principal ejecutadose\n");
    //print(hex(ND_TIMER_TICKS));
    print("\n");
    print(str);
    set_color_next(_stdio.black, _stdio.LightGreen);
    print((const char*)pointer);
    //ND_Keyboard_Handler();
    print("func nueva: ");

    //print(hex(ND_TIMER_TICKS));
    //print(GetString());

    /*while (true){
    
        ND_Keyboard_Handler();
        print(GetString());
        print("\n");
    }*/

    for(;;);


    return;
}

