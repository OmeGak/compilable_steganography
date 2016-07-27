/**
 * @file escribir.c
 * @author Antonio Sánchez Pera
 * @date 3/6/2009
 */

#include "escribir.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * @brief Reserva la memoria para la estructura.
 * @param const char *src, archivo para el que se creará la estructura y con el que trabajaremos.
 * @pre Debe haber memoria suficiente. Puede ser un problema si el archivo es demasiado grande.
 * @return La estructura creada.
 */

vector *escribir_reserva_mem(const char *src) {
	FILE *fd;
	vector *toRet;
	long int tam;
	fd=fopen(src,"ab");
	if (!fd) { 
		printf("No existe el archivo!\n");
		fflush(stdout);
		return NULL;
	}
	//Ahora vemos cuanto ocupa el fichero.
	tam=ftell(fd);
	fclose(fd);
	//ese es el tamaño en bytes, así que reservamos un vector de esos bytes.
	if (tam>0) {
		toRet=(vector *)malloc(sizeof(vector)); //reservamos memoria para la estructura
		toRet->vector=(unsigned char *)malloc(tam*sizeof(unsigned char));
		toRet->tam=tam;
		toRet->leyendo=0;
		return toRet;
	}
	return NULL;	
}

/**
 * @brief Con esto cargamos el archivo en la estructura.
 * @param vector *vec, estructura con la que vamos a trabajar, ya creada.
 * @param const char *src, archivo desde el que tomaremos los valores.
 */
 
void escribir_carga_archivo(vector *vec, const char *src) {
	FILE *fd;
	long int i;
	fd=fopen(src,"rb");
	for(i=0;i<vec->tam;++i) {
		fread(&(vec->vector[i]), sizeof(char), 1, fd );
	}	
	fclose(fd);
}

/**
 * @brief Escribe las cabeceras del archivo que vamos a generar.
 * @post Genera temporalmente el archivo head_tmp
 */
 
void escribir_head(const char *dest) {
	FILE *fd;
	fd=fopen(dest,"wt"); //Abrimos destruyendo el contenido anterior.
	fprintf(fd,"#include <stdlib.h>\n#include <stdio.h>\n");
	fprintf(fd,"#ifndef WIN32\n\t#include <unistd.h>\n#else\n\t#include <windows.h>\n#endif\n\n");
	fclose(fd);
}

/**
 * @brief Función para generar el main.
 * @param vector *vec, estructura que hemos creado con el archivo original
 * @return La función devolverá 1 si todavía quedan palabras para generar más funciones.
 * @return 0 en caso contrario.
 */
 
