/* 
 * File:   encrypt.c
 * Author: ome_gak
 *
 * Created on 8 de mayo de 2009, 18:38
 */

#include <stdio.h>
#include "encrypt_decrypt.h"
#include "rijndael.h"


#define KEYBITS 256

/**
 * @brief   Encripta un archivo de texto con RIJNDAEL
 * @param   *ruta_input es la ruta del archivo de texto del que se encriptará el mensaje
 * @param   *ruta_output es la ruta del archivo en que se almacenará el mensaje encriptado
 * @param   *password es la semilla de la que se obtendrá la clave
 */
int encrypt(const char *ruta_input, const char *ruta_output, const char *password) {
    unsigned long rk[RKLENGTH(KEYBITS)];
    unsigned char key[KEYLENGTH(KEYBITS)];
    int i;
    int nrounds;
    FILE *input;
    FILE *output;

    // Error si falta el password
    if (!password) {
        fputs("encrypt: Falta password\n", stderr);
        return 0;
    }

    // Abrir los ficheros
    input = fopen(ruta_input, "r");
    output = fopen(ruta_output, "wb");
    if (!input || !output){
        fputs("encrypt: Error al abrir ficheros\n", stderr);
        return 0;
    }

    // Cálculo de la llave
    for (i = 0; i < sizeof (key); i++)
        key[i] = *password != 0 ? *password++ : 0;

    // Cifrado
    nrounds = rijndaelSetupEncrypt(rk, key, 256);
    while (!feof(input)) {
        unsigned char plaintext[16];
        unsigned char ciphertext[16];
        int j;
        for (j = 0; j < sizeof (plaintext); j++) {
            int c = fgetc(input);
            if (c == EOF)
                break;
            plaintext[j] = c;
        }
        if (j == 0)
            break;
        for (; j < sizeof (plaintext); j++)
            plaintext[j] = ' ';
        rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);
        if (fwrite(ciphertext, sizeof (ciphertext), 1, output) != 1) {
            fclose(output);
            fputs("encrypt: Error en escritura del fichero\n", stderr);
            return 0;
        }
    }
    fclose(input);
    fclose(output);
    
    return 1;
}
