/** PRACTICA DE TRAZADO DE RAYOS
Prof: R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011
Alumno:                                        **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/Glut.h>
#include "EscenaIncompleta.h"

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

/* Variables globales */

/* Vector que almacenará en los valores del raster, usando RGB */
unsigned char *raster = NULL;

/* Tamaño del raster en píxeles*/
int ancho, alto;

/* Flag que indica si hay que redibujar la imagen */
int sucio;

/* Definición de la escena */
Escena *escena = NULL;

/* Parámetros de la cámara */
double AspectRatio, fov; 

/* Posición de la cámara sobre el eje Z */
float pos_z = 3.0f;	


// Funciones auxiliares
double calc_b(float pos_z, double fov){
	return 2 * pos_z * tan(DEG2RAD(fov)/2);
}

double calc_x1(double a, int ancho, int i){
	return ((a*i)/ancho) - a/2;
}

double calc_y1(double b, int alto, int j){
	return ((j*b)/alto) - b/2;
}


void crear_escena(void) {
	Esfera *amarilla, *blanca, *roja, *verde, *azul;
	float radio = 0.59673710213f;

	// Creamos la escena
	if (escena==NULL) escena = new Escena();

	// Esfera blanca (detrás)
	blanca = new Esfera(Punto(0.0f, 0.0f, -2.0f), radio);
	blanca->setColor(Color::BLANCO);
	escena->add(blanca);
	// Esfera amarilla (NO)
	amarilla = new Esfera(Punto(-radio, radio,0.0f),radio);
	amarilla->setColor(Color::VERDE+Color::ROJO);
	escena->add(amarilla);
	// Esfera roja (SO)
	roja = new Esfera(Punto(-radio,-radio,0.0f), radio);
	roja->setColor(Color::ROJO);
	escena->add(roja);
	// Esfera azul (SE)
	azul = new Esfera(Punto(radio, -radio,0.0f), radio);
	azul->setColor(Color::AZUL);
	escena->add(azul);
	// Esfera verde (NE)
	verde = new Esfera(Punto(radio, radio, 0.0f), radio);
	verde->setColor(Color::VERDE);
	escena->add(verde);
	
}

void traza(void) {
	int i, j;
	Color color;
	double x1, y1, a, b;
	unsigned char *t;

	/* Si es la primera vez, o hay que redibujar la escena... */
	if(raster==NULL) {
		raster = (unsigned char *)malloc(sizeof(unsigned char)*3*ancho*alto);
		if(raster==NULL) {
			fprintf(stderr, "Sin memoria\n");
			exit(-1);
		}
	
	}
	b = calc_b(pos_z, fov);
	a = AspectRatio * b;
	t = raster;
	for(j=0; j<alto; j++){
		y1 = calc_y1(b, alto, j);
		for(i=0;i<ancho;i++){
			x1 = calc_x1(a, ancho, i);
			color = escena->rayTrace(Punto(/* de inicio del rayo */),Vector(/* direccion del rayo */));
			*t++ = (unsigned char)(color.r()*255);
			*t++ = (unsigned char)(color.g()*255);
			*t++ = (unsigned char)(color.b()*255);
		}
	}
	sucio=0;
}

void myinit(void){

    glClearColor(0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	crear_escena();
}

void display(void){

	if (raster==NULL || sucio) 
		traza();
	
	glRasterPos2i(0,0);
	glDrawPixels(ancho, alto, GL_RGB, GL_UNSIGNED_BYTE, raster);
	glFlush();
}


void myReshape(GLsizei w, GLsizei h){

    h = (h == 0) ? 1 : h;
    glViewport(0, 0, w, h);
    
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)w, 0.0, (GLfloat)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	ancho = MAX(w, 1);
	alto = MAX(h, 1);
	free(raster);
	raster = NULL;
	AspectRatio = (GLdouble)ancho/alto;
	fov = 45.0f;
}


void myKeyboard(unsigned char tecla, int x, int y){

	switch(tecla){
	case ' ':
		printf("%i;%i\n",x,alto-y);
		break;
	case 27:
		exit(0);
	default:;
	};
	return;
}

int main(int argc, char** argv){

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize (400, 400); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Trazador de rayos");
   myinit();
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   glutKeyboardFunc(myKeyboard);

   glutMainLoop();
   return(0);
}
 