int escribir_genera_main(vector *vec) {

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
	//El archivo temporal donde se guardará el main es main_tmp
	long int i;
	int toRet=1,repeat=0,tam_var=0;	
	FILE *fd;
	char var[4][32];
	
	
	srand(time(NULL));
	fd=fopen("main_tmp","w");
	while (repeat<4) {
	//Escribimos 4 veces esto.
		if (vec->tam-vec->leyendo>3) {
			sprintf(var[repeat],"%s%s%s%i",v[vec->vector[vec->leyendo]],v[vec->vector[(vec->leyendo)+1]],v[vec->vector[(vec->leyendo)+2]],(int)vec->vector[(vec->leyendo)+3]);
			vec->leyendo+=4;
			++tam_var;
			if ((vec->tam)==(vec->leyendo)) { 
				toRet=0; 
				repeat=4; 
			}
		}
		else {
			printf("entra else\n");
			if (vec->tam-vec->leyendo>0) {
				var[repeat][0]='\0';
				for(i=vec->leyendo;i<vec->tam;++i) {
					strcat(var[repeat],v[vec->vector[vec->leyendo]]);
					vec->leyendo++;
				}
				toRet=0;
				++tam_var;
				repeat=4; //Para que se salga del bucle.
			}
		}
		++repeat;
	}
	//ahora escribimos el código del main
	fprintf(fd,"int main() {\n");
	for(i=0;i<tam_var;++i) {
		fprintf(fd,"\tint %s;\n",var[i]);	
	}
	fprintf(fd,"\n\n");
	//Ahora damos uso a las variables.
	for(i=0;i<tam_var;++i) {
		fprintf(fd,"\t%s=%li;\n",var[i],i*((rand()+1)%3));	
	}
	fprintf(fd,"\n\n");
	//Ahora escribimos cosas varias.
	fprintf(fd,"\tprintf(\"En este código de prueba se generan caritas\\n\");\n");
	fprintf(fd,"\tprintf(\"Pulsa una tecla para comenzar...\");\n");
	fprintf(fd,"\tgetc(stdin);\n");
	//Ahora la carita.
	fprintf(fd,"\tprintf(\"                        ||||||||||||||||||\\n\");\n");
  	fprintf(fd,"\tprintf(\"                       $$$$$$$$$$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"                     $$$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"                  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$            $$   $$ $$\\n\");\n");
	fprintf(fd,"\tprintf(\"   $ $ $$        $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$ $$ $$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"$$ $ $ $$      $$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$      $$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"$$$$$$$$$     $$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$    $$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"  $$$$$$$    $$$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  $$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  $$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"  $$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    $$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\" $$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\" $$$$$$$$$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\" $$$$$$$$$$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$$$$$$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"$$$$       $$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"          $$$$$     $$$$$$$$$$$$$$$$$$$$$$$$$         $$$\\n\");\n");
	fprintf(fd,"\tprintf(\"            $$$$          $$$$$$$$$$$$$$$           $$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"             $$$$$                 $$             $$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"              $$$$$$      $$$$$$$$$$$$$$        $$$$$\\n\");\n");
	fprintf(fd,"\tprintf(\"                $$$$$$$$     $$$$$$$$$$$$$   $$$$$$$ \\n\");\n");
	fprintf(fd,"\tprintf(\"                   $$$$$$$$$$$  $$$$$$$$$$$$$$$$$   \\n\");\n");
	fprintf(fd,"\tprintf(\"                      $$$$$$$$$$$ $$$$$$$$$$         \\n\");\n");
	fprintf(fd,"\tprintf(\"                              $$$$$$$$$$$$$$$        \\n\");\n");
	fprintf(fd,"\tprintf(\"                                  $$$$$$$$$$$$       \\n\");\n");
	fprintf(fd,"\tprintf(\"                                   $$$$$$$$$$$      \\n\");\n");
	fprintf(fd,"\tprintf(\"                                    $$$$$$$$\\n\");\n");
	fprintf(fd,"\n\n\n");	
	fclose(fd);	
	return toRet;
}

/**
 * @brief Función para generar funciones. (Valga la redundancia).
 * @param vector *vec, estructura que hemos creado con el archivo original
 * @param int tipo_cara, dependiendo si este número es positivo o negativo, generará un tipo distinto de cara.
 * @return La función devolverá 1 si todavía quedan palabras para generar más funciones.
 * @return 0 en caso contrario.
 */
 
