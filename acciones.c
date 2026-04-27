/* MONOPOLY 27/4/26 */

#include "acciones.h"

s2 comprar(u1 nj,u1 nc) {
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

s2 vender(u1 nj,u1 nc) {
    Jugador* j=jugadores+nj;
    Casilla* c=tablero+nc;
    if(c->tipo==CALLE
