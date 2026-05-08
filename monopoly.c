/* MONOPOLY 5/5/26 */

#include "humano.h"

static void inicio() {
    tabinit();
    juginit();
}

static s1 find_humano() {
    for(int k=0;k<numero_jugadores;k++) {
        Jugador j=jugadores[k];
        if(j.humano) return k;
    }
    return -1; 
}

static void cabecera() {
    const int TERMW=80;
    system("clear");
    printf("\033[7mMONOPOLY\033[0m\n");
    printf("oSCAR jIMENEZ pUIG\n");
    printf("2026\n");
    for(int k=0;k<TERMW;k++) printf("=");
    printf("\n");
}

int main() {
    inicio();
    u1 nhumano=find_humano();
    cabecera();
    presentacion_humano(nhumano);
    while(1) {
        for(u1 k=0;k<numero_jugadores;k++) {
            if(k==nhumano) turno_humano(k);
        }
    }
}