int escribir_funcion(vector *vec, int tipo_cara) {
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
	//El archivo temporal donde se guardará el main es func_tmp
	//La cabecera en head_tmp
	int toRet=1, repeat=0, tam_var=0, i;
	FILE *func, *head;
	//Entre 6 y 12. +2 de los parámetros locales.
	int max=((rand()+1)%7)+6+2;
	
	char var[14][128];
	func=fopen("func_tmp","a");
	while (repeat<max) {
	//Escribimos max veces esto.
		if (vec->tam-vec->leyendo>=4) {
			sprintf(var[repeat],"%s%s%s%i",v[vec->vector[vec->leyendo]],v[vec->vector[(vec->leyendo)+1]],v[vec->vector[(vec->leyendo)+2]],(int)vec->vector[(vec->leyendo)+3]);
			vec->leyendo+=4;
			++tam_var;
			if ((vec->tam)==(vec->leyendo)) { 
				toRet=0;
				repeat=max+1;				
				}
		}
		else {
			if (vec->tam-vec->leyendo>0) {
				var[repeat][0]='\0';
				for(i=vec->leyendo;i<vec->tam;++i) {
					strcat(var[repeat],v[vec->vector[vec->leyendo]]);
					vec->leyendo++;	
				}
				toRet=0;
				++tam_var;		
				repeat=max+1; //Para que se salga del bucle.;
			}
			else { toRet=0; repeat=max+1; } //No se puede escribir ninguno.
		}
		++repeat;
	}
	//Ahora escribimos
	head=fopen("head_tmp","a");
	//si el numero de var es <=2, sólo escribimos la cabecera, sin ninguna variable dentro de la función.
	if (tam_var==1) { //Sólo escribe nombre de función
		fprintf(head,"0|void %s();\n",var[0]);
		fprintf(func,"void %s() {\n}\n",var[0]);
	}
	if (tam_var==2) { //nombre de función + 1 parámetro
		fprintf(head,"1|void %s(int %s);\n",var[0],var[1]);
		fprintf(func,"void %s(int %s) {\n",var[0],var[1]);
		fprintf(func,"\t%s=%i;\n",var[1],4*(2%((rand()+1)%56)));
	}	
	if (tam_var==3) { //nombre de función + 2 parámetros
		fprintf(head,"2|void %s(int %s, int %s);\n",var[0],var[1],var[2]);
		fprintf(func,"void %s(int %s, int %s) {\n",var[0],var[1],var[2]);
		fprintf(func,"\t%s=%i;\n",var[1],4*(2%((rand()+1)%56)));
		fprintf(func,"\t%s=%i;\n\n",var[2],3*(2%((rand()+1)%56)));
	}		
	if (tam_var>3) { //nombre de función + 2 parámetros + definición de variables.
		fprintf(head,"2|void %s(int %s, int %s);\n",var[0],var[1],var[2]);
		fprintf(func,"void %s(int %s, int %s) {\n",var[0],var[1],var[2]);
		//Primero defino las variables.
		for(i=3;i<tam_var;++i) {
			fprintf(func,"\tint %s;\n",var[i]);
		}
		fprintf(func,"\n\n");
		for(i=1;i<tam_var;++i) { //damos valores a variables.
			fprintf(func,"\t%s=%i;\n",var[i],i*(2%(((rand()+1)%56)+1)));
		}
	}
	fflush(func);
	//Y ahora tomamos la carita.
	if (tipo_cara>0) {
		fprintf(func,"\tprintf(\"                        ||||||||||||||||||\\n\");\n");
  		fprintf(func,"\tprintf(\"                       $$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                     $$$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$            $$   $$ $$\\n\");\n");
		fprintf(func,"\tprintf(\"   $ $ $$        $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$ $$ $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$ $ $ $$      $$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$      $$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$$$$$$$     $$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$    $$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$$$$$    $$$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  $$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    $$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$$$$$$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$$$$$$$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$$       $$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"          $$$$$     $$$$$$$$$$$$$$$$$$$$$$$$$         $$$\\n\");\n");
		fprintf(func,"\tprintf(\"            $$$$          $$$$$$$$$$$$$$$           $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"             $$$$$                 $$             $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"              $$$$$$      $$$$$$$$$$$$$$        $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                $$$$$$$$     $$$$$$$$$$$$$   $$$$$$$ \\n\");\n");
		fprintf(func,"\tprintf(\"                   $$$$$$$$$$$  $$$$$$$$$$$$$$$$$   \\n\");\n");
		fprintf(func,"\tprintf(\"                      $$$$$$$$$$$ $$$$$$$$$$         \\n\");\n");
		fprintf(func,"\tprintf(\"                              $$$$$$$$$$$$$$$        \\n\");\n");
		fprintf(func,"\tprintf(\"                                  $$$$$$$$$$$$       \\n\");\n");
		fprintf(func,"\tprintf(\"                                   $$$$$$$$$$$      \\n\");\n");
		fprintf(func,"\tprintf(\"                                    $$$$$$$$\\n\");\n");

	}
	else {
		fprintf(func,"\tprintf(\"                        ||||||||||||||||||\\n\");\n");
		fprintf(func,"\tprintf(\"                       $$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                     $$$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$            $$   $$ $$\\n\");\n");
		fprintf(func,"\tprintf(\"   $ $ $$        $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$ $$ $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$ $ $ $$      $$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$      $$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$$$$$$$     $$$$$$$$$$  $$  $$$$$$$$$$$  $$  $$$$$$$$$$$    $$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$$$$$    $$$$$$$$$$$  $$  $$$$$$$$$$$  $$  $$$$$$$$$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  $$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"  $$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    $$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$$$$$$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\" $$$$$$$$$$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$$$$$$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"$$$$       $$$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$      $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"          $$$$$     $$$$$$$$$$$$$$$$$$$$$$$$$         $$$\\n\");\n");
		fprintf(func,"\tprintf(\"            $$$$          $$$$$$$$$$$$$$$           $$$$\\n\");\n");
		fprintf(func,"\tprintf(\"             $$$$$                                $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"              $$$$$$                           $$$$$\\n\");\n");
		fprintf(func,"\tprintf(\"                $$$$$$$$                    $$$$$$$ \\n\");\n");
		fprintf(func,"\tprintf(\"                   $$$$$$$$$$$            $$$$$$$$   \\n\");\n");
		fprintf(func,"\tprintf(\"                      $$$$$$$$$$$$$$$$$$$$$         \\n\");\n");
		fprintf(func,"\tprintf(\"                            $$$$$$$$$$$$$$$        \\n\");\n");
		fprintf(func,"\tprintf(\"                              $$$$$$$$$$$$       \\n\");\n");
		
	}
	fprintf(func,"\n");
	fprintf(func,"#ifndef WIN32\n");
    fprintf(func,"\tsleep(1);\n");
  	fprintf(func,"#else\n");
    fprintf(func,"\tSleep(1000);\n");
 	fprintf(func,"#endif\n\n\n}\n");
	fclose(func);
	fclose(head);	
	return toRet;
}
/**
 * @brief Esta función libera la memoria que hemos reservado para la estructura.
 * @param La estructura creada.
 */

