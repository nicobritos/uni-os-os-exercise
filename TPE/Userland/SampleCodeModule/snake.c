//This is a personal academic project. Dear PVS-Studio, please check it.
//PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include "snake.h"
#include "stdio.h"
#include "stdlib.h"
#include "newSyscalls.h"

void drawString(int x, int y, char * str , unsigned char r, unsigned char g, unsigned char b, unsigned char size);

char fonts[130][8] = {

    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00}, // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00}, // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00}, // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00}, // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00}, // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00}, // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00}, // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00}, // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00}, // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06}, // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00}, // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00}, // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00}, // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00}, // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00}, // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00}, // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00}, // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00}, // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00}, // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00}, // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00}, // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00}, // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00}, // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00}, // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06}, // U+003B (//)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00}, // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00}, // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00}, // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00}, // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00}, // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00}, // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00}, // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00}, // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00}, // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00}, // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00}, // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00}, // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00}, // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00}, // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00}, // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00}, // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00}, // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00}, // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00}, // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00}, // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00}, // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00}, // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00}, // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00}, // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00}, // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}, // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00}, // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00}, // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00}, // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00}, // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00}, // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00}, // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00}, // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}, // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00}, // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00}, // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00}, // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00}, // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00}, // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00}, // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F}, // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00}, // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E}, // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00}, // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00}, // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00}, // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00}, // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F}, // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78}, // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00}, // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00}, // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00}, // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00}, // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00}, // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00}, // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00}, // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F}, // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00}, // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00}, // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00}, // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00}, // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // U+007F
    { 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00}, // U+0080
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} // U+0080
};

typedef struct{
  uint64_t x,y;
  uint64_t ModX, ModY;
}snk;

/*
 * ModX = 1 , ModY = 0 => Avanza a la derecha
 * ModX = 0 , ModY = 1 => Avanza a para abajo
 * ModX = -1, ModY = 0 => Avanza a la izquierda
 * ModX = 0 , ModY = -1 => Avanza a para arriba
 */

static snk snake[MAX_SNAKE];
static int snake_size = INITIAL_SNAKE_SIZE;
static char score[] = "Score: ";
static char start_message[] = "Press any key to start";
static char commands[] = "'w' Up ; 's' Down ; 'a' Left ; 'd' Right";

static uint64_t ticks = 0;
int ticks_to_bigger = 15;
int ticksTillRefresh = 3;
double snake_speed = 1.5;

int start_seconds ;
int play_seconds;

int count = 0;
int totalScore = 0;

void drawPixel(uint64_t x, uint64_t y, uint64_t r ,uint64_t g,uint64_t b) {
  //os_draw(x,y,r,g,b);
  sys_draw(x,y,r,g,b);
  return;
}


void drawWindow(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t r,uint64_t g,uint64_t b) {
  for (int i = x; i < width + x; i++){
    for (int j = y; j < height + y; j++){
      drawPixel(i,j, r,g,b);
    }
  }
}


/* Exactamente igual al drawWindow pero este pinta todo de negro, siempre */
/* Por mas que sea igual, creo que no deberia ser la misma funcion ya que estaria mal que llamemos "drawWindow" cuando queremos borrar algo */
void delete(uint64_t x, uint64_t y, uint64_t width, uint64_t height) {
  for (int i = x; i < width + x; i++){
    for (int j = y; j < height + y; j++){
      drawPixel(i,j, 0,0,0);
    }
  }
}

void printSnake(){
    delete(snake[snake_size - 1].x - (SIZE * snake[snake_size-1].ModX), snake[snake_size - 1].y - (SIZE * snake[snake_size-1].ModY),SIZE,SIZE); //borra ultimo
    drawWindow(snake[0].x , snake[0].y, SIZE, SIZE,0,170,170); //cabeza
    for (int i = 1; i < snake_size ; i++){ //cuerpo
      drawWindow(snake[i - 1].x - (SIZE * snake[i-1].ModX) , snake[i - 1].y - (SIZE * snake[i-1].ModY),SIZE,SIZE,0,(i*40) % 255,170);
    }
}

