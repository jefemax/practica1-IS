#include <stdio.h>
#include <iostream>
#include <thread>        
#include <conio.h>
#include <time.h>

//Constantes
#define ANCHO_MUNDO 79
#define DERECHA 1
#define IZQUIERDA 0

typedef unsigned int uint;
typedef unsigned char uchar;

//Constantes
const double ESPERA_EN_MILISEGUNDOS = 10;
const double MIL = 1000;

//variables personaje
uint g_posxPersonaje = ANCHO_MUNDO/2;
uchar g_personaje_actual = '>';
uchar g_piso_C = '_';
uchar g_personaje_d = '>';
uchar g_personaje_i = '<';

//variables bala
uint g_posxBala = 2;
uint g_velBala = 100;
bool g_direccion_bala = DERECHA;
bool g_balaActiva = false;
uchar g_personaje_bala = 'o';
double g_incrementoBala = 0.0;

//variables enemigos
uint g_posxEnem = 0;
uint g_velEnem = 20;
bool g_enemActivo = false;
//al inicio se cambia la direccion por eso empiezo a la izquierda
bool g_direccion_enemigo = IZQUIERDA;
uchar g_personajeEnem = '|';
double g_incrementoEnem = 0.0;

//variables generales
bool g_direccion = DERECHA;
bool g_mover = false;
unsigned int g_random = 0;
bool g_randomSet = false;
uint g_cuentaParaEnemigo;
double g_elapsepdTime = 0.0;
double g_frameRate = 0.0;
char g_key = ' ';
char *g_piso;
bool g_muerte = false;

//Funciones
bool getInput(char *c);
void ocultaEnemigos();
bool run();
bool draw();
void cleanWorld();
void muerte();



int main(int argc, char* argv[])
{

	printf("\n\n\n\n\n");
	printf("PRACTICA 1 INGENIERIA DE SOFTWARE\n");
	printf("PARA JUGAR USAR LAS TECLAS:\n");
	printf("A MUEVE A LA IZQUIERDA\n");
	printf("B MUEVE A LA DERECHA\n");
	printf("D DISPARA\n");
	printf("\n\n\n\n\n\n");
	
    srand(time(NULL));

	//Obtengo el tiempo entre cada iteracion
	g_frameRate = ESPERA_EN_MILISEGUNDOS / MIL;

	g_piso = new char[ANCHO_MUNDO];
    g_piso[ANCHO_MUNDO] = '\0';
    for (uint x = 0; x < ANCHO_MUNDO; x++){
        g_piso[x] = '_';
    }

	for (g_posxBala = 2; g_posxPersonaje < ANCHO_MUNDO - 1 && g_posxPersonaje>0;)
    {
        if (!g_muerte)
        {
            if (run())
                break;
            draw();
        }
        else
        {
            printf("Has muerto tu score es: Presiona barra para volver a jugar o \"q\" para salir");
            printf("\r");
            if (getInput(&g_key))
            {
                if (g_key == 32)
                {
                    g_muerte = false;
                }
                if (g_key == 'q')
                {
                    return 0;
                }
            }
        }
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(ESPERA_EN_MILISEGUNDOS)));

    }
	delete g_piso;
    return 0;
}

bool getInput(char *c)
{
    if (_kbhit())
    {
        *c = _getch();
        return true;
    }
    return false;
}