void escribir_free(vector *vec) {
	free(vec->vector);
	free(vec);	
}

/**
 * @brief Esto empaqueta todos los archivos temporales en un .c definitivo.
 * @param const char *dest, archivo de destino(se recomienda que tenga extensión .c)
 * @pre Si el archivo pasado como parámetro existe, se destruirá.
 * @post Destruye los archivos temporales. Si está definido DEBUG_ los mantiene.
 */
	
void escribir_empaquetar(const char *dest) {
	FILE *fd, *tmp;
	int n_param, cont_var, rand1, primera, i;
	char buffer[512];
	char *ptr;
	char var[4][32];
	char str_par[16];
	//Primero escribimos cabeceras
	escribir_head(dest);
	fd=fopen(dest,"at"); //abrimos ahora, porque al escribir las cabeceras se ha limpiado y abierto.
	
	//Ahora escribimos las definiciones de las funciones.
	tmp=fopen("head_tmp","rt");
	while (!feof(tmp)) {
		//buffer[0]='\0'; //Pongo esto por problemas con el EOF.
		if (fscanf(tmp, "%d|%[^\n]", &n_param, buffer)>0)
			fprintf(fd,"%s\n",buffer);
		if (!feof(tmp)) fgetc(tmp);
	}
	fclose(tmp);
	fprintf(fd,"\n\n");
	
	//Ahora escribimos el main
	tmp=fopen("main_tmp","rt");
	while(!feof(tmp)) {
		fgets(buffer, 512, tmp);
		fprintf(fd,"%s",buffer);
	}
	fclose(tmp);
	
	//Ahora escribimos dentro del main, las llamadas a las funciones generadas.
		//Primero extraigo las variables que hemos usado en el main.
	tmp=fopen("main_tmp","rt");
	cont_var=0;
		//Primero me salto la cabecera del main.
	fgets(buffer, 512, tmp);
	fgetc(tmp);
	primera=1;
	while(buffer[0]!='\0') {
		buffer[0]='\0';
		fscanf(tmp, "%[^\n]", buffer);
		if (buffer[0]!='\0') {
			if (primera) { strcpy(var[cont_var],buffer+4); primera=0; }
			else { strcpy(var[cont_var],buffer+5); }
			var[cont_var][strlen(var[cont_var])-1]='\0'; //Para quitar el ; del final.
			fgetc(tmp); //Quito el \n
			++cont_var;
		}
	}
	fclose(tmp);	
		//Ahora escribo las llamadas
	tmp=fopen("head_tmp","rt");
	strcpy(str_par,"(");
	while (!feof(tmp)) {
		buffer[0]='\0'; //Pongo esto por problemas con el EOF.
		if (fscanf(tmp, "%d|%[^\n]", &n_param,buffer)>0) {
			ptr=strstr(buffer,str_par);
			*ptr='\0';
			fprintf(fd,"\t%s(",buffer+5);
			primera=1;
			for(i=0;i<n_param;++i) {
				rand1=(rand()+1)%cont_var;
				if (primera) { fprintf(fd,"%s",var[rand1]); primera=0; }
				else { fprintf(fd,", %s",var[rand1]); }
			}
			fprintf(fd,");\n");
		}
		if (!feof(tmp)) fgetc(tmp);
		
	}
	//Un par de intros y se cierra el main.
	fprintf(fd,"\n\n\treturn 1;\n}\n\n");
	fclose(tmp);
	//Ahora escribimos todas las funciones que hemos creado (archivo func_tmp).
	tmp=fopen("func_tmp","rt");
	fgets(buffer,512,tmp);
	while(!feof(tmp)) {
		fprintf(fd,"%s",buffer);
		fgets(buffer,512,tmp);
	}
	fclose(tmp);
	//Eliminamos archivos temporales.
	remove("main_tmp");
	remove("head_tmp");
	remove("main_tmp");

	fclose(fd);
}
	