void window(){

    /* Dejo una linea de SIZE * SIZE libre arriba para el SCORE */
    drawWindow(SIZE, SIZE, RIGHT, SIZE,255,255,255);
    drawWindow(0, BOTTOM - SIZE, RIGHT, SIZE,255,255,255);
    drawWindow(0,SIZE,SIZE,BOTTOM ,255,255,255);
    drawWindow(RIGHT-SIZE,SIZE,SIZE,BOTTOM,255,255,255);

    // drawString(RIGHT/2 - 300,0,score,255,0,0,2);
    drawString((RIGHT - 2 * SIZE)/2 - 200, (BOTTOM - 2 * SIZE) / 2 - 200, start_message,0,255,0, 2);
    drawString((RIGHT - 2 * SIZE)/2 - 200, (BOTTOM - 2 * SIZE) / 2 - 100, commands,0,255,0, 2);

}

void setupSnake(){
  /* La cabeza en coordenadas x , y */
  snake[0].x = ( RIGHT - 2 *SIZE )/2; 
    snake[0].y = ( BOTTOM - 2 * SIZE )/2; 

  /* Seteo para donde va a moverse */
    for(int i = 0; i < snake_size; i++) {
      snake[i].ModX = 1;
      snake[i].ModY = 0;
    }
  
  /* El resto del cuerpo esta SIZE pixeles detras del anterior (20 es el size de cada cuadrado) 
   * En este caso esta atras por que ModX = 1 y ModY = 0, si fueran distintos, va distinto */
    for (int i = 1; i < snake_size; i++){
      snake[i].x = snake[i - 1].x - SIZE;
      snake[i].y = snake[i - 1].y;
    }
  
}

void start(){

  /* vuelvo a poner el snake size en INITIAL
   * por si se juega mas de una vez,
   * al ser estatico queda guardado el anterior 
   * y comenzaria con la ultima longitud que tuvo */
  snake_size = INITIAL_SNAKE_SIZE;

  //Clear the screen 
  //os_clear();
  sys_clear();
  setupSnake();
  window();

}

void gameOver(){
  drawString((int)(RIGHT/2) - 400,(int)(BOTTOM / 2) - 200,"GAME OVER",255,0,0,5);
        drawString((int)(RIGHT/2) - 400,(int)(BOTTOM / 2) - 100,"Score : ",255,0,0,5);
        char buffer[30];
        itoa(totalScore,buffer,10);
        drawString((int)(RIGHT/2) - 50,(int)(BOTTOM / 2) - 100,buffer,255,0,0,5);

}


void incrementSize(){
  if(count==5){
    count=0;
    snake_size++;
    totalScore++;
  }
}

int move(){
  char c;
  while( (c = getCharWithZero()) ){
      count++;
      incrementSize();
      for (int i = snake_size - 1; i > 0 ; i--) {
          snake[i].x = snake[i - 1].x;
          snake[i].y = snake[i - 1].y;
          snake[i].ModX = snake[i - 1].ModX;
          snake[i].ModY = snake[i - 1].ModY;
      }

      switch(c){
        /* Avanza a la izquierda */
        case 'A':     
        case 'a':{ 
          if( snake[0].ModX == 1){
            break;
          }
          snake[0].ModX = -1;
          snake[0].ModY = 0 ;   
        }
        break;
        /* Avanza hacia arriba */ 
        case 'W':
        case 'w':{
          if( snake[0].ModY == 1){
            break;
          }
          snake[0].ModX = 0;
          snake[0].ModY = -1;
        }
        break;
        /* Avanza hacia abajo */ 
        case 'S':
        case 's':{
          if( snake[0].ModY == -1){
            break;
          }
          snake[0].ModX = 0;
          snake[0].ModY = 1;
        }
        break;
        /* Avanza a la derecha */
        case 'D':     
        case 'd':{ 
          if( snake[0].ModX == -1){
            break;
          } 
          snake[0].ModX = 1;
          snake[0].ModY = 0;   
        }
        break;
        default:{
          break;
        }
      }
      snake[0].x += (snake[0].ModX * 20);
      snake[0].y += (snake[0].ModY * 20);

      // if (increaseSize(getseconds())) {
      //   snake_size++;
      // }
      
      printSnake();

      /*   Se fija si murió */
      if (snake[0].x <= SIZE + SIZE/2  || snake[0].x >= RIGHT - (SIZE + SIZE) || snake[0].y <= 2 * SIZE || snake[0].y >= BOTTOM - ( SIZE + SIZE/2 ) ){ 
        gameOver();
        return 1;
      }else{
        for (int i = 1; i < snake_size; i++) {
          if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            gameOver();
            return 1;
          }
        }
      }
    }
    for (int i = snake_size - 1; i > 0 ; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
        snake[i].ModX = snake[i - 1].ModX;
        snake[i].ModY = snake[i - 1].ModY;
    }
    snake[0].x += (snake[0].ModX * 20);
    snake[0].y += (snake[0].ModY * 20);
      
    printSnake();

    if (snake[0].x <= SIZE + SIZE/2  || snake[0].x >= RIGHT - (SIZE + SIZE) || snake[0].y <= 2 * SIZE || snake[0].y >= BOTTOM - ( SIZE + SIZE/2 ) ){ 
      gameOver();
      return 1;
    }else{
      for (int i = 1; i < snake_size; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
          gameOver();
          return 1;
        }
      }
    }
    return 0; // avisa que no murio
}


