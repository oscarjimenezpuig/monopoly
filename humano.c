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

static void humano_venta(u1 nj) {
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

static void humano_casa(u1 nj) {
   /* esta funcion dice si podemos comprar una casa */
    u1 cac[TABSIZ];
    u1 cacs=puede_comprar_casa(nj,cac);
    if(cacs) {
        prts("COMPRA DE CASAS O HOTELES");
        prts("Puedes comprar casas o hoteles en las siguientes calles:");
        for(int k=0;k<cavs;k++) {
            Casilla c=tablero[cac[k]];
            u2 precio=c.comprable.calle.precio_casa;
            prt("%i. %s (%i)",k+1,c.nombre,precio);
            nln;
        }
        prts("Introduce el numero (0 si no quieres comprar ninguna casa) ");
        char sval[3];
        input(2,sval);
        u1 val;
        sscanf(sval,"%hhi",&val);
        s1 res=0;
        if(val && (res=comprar_casa(nj,cac[val-1]))) {
            if(res==1) prts("Comprada casa");
            else if(res==2) prts("Comprado hotel");
        }
    }
}

static void humano_descripcion(u1 nj) {
    /* funcion que hace una descripcion al inicio del turno */
    Jugador j=jugadors[nj];
    prt("TURNO DE %s (HUMANO)",j.nombre);
    nln;
    prt("Estas en:");
    nln;
    casprt(j.casilla); 
    s1 res=0;
    do {
        char ans[2];
        prts("Quieres ver tus posesiones (Y/n)? ");
        input(1,ans);
        res=(*ans=='Y')?1:(*ans=='n')?-1:0;
    }while(!res);
    if(res==1) {
        u1 posesion=0;
        for(int k=0;k<TABSIZ;k++) {
            Casilla c=tablero[k];
            if(c.tipo==CALLE || c.tipo==NEGOCIO || c.tipo==TRENES) {
                Comprable cc=c.comprable;
                if(cc.poseedor==nj) {
                    posesion=1;
                    if(c.tipo==CALLE) {
                        tab;prt("%s (%s)",c.nombre,barrios[cc.calle.barrio]);nln;
                    } else {
                        tab;prt("%s",c.nombre);nln;
                    }
                }
            }
        }
        if(posesion==0){
            prts("No tienes nada");nln;
        }
    }
}


                        


