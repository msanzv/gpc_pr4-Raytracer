/** PRACTICA DE TRAZADO DE RAYOS
Prof: R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011
Alumno:                                        **/

#include <iostream>
#include <windows.h>
#include <GL/GL.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "EscenaIncompleta.h"

/** Añade aquí tu código **/

// Clase Objeto -------------------------
// Objeto gráfico con volumen
Objeto::Objeto(void){

	colDifuso = Color::ROJO;
	ka = 0.5f;
	kd = 1.0f;
	ks = 0.3f;
	m = 1;
}

Objeto::~Objeto(void){
}

void Objeto::setColor(Color cd,Color ce,float Ka,float Kd,float Ks,int em){

	colDifuso = cd;
	colEspecular = ce;
	ka = Ka;
	kd = Kd;
	ks = Ks;
	m = em;
}

Vector Objeto::normal(Punto p) const{

	return Vector();
}

int Objeto::rayIntersection(Punto p, Vector v, float &t) const{

	t = 0;
	return 0;
}




// Clase Esfera -------------------------
// Deriva de Objeto
Esfera::Esfera(){

	centro = Punto();
	radio = 1.0f;
}

Esfera::Esfera(Punto c, float r, Transformacion t){

	centro = c.transform(t);
	/* Sólo como aproximación, para la esfera, tengo en cuenta el escalado
	del radio */
	radio = r*t.elem(0,0);
}

Esfera::~Esfera(void){

	return;
}

Vector Esfera::normal(Punto p) const{

	return (p-centro).unitary();
}


int Esfera::rayIntersection(Punto p, Vector v, float &t) const{
//Calcula la intersección con el rayo(p,v)
//Devuelve (0:no hay interseccion) (1:interseccion externa) (-1:interseccion interna)

	int ni; 
	float alfa,beta,gamma;
	float raiz;
	float t1,t2;
	Vector pcentro = (p-centro);
	
	// El primer punto, en el caso que haya 2, será el más cercano al inicio del rayo
	
	alfa = v*v;
	beta = 2*(pcentro*v);
	gamma = (pcentro*pcentro) - radio*radio;

	raiz = beta*beta-4*alfa*gamma;
	if (raiz<0.0f) return 0; // Raíces imaginarias
	
	raiz = (float)sqrt(raiz);
	t2 = (-beta+raiz)/(2*alfa);
	t1 = (-beta-raiz)/(2*alfa);
	//Debido a que alfa>0, t1<t2

	if(t1>0){	//Hay dos intersecciones despues del inicio del rayo
		t = t1;
		ni = 1;
	}
	else if(t2>0){ //Hay una interseccion despues del inicio del rayo, estamos dentro
		t = t2;
		ni = -1;
	}
	else{ //Las dos intersecciones ocurren antes del inicio del rayo
		ni = 0;
	};
	return ni;
}



// Clase Caja -------------------------
// Deriva de Objeto
Caja::Caja(Transformacion t){

	Punto p_puntos[24]= {Punto(-1,-1,-1), Punto(-1, 1,-1), Punto( 1, 1,-1), Punto( 1,-1,-1),
					     Punto(-1, 1,-1), Punto(-1, 1, 1), Punto( 1, 1, 1), Punto( 1, 1,-1), 
					     Punto( 1, 1,-1), Punto( 1, 1, 1), Punto( 1,-1, 1), Punto( 1,-1,-1),
				 	     Punto(-1, 1, 1), Punto(-1,-1, 1), Punto( 1,-1, 1), Punto( 1, 1, 1),    
					     Punto(-1, 1,-1), Punto(-1,-1,-1), Punto(-1,-1, 1), Punto(-1, 1, 1),
					     Punto(-1,-1,-1), Punto( 1,-1,-1), Punto( 1,-1, 1), Punto(-1,-1, 1)};
	Punto p_caras[4];
	int n = 0;
	for(int i=0; i<6; i++){
		for(int j=0; j<4; j++){
			p_caras[j] = p_puntos[n].transform(t);
			n++;
		}
		Cuadrilatero c(p_caras);
		caras.push_back(c);
	}
}

