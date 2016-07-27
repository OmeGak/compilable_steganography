/* 
 * File:   leer.c
 * Author: ome_gak
 *
 * Created on 30 de mayo de 2009, 18:57
 */

#include <stdio.h>
#include <stdlib.h>
#include "leer.h"
#include <string.h>

/**
 * @brief   Lee la plantilla y va traduciendo sus variables a cadenas de bits
 * @param   ruta es la ruta del archivo que contiene el código fuente
 * @post    Obtiene todas las variables y las traduce a su cadena de bits correspondiente, escribe el resultado en un archivo temp
 * @return  0 si falla, 1 si ok
 */
int leer(const char *ruta, const char *dest) {
    FILE *input;
    FILE *temp;
    char lin[MAXLIN];
    char var[MAXVAR];
    int i;
    int est = 0;    // 0 = buscando "int main(){"
                    // 1 = listo para analizar variables

    input = fopen(ruta, "rt");
    temp = fopen(dest, "wb");
    if(!input) return 0;
    // Recorremos el archivo entero
    while(!feof(input)){
        // Leemos una linea del código fuente
        fscanf(input, "%[^\n]\n", lin);

        // ANÁLISIS DE VARIABLES
        // Si se trata de una definición de variable dentro de un procedimiento
        if ((est == 1) && (lin[0] == 'i') && (lin[1] == 'n') && (lin[2] == 't') && (lin[3] == ' ')){
            // Primero le quitamos el símbolo ";"
            for(i=0;lin[i]!='\0';++i){
                if(lin[i] == ';')
                    lin[i] = '\0';
            }
            // Guardamos la variable
            strcpy(var,lin+4);
            // Y la desciframos
            VarToBit(var, temp);
        }
        // Si se trata de la cabecera de un procedimiento
        if ((est == 1) && (lin[0] == 'v') && (lin[1] == 'o') && (lin[2] == 'i') && (lin[3] == 'd') && (lin[4] == ' '))
            VarCabecera(lin, temp);

        // CAMBIOS DE ESTADO
        // Cuando encontramos el main() activamos el centinela
        if ((est == 0) && (strcmp(lin, BEGIN) == 0))
            est= 1;     // Listo para analizar variables
    }

    fclose(temp);
    fclose(input);
    return 1;
}


/**
 * @brief   Analiza las variables que se hayan en la cabecera de un procedimiento
 * @param   lin es la linea de la cabecera a analizar
 * @param   temp es el archivo en que se guardará la traducción de la variable
 * @post    Traduce las variables que se encuentran en la cabecera a cadenas de bits y las guarda en temp
 */
void VarCabecera(char *lin, FILE *temp) {
    char var[MAXVAR];
    int i;
    int j = 0;
    int est = 0;    // 0 = buscando variable
                    // 1 = leyendo variable

    for(i=0;lin[i]!='\0';++i){
        // Si estamos leyendo variable
        if (est == 1){
            // Si no se encuentra '(', ',' o ')' entonces es parte de la variable y se almacena
            if ((lin[i] != '(') && (lin[i] != ',') && (lin[i] != ')'))
                var[j++] = lin[i];
            // Si es alguno de ellos la variable se cierra y traduce
            else {
                var[j] = '\0';
                VarToBit(var, temp);
                j = 0;                  // Contador de cadena se inicializa
                est = 0;                // Volvemos a buscar
            }
        }

        if ((est == 0) && (lin[i] >= 65) && (lin[i] <= 90)){
            var[j++] = lin[i];
            est = 1;
        }
    }
}


/**
 * @brief   Convierte una variable a bits
 * @param   var es la variable
 * @param   temp es el archivo en que se guardará la traducción de la variable
 * @post    Se escribe el resultado en un archivo temporal tmp
 */
