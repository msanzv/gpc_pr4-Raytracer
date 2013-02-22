#ifndef _CALGEBRA
#define _CALGEBRA

#define PI 3.14159265f
#define DEG2RAD(x) (((x)*PI)/180.0)
#define RAD2DEG(x) (((x)*180.0)/PI)
#define EPSILON 0.0001f
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a<b?b:a)

//Clases basicas de geometria y algebra

class Real4;
class Punto;
class Vector;
class Color;
class Matriz;
class Transformacion;
class Bloque;

class Real4
//Clase madre de otras que maneja cuatro valores reales
{
protected:
	float co[4];
public:
	Real4(float x=0.0, float y=0.0,					//Constructor
		  float z=0.0, float w=0.0);
	Real4 operator =(const Real4 &);				//Asignación por copia de valores
	virtual int operator ==(const Real4 &)const;	//Igualdad entre vectores
	virtual float operator *(const Real4 &) const;	//Producto Real4 fila por Real4 columna
	virtual Real4 operator *(const Matriz &) const;	//Producto por una matriz
	virtual Matriz operator *(const Bloque &) const;//Producto por capas generando filas de la matriz
	float elem(int i) const;						//Devuelve el valor del elemento i
	Real4 setelem(int i, float value);				//Cambia el valor del elemento i. Se devuelve
	float x() const;								//Devuelve x (valor de co[0])
	float y() const;								//Devuelve y (valor de co[1])
	float z() const;								//Devuelve z (valor de co[2])
	float w() const;								//Devuelve w (valor de co[3])
	Real4 setx(float x);							//Cambia el valor de la x. Se devuelve
	Real4 sety(float y);							//Cambia el valor de la y. Se devuelve
	Real4 setz(float z);							//Cambia el valor de la z. Se devuelve
	void print()const;								//Escribe los valores
};

class Punto: public Real4
//Punto afin homogéneo
{
public:
	Punto(float x=0.0, float y=0.0,	float z=0.0);	//Constructor
	Punto operator =(const Real4 &);				//Asignación por copia de valores
	float operator *(const Punto &)const;			//Genera un error. El producto no esta definido para puntos
	Punto homogen()const;							//Divide por la w
	Punto operator +(const Vector &)const;			//Nuevo punto
	Vector operator -(const Punto &)const;			//Resta de puntos genera vector
	Punto transform(const Transformacion &)const;	//Devuelve el punto transformado
	Punto negated()const;							//Punto con el signo cambiado
	Vector asVector()const;							//Devuelve el vector posición del punto
};
class Vector: public Real4
//Vector afin
{
public:
	Vector(float x=0.0, float y=0.0, float z=0.0);	//Constructor
	Vector operator =(const Real4 &);				//Asignación por copia de valores
	float operator *(Vector) const;					//Producto escalar
	Vector operator ^(Vector) const;				//Producto vectorial
	Vector operator +(Vector) const;				//Suma de dos vectores
	Vector operator -(Vector) const;				//Resta de vectores
	Vector operator *(float k)const;				//Producto por un escalar
	Vector negated() const;							//Vector en sentido contrario
	Vector unitary() const;							//Vector unitario es esa direccion
	float module() const;							//Calcula el modulo del vector
	float longitude() const;						//Calcula la longitud esferica del vector
	float latitude() const;							//Calcula la latitud esferica del vector
};
class Color: public Real4
 {
 public:
	 static const Color NEGRO;
	 static const Color BLANCO;
	 static const Color ROJO;
	 static const Color VERDE;
	 static const Color AZUL;

	 Color();										//Constructor por defecto a negro
	 Color(float r, float g, float b, float a=1.0f);//Constructor
	 Color operator *(const float k)const;			//Multiplica por un escalar
	 Color operator *(const Color c)const;			//Devuelve el color multiplicado componente a componente
	 Color operator +(const Color &) const;			//Suma de colores
	 Color setColor(const Color &);					//Cambia el color al indicado
	 float r() const;								//Componente roja
	 float g() const;								//Componente verde
	 float b() const;								//Componente azul
};
	

class Matriz
//Matriz de 4x4 coordenadas o Matriz de 4 puntos
{
protected:
	float e[4][4];
public:
	Matriz();										//Construye la identidad por defecto
	Matriz(Real4 e1,Real4 e2,						//Constructor por filas
		   Real4 e3,Real4 e4);
	float elem(int i,int j) const;					//Devuelve el elemento i,j
	Matriz setelem(int i,int j,float valor);		//Cambia el valor del elemento i,j
	Real4 row(int i) const;							//Devuelve un punto con la fila i(0..3)
	Real4 column(int i) const;						//Devuelve un punto con la columna i(0..3)
	Matriz operator =(const Matriz &);				//Asignacion de matrices
	Matriz operator *(const Matriz &)const;			//Producto de matrices
	Real4 operator *(const Real4 &)const;			//Producto por un vector columna
	Bloque operator *(const Bloque &)const;			//Producto por capas con un bloque
	Matriz operator *(float k)const;				//Producto por un escalar
	Matriz operator !()const;						//Traspone filas y columnas
	void print()const;								//Escribe la matriz
};
class Transformacion: public Matriz
// Matriz de transformación derivada de matriz que acumula transformaciones 3D
{
public:

	//La acumulacion es LIFO sobre el punto pensando en coordenadas fijas
	//y se debe aplicar por la izquierda del punto en columna (idem GL)
	//Ej: P' = M1*M2*M3*P  (M3 fue la ultima en acumularse y la primera en aplicarse)

	Transformacion();								//Constructor
	Transformacion operator =(const Matriz &);		//Asignacion de transformaciones
	Transformacion operator *(const Matriz &)const;	//Devuelve la transformación compuesta
	Transformacion append(const Matriz &);			//Acumula por la derecha
	Transformacion translation(Real4);				//Acumula la traslación indicada
	Transformacion scale(float sx=1.0,float sy=1.0,float sz=1.0, Punto centro=Punto());//Acumula el escalado
	Transformacion rotation(float grados, Vector vectorEje, Punto enRecta=Punto() );//Acumula un giro alrededor de la recta
	Transformacion rotX(float grados);				//Acumula un giro alrededor de X
	Transformacion rotY(float grados);				//Acumula un giro alrededor de Y
	Transformacion rotZ(float grados);				//Acumula un giro alrededor de Z
	Transformacion rotation(Vector u, Vector v, Vector w);	//Acumula el giro para que los ejes u,v,w coincidan con X,Y,Z
	Transformacion reset();							//Restaura la identidad
};

class Bloque
//Matriz cubica de 4x4x4 (filas X columnas X capas)
{
protected:
	float e[4][4][4];
public:
	Bloque();
	Bloque(Matriz , Matriz , Matriz , Matriz );		//Constructor por columnas
	Real4 drawer(int i,int j)const;					//Devuelve el punto en fila i, columna j
	Bloque setdrawer(int i, int j, const Real4 &);	//Mete el punto en la posicion i,j
	Bloque operator =(const Bloque &);				//Asignación entre bloques
	Bloque operator *(const Matriz &)const;			//Producto por capas con una matriz
	Matriz layer(int k)const;						//Devuelve una matriz con la capa indicada
	Bloque setlayer(int k, const Matriz &);			//Cambia la capa por la matriz
	Bloque addrowsup();								//Añade cada fila inferior a la superior
	void print()const;
};

#endif 