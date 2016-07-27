/**
 * @file escribir.h
 * @author Antonio Sánchez Pera
 * @date 3/6/2009
 */

#ifndef ESCRIBIR_H_
#define ESCRIBIR_H_

#include <stdio.h>

#define DEBUG_

typedef struct t_vector {
	unsigned char *vector;
	long int tam;
	long int leyendo;	
}vector;


vector *escribir_reserva_mem(const char *src);
void escribir_carga_archivo(vector *vec, const char *src);
void escribir_head(const char *dest);
int escribir_genera_main(vector *vec);
int escribir_funcion(vector *vec, int tipo_cara);
void escribir_free(vector *vec);
void escribir_empaquetar(const char *dest);


#endif /*ESCRIBIR_H_*/