Caja::Caja(Punto caras[24]){

	Punto p_caras[4];
	int n = 0;
	for(int i=0; i<6; i++){
		for(int j=0; j<4; j++){
			p_caras[j] = caras[n++];
		}
	}
}

//Caja::Caja(float ladomayor, float ladomenor, Transformacion t=Transformacion()){

Vector Caja::normal(Punto p) const{

	int cara = -1; 
	float D, dist, mindist = INFINITO;
	Punto pto;
	Vector v1, v2, vn;
	for(unsigned int i=0; i<caras.size(); ++i){
		pto = caras[i].vertices[0];
		v1 = caras[i].vertices[1] - caras[i].vertices[0]; 
		v2 = caras[i].vertices[2] - caras[i].vertices[0];
		vn = (v1^v2).unitary();
		D = 0;
		for(int k=0; k<3; ++k)
			D -= vn.elem(k)*pto.elem(k);
		dist = 0;
		for(int k=0; k<3; ++k)
			dist += vn.elem(k)*p.elem(k);
		dist = abs(dist + D);
		//dist = abs(dist + ((pto.x()*vn.x() + pto.y()*vn.y() + pto.z()*vn.z())));
		if(dist < mindist){
			cara = i;
			mindist = dist;
		}
	}
	pto = caras[cara].vertices[0];
	v1 = caras[cara].vertices[1] - caras[cara].vertices[0]; 
	v2 = caras[cara].vertices[2] - caras[cara].vertices[0];
	vn = (v1^v2).unitary();
	return vn;
}

int Caja::rayIntersection(Punto inicio, Vector direccion, float &t) const{

	float te = 0;
	float ts = INFINITO;

	for(unsigned int i=0; i<caras.size(); ++i){
		Vector v1 = caras[i].vertices[1] - caras[i].vertices[0];
		Vector v2 = caras[i].vertices[2] - caras[i].vertices[0];
		Vector normal = (v1^v2).unitary();

		Punto a = caras[i].vertices[0];
		float rN = normal * direccion;
		if(rN == 0){
			if(((inicio-a)*normal) > 0) 
				return 0;
		}else if(rN > 0){
			t = (normal*(a-inicio))/rN;
			ts = min(ts,t);
		}else{
			t = (normal*(a-inicio))/rN;
			te = max(te,t);
		}
	}
	if(ts<te) return 0;
    else{
        if (te==0) return -1;
        else{
            t=te;
            return 1;
        }
	}
}

Caja::~Caja(){
	while(!caras.empty())
		caras.pop_back();
}


// Clase FuenteLuminosa ---------------------

/********** DESCOMENTAR PARA LA PRACTICA 4.3 ********/
FuenteLuminosa::FuenteLuminosa(){

	I = Color(1.0f,1.0f,1.0f);
	posicion = Punto();
	encendida = ON;
}

FuenteLuminosa::FuenteLuminosa(Color intensidad, Punto pos){

	I = intensidad;
	posicion = pos;
	encendida = ON;
}

Vector FuenteLuminosa::L(Punto p) const{

	return (posicion-p).unitary(); 
}

Color FuenteLuminosa::intensity(Punto p) const{

	return I;
}

void FuenteLuminosa::setColor(Color c){

	I = c;
}

Punto FuenteLuminosa::position() const{

	return posicion;
}

void FuenteLuminosa::setPosition(Punto pos){

	posicion = pos;
}

void FuenteLuminosa::switchLight(int ONOFF){

	encendida = ONOFF;
}

int FuenteLuminosa::switchOn() const{

	return encendida;
}

// Añade aquí tu código

// Iluminacion ambiental
Ambiental::Ambiental(){

	I = Color(1.0f,1.0f,1.0f);
	posicion = Punto();
	encendida = ON;
}

Vector Ambiental::L(Punto p) const{

	return Vector(0, 0, 0);
}

// Iluminacion puntual
Puntual::Puntual(){
}

Puntual::Puntual(Color c, Punto pos){

	I = c;
	posicion = pos;
	encendida = ON;
}

