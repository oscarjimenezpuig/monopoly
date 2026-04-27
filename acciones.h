/* MONOPOLY 27/4/26 */

#include "tablero.h"

#define PVC 80 /* tanto por ciento del precio de venta de una casilla */

s2 comprar(u1 jugador,u1 casilla);
/* un jugador compra una casilla 
 * 1: Comprado
 * -1: No hay suficiente dinero
 * -2: No esta libre
 * -3: No es casilla comprable
 */

s2 vender(u1 jugador,u1 casilla);
/* un jugador pone a la venta la casilla
 *
