/**
 * @file estega.c
 * @author Antonio Sánchez Perea
 * @date 3/6/2009
 */

#include <stdlib.h>
#include <stdio.h>
#include "zcomp.h" //zlib
#include "encrypt_decrypt.h" //Rijndael
#include "escribir.h" //Esteganografiar
#include "leer.h" //Deses


/**
 * @brief Esta funcion genera un código esteganografiado en un código fuente
 * @param const char *src, archivo que se desea ocultar.
 * @param const char *dest, archivo de salida. (será .c)
 * @param const char *pass, passphrase para cifrar el archivo.
 * @return 1 si todo ha ido correcto, 0 en caso contrario.
 */

int estega(const char *src, const char *dest, const char *pass) {
	//Definición de variables
	FILE *fdsrc, *fddest;
	int sign, tipo_cara=1;  //sign nos sirve para saber si todavía se tiene que seguir escribiendo código.
	int cont=0;
	vector *estructura;
	int errN;
	
	//1º Comprimimos (Usamos zlib)
	//Generamos el archivo temporal sal_zlib
	printf("Comprimimos...\n");
  	if (!(fdsrc=fopen(src,"rb"))) {
  		printf("No se ha podido encontrar el archivo %s, se interrumpe la ejecución\n",src);	
  		return 0;
  	}
  	
  	fddest=fopen("sal_zlib","wb");
	errN=zdef(fdsrc, fddest, Z_DEFAULT_COMPRESSION);
	if (errN != Z_OK) {
		zerr(errN);
		return 0;
	}
	fclose(fdsrc);
	fclose(fddest);	
	//2ºCiframos con Rijndael
	printf("Ciframos...\n");
	errN=encrypt("sal_zlib", "sal_rdl", pass);
	if (!errN) { printf("Error al cifrar el archivo\n"); return 0; }

	remove("sal_zlib");

	//3ºOcultamos en código.	
	//Lo tenemos que sacar del archivo sal_rdl
	printf("Ocultamos... (Puede tardar un poco dependiendo del tamaño del archivo)\n");
	if ((estructura=escribir_reserva_mem("sal_rdl"))) {
		escribir_carga_archivo(estructura, "sal_rdl"); //cargamos el archivo en memoria.

		remove("head_tmp"); //Por si existe, lo borramos
		remove("func_tmp"); //Por si existe, lo borramos
		remove("sal_rdl"); //Ya no hace falta este archivo
		
		sign=escribir_genera_main(estructura); //Genera el main temporal

		while (sign) {
			cont++;
			sign=escribir_funcion(estructura, tipo_cara); //escribe una función
			tipo_cara*=-1;
		}
		escribir_empaquetar(dest);
		//Liberamos memoria
		escribir_free(estructura);
	}
	else {
		printf("Error: Memoria insuficiente\n");
		return 0;
	}
	//Por si existen, los borramos
	remove("head_tmp"); 
	remove("func_tmp");
	remove("main_tmp");
	
	printf("Terminado!\n\a");
	return 1;
}

/**
 * @brief Esta funcion genera un mensaje en claro desde un código fuente
 * @param const char *src, archivo fuente.
 * @param const char *dest, archivo en claro.
 * @param const char *pass, passphrase para descifrar el mensaje.
 * @return 1 si todo ha ido correcto, 0 en caso contrario.
 */

int desestega(const char *src, const char *dest, const char *pass) {
	int errN;
	FILE *fdsrc, *fddest;
	
	//1º pasamos de fuente al cifrado
	printf("Leemos... (Esto puede tardar un poco dependiendo del tamaño del archivo)\n");
	if (!leer(src, "fnt_tmp")) {
		printf("Error: No se ha encontrado el archivo %s\n",src);
		return 0;
	}	
	
	//2ºDescriframos
	printf("Desciframos...\n");
	errN=decrypt("fnt_tmp", "desc_tmp", pass);
	if (!errN) { printf("Error al descifrar el archivo\n"); return 0; }
	
	remove("fnt_tmp");
	
	//3ºdescomprimimos.
	printf("Descomprimimos...\n");
	fdsrc=fopen("desc_tmp","rb");
	fddest=fopen(dest,"wb");
	errN=zinf(fdsrc, fddest);
	if (errN != Z_OK) {
		remove ("desc_tmp");
		zerr(errN);
		return 0;
	}
	
	fclose(fdsrc);
	fclose(fddest);	
	
	remove ("desc_tmp");
	printf("Terminado!\n\a");
	
	return 1;
}