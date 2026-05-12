/* MONOPOLY 9/5/26 */

#include "ordenador.h"

typedef u1 Condition(Casilla*);

#define jo(A) jugadores[(A)]
#define jon(A) jo(A).nombre
#define jod(A) jo(A).dinero
#define joc(A) jo(A).casilla

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
    if(c && (c->tipo==CALLE || c->tipo==TRENES || c->tipo==NEGOCIO) && c->comprable.poseedor==jugest) return 1;
    return 0;
}

static u1 getpos(u1 nj,u1* p) {
    /* da todas las posesiones de un ordenador jugador */
    jugest=nj;
    u1* pp=p;
    Casilla* pc=pttab(1,condpro);
    while(pc) {
        *pp++=pc->numero;
        pc=pttab(0,condpro);
    }
    return pp-p;
}

static void ordenador_venta(u1 nj) {
    /* el ordenador ha decidido vender una de sus posesiones */
    u1 caspos[TABSIZ];
    u1 size=getpos(nj,caspos);
    if(size) {
        u1 ocaspos[size];
        ordpos(nj,ocaspos,size,caspos);
        u1 cav=ocaspos[size-1];
        Casilla c=tablero[cav];
        prt("%s ha vendido %s.",jon(nj),c.nombre);
        nln;
        vender(nj,cav);
    }
}

static u2 puedecomprarcasa(u1 nj,u1 nc) {
    Casilla c=tablero[nc];
    Jugador j=jugadores[nj];
    u2 dinero=j.dinero;
    u2 riesgo=j.riesgo;
    u1 casas=c.comprable.calle.casas;
    u1 pcasa=c.comprable.calle.precio_casa;
    int dif=dinero-pcasa-riesgo;
    if(dif>=0 && casas<(NUMCASHOT+1)) return c.comprable.calle.alquiler[casas+1];
    else return 0;
}

static s1 ordenaalquiler(u1 nj,u1 sc,u1* c) {
    s1 ces=-1;
    u2 rent=0;
    for(u1 k=0;k<sc;k++) {
        u2 pc=puedecomprarcasa(nj,c[k]);
        if(pc>rent) ces=c[k];
    }
    return ces;
}

static void ordenador_casa(u1 nj) {
    u1 ccc[TABSIZ];
    u1 sccc=puede_comprar_casa(nj,ccc);
    s1 ce=ordenaalquiler(nj,sccc,ccc);
    if(ce>0) {
        Casilla c=tablero[ce];
        prt("%s compra edificio en %s.",jon(nj),c.nombre);
        nln;
        comprar_casa(nj,ce);
    }
}


static void ordenador_descripcion(u1 nj) {
   /* funcion que hace una descripcion al inicio del turno */
    prt("\033[1mTURNO DE %s (ORDENADOR)\033[0m",jon(nj));
    nln;
    prt("Esta en:");
    nln;
    casprt(joc(nj)); 
    prt("Tiene: \033[7m%i\033[0m",jod(nj));
    nln;
}


static void ordenador_ir_carcel(u1 nj) {
    /* funcion que envia al ordenador a la carcel */
    prt("%s ha sido condenado a la carcel...",jon(nj));
    nln;
    ir_carcel(nj);
}

static void ordenador_en_carcel(u1 nj) {
    /* funcion que evalua si un ordenador esta en la carcel */
    u1 condenado=jugadores[nj].condenado;
    if(condenado) {
        prt("%s esta condenado en la carcel...",jon(nj));
        nln;
        s1 res=en_carcel(nj);
        switch(res) {
           case 1:
               prts("Sale de la carcel porque tienes la Carta de Perdon.");
               break;
           case 2:
               prts("Sale de la carcel por un error judicial.");
               break;
           case 3:
               prts("Sale de la carcel por que pagas la multa.");
               break;
           default:
               prts("Todavia no ha cumplido su condena");
       }
    } else {
        prt("%s esta de visita en la carcel.",jon(nj));
    }
   nln;
}

static void ordenador_suerte(u1 nj) {
    /* extraccion de carta de la suerte de ordenador */
    prt("%s extrae una carta de suerte...",jon(nj));
    nln;
    extrae_suerte(nj);
}

static void ordenador_comunidad(u1 nj) {
    /* extraccion de carta de comunidad de ordenador */
    prt("%s extrae una carta de comunidad...",jon(nj));
    nln;
    extrae_comunidad(nj);
}

static void ordenador_alquiler(u1 nj) {
    /* el ordenador ha de pagar el alquiler */
    Jugador j=jugadores[nj];
    Casilla c=tablero[j.casilla];
    u1 np=c.comprable.poseedor;
    if(nj==np) {
        prt("%s acaba de llegar a una calle de su propiedad.",jon(nj));
        nln;
    } else {
        Jugador p=jugadores[c.comprable.poseedor];
        if(p.humano) {
            prt("%s acaba de llegar a una calle de tu propiedad (%s).",jon(nj),c.nombre);
            prt("Te ha pagado un alquiler de %i.\n",pagar_alquiler(nj));
        } else {
            prt("%s acaba de llegar a una propiedad de %s.\n",jon(nj),p.nombre);
            prt("Le paga un alquiler de %i.\n",pagar_alquiler(nj));
        }
    }
}

static void ordenador_comprar(u1 nj,u1 nc) {
    /* el ordenador intenta comprar una casilla comprable */
    Jugador j=jo(nj);
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

static void ordenador_impuesto(u1 nj) {
    prt("%s debe pagar un Impuesto de Lujo.",jon(nj));
    nln;
    prt("El pago es de %i.\n",impuesto_lujo(nj));
}

static void ordenador_mueve(u1 nj) {
    u1 flag=mover(nj);
    if(flag & 8) {
        prt("Por exceso de velocidad, %s es condenado a la carcel...",jon(nj));
        nln;
    } else  {
        if(flag & 4) {
            prt("%s Acelera...",jon(nj));
            nln;
        }
        if(flag & 2) {
            prt("%s ha pasado un ciclo, se llevara ingresos de premio...",jon(nj));
            nln;
        }
        if((flag & 1)) {
            prt("%s Avanza...",jon(nj));
            nln;
        }
    }
}

