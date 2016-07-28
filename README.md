# Compilable steganography

Compilable steganography was a project realized in *Criptography and security* classes in University of Jaén in 2009. The work was made by:
- Antonio Sánchez Perea ([plutec](https://github.com/plutec/)) 
- Alejandro Avilés del Moral ([OmeGak](https://github.com/OmeGak))
- Carlos Morera de la Chica ([CarlosMChica](https://github.com/CarlosMChica))

Classes were directed by [Manuel Lucena](http://wwwdi.ujaen.es/?q=english/mlucena).

# Compiling
```bash
$ cd src
$ make 
```
This will generate an executable file called **his** in the same folder.

# Usage
## Hidding files
```bash
./his -e -i file.txt -o file_hide.c -p mypassphrase
Comprimimos...
Ciframos...
Ocultamos... (Puede tardar un poco dependiendo del tamaño del archivo)
Terminado!
```
This creates a file called file_hide.c which is totally compilable and executable.

**WARNING**
The size of the generated C source code is much bigger than the original.
```bash
-rw-r--r-- 1 user user   28 Jul 27 21:41 file.txt
-rw-r--r-- 1 user user 4,9K Jul 27 21:41 file_hide.c
```

## Recovering file
```bash
./his -d -i file_hide.c -o file2.txt -p mypassphrase
Leemos... (Esto puede tardar un poco dependiendo del tamaño del archivo)
Desciframos...
Descomprimimos...
Terminado!
```

## Checking
We check both files, the original file and the recovered one, they are the same:
```bash
$ sha256sum file.txt file2.txt 
ce55a98971f8b4cc69878e8c579e3f5009326c66768991083bfcb7e17511b048  file.txt
ce55a98971f8b4cc69878e8c579e3f5009326c66768991083bfcb7e17511b048  file2.txt
```
