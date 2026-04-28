/* MONOPOLY 27/4/26 */

#include "acciones.h"

s1 comprar(u1 nj,u1 nc) {
    Jugador* j=jugadores+nj;
    Casilla* c=tablero+nc;
    if(c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) {
        if(c->comprable.poseedor==-1) {
            if(c->comprable.precio<=j->dinero) {
                j->dinero-=c->comprable.precio;
                c->comprable.poseedor=nj;
                u1 pos=nc/8;
                u1 bit=1<<(nc%8);
                j->posesion[pos]|=bit;
                return 1;
            } else return -1;
        } else return -2;
    } else return -3;
}

s2 precio_venta(u1 nc) { 
    Casilla c=tablero[nc];
    Comprable cc=c.comprable;
    s2 precio=cc.precio;
    if(c.tipo==CALLE) {
        Calle cl=cc.calle;
        precio+=(cl.casas+cl.hotel)*cl.precio_casa;
    } else if(c.tipo!=NEGOCIO && c.tipo!=TRENES) {
        precio=0;
    }
    precio=(precio*PVC)/100;
    return precio;
}

s1 vender(u1 nj,u1 nc) {
    Jugador* j=jugadores+nj;
    Casilla* c=tablero+nc;
    if(c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) {
        if(c->comprable.poseedor==nj) {
            j->dinero+=precio_venta(nc);
            c->comprable.poseedor=0;
            u1 pp=nc/8;
            u1 bit=1<<(nc%8);
            j->posesion[pp]&=(~bit);
            return 1;
        } else return -1;
    } else return -2;
}

s1 subastar(u1 ns,u1* o) {
    s1 poseedor=-1;
    Jugador s=jugadores+ns;
    u1 nc=s.casilla;
    Casilla* c=tablero+nc;
    if(c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) {
        u1 dinero=0;
        for(u1 k=0;k<numero_jugadores;k++) {
            if(k!=ns) {
                if(o[k]>dinero) {
                    dinero=o[k];
                    poseedor=k;
                }
            }
        }
        if(poseedor>=0) {
            Jugador* j=jugadores+poseedor;
            j->dinero-=dinero;
            c->comprable.poseedor=poseedor;        
            u1 pos=nc/8;
            u1 bit=1<<(nc%8);
            j->posesion[pos]|=bit;
        } 
    }
    return poseedor;
}

u1 casilla_actual(u1 nj) {
    Jugador* j=jugadores+nj;
    Casilla c=tablero[j->casilla];
    u1 t=c.tipo;
    if(t==SUERTE) {
        return 3;
    } else if(t==COMUNIDAD) {
        return 2;
    } else if(t==IR_CARCEL) {
        return -2;
    } else if(t==IMPUESTO) {
        return -3;
    } else if(t==CALLE || t==TRENES || t==NEGOCIO) {
        return 1;
    } else return 0;
}

u1 mover(u1 nj) {
    u1 ret=1;
    Jugador* j=jugadores+nj;
    u1 actual=j->casilla;
    u1 d1=dado(1);
    u1 d2=dado(2);
    ret=(d1==d2)?4:1;
    u1 avance=d1+d2;
    if(actual+avance>=TABSIZ) {
        j->casilla=(j->casilla+avance)%TABSIZ;
        ret|=2;
        j->dinero+=tablero[0].salida.premio;
    } else {
        j->casilla+=avance;
    }
    return ret;
}

    




    

