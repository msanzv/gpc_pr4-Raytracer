#ifndef _CPRIMITIVAS
#define _CPRIMITIVAS

#include <vector>
#include "Algebra.h"

using namespace std;

class Primitiva;
class Flecha;
class Arista;
class Cubo;
class Poligono;
class Cuadrilatero;


class Primitiva
{
protected:
	Color color;
public:
	Primitiva();
	void setcolor(Color c);
	virtual void draw() const=0;
};
class Flecha: public Primitiva
{
private:
	Vector vector;					//Dirección
public:
	Flecha(void);
	Flecha (Vector dir, Color col);
	void draw()const;				//Dibuja la flecha en el origen
	void drawAt(const Punto &en)const;//Dibuja la flecha desde el Punto en
};

class Arista: public Primitiva
{
public:	
	Punto vertice1, vertice2;		// Vértices que la definen
	Arista(void);
	Arista (Punto v1, Punto v2, Color col=Color::ROJO);
	void draw()const; // Dibujo de la arista
};

class Cubo: public Primitiva
{
private:
	Arista aristas[12]; // Aristas que forman el cubo
public:
	Cubo(void);
	Cubo (Arista a[12], Color col=Color::NEGRO);
	void draw()const; // Dibuja el cubo según los vértices de sus aristas
	void drawWith(const Transformacion &t)const; // Dibuja el cubo transformado por t
};


class Poligono{
public:
	vector<Punto> vertices;
};


class Cuadrilatero: public Poligono{
	//Punto vertices[4];
public:
	Cuadrilatero();
	Cuadrilatero(Punto puntos[4]);
};


#endif 