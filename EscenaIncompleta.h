/** PRACTICA DE TRAZADO DE RAYOS
Prof: R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011
Alumno:                                        **/

#ifndef _ESCENA_H
#define _ESCENA_H

#include "Primitivas.h"
#include <vector>

using namespace std;

#define INFINITO 3.4E38f

/** EJEMPLO DE ESTRUCTURA DE CLASES 
class Poligono;			//Deriva de Primitiva
  class Cuadrilatero;
  class Triangulo;

class Objeto;
  class Esfera;
  class Poliedro;
	class Caja;

class FuenteLuminosa;
  class Ambiental;
  class Puntual;
  class Direccional;
  class Focalizada;

class Escena;
***********************************/


//Objeto gráfico con volumen
class Objeto{
public:
	Color colDifuso, colEspecular; // Colores difuso y especular
	float ka, kd, ks;	// Constantes ambiental, difusa y especular
	int m;			// Parámetro de la reflexión especular
	Objeto();
	virtual ~Objeto();
	virtual Vector normal(Punto p)const;	//Devuelve la normal en p
	virtual int rayIntersection(Punto p, Vector v, float &t)const; //Devuelve si hay (1) o no interseccion (0).
																   //Si hay, se produce en P(t)= p+v*t
	void setColor(Color cd,
				  Color ce = Color::BLANCO,
				  float Ka = 0.3f,
				  float Kd = 0.8f,
				  float Ks = 0.3f,
				  int em = 1);
};


class Esfera: public Objeto{
protected:
	Punto centro;
	float radio;
public:
	Esfera();
	~Esfera();
	Esfera(Punto centro, float radio, Transformacion t=Transformacion());	// Constructor (centro, radio, escalado)
	Vector normal(Punto p)const;
	int rayIntersection(Punto p, Vector v, float &t)const;
};


class Poliedro: public Objeto{
public:
	vector<Poligono> caras;
};


class Caja: public Poliedro{
protected:
public:
	Caja(Transformacion);
	Caja(Punto caras[]);
	~Caja();
	Vector normal(Punto p) const;
	int rayIntersection(Punto p, Vector v, float &t) const;
};


/****** DESCOMENTAR PARA LA PRÁCTICA 4.3 ******/

#define ON 1
#define OFF 0

class FuenteLuminosa {
protected:
	Color I;
	int encendida;
	Punto posicion;
public:	
	FuenteLuminosa();
	FuenteLuminosa(Color intensidad,Punto pos=Punto()); //Constructor 
	virtual Color intensity(Punto p)const;				//Devuelve la intensidad vista desde p
	virtual Vector L(Punto p)const;						//Vector unitario desde p hacia la luz
	void setColor(Color c);								//Fija el color (intensidad)
	Punto position() const;								//Devuelve la posición de la luz
	void setPosition(Punto pos);						//Fija la posicion de la luz
	void switchLight(int);								//Enciende (1) o apaga (0)
	int switchOn()const;								//Devuelve el estado de la luz (1) encendida, (0) apagada
};



class Ambiental: public FuenteLuminosa{
public:
	Ambiental();
	Vector L(Punto p)const;
};


class Puntual: public FuenteLuminosa{
public:
	Puntual();
	Puntual(Color c, Punto pos=Punto());
};


class Direccional: public FuenteLuminosa{
protected:
	Vector direccion;
public:
	Direccional();
	Direccional(Color intens, Vector direc=Vector(0,-1,0));
	void setDirection(Vector d);
	Vector L(Punto p)const;
};


class Focalizada: public FuenteLuminosa{
protected:
	Vector direccion;	// Dirección de central del foco
	double p;			// Parámetro que regula la distribución de la 
						// intensidad en el foco, según el modelo de Warn
	double semiapertura;// Coseno entre el eje y la generatriz del cono
public:
	Focalizada();
	Focalizada(Color inten,Punto posic,Vector direc,double concentracion,double angulo);
	Color intensity(Punto pt)const;
	void setShape(Vector dir, double concentracion, double angulo);
};
/******** DESCOMENTAR PARA LA PRACTICA 4.3 ******/


class Escena{
protected:
	//Añade aquí tu código. La escena debe manejar los objetos (y las luces en 4.3)
	vector<Objeto *> objetos;
	vector<FuenteLuminosa *> luces;

public:
	//Añade aquí tu código. Métodos públicos sobre Escena
	int add(Objeto *o);										//Añade un objeto a la lista manejada por la escena
	int addLight(FuenteLuminosa *fl);
	Color rayTrace(Punto inicio,Vector direccion) const;	//Trazador (recursivo en practica 4.4)

	FuenteLuminosa *getLight(int pos) const {
		return luces[pos];
	}

	void switchShadow(int onoff){
		//
	}
};


#endif