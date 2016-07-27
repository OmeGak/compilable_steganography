/* 
 * File:   leer.h
 * Author: ome_gak
 *
 * Created on 30 de mayo de 2009, 22:32
 */

#ifndef _LEER_H
#define	_LEER_H

#define BEGIN "int main() {"    // Comienzo del programa
#define MAXLIN 512              // Máximos caracteres por línea
#define MAXVAR 31               // Una variable como máximo tendrá 30(+1 para '\0') caracteres

int leer(const char *ruta,const char *dest);
void VarCabecera(char *lin, FILE *temp);
void VarToBit(char *var, FILE *temp);
unsigned int buscaPos(char *var);

#endif	/* _LEER_H */

