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

static u2 puncall(u1 nj,u1 cc) {
    /* da una puntuacion a una calle */
    /* si un barrio es compartido o lo tiene otro jugador 10000 */
    /* si un barrio lo tiene un jugador solo 1000*porcentaje */
    /* si un barrio no lo tiene nadie 100 */
    /* se le suma alquiler de la casilla a partir de la posesion */
    float puntos=0;
    Calle cll=tablero[cc].comprable.calle;
    u1 tenido=0;
    if(barrcom(cll.barrio,nj)) {
        tenido=0;
        puntos+=10000;
    } else {
        float p=porbarr(nj,cll.barrio);
        puntos+=p*1000;
        puntos=(puntos==0)?100:puntos;
        tenido=NUMCASHOT+1;
    }
    precio+=cll.alquiler[tenido];
    return (u2)puntos;
}
        
static void vecins(u1 c,u1 p,u1 dv,u1* v) {
    /* se inserta en la posicion p del vector v el valor c,dv es la dimension de v */
    for(int k=dv-1;k>=p;k--) v[k+1]=v[k];
    v[p]=c;
}      

static u1 isgrt(u1 ca,u1 cb) {
    /* dice si la casilla ca es mayor que la casilla cb */
    Casilla a=tablero[ca];
    Casilla b=tablero[cb];
    if(a.tipo==NEGOCIO) return 1;
    else if(a.tipo==TRENES && b.tipo!=NEGOCIO) return 1;
    else if(a.tipo==CALLE && b.tipo==CALLE) {

    }
    return 0;
}

static void ordpos(u1* cd,u1 dco,u1* co) {
    /* ordena las posesiones a partir de un baremo, primero los negocios y luego 
     * las estaciones y finalmente las calles ordenadas
     * cd es el vector ordenado, co es el vector desordenado, dco es la dimension */
    u1* pco=co;
    while(pco!=co+dco) {


    
}
