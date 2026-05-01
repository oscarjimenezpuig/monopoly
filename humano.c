/* MONOPOLY 1/5/26 */

#include "humano.h"

u1 input(u1 len,char* str) {
    char* ps=str;
    char c=0;
    while(ps-str<len && (c=getchar())!=EOI) {
        *ps++=c;
    }
    *ps=EOS;
    return ps-str;
}

static void humano_venta(u1 nj,u1* cas) {
    /* esta funcion decide si puede vender */
    u1 cav[TABSIZ];
    u2 pcav[TABSIZ];
    u1 cavs=puede_vender(nj,cav,pcav);
    if(cavs>0) {
        prts("VENTA DE POSESIONES");nln;
        prts("Puedes vender las siguientes calles:");nln;
        for(int k=0;k<cavs;k++) {
            Casilla c=tablero[cav[k]];
            prt("%i. %s (%i)",k+1,c.nombre,pcav[k]);
            nln;
        }
        prts("Introduce el numero (0 si no quieres vender nada) ");
        char sval[3];
        input(2,sval);
        u1 val;
        sscanf(sval,"%hhi",&val);
        if(val && vender(nj,cav[val-1])) {
            prts("Vendido");nln;
        }
    }
}

static void humano_casa(u1 nj,u1* cas)

