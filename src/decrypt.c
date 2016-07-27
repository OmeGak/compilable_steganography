/* 
 * File:   decrypt.c
 * Author: ome_gak
 *
 * Created on 8 de mayo de 2009, 19:59
 */

#include <stdio.h>
#include "rijndael.h"
#include "encrypt_decrypt.h"

#define KEYBITS 256

/**
 * @brief   Desencripta desde un archivo cifrado con RIJNDAEL
 * @param   *ruta_input es la ruta del archivo cifrado
 * @param   *ruta_output es la ruta del archivo en que se guardará el resultado
 * @param   *password es la semilla desde la que se obtendrá la clave
 */
int decrypt(const char *ruta_input, const char *ruta_output, const char *password) {
    unsigned long rk[RKLENGTH(KEYBITS)];
    unsigned char key[KEYLENGTH(KEYBITS)];
    int i;
    int nrounds;
    FILE *input;
    FILE *output;

    // Error si falta el password
    if (!password) {
        fputs("decrypt: Falta password\n", stderr);
        return 0;
    }

    // Abrir los ficheros
    input = fopen(ruta_input, "rb");
    output = fopen(ruta_output, "w");
    if (!input || !output){
        fputs("decrypt: Error al abrir ficheros\n", stderr);
        return 0;
    }

    // Cálculo de la llave
    for (i = 0; i < sizeof (key); i++)
        key[i] = *password != 0 ? *password++ : 0;

    // Descifrado
    nrounds = rijndaelSetupDecrypt(rk, key, 256);
    while (1) {
        unsigned char plaintext[16];
        unsigned char ciphertext[16];
        if (fread(ciphertext, sizeof (ciphertext), 1, input) != 1)
            break;
        rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);
        fwrite(plaintext, sizeof (plaintext), 1, output);
    }
    fclose(input);
    fclose(output);
    
    return 1;
}
