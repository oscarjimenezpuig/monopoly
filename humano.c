/* MONOPOLY 1/5/26 */

#include "humano.h"

u1 input(u1 len,char* str) {
    char* ps=str;
    char c=0;
    while((c=getchar())!=EOI) {
        if(ps-str<len) *ps++=c;
    }
    *ps=EOS;
    return ps-str;
}

void presentacion_humano(u1 nj) {
    Jugador j=jugadores[nj];
    prts("Bienvenido al MONOPOLY Barcelona.");
    nln;
    prt("Eres %s y dispones de %i para batir a los restantes %i jugadores.",j.nombre,j.dinero,numero_jugadores-1);
    nln;
    prts("SUERTE!!!");
    nln;
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
    } else {
        prts("No tienes nada para vender...");
        nln;
    }
}

static void humano_casa(u1 nj) {
   /* esta funcion dice si podemos comprar una casa */
    u1 cac[TABSIZ];
    u1 cacs=puede_comprar_casa(nj,cac);
    if(cacs) {
        prts("COMPRA DE CASAS O HOTELES");
        prts("Puedes comprar casas o hoteles en las siguientes calles:");
        for(int k=0;k<cacs;k++) {
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
    } else {
        prts("No puedes comprar casas.");
        nln;
    }
}

static void humano_posesiones(u1 nj) {
    u1 posesion=0;
    for(int k=0;k<TABSIZ;k++) {
        Casilla c=tablero[k];
        if(c.tipo==CALLE || c.tipo==NEGOCIO || c.tipo==TRENES) {
            Comprable cc=c.comprable;
            if(cc.poseedor==nj) {
                posesion=1;
                if(c.tipo==CALLE) {
                    tab;prt("%s (%s)",c.nombre,barrios[cc.calle.barrio].nombre);nln;
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

static void humano_descripcion(u1 nj) {
    /* funcion que hace una descripcion al inicio del turno */
    Jugador j=jugadores[nj];
    prt("\033[1mTURNO DE %s (HUMANO)\033[0m",j.nombre);
    nln;
    prt("Estas en:");
    nln;
    casprt(j.casilla); 
    prt("Tienes: %i",j.dinero);
    nln;
}

static void humano_ir_carcel(u1 nj) {
    /* funcion que envia al humano a la carcel */
    prts("Has sido condenado a la carcel...");
    nln;
    ir_carcel(nj);
}

static void humano_en_carcel(u1 nj) {
    /* funcion que evalua si un humano esta en la carcel */
    Jugador* j=jugadores+nj;
    if(j->condenado) {
        prts("Estas condenado en la carcel...");
        nln;
        s1 res=en_carcel(nj);
        switch(res) {
           case 1:
               prts("Sales de la carcel porque tienes la Carta de Perdon.");
               break;
           case 2:
               prts("Sales de la carcel por un error judicial.");
               break;
           case 3:
               prts("Sales de la carcel por que pagas la multa.");
               break;
           default:
               prts("Todavia no has cumplido tu condena");
       }
    } else {
        prts("Estas de visita en la carcel.");
    }
   nln;
}

static void humano_suerte(u1 nj) {
    /* extraccion de carta de la suerte de humano */
    prts("Extraes una carta de suerte...");
    nln;
    extrae_suerte(nj);
}

static void humano_comunidad(u1 nj) {
    /* extraccion de carta de comunidad de humano */
    prts("Extraes una carta de comunidad...");
    nln;
    extrae_comunidad(nj);
}

static void humano_alquiler(u1 nj) {
    /* el humano ha de pagar el alquiler */
    Jugador j=jugadores[nj];
    Casilla c=tablero[j.casilla];
    u1 np=c.comprable.poseedor;
    if(nj==np) {
        prts("Acabas de llegar a una calle de tu propiedad.");
        nln;
    } else {
        Jugador p=jugadores[c.comprable.poseedor]; 
        prt("Acabas de llegar a una propiedad de %s.\n",p.nombre);
        prt("Has pagado un alquiler de %i.\n",pagar_alquiler(nj));
    }
}

static void humano_comprar(u1 nj) {
    /* el humano cae en una casilla que puede comprar */
    Jugador j=jugadores[nj];
    Casilla c=tablero[j.casilla];
    if(c.comprable.precio<=j.dinero) {
        prts("Esta propiedad no tiene propietario. Puedes comprarla.");
        nln;
        prts("Si la propiedad no se compra, pasara a subastarse...");
        nln;
        char rsp[2];
        do {
            prts("Quieres comprarla (Y/n)? ");
            input(1,rsp);
        }while(*rsp!='Y' && *rsp!='n');
        if(*rsp=='n') goto subasta;
        else {
            comprar(nj);
            prts("Has comprado la propiedad.");
            nln;
        }
    } else {
        prts("No tienes suficiente dinero para comprarla...");
        nln;
subasta:
        prts("La propiedad pasara a subastarse...");
        nln;
        subasta_flag_on(nj);
    }
}

static void humano_impuesto(u1 nj) {
    prts("Debes pagar un Impuesto de Lujo.");
    nln;
    prt("El pago es de %i.\n",impuesto_lujo(nj));
}

static void humano_mueve(u1 nj) {
    u1 flag=mover(nj);
    if(flag & 8) {
        prts("Por exceso de velocidad, eres condenado a la carcel...");
        nln;
    } else  {
        if(flag & 4) {
            prts("Aceleras... Cuidado no te saltes los limites!!!");
            nln;
        }
        if(flag & 2) {
            prts("Ha pasado un ciclo, te llevaras ingresos de premio...");
            nln;
        }
        if((flag & 1)) {
            prts("Avanzas...");
            nln;
        }
    }
}

static void humano_que_haces(u1 nj) {
    Jugador* j=jugadores+nj;
pregunta:
    prts("Que quieres hacer?");
    nln;
    tab;prts("1. Ver posesiones.");nln;
    tab;prts("2. Vender posesiones. ");nln;
    tab;prts("3. Comprar casa o hotel.");nln;
    if(j->condenado) {
        tab;prts("4. Seguir mi condena.");nln;
    } else {
        tab;prts("4. Avanzar.");nln;
    }
    char sop[2];
    input(1,sop);
    if(*sop=='1') {
        humano_posesiones(nj);
        goto pregunta;
    } else if(*sop=='2') {
        humano_venta(nj);
        goto pregunta;
    } else if(*sop=='3') {
        humano_casa(nj);
        goto pregunta;
    } else if(*sop=='4') {
        if(j->condenado==0) humano_mueve(nj);
    } else goto pregunta;
    j->inicio=0;
}

void turno_humano(u1 nj) {
    humano_descripcion(nj);
    s1 ca=casilla_actual(nj);
    switch(ca) {
        case 3:
            humano_suerte(nj);
            break;
        case 2:
            humano_comunidad(nj);
            break;
        case 1:
            humano_comprar(nj);
            break;
        case -1:
            humano_alquiler(nj);
            break;
        case -2:
            humano_ir_carcel(nj);
            break;
        case -3:
            humano_impuesto(nj);
            break;
        case -4:
            humano_en_carcel(nj);
            break;
    }
    s1 na=no_arruinado(nj);
    if(na==1) {
        humano_que_haces(nj);
    } else if(na==0) {
        prts("Lo siento... TE HAS ARRUINADO!!!");
        nln;
    }
}

u2 subasta_humano(u1 nj,u1 c) {
    Jugador j=jugadores[nj];
    prts("SUBASTA");
    nln;
    prts("Se subasta: ");
    casprt(c);
    while(1) {
        prt("Tienes %i. Cual es tu oferta? ",j.dinero);
        char sof[20];
        input(19,sof);
        u2 of;
        sscanf(sof,"%hi",&of);
        if(of>0 && of<=j.dinero) return of;
    }
    return 0;
}




    
    
    

        
    



        





                        


