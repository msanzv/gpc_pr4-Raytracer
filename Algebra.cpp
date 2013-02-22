//Clases basicas de geometria y algebra

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Algebra.h"

using namespace std;

//Real4---------------------------

Real4::Real4(float x, float y, float z, float w)
//Constructor por defecto
{
	co[0]=x; co[1]=y; co[2]=z; co[3]=w;
}
Real4 Real4::operator =(const Real4 &r4)
//Asignación por copia de valores
{
	for(int i=0;i<4;i++) co[i]=r4.elem(i);
	return *this;
}
int Real4::operator ==(const Real4 &r4) const
{
	return (x()==r4.x() && y()==r4.y() && z()==r4.z() && w()==r4.w());
}
float Real4::operator *(const Real4 &r4) const
//Producto Real4 fila por Real4 columna
{
	float r=0.0;
	for(int i=0;i<4;i++) r+= co[i]*r4.elem(i);
	return r;
}
Real4 Real4::operator *(const Matriz &m) const
//Producto por una matriz
{
	return Real4((*this)*m.column(0),(*this)*m.column(1),(*this)*m.column(2),(*this)*m.column(3));
}
Matriz Real4::operator *(const Bloque &b) const
//Producto por capas generando filas de la matriz
{
	return Matriz((*this)*b.layer(0),(*this)*b.layer(1),(*this)*b.layer(2),(*this)*b.layer(3));
}
float Real4::elem(int i) const
//Devuelve el valor del elemento i
{
	return co[i];
}
Real4 Real4::setelem(int i, float value)
//Cambia el valor del elemento i. Se devuelve
{
	co[i]=value;
	return (*this);
}
float Real4::x() const
//Devuelve x (valor de co[0])
{
	return elem(0);
}
float Real4::y() const
//Devuelve y (valor de co[1])
{
	return elem(1);
}
float Real4::z() const
//Devuelve z (valor de co[2])
{
	return elem(2);
}
float Real4::w() const
//Devuelve w (valor de co[3])
{
	return elem(3);
}
Real4 Real4::setx(float x)
//Cambia el valor de la x. Se devuelve
{
	return setelem(0,x);
}
Real4 Real4::sety(float y)
//Cambia el valor de la y. Se devuelve
{
	return setelem(1,y);
}
Real4 Real4::setz(float z)
//Cambia el valor de la z. Se devuelve
{
	return setelem(2,z);
}
void Real4::print()const
//Escribe los valores
{
	printf("(%4.2f,%4.2f,%4.2f,%4.2f)\n",x(),y(),z(),w());
}

//Punto ------------------------
Punto::Punto(float x, float y, float z):Real4(x,y,z,1)
//Constructor
{
}
Punto Punto::operator =(const Real4 &r)
//Asignacion por copia
{
	for(int i=0;i<4;i++) co[i]=r.elem(i);
	return *this;
}
float Punto::operator *(const Punto &p)const
//Genera un error. El producto no esta definido para puntos
{
	cerr << "Operacion no permitida entre Puntos \n";
	return 0.0;
}
Punto Punto::homogen()const
//Divide por la w
{
	return Punto(x()/w(),y()/w(),z()/w());
}
Punto Punto::operator +(const Vector &v)const
//Nuevo punto
{
	return Punto(x()+v.x(),y()+v.y(),z()+v.z());
}
Vector Punto::operator -(const Punto &p)const
//Resta de puntos genera vector
{
	return Vector(x()-p.x(),y()-p.y(),z()-p.z());
}

