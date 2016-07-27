/*
 * File:   main.c
 * Author: ome_gak
 *
 * Created on 1 de junio de 2009, 13:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estega.h"

void errSintax();

/**
 * @brief	Parseador de argumentos
 * @param	argc número de argumentos de la llamada
 * @param	argv lista de argumentos
 * @post	Comprueba que la llamada del sistema sea sintacticamente correcta.
 * 			Llama a escribir o leer dependiendo del modo (e) o (d)
 * @return	0 si OK, 1 si falla
 */
int main(int argc, char** argv) {
    char i[256];		// Archivo de entrada
    char o[256];		// Archivo de salida
    char p[33];			// Passphrase
    int c = 1;          // Contador de argumentos
    int modo = -1;		// Modo (e) o (d)

    if ((argc < 8) || (argc > 23))
    	errSintax();
    
    // Argumento de modo
    if (strcmp(argv[1], "-e") == 0)
        modo = 0; // Modo de cifrado (El modo -e no es de "encriptar", es de esteganografiar ;) )
    else if (strcmp(argv[1], "-d") == 0)
        modo = 1; // Modo de descifrado

    // Archivo de entrada y salida
    for (c=2;c<6;c+=2){
	if (strcmp(argv[c], "-i") == 0)
            strcpy(i, argv[c+1]);
        else if (strcmp(argv[c], "-o") == 0)
            strcpy(o, argv[c+1]);
        else
            errSintax();
    }



    // Captura de passphrase
    if (strcmp(argv[c], "-p") == 0){
        strcpy(p, "");
        for(c+=1;c<argc;c++){
            strcat(p, argv[c]);
            strcat(p, " ");
        }
    }
    else
        errSintax();

    // Comprobación de tamaño de passphrase
    if (strlen(p) > 32)
        errSintax();

    // Llamada a procedimientos principales
    switch (modo) {
        case 0:
            return estega(i, o, p);
            break;
        case 1:
            return desestega(i, o, p);
            break;
        default:
            printf("Modo incorrecto.\n");
            return 1;
    }

    return 0;
}

/**
 * @brief	Lanza el manual de uso
 */
void errSintax(){
		printf("Sintaxis incorrecta.\n");
        printf("Las siguientes opciones deben proseguir a la llamada del programa:\n");
        printf("\t-e/-d\t\tSeleciona el modo\n");
        printf("\t-i [INPUT]\tArchivo a procesar o descifrar\n");
        printf("\t-o [OUTPUT]\tArchivo de salida\n");
        printf("\t-p [PASSPHRASE]\tFrase de 32 letras\n");
        exit(0);
}