void VarToBit(char *var, FILE *temp){
    char var1[10];
    char var2[10];
    char var3[10];
    char varnum[4];         // Parte numérica de la variable
    unsigned int pos[4];    // Vector de posiciones en el diccionario de las palabras
    int est = 0;
    int i;
    int j;

    // Separamos la variable en 3 palabras
    for (i=0;var[i]!='\0';++i) {
        // Si se encuentra con una mayúscula pasa al estado siguiente
        if (((var[i] >= 65) && (var[i] <= 90)) ||
            ((est == 3) && (var[i] >= 48) && (var[i] <= 57))){
            est++;
            j = 0;
        }

        if (est == 1){
            var1[j] = var[i];
            var1[++j] = '\0';
        }
        if (est == 2){
            var2[j] = var[i];
            var2[++j] = '\0';
        }
        if (est == 3){
            var3[j] = var[i];
            var3[++j] = '\0';
        }
        if (est == 4){
            varnum[j] = var[i];
            varnum[++j] = '\0';
        }
    }

    // Buscamos la posición de la variable en el diccionario
    if (est > 0)
        pos[0] = buscaPos(var1);
    if (est > 1)
        pos[1] = buscaPos(var2);
    if (est > 2)
        pos[2] = buscaPos(var3);
    if (est > 3)
        pos[3] = atoi(varnum);

    // Guardamos tantas como número de partes tenía la variable
    for(i=0;i<est;++i){
        fputc(pos[i], temp);
    }
    
}


/**
 * @brief   Encuentra la posición de una palabra de una variable en el diccionario
 * @param   var es la palabra de la variable
 * @return  La posición que ocupa esa palabra en el diccionario
 */
unsigned int buscaPos(char *var) {
    unsigned int i;
    typedef char palabra[10];
	palabra v[256] = {"Aux", "Var", "Ret", "To", "Mark", "Inc", "Cont", "Ext", "Time", "Code", "Ring", "Pass", "Phrase", "Word", "Obs", "Number", "Text", "Sum", "Mul", "Rest"
                		, "Pow", "Sol", "Option", "Val", "Loop", "Path", "Atribute", "Ip", "Det", "Num", "Ptr", "Dot", "Calc", "Hide", "Character", "Pull", "Pop", "Trash", "Set", "Match"
				, "Connect", "Vel", "Size", "Length", "Wide", "Wires", "Elec", "Fire", "Protocol", "Trace", "Bug", "Error", "Report", "Day", "Hour", "Minutes", "Seconds", "Year", "Max", "Min"
				, "Known", "Temp", "Bin", "Mov", "Array", "Signal", "Wait", "Prompt", "Ftp", "Url", "Http", "Img", "Sound", "Make", "Zone", "Space", "Try", "Web", "Blog", "Log"
				, "Tec", "Mouse", "Keyboard", "Screen", "Hdd", "Speaker", "Gpu", "Cpu", "Ram", "Rom", "Eprom", "Window", "Button", "Pad", "Tab", "Esc", "Enter", "Arrow", "Line", "Row"
				, "Tag", "Cursor", "Wall", "Board", "Mem", "Alt", "Control", "Shift", "Caps", "Lock", "Supr", "Pause", "Ins", "Desp", "Start", "End", "Column", "Page", "Up", "Down"
				, "Left", "Right", "Plus", "Minus", "Join", "Concat", "Proc", "Insert", "Impr", "Petsis", "Bright", "Contrast", "Play", "Fordward", "Back", "Stop", "Area", "Sleep", "Computer", "Game"
				, "Level", "Cut", "Copy", "Paste", "Search","Old" , "Bold", "Italic", "Sub", "All", "None", "Push", "Sms", "Driver", "Bgm", "Sfx", "Mms", "Email", "Desktop", "Site", "Cam"
				, "Cloud", "Pen", "Correct", "Ok", "Fail", "Success", "Problem", "Exception", "Pixel", "Bit", "Byte", "Kilo", "Mega", "Giga", "Tera", "Peta", "Hexa", "Yota", "Binary", "Dec"
				, "Buffer", "Usb", "Cd", "Dvd", "Blueray", "Rss", "Floppy", "Ethernet", "Wifi", "Virtual", "Source", "Project", "Main", "Telnet", "Folder", "Home", "Multi", "Media", "Codec", "Rate"
				, "Link", "Book", "Pdf", "Doc", "Txt", "Program", "Input", "Output", "Mpeg", "Avi", "Divx", "Jpg", "Bmp", "Rtf", "Core", "Kernel", "Digital", "Analog", "Circuit", "Arith"
				, "Oper", "Math", "Complex", "Simple", "Patch", "Display", "Machine", "Delay", "Lcd", "World", "Dev", "Ubuntu", "Mac", "Little", "Big", "Medium", "Add", "Half", "Instruc", "Exec"
				, "Herz", "Version", "Rgb", "Open", "Close", "Seek", "Used", "Read", "Write", "Track", "Delete", "Free", "Gnu", "Title", "Upper"};

    for (i=0;i<256;++i)
        if (strcmp(v[i], var) == 0)
            return i;

    return -1;
}










