/** PRACTICA DE TRAZADO DE RAYOS
Prof: R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011
Código de generación de Rayos Primarios y relleno
del framebuffer con el resultado del trazador.

Alumno:                                        **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/Glut.h>
#include "EscenaIncompleta.h"

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

// Variables globales

// Vector que almacenará en los valores del raster, usando RGB 
unsigned char *raster = NULL;
// Tamaño del raster en píxeles
int ancho=500, alto=400;
// Flag que indica si hay que redibujar la imagen 
int sucio;
// Definición de la escena 
Escena *escena = NULL;
// Parámetros de la cámara 
double AspectRatio, fov = 65.0;   
// Posición de la cámara sobre el eje Z
float pos_z = 3.0f;		
// Control de la iluminacion con el teclado
bool iluminacion = false, sombras = false, reflexiones = false, antialiasing = false;


// Funciones auxiliares
double calc_b(float pos_z, double fov){
	return 2*pos_z*tan(DEG2RAD(fov)/2);
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
	if (escena==NULL) 
		escena = new Escena();

	//Esfera
	Esfera *esfera = new Esfera(Punto(0,0.1,-2), 0.8);
	esfera->setColor(Color(1,0,0), Color(1,1,1), 0.3, 0.3, 0.7, 40);
	escena->add(esfera);

	//Caja suelo
	Transformacion t1;
	t1.translation(Real4(0.0, -1.0, -1.5));
	t1.scale(2, 0.1, 2);

	Caja *cajasuelo = new Caja(t1);
	cajasuelo->setColor(Color(1,1,1), Color::BLANCO, 0.1, 0.3, 0.9, 100);
	escena->add(cajasuelo);

	//Caja cubo
	Transformacion t2;
	t2.translation(Real4(-1.0, -0.3, -0.3));
	t2.rotation(60, Vector(1,1,1));
	t2.scale(0.5, 0.5, 0.5);

	Caja *cajacubo = new Caja(t2);
	cajacubo->setColor(Color(0,1,0), Color::BLANCO, 0.1, 0.8, 0.1, 1);
	escena->add(cajacubo);

	// Esfera blanca (detrás)
	/*blanca = new Esfera(Punto(0.0f, 0.0f, -2.0f), radio);
	blanca->setColor(Color::BLANCO);
	escena->add(blanca);*/

	// Esfera amarilla (NO)
	/*amarilla = new Esfera(Punto(-radio, radio,0.0f),radio);
	amarilla->setColor(Color::VERDE+Color::ROJO);
	escena->add(amarilla);*/

	// Esfera roja (SO)
	/*roja = new Esfera(Punto(-radio,-radio,0.0f), radio);
	roja->setColor(Color::ROJO);
	escena->add(roja);

	// Esfera azul (SE)
	/*azul = new Esfera(Punto(radio, -radio,0.0f), radio);
	azul->setColor(Color::AZUL);
	escena->add(azul);/*

	// Esfera verde (NE)
	/*verde = new Esfera(Punto(radio, radio, 0.0f), radio);
	verde->setColor(Color::VERDE);
	escena->add(verde);*/
	
	/*Transformacion t1;
	//t1.translation(Real4(2,2,2,0));
	t1.rotation(30, Vector(0,1,0));
	t1.rotation(-15, Vector(1,0,0));
	t1.scale(0.5, 0.4, 0.3);

	Caja *cajaverde = new Caja(t1);
	cajaverde->setColor(Color::VERDE);
	escena->add(cajaverde);*/
	
	//escena->add(amarilla);
	/*for(int i=0;i<6;i++){
		for(int j=0;j<4;j++){
			printf("%.2f %.2f ", cajaverde->caras[i].vertices[j].x(), cajaverde->caras[i].vertices[j].y());
		}
		printf("\n");
	}*/
}

void crear_iluminacion(void){

	FuenteLuminosa *ambiental, *puntual, *direccional, *focalizada;

	ambiental = new Ambiental();
	escena->addLight(ambiental);

	puntual = new Puntual(Color(1,1,1), Punto(1,0.5,0));
	escena->addLight(puntual);

	focalizada = new Focalizada(Color(1,1,0), Punto(0,2,-1), Vector(0,-1,0), 8, 60);
	escena->addLight(focalizada);

	//direccional = new Direccional(Color(1,0,1), Vector(0,-1,0));
	//escena->addLight(direccional);

}

void traza(void) {
	int i, j;
	Color color;
	double x1, y1, a, b;
	unsigned char *t;

	//Si es la primera vez, o hay que redibujar la escena...
	if(raster==NULL){
		raster = (unsigned char *)malloc(sizeof(unsigned char)*3*ancho*alto);
		if (raster==NULL){
			fprintf(stderr, "Sin memoria\n");
			exit(-1);
		}
	}
	b = calc_b(pos_z, fov);
	a = AspectRatio * b;
	t = raster;
	for(j=0; j<alto; j++){
		y1 = calc_y1(b, alto, j);
		for(i=0; i<ancho; i++){
			x1 = calc_x1(a, ancho, i);
			color = escena->rayTrace(Punto(0, 0, pos_z), Vector(x1, y1, -pos_z));
			*t++ = (unsigned char)(color.r()*255);
			*t++ = (unsigned char)(color.g()*255);
			*t++ = (unsigned char)(color.b()*255);
		}
	}
	sucio = 0;
}

void myinit(void){

    glClearColor (0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	crear_escena();
	crear_iluminacion();
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
}


void myKeyboard(unsigned char tecla, int x, int y){

	switch(tecla) {
		/* INTRODUCE AQUÍ TU CÓDIGO */
			/* Mayúsculas activan, minúsculas desactivan */
			// L,l: iluminación
			// S,s: sombras
			// R,r: reflexiones
			// A,a: antialiasing
		case 'L': escena->getLight(1)->switchLight(ON);
				  escena->getLight(2)->switchLight(ON); break;
		case 'l': escena->getLight(1)->switchLight(OFF);
				  escena->getLight(2)->switchLight(OFF); break;
		case 'S': escena->switchShadow(ON); break;
		case 's': escena->switchShadow(OFF); break;
		case 27: exit(0);
		default:;
	};
	sucio = 1;
	display();
	return;
}

int main(int argc, char** argv){

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize(500, 400); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Trazador de rayos");
   myinit();
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   glutKeyboardFunc(myKeyboard);

   glutMainLoop();
   return(0);
}
 