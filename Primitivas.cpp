#include <iostream>
//#include <windows.h>
#include <GL/GLUT.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
#include "Primitivas.h"

Primitiva::Primitiva()
{
	color=Color::ROJO;
}
void Primitiva::setcolor(Color c)
{
	color=c;
}
Flecha::Flecha(void)
{
	vector=Vector(1.0f,0.0f,0.0f);
}

Flecha::Flecha(Vector dir, Color col)
{
	color=col;
	vector=dir;
}
void Flecha::draw(void)const
{
	drawAt(Punto());
}

void Flecha::drawAt(const Punto &en)const
{
	float largo= vector.module();
	Punto punta= en+vector;
	glColor3f(color.r(),color.g(),color.b());
	if(largo<0.001){ //Es muy pequeño o nulo; lo dibujamos como un punto
		glPointSize(4.0);
		glBegin(GL_POINTS);
			glVertex3d(punta.x(),punta.y(),punta.z());
		glEnd();
	}
	else { //Si es más grande como un flecha
		glBegin(GL_LINES);
			glVertex3d(en.x(),en.y(),en.z());
			glVertex3d(punta.x(),punta.y(),punta.z());
		glEnd();
		glPushMatrix();
		glTranslatef(punta.x(),punta.y(),punta.z());
		glRotatef(vector.longitude(),0.0,1.0,0.0);
		glRotatef(-vector.latitude(),1.0,0.0,0.0);
		glTranslatef(0.0,0.0,-largo/10.0);
		glutWireCone(largo/50.0,largo/10.0,10,1);
		glPopMatrix();
	};
}


Arista::Arista(void)
{
	vertice1=Punto(0.0f,0.0f,0.0f);;
	vertice2=Punto(1.0f,0.0f,0.0f);;
}

Arista::Arista(Punto v1, Punto v2, Color col)
{
	vertice1=v1;
	vertice2=v2;
	color=col;
}
void Arista::draw(void)const
{
	//Antes de dibujar se asegura la pertenencia al espacio homogeneo
	Punto vh1= vertice1.homogen();
	Punto vh2= vertice2.homogen();
	
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_LINES);
		glVertex3d(vh1.x(),vh1.y(),vh1.z());
		glVertex3d(vh2.x(),vh2.y(),vh2.z());
	glEnd();
}

Cubo::Cubo(void)
{
	aristas[0]=Arista(Punto(-1.0f,-1.0f,1.0f),Punto(1.0f,-1.0f,1.0f),color);
	aristas[1]=Arista(Punto(1.0f,-1.0f,1.0f),Punto(1.0f,1.0f,1.0f),color);
	aristas[2]=Arista(Punto(1.0f,1.0f,1.0f),Punto(-1.0f,1.0f,1.0f),color);
	aristas[3]=Arista(Punto(-1.0f,1.0f,1.0f),Punto(-1.0f,-1.0f,1.0f),color);
	aristas[4]=Arista(Punto(-1.0f,-1.0f,-1.0f),Punto(1.0f,-1.0f,-1.0f),color);
	aristas[5]=Arista(Punto(1.0f,-1.0f,-1.0f),Punto(1.0f,1.0f,-1.0f),color);
	aristas[6]=Arista(Punto(1.0f,1.0f,-1.0f),Punto(-1.0f,1.0f,-1.0f),color);
	aristas[7]=Arista(Punto(-1.0f,1.0f,-1.0f),Punto(-1.0f,-1.0f,-1.0f),color);
	aristas[8]=Arista(Punto(-1.0f,-1.0f,1.0f),Punto(-1.0f,-1.0f,-1.0f),color);
	aristas[9]=Arista(Punto(1.0f,-1.0f,1.0f),Punto(1.0f,-1.0f,-1.0f),color);
	aristas[10]=Arista(Punto(1.0f,1.0f,1.0f),Punto(1.0f,1.0f,-1.0f),color);
	aristas[11]=Arista(Punto(-1.0f,1.0f,1.0f),Punto(-1.0f,1.0f,-1.0f),color);
}

Cubo::Cubo(Arista a[12], Color col)
{
	for (int i=0; i<12; i++)
		aristas[i]=Arista(a[i].vertice1,a[i].vertice2,col);
	color=col;
}
void Cubo::draw(void)const
{
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_LINES);
		for (int i=0; i<12; i++)
			aristas[i].draw();
	glEnd();
}

void Cubo::drawWith(const Transformacion &t)const
{
	glColor3f(color.r(),color.g(),color.b());
	glBegin(GL_LINES);
		for (int i=0; i<12; i++)
			Arista(aristas[i].vertice1.transform(t), aristas[i].vertice2.transform(t), color).draw();
	glEnd();
}


Cuadrilatero::Cuadrilatero(){
	vertices.push_back(Punto(0,0,0));
	vertices.push_back(Punto(0,1,0));
	vertices.push_back(Punto(1,1,0));
	vertices.push_back(Punto(1,0,0));
}
Cuadrilatero::Cuadrilatero(Punto puntos[4]){
	for(int i=0; i<4; i++)
		vertices.push_back(puntos[i]);
}