Punto Punto::transform(const Transformacion &t)const
//Devuelve el punto transformado
{
	Punto p;
	for(int i=0;i<4;i++) p.setelem(i,t.row(i)*(*this));
	return p;
}
Punto Punto::negated()const
//Punto con el signo cambiado
{
	return Punto(-x(),-y(),-z());
}
Vector Punto::asVector() const
//Devuelve el punto como un vector posicion
{
	return (*this)-Punto();
}
//Vector ----------------------------------
Vector::Vector(float x, float y, float z): Real4(x,y,z,0)
//Constructor
{
}
Vector Vector::operator =(const Real4 &r)
//Asignacion por copia
{
	for(int i=0;i<4;i++) co[i]=r.elem(i);
	return *this;
}
float Vector::operator *(Vector v) const
//Producto escalar
{
	return x()*v.x()+y()*v.y()+z()*v.z();
}
Vector Vector::operator ^(Vector v) const
//Producto vectorial
{
	Vector pv= Vector( y()*v.z()-z()*v.y(), z()*v.x()-x()*v.z(), x()*v.y()-y()*v.x());
	for(int i=0;i<3;i++) if(fabs(pv.elem(i))<EPSILON) pv.setelem(i,0);
	return pv;
}
Vector Vector::operator +(Vector v) const
//Suma de dos vectores
{
	return Vector(x()+v.x(),y()+v.y(),z()+v.z());
}
Vector Vector::operator -(Vector v) const
//Resta de vectores
{
	return (*this)+v.negated();
}
Vector Vector::operator *(float k)const
//Producto por un escalar
{
	return Vector(x()*k, y()*k, z()*k);
}
Vector Vector::negated() const
//Vector en sentido contrario
{
	return Vector(-x(),-y(),-z());
}
Vector Vector::unitary() const
//Vector unitario es esa direccion
{
	return (*this)*(1.0f/module());
}
float Vector::module() const
//Calcula el modulo del vector
{
	return (float)sqrt((*this)*(*this));
}
float Vector::longitude() const
//Calcula la longitud (0..360º) esferica del vector en grados
{
	float lon;
	if(x()+z()==0.0) return 0.0;
	lon= (float)acos(z()/sqrt(x()*x()+z()*z()));		
	if(x()<0) lon=(float)(2*PI-lon);
	return (float)RAD2DEG(lon);
}
float Vector::latitude() const
//Calcula la latitud (-90º..90º) esférica del vector en grados
{
	float lat;
	lat= (float)acos(sqrt(x()*x()+z()*z())/module());		
	if(y()<0) lat= -lat;
	return (float)RAD2DEG(lat);
}
//Color -----------------------------------
const Color Color::NEGRO=Color(0,0,0);
const Color Color::BLANCO=Color(1,1,1);
const Color Color::ROJO=Color(1,0,0);
const Color Color::VERDE=Color(0,1,0);
const Color Color::AZUL=Color(0,0,1);

Color::Color():Real4(0,0,0,1)
//Constructor por defecto a negro
{
}
Color::Color(float r, float g, float b, float a)
//Constructor
{
	co[0]=0; if(r>0.0) co[0]=r;
	co[1]=0; if(g>0.0) co[1]=g;
	co[2]=0; if(b>0.0) co[2]=b;
	co[3]=0; if(a>0.0) co[3]=a;
	for(int i=0; i<4; i++) if(co[i]>1.0) co[i]=1.0;
}
Color Color::operator *(const float k)const
//Multiplica por un escalar
{
		return Color(k*x(),k*y(),k*z(),w());
}
Color Color::operator *(const Color c) const
//Devuelve el producto de colores por componentes
{
	float min=c.w();
	if(w()<min) min=w();
	return Color(w()*x()*c.w()*c.x(),w()*y()*c.w()*c.y(),w()*z()*c.w()*c.z(),min);
}
Color Color::operator +(const Color &c) const
//Suma de colores
{
	float max=c.w();
	if(w()>max) max=w();
	return Color(w()*x()+c.w()*c.x(),w()*y()+c.w()*c.y(),w()*z()+c.w()*c.z(),max);
}
Color Color::setColor(const Color &c)
//Cambia el color al indicado
{
	for(int i=0; i<4; i++) setelem(i,c.elem(i));
	return (*this);
}
float Color::r() const
//Devuelve la componente roja
{
	return x();
}
float Color::g() const
//Devuelve la componente verde
{
	return y();
}
float Color::b() const
//Devuelve la componente azul
{
	return z();
}
//Clase Matriz -------------------------------------------

Matriz::Matriz()
//Construye la identidad por defecto
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(j==i) e[i][j]=1.0f;
			else e[i][j]=0.0f;
		}
}