bool run(){

	if (getInput(&g_key))
	{
		if (g_key == 's')
		{
			g_direccion = DERECHA;
			g_personaje_actual = g_personaje_d;
			g_posxPersonaje++;
		}
		if (g_key == 'a')
		{
			g_direccion = IZQUIERDA;
			g_personaje_actual = g_personaje_i;
			g_posxPersonaje--;
		}
		if (g_key == 'd')
		{
			if (g_direccion == IZQUIERDA){
				g_direccion_bala = IZQUIERDA;
			} 	else if (g_direccion == DERECHA){
				g_direccion_bala = DERECHA;
			}
			g_balaActiva = true;
			g_posxBala = g_posxPersonaje;

		}
        if (g_key == 32)
        {
            g_muerte = false;
        }
        if (g_key == 'q')
        {
            return 0;
        }
	}
	if (g_balaActiva)
	{
		g_incrementoBala += g_frameRate*g_velBala;
		if (g_direccion_bala == DERECHA)
		{
			if (g_incrementoBala >= 1)	{ g_posxBala += 1;			g_incrementoBala = 0; }
		}
		else if (g_direccion_bala == IZQUIERDA)
		{
			if (g_incrementoBala >= 1)	{ g_posxBala -= 1;			g_incrementoBala = 0; }
		}

		if (g_posxBala > ANCHO_MUNDO || g_posxBala < 0)
			g_balaActiva = false;
	}


	if (!g_enemActivo && !g_randomSet)
	{
		g_random = rand() % 100;
		g_randomSet = true;
	}

	if (g_randomSet)
	{
		if (g_cuentaParaEnemigo == g_random)
		{
            g_direccion_enemigo = !g_direccion_enemigo;

            if (g_direccion_enemigo == DERECHA)
                g_posxEnem = ANCHO_MUNDO;
            else
                g_posxEnem = 0;

			g_enemActivo = true;

		}
		g_cuentaParaEnemigo++;
	}
	if (g_enemActivo)
	{
		g_incrementoEnem += g_frameRate*g_velEnem;

        if (g_posxEnem > ANCHO_MUNDO || g_posxEnem < 0)
        {
            ocultaEnemigos();
        }
        if (g_direccion_enemigo == DERECHA){
			if (g_incrementoEnem >= 1)
            {
                g_posxEnem -= 1;
                g_incrementoEnem = 0; 
            }
            if (g_posxEnem <= g_posxBala && g_balaActiva)
            {
                ocultaEnemigos();

                g_posxBala = 0;
                g_balaActiva = false;
            }

            if (g_enemActivo == true && g_posxEnem <= g_posxPersonaje)
            {
                ocultaEnemigos();
                muerte();
            }
        }
        else {

			if (g_incrementoEnem >= 1)	{ g_posxEnem += 1;			g_incrementoEnem = 0; }
            if (g_posxEnem >= g_posxBala && g_balaActiva)
            {
                ocultaEnemigos();

                g_posxBala = 0;
                g_balaActiva = false;
            }

            if (g_enemActivo == true && g_posxEnem >= g_posxPersonaje)
            {
                ocultaEnemigos();
                muerte();
            }
        }

	}

    return false;
}

bool draw()
{

	for (uint x = 0; x < ANCHO_MUNDO; x++){
		g_piso[x] = '_';
	}
    for (uint x = 0; x < ANCHO_MUNDO; x++){
		if (x == g_posxPersonaje)
            g_piso[x] = g_personaje_actual;
		if (g_balaActiva) {
			if (x == g_posxBala)
					g_piso[x] = g_personaje_bala;
		}
		if (g_enemActivo) {
			if (x == g_posxEnem)
				g_piso[x] = g_personajeEnem;
			}
    }
	printf("%s", g_piso);
    printf("\r"); 

    return false;
}

void muerte()
{
    g_muerte = true;
    g_posxPersonaje = ANCHO_MUNDO / 2;
    cleanWorld();
}

void cleanWorld()
{
    char *clean = new char[ANCHO_MUNDO];
    for (int x = 0; x < ANCHO_MUNDO; x++){
        clean[x] = ' ';
    }
    clean[ANCHO_MUNDO] = '\0';
    printf("%s", clean);
    printf("\r");
}

void ocultaEnemigos()
{
    g_posxEnem = 0;
    g_enemActivo = false;
    g_randomSet = false;
    g_random = 0;
    g_cuentaParaEnemigo = 0;

    g_posxBala = 0;
    g_balaActiva = false;

}


