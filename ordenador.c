/* MONOPOLY 9/5/26 */

#include "ordenador.h"

typedef u1 Condition(Casilla*);

static Casilla* pttab(u1 ini,Condition con) {
    /* recorre todas las casillas, cuando ini, se inica desce el principio */
    static Casilla* ptr=NULL;
    if(ini) ptr=tablero;
count:
    if(ptr!=tablero+TABSIZ) {
        if(con(ptr)) return ptr++;
        else {
            ++ptr;
            goto count;
        }
    } else return NULL;
}

u1 concall(Casilla* c) {
    return (c->tipo==CALLE);
}

static float porbarr(u1 nj,u1 nb) {
    /* da el porcentaje de barrio que tiene un jugador */
    float pos=0;
    Casilla* c=pttab(1,concall);
    while(c) {
        if(c->comprable.calle.barrio==nb && c->comprable.poseedor==nj) pos++;
        c=pttab(0,concall);
    }
    float total=barrios[nb].calles;
    return pos/total;
}

static u1 barrcom(u1 nb,u1 nj) {
    /* dice si un barrio es compartido */
    Casilla* c=pttab(1,concall);
    while(c) {
        if(c->comprable.calle.barrio==nb && c->comprable.poseedor!=nj && c->comprable.poseedor!=-1) return 1;
        c=pttab(0,concall);
    }
    return 0;
}

static void ordpos(u1* cd,u1* co) {
    /* ordena las posesiones a partir de un baremo, primero los negocios y luego */

}