Matriz::Matriz(Real4 e1,Real4 e2,Real4 e3,Real4 e4)
//Construye la matriz por filas
{
	int j;
	for(j=0;j<4;j++){
		e[0][j]=e1.elem(j);
		e[1][j]=e2.elem(j);
		e[2][j]=e3.elem(j);
		e[3][j]=e4.elem(j);
	}
}
float Matriz::elem(int i, int j) const
//Devuelve el elemento i,j
{
	return e[i][j];
}
Matriz Matriz::setelem(int i, int j, float valor)
//Cambia el valor del elemento i,j
{
	e[i][j]=valor;
	return (*this);
}
Real4 Matriz::row(int i) const
//Devuelve un punto formado por la fila i (0..3)
{
	if(i<4 && i>=0) return Real4(e[i][0],e[i][1],e[i][2],e[i][3]);
	else {		
		cerr << "Matriz:Indice de fila fuera de rango\n";
		exit(1);
	}
	return Real4();
}

Real4 Matriz::column(int i)const
//Devuelve un punto con la columna i (0..3)
{
	if(i<4 && i>=0) return Real4(e[0][i],e[1][i],e[2][i],e[3][i]);
	else {		
		cerr << "Matriz:Indice de columna fuera de rango\n";
		exit(1);
	}
	return Real4();
}


Matriz Matriz::operator =(const Matriz &m)
//Asignacion de matrices
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++) e[i][j]=m.elem(i,j);
	return *this;
}

Matriz Matriz::operator *(const Matriz &m)const
//Producto de matrices. Devuelve una nueva matriz
{
	int i,j,k;
	Matriz pr=Matriz(Real4(),Real4(),Real4(),Real4()); //Todo ceros

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++)
				pr.setelem(i,j,pr.elem(i,j)+e[i][k]*m.elem(k,j));
	return pr;
}

Real4 Matriz::operator *(const Real4 &p)const
//Producto por un punto POR LA DERECHA (M*P)
{
	return Real4(row(0)*p,row(1)*p,row(2)*p,row(3)*p);
}

Bloque Matriz::operator *(const Bloque &m3d)const
//Multiplica por capas (k cte.) y devuelve un bloque B'=M*B
{
	Bloque mr;	
	for(int k=0; k<4; k++) mr.setlayer(k, (*this)*m3d.layer(k));
	return mr;
}


Matriz Matriz::operator *(float k)const
//Producto por un escalar (M*k). Devuelve una nueva matriz
{
	int i,j;
	Matriz pr;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			pr.setelem(i,j,e[i][j]*k);

	return pr;
}

Matriz Matriz::operator !()const
//Devuelve la traspuesta en una nueva matriz. 
//Usar parentesis para asegurar precedencia (!M)
{
	int i,j;
	Matriz t;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			t.setelem(i,j,e[j][i]);

	return t;
}

void Matriz::print()const
//Escribe la matriz por filas en la salida estandar
{
	int i,j;
	printf("\n");
	for(i=0;i<4;i++){
		printf("|");
		for(j=0;j<4;j++)
			printf("%4.2f ",e[i][j]);
		printf("|\n");
	}

}

//Clase Transformacion (subclase de Matriz) -------------------------
Transformacion::Transformacion()
//Constructor 
{
}
Transformacion Transformacion::operator =(const Matriz &m)
//Asignacion de transformaciones
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++) e[i][j]=m.elem(i,j);
	return *this;
}
Transformacion Transformacion::operator *(const Matriz &m)const
//Devuelve el producto de las dos matrices
{
	int i,j,k;
	Transformacion pr;
	pr= Matriz(Real4(),Real4(),Real4(),Real4()); 

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++)
				pr.setelem(i,j,pr.elem(i,j)+e[i][k]*m.elem(k,j));
	return pr;
}

Transformacion Transformacion::append(const Matriz &m)
//Acumula por la derecha
{
	(*this)= (*this)*m;
	return (*this);
}

Transformacion Transformacion::translation(Real4 d)
//Acumula la traslacion propuesta
{
	Matriz t;
	int i;
	for (i=0;i<3;i++) t.setelem(i,3,d.elem(i));
	return append(t);
}

Transformacion Transformacion::scale(float sx,float sy,float sz, Punto centro)
//Acumula el escalado sobre un punto generico
{
	Matriz s;
	translation(centro);
	s.setelem(0,0,sx); s.setelem(1,1,sy); s.setelem(2,2,sz);
	append(s);
	return translation(centro.negated());
}

