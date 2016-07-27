/* 
 * File:   encrypt_decrypt.h
 * Author: ome_gak
 *
 * Created on 8 de mayo de 2009, 18:59
 */

#ifndef _ENCRYPT_DECRYPT_H
#define	_ENCRYPT_DECRYPT_H

int encrypt(const char *ruta_input, const char *ruta_output, const char *password);
int decrypt(const char *ruta_input, const char *ruta_output, const char *password);

#endif	/* _ENCRYPT_DECRYPT_H */

