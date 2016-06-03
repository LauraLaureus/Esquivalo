#include <stdio.h>
#include <GLUT/GLUT.h>
#include <stdlib.h>
#include <random>
#include "LaserManager.hpp"

// Espacio para las variables globales de la ventana
std::default_random_engine generador;
std::normal_distribution<double> aleatorio(1.3, 2.0);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 49);
std::uniform_int_distribution<> disY(40, 150);
std::normal_distribution<> disColor(0.8, 1.0);
std::normal_distribution<> pulse(0.1, 0.3);

float gl_ancho = 100.0, gl_alto = 100.0, gl_cerca = 0.0, gl_lejos = 5.0; //cambiar según el proyecto
int w_ancho = 500, w_alto = 500;
int nBolas = 10;
float bolas[10][2];
float colorBolas[10][3];
int pasos_segundo = 20;
float naveX = 0.0f;
float naveY = -47.0f;
float score = 0;

// Espacio para la declaración de funciones
void InitGlew();
void InitGL();
void Display();
void ReshapeSize(int ancho, int alto);
void Timer(int v);
void Teclas(int key, int x, int y);


void reset(){
    for (int i = 0; i < nBolas; i++){
        bolas[i][1] = disY(gen);
        bolas[i][0] = dis(gen);
        colorBolas[i][0] = disColor(gen);
        colorBolas[i][1] = disColor(gen);
        colorBolas[i][2] = disColor(gen);
        if (rand() % 2 > 0){
            bolas[i][0] = bolas[i][0] * -1;
        }
    }
}

//main
int main(int argc, char *argv[]){
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(w_ancho, w_alto);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutCreateWindow("OpenGL");
    InitGL();
    glutDisplayFunc(Display);
    glutReshapeFunc(ReshapeSize);
    glutTimerFunc(100, Timer, 0);
    glutSpecialFunc(Teclas);
    reset();
    if(!LaserManager::open()){
        exit(-1);
    }
    LaserManager::setWorkSpaceVertex(1000, 2000, 1000, 50);
    glutMainLoop();
    return 0;
}




void Timer(int v){
    score += 0.1;
    PlanePoint p = LaserManager::readLaser();
    
    
    int posX =  (p.x * 100)/2000;
    int posY = (p.y * 100)/2000;
    
     posX -= 0;
     posY -= 50;
    
     printf("%d , %d : %d \n", posX, posY, v);
    
    naveX = posX;
    naveX *= -1;
    
    naveY = posY;
    naveY *= -1;
    
    
    for (int i = 0; i < nBolas; i++){
        float rand = aleatorio(generador);
        bolas[i][1] = bolas[i][1] - rand;
        if (bolas[i][1] < -50){
            bolas[i][1] = disY(gen);
            bolas[i][0] = dis(gen);
            if (dis(gen) < 25){
                bolas[i][0] = bolas[i][0] * -1;
            }
        }
    }
    
    
    
   
    
    glutPostRedisplay();
    glutTimerFunc(1000 / pasos_segundo, Timer, v+1);
}

void InitGL(){
    
    if (gl_ancho / w_ancho != gl_alto / w_alto){
        fprintf(stderr, "La relación de aspecto no es correcta\n");
    }
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-gl_ancho, gl_ancho, -gl_alto, gl_alto, gl_cerca, gl_lejos);
    glMatrixMode(GL_MODELVIEW);
}

void ReshapeSize(int ancho, int alto){
    
    float dx = gl_ancho;
    float dy = gl_alto;
    
    dx *= (float)ancho / (float)w_ancho;
    dy *= (float)alto / (float)w_alto;
    
    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-dx / 2.0, dx / 2.0, -dy / 2.0, dy / 2.0, gl_cerca, gl_lejos);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

/*void DisplayResourceNAMessageBox()
{
    char buffer[50];
    sprintf_l(buffer, "SCORE =  %f\n Try again?", score);
    int msgboxID = MessageBox(
                              NULL,
                              buffer,
                              "GAME OVER!!!",
                              MB_ICONWARNING | MB_RETRYCANCEL | MB_DEFBUTTON2
                              );
    
    switch (msgboxID){
        case IDCANCEL:
            exit(1);
            break;
        case IDRETRY:
            score = 0;
            naveX = 0.0f;
            naveY = -47.0f;
            reset();
            break;
    }
}*/


void Display(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(naveX, naveY, -2.5f);
    glutWireTorus(1.0f, 4.0f, 20, 20);
    glFlush();
    
    float Xs = naveX + 6;
    float Ys = naveY + 6;
    float Xi = naveX - 6;
    float Yi = naveY - 6;
    
    for (int i = 0; i < nBolas; i++){
        glLoadIdentity();
        glColor3f(colorBolas[i][0], colorBolas[i][1], colorBolas[i][2]);
        glTranslatef(bolas[i][0], bolas[i][1], -2.5f);
        glutWireSphere(1.7f + pulse(gen) , 20, 20);
        glFlush();
        
        if (bolas[i][0] + 2.0 > Xi && bolas[i][0] + 2.0<Xs && bolas[i][1] + 2.0>Yi && bolas[i][1] + 2.0 < Ys){
            //DisplayResourceNAMessageBox();
            exit(1);
        }
    }
    
    
    
    glutSwapBuffers();
}

void Teclas(int key, int x, int y){
    switch (key){
        case GLUT_KEY_LEFT:
            if (naveX - 2 > -48)
                naveX -= 2;
            break;
        case GLUT_KEY_RIGHT:
            if (naveX + 2 < 48)
                naveX += 2;
            break;
        case GLUT_KEY_UP:
            if (naveY + 2 < 48)
                naveY += 2;
            break;
        case GLUT_KEY_DOWN:
            if (naveY - 2 > -48)
                naveY -= 2;
            break;
        default: return;
    }
    glutPostRedisplay();
}



