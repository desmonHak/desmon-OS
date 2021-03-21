//extern keyboard

#include "stdio.h"

void set_color_next(uint8_t background, uint8_t foreground){
    screen.background = background*16;
    screen.foreground = foreground;
}

void set_color_now(){
    for(uint16_t contador = 0; all_screen >= contador;contador+=2){
        vidptr[contador+1] = screen.background+screen.foreground;
    }
}


void print(const char *str){
    char end='\0';
    int contador = 0;

    while(str[contador] != end) {
        if ((int)str[contador] == line_salto){
            //vidptr[ultima_direcion] = (char)line_salto;
            ultima_direcion = 80*(1+columna);
            if ((ultima_direcion/80) != 1){
                columna+=2;
            }
        } else{
            vidptr[ultima_direcion] = str[contador];
            vidptr[ultima_direcion+1] = screen.background+screen.foreground;
            ultima_direcion = ultima_direcion + 2;
        }
        
        ++contador;
        
    }

}

void clear_screen(){

    int contador = 0;
    while(contador < 80 * 25 * 2) {
        vidptr[contador] = ' '; 
        vidptr[contador+1] = screen.background+screen.foreground;         
        contador = contador + 2;
    }
    ultima_direcion = 0;

}
