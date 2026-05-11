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
    puntos+=cll.alquiler[tenido];
    return (u2)puntos;
}
        
static u1 isgrt(u1 nj,u1 ca,u1 cb) {
    /* dice si la casilla ca es mayor que la casilla cb */
    Casilla a=tablero[ca];
    Casilla b=tablero[cb];
    if(a.tipo==NEGOCIO) return 1;
    else if(a.tipo==TRENES && b.tipo!=NEGOCIO) return 1;
    else if(a.tipo==CALLE && b.tipo==CALLE) {
        return (puncall(nj,ca)>puncall(nj,cb));
    }
    return 0;
}

static void ordpos(u1 nj,u1* cd,u1 dco,u1* co) {
    /* ordena las posesiones a partir de un baremo, primero los negocios y luego 
     * las estaciones y finalmente las calles ordenadas
     * cd es el vector ordenado, co es el vector desordenado, dco es la dimension */
    u1 ver[dco];
    u1* pv=ver;
    while(pv!=ver+dco) *pv++=0;
    u1* pd=cd;
    while(pd!=cd+dco) {
        u1* max=NULL;
        u1* pc=co;
        pv=ver;
        while(pc!=co+dco) {
            if(!max || (*pv==0 && isgrt(nj,*pc,*max))) max=pc;
            pc++;
            pv++;
        }
        *pd++=*max;
        *(ver+(max-co))=1;
    }
}

static u1 jugest=0;
static u1 condpro(Casilla* c) {
    if(c && (c->tipo==CALLE || c->tipo==TRENES || c->tipo==NEGOCIO) && c.comprable.poseedor==jugest) return 1;
    return 0;
}

static u1 getpos(u1 nj,u1* p) {
    /* da todas las posesiones de un ordenador jugador */
    jugest=nj;
    u1* pp=p;
    Casilla* pc=pttab(1,condpro);
    while(pc) {
        *pp++=*pc;
        pc=pttab(0,condpro);
    }
    return pp-p;
}

static void ordenador_vender(u1 nj) {
    /* el ordenador ha decidido vender una de sus posesiones */
    u1 caspos[TABSIZ];
    u1 size=getpos(nj,caspos);
    if(size) {
        u1 ocaspos[size];
        ordpos(nj,ocaspos,size,caspos);
        u1 cav=ocaspos[size-1];
        Jugador j=jugadores[nj];
        Casilla c=tablero[cav];
        prt("%s ha vendido %s.",j.nombre,c.nombre);
        nln;
        vender(nj,cav);
    }
}

static void ordenador_comprar(u1 nj,u1 nc) {
    /* el ordenador intenta comprar una casilla comprable */
    Jugador j=jugadores[nj];
    Casilla c=tablero[nc];
    Comprable cc=c.comprable;
    int prac=j.dinero-cc.precio;
    if(prac>=j.riesgo) {
        prt("%s compra %s.",j.nombre,c.nombre);
        nln;
        comprar(nj);
    } else {
        prt("%s no compra %s, la propiedad pasara a subastarse.",j.nombre,c.nombre);
        subasta_flag_on(nj);
    }
}
    