int increaseSize( int num ){
  if(play_seconds <= num - ticks_to_bigger){
      play_seconds = num;
      return 1;
  }
  return 0;
}


void play(){
  int dead = 0;
  getchar();
  delete((int)(RIGHT - 2 * SIZE)/2 - 200,(int)(BOTTOM - 2 * SIZE) / 2 - 200,SIZE * strlen(start_message),SIZE); // Borra el mensaje cuando aprieta la primer letra
  delete(SIZE,(int)(BOTTOM - 2 * SIZE) / 2 - 100,RIGHT-2*SIZE,SIZE);
  
  // start_seconds = getseconds();
  // play_seconds = getseconds();

  uint64_t auxTicks;

  while(dead == 0){
    sys_ticks(&auxTicks);

    char sec[10];
    // itoa(play_seconds-start_seconds,sec,10);
    
    drawString(RIGHT/2 - 200,0,score,255,0,0,2);
    itoa(totalScore,sec,10);
    drawString(RIGHT/2 - 100,0,sec,255,0,0,2);
    int aux = totalScore;

    if(auxTicks >= (ticks + ticksTillRefresh)) {   
      //ticks = os_ticks();
      ticks = sys_ticks(&auxTicks);
      dead = move();
    }
    if(aux != totalScore){
      delete(RIGHT/2 - 100,0,2*SIZE,SIZE);
    }
  }

}


void showScore(){
    drawString(200,BOTTOM / 2 + 200,"Press enter to continue",255,0,0,3);
    while(getchar() != '\n');
}

int getseconds(){
  int currTime[6];
  sys_time(currTime);

  // printf("%d%d , %d%d, %d%d",currTime[5],currTime[4],currTime[3],currTime[2],currTime[1],currTime[0]);

  int total = (currTime[0] - '0' * 10 + currTime[1] - '0' ) * 3600 + (currTime[2] - '0' * 10 + currTime[3] - '0') * 60 +
        (currTime[4] - '0' * 10 + currTime[5] - '0');
  return total;
}


int snake_game(int argc, char * argv[]){
  start();
  printSnake();

  play();

  showScore();

  //os_clear();
  sys_clear();
  return 0;
}

/* Sacado de internet, no recuerdo que pagina */
void drawChar(int  x, int  y, unsigned char myChar, unsigned char r, unsigned char g, unsigned char b, unsigned char size) {

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (fonts[myChar][i] & (1<<j)){
        for (int k = 1; k <= size; k++){
          for (int l = 1; l <= size; l++){
            drawPixel(x + j * size + l, y + i * size + k, r,g,b);
          }
        }
      }
    }
  }
}

void drawString(int x, int y, char * str , unsigned char r, unsigned char g, unsigned char b, unsigned char size) {
  uint64_t lenght = strlen(str);
  for (int i = 0; i < lenght; i++){
    drawChar(x, y, str[i],r,g,b, size);
    x += 8 * size;
  }
}
