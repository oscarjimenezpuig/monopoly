/* MONOPOLY 27/4/26 */

#include "acciones.h"

static s1 subasta_flag=-1;

s2 pagar_alquiler(u1 nj) {
    Jugador* j=jugadores+nj;
    Casilla* c=tablero+(j->casilla);
    s2 alquiler=-1;
    if((c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) && c->comprable.poseedor!=-1 && c->comprable.poseedor!=nj) {
        alquiler=0;
        Comprable* cc=&(c->comprable);
        if(c->tipo==CALLE) {
            Calle cll=cc->calle;
            alquiler=cll.alquiler[cll.casas+cll.hotel];
        } else if(c->tipo==NEGOCIO) {
            Negocio* neg=&(cc->negocio);
            alquiler=neg->alquiler;
            neg->alquiler=neg->alquiler_base*dado(1);
        } else if(c->tipo==TRENES) {
            u1 estaciones=0;
            Casilla* pc=tablero;
            while(pc!=tablero+TABSIZ) {
                if(pc->tipo==TRENES && pc->comprable.poseedor==c->comprable.poseedor) estaciones++;
                pc++;
            }
            alquiler=cc->tren.alquiler*estaciones;
        }
    }
    return alquiler;
}           

s1 comprar(u1 nj) {
    Jugador* j=jugadores+nj;
    u1 nc=j->casilla;
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

static u1 barrio_entero(u1 nj,u1 nb) {
    /* esta funcion dice si el barrio nb entero pertenece al jugador nj */
    Casilla* pc=tablero;
    while(pc!=tablero+TABSIZ) {
        if(pc->tipo==CALLE) {
            Calle cll=pc->comprable.calle;
            if(cll.barrio==nb) break;
        }
        pc++;
    }
    u1 ncb=barrios[nb].calles;
    Casilla* fin=pc+ncb;
    while(pc!=fin) {
        if(pc->comprable.poseedor!=nj) return 0;
        pc++;
    }
    return 1;
}

u1 puede_comprar_casa(u1 nj,u1* c) {
    Casilla* pc=tablero;
    u1* npc=c;
    Jugador j=jugadores[nj];
    while(pc!=tablero+TABSIZ) {
        if(pc->tipo==CALLE && pc->comprable.poseedor==nj) {
            if(barrio_entero(nj,pc->comprable.calle.barrio)) {
                if(pc->comprable.calle.hotel==0 && pc->comprable.calle.precio_casa<=j.dinero) {
                    *npc=pc->numero;
                    npc++;
                }
            }
        }
        pc++;
    }
    return npc-c;
}

s1 comprar_casa(u1 nj,u1 nc) {
    u1 cpc[TABSIZ];
    u1 cpcs=0;
    u1 found=0;
    if((cpcs=puede_comprar_casa(nj,cpc))) {
        u1* pcpc=cpc;
        while(pcpc!=cpc+cpcs && !found) {
            if(*pcpc++==nc) found=1;
        }
        if(found) {
            Jugador* j=jugadores+nj;
            Casilla* c=tablero+nc;
            u1 precio_casa=c->comprable.calle.precio_casa;
            if(j->dinero>=precio_casa) {
                j->dinero-=precio_casa;
                if(c->comprable.calle.casas==NUMCASHOT) {
                    c->comprable.calle.hotel=1;
                    return 2;
                } else {
                    c->comprable.calle.casas+=1;
                    return 1;
                }
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

u1 puede_vender(u1 nj,u1* c,u2* pc) {
    Casilla* punt=tablero;
    u1* punc=c;
    u2* punpc=pc;
    while(punt!=tablero+TABSIZ) {
        if((punt->tipo==CALLE || punt->tipo==NEGOCIO || punt->tipo==TRENES) && (punt->comprable.poseedor==nj)) {
            *punc++=punt->numero;
            *punpc++=precio_venta(punt->numero);
        }
        punt++;
    }
    return punc-c;
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
    Jugador* s=jugadores+ns;
    u1 nc=s->casilla;
    Casilla* c=tablero+nc;
    if(c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) {
        u1 dinero=0;
        for(u1 k=0;k<numero_jugadores;k++) {
            if(k!=ns) {
                if(o[k]>dinero && jugadores[k].dinero>=o[k]) {
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

void ir_carcel(u1 nj) {
    static s1 poscar=-1;
    if(poscar==-1) {
        Casilla* pc=tablero;
        while(pc!=tablero+TABSIZ && poscar==-1) {
            if(pc++->tipo==CARCEL) poscar=pc->numero;
        }
    }
    Jugador* j=jugadores+nj;
    j->condenado=1;
    j->casilla=poscar;
} 

s1 en_carcel(u1 nj) {
    Jugador* j=jugadores+nj;
    int ret=0;
    if(j->condenado) {
        ret=-1;
        if(j->carta) {
            j->carta=0;
            ret=1;
        } else if(dado(1)==dado(1)) ret=1;
        else {
            Casilla c=tablero[j->casilla];
            u2 san=c.carcel.sancion;
            if(j->dinero>san) {
                j->dinero-=san;
                ret=3;
            }
        }
    }
    if(ret>0) j->condenado=0;
    return ret;
}

u1 extrae_comunidad(u1 nj) {
    carta_comunidad(nj);
    return 1;
}

u1 extrae_suerte(u1 nj) {
    carta_suerte(nj);
    return 1;
}

u1 impuesto_lujo(u1 nj) {
    const s2 IDL=100;
    Jugador* j=jugadores+nj;
    j->dinero-=IDL;
    return 1;
}

s1 no_arruinado(u1 nj) {
    Jugador* j=jugadores+nj;
    if(j->arruinado==0) {
        if(j->dinero<0) {
            j->arruinado=1;
            for(int k=0;k<TABSIZ;k++) {
                Casilla* c=tablero+k;
                if((c->tipo==CALLE || c->tipo==NEGOCIO || c->tipo==TRENES) && c->comprable.poseedor==nj) {
                    c->comprable.poseedor=-1;
                }
            }
            return 0;
        } else return 1;
    } else return -1;
}

u1 es_ganador(u1 nj) {
    for(int k=0;k<numero_jugadores;k++) {
        if((k==nj && jugadores[k].arruinado) || (k!=nj && !jugadores[k].arruinado)) return 0;
    }
    return 1;
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

void subasta_flag_on(u1 f) {
    subasta_flag=f;
}

void subasta_flag_off() {
    subasta_flag=-1;
}

s1 subasta_flag_get() {
    return subasta_flag;
}