// Iluminacion direccional
Direccional::Direccional(){
}

Direccional::Direccional(Color intens, Vector direc){

	I = intens;
	direccion = direc;
}

void Direccional::setDirection(Vector d){

	direccion = d;
}

Vector Direccional::L(Punto p) const{

	return (posicion - p).negated().unitary();
}

// Iluminación focalizada
Focalizada::Focalizada(){
}

Focalizada::Focalizada(Color inten, Punto posic, Vector direc, double concentracion, double angulo){
        encendida = ON;
        I = inten;
        posicion = posic;
        direccion = direc;
        semiapertura = cos(DEG2RAD(angulo/2));
        p = concentracion;
}

Color Focalizada::intensity(Punto pt) const{
    Vector L = pt - posicion;
	float cosgamma = (L*direccion) / 
					 (L.module()*direccion.module());
	//printf("%f < %f\n",cosgamma, semiapertura);
	if(cosgamma < semiapertura)
        return Color(0.0,0.0,0.0,1);
	else
		return I * pow(cosgamma, (float)p);
}

void Focalizada::setShape(Vector dir, double concentracion, double angulo){
    direccion = dir;
    p = concentracion;
    semiapertura = cos(DEG2RAD(angulo/2));
}


/****** DESCOMENTAR PARA LA PRACTICA 4.3 *******/



// Clase Escena ------------------------
/** Añade aquí tu código **/

int Escena::add(Objeto *o){

	/** Añade aquí tu código **/
	objetos.push_back(o);
	return objetos.size() - 1;
}

Color Escena::rayTrace(Punto inicio, Vector direccion, int numReflex) const{
	
	float t, sht, rft;
	float min = INFINITO;
	int minpos;
	for(unsigned int i=0; i<objetos.size(); ++i){
		int ri = objetos[i]->rayIntersection(inicio, direccion, t);
		if(ri != 0 && t<min){
			min = t;
			minpos = i;
		}
	}
	if(min < INFINITO){
		Punto corte = inicio + direccion*min;

		Vector normal = objetos[minpos]->normal(corte);
		Color total = objetos[minpos]->colDifuso * 
					  objetos[minpos]->ka *
					  luces[0]->intensity(corte);			// I = ka*Cd*Ia  (Luz ambiental)
	 
		for(unsigned int i=1; i<luces.size(); ++i){				//Para cada otra luz
			//Sombras
			bool inShadow = false;									// Comprobación de sombras
			if(this->shadowsOn){
				for(unsigned int o=0; o<objetos.size(); ++o) {
					if(o!=minpos && objetos[o]->rayIntersection(corte, luces[i]->position()-corte, sht)) {
						inShadow = true;
						break;
					}
				}
			}
			if(!inShadow && luces[i]->switchOn()){					//si luz encendida
				Vector L = luces[i]->L(corte);							//L = rayo desde p a la luz
				Vector V = Punto(0,0,3) - corte;
				Vector H = (L+V); H = H*(1/H.module());
				float NL = 0, NH = 0;
				for(int k=0; k<3; ++k){
					NL += normal.elem(k)*L.elem(k);
					NH += normal.elem(k)*H.elem(k);
				}
				total = total + (luces[i]->intensity(corte) *
								 objetos[minpos]->colDifuso *
								 objetos[minpos]->kd *
								 NL);									// Ii*kd*Cd*(N*L)
				total = total + (luces[i]->intensity(corte) *
								 objetos[minpos]->ks *
								 pow(NH, objetos[minpos]->m));			// Ii*ks*(N*H)^m
			}	
			// Reflexion
			if(this->reflexOn){
				if(numReflex < this->maxReflex){
					total = total + this->rayTrace(corte+normal*0.01, normal, numReflex + 1);
				}
			}
		} 
		return total;
	}
	else												//No existe interseccion
		return Color(0, 0, 0, 1);							//Devolver color de fondo
}	

int Escena::addLight(FuenteLuminosa *fl){

	luces.push_back(fl);
	return luces.size() - 1;
}