Transformacion Transformacion::rotation(float ang, Vector vectorEje, Punto enRecta )
//Acumula el giro alrededor de un eje generico
{
	float lat,lon;

	lat= vectorEje.latitude();		//latitud
	lon= vectorEje.longitude();		//longitud

	//Recordar acumulacion LIFO
	translation(enRecta);			//Rehacer la traslacion
	rotY(lon);						//Rehacer la longitud
	rotX(-lat);						//Rehacer la latitud
	rotZ(ang);						//Girar el angulo
	rotX(lat);						//Deshacer la latitud
	rotY(-lon);						//Deshacer la longitud
	return translation(enRecta.negated());	//Traslacion al origen
}

Transformacion Transformacion::rotation(Vector u, Vector v, Vector w)
//Acumula el giro por cambio de base
{
	Matriz g= Matriz(u,v,w,Punto());  //Por filas
	return append(g);
}
Transformacion Transformacion::rotX(float ang)
//en X
{
	Matriz g;
	g.setelem(1,1,(float)cos(DEG2RAD(ang)));
    g.setelem(1,2,(float)-sin(DEG2RAD(ang)));
	g.setelem(2,1,-g.elem(1,2));
	g.setelem(2,2,g.elem(1,1));

	return append(g);
}
Transformacion Transformacion::rotY(float ang)
//en Y
{
	Matriz g;
	g.setelem(0,0,(float)cos(DEG2RAD(ang)));
    g.setelem(0,2,(float)sin(DEG2RAD(ang)));
	g.setelem(2,0,-g.elem(0,2));
	g.setelem(2,2,g.elem(0,0));

	return append(g);
}
Transformacion Transformacion::rotZ(float ang)
//en Z
{
	Matriz g;
	g.setelem(0,0,(float)cos(DEG2RAD(ang)));
    g.setelem(0,1,(float)-sin(DEG2RAD(ang)));
	g.setelem(1,0,-g.elem(0,1));
	g.setelem(1,1,g.elem(0,0));

	return append(g);
}

Transformacion Transformacion::reset()
{
	Transformacion identidad;
	*this=identidad;
	return (*this);
}

// Clase Bloque -------------------------------------------


Bloque::Bloque()
//Constructor por defecto. Matriz toda a 0.0
{
	int i,j,k;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++) e[i][j][k]=0.0f;
}

Bloque::Bloque(Matriz c1, Matriz c2, Matriz c3, Matriz c4)
//Contruye el bloque con las matrices como columnas
{
	int i,k;

	for(i=0;i<4;i++)
		for(k=0;k<4;k++){
			e[i][0][k]=c1.elem(k,i);
			e[i][1][k]=c2.elem(k,i);
			e[i][2][k]=c3.elem(k,i);
			e[i][3][k]=c4.elem(k,i);
	}
}

Bloque Bloque::operator =(const Bloque &m)
//Asignacion de matrices 3d
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			setdrawer(i,j,m.drawer(i,j));
	return *this;
}

Bloque Bloque::operator *(const Matriz &m)const
//Multiplica por capas y devuelve la nueva matriz 3d
{
	Bloque br;
	for(int i=0; i<4; i++) br.setlayer(i,layer(i)*m);
	return br;
}

Real4 Bloque::drawer(int i, int j)const
//Devuelve el punto para esa fila,columna 
{
	return Real4(e[i][j][0], e[i][j][1],e[i][j][2],e[i][j][3]);
}
Bloque Bloque::setdrawer(int i, int j, const Real4 &r)
//Pone el punto en la fila,columna
{
	for(int k=0; k<4; k++) e[i][j][k]=r.elem(k);
	return (*this);
}
Matriz Bloque::layer(int k)const
//Devuelve la capa k como una matriz
{
	Matriz m;
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++) m.setelem(i,j,e[i][j][k]);
	return m;
}
Bloque Bloque::setlayer(int k, const Matriz &m)
//Cambia la capa por la matriz m
{
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++) e[i][j][k]=m.elem(i,j);
	return (*this);
}
Bloque Bloque::addrowsup()
//Añade cada fila inferior a la inmediatamente superior
{
	for(int i=0;i<3;i++)				
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				e[i][j][k]+=e[i+1][j][k];
	return *this;
}
void Bloque::print()const
//escribe la matriz por la salida estandar por columnas
{
	int i,j,k;

	for(j=0;j<4;j++){
		printf("\n");
		for(i=0;i<4;i++){
			printf("\n");
			for(k=0;k<4;k++) 
				printf("%4.2f ",e[i][j][k]);
		}
	}
	printf("\n");
}



