/******************************************************************************/
/**                                                    ,---------------.     **/
/**    COMPUTACAO GRAFICA                              |        .-.    |     **/
/**    MIEEC - FCTUC                                   |        !o|    |     **/
/**                                                    |>-,   ,-' | ,-<|     **/
/**    AUTORES: - ...................                  |  `---'   `-'  |     **/
/**             - ...................                  `---------------'     **/
/******************************************************************************/
/* DESCRICAO: Texto que devera descrever os objectivos e os resultados .....  */
/*            do programa. .................................................  */
/*		      ..............................................................  */
/******************************************************************************/
// 
//	Bibliotecas a incluir //////////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <GL/glut.h>
//
//	Definições /////////////////////////////////////////////////////////////////
//
// Exemplo da definição de uma constante
#define PI    3.1415927

//Size of the window
int winw = 1200;
int winh = 600;
//Size view
int view = 900;
int vieh = 600;
//Size menu
int menuw = 300;
int menuh = 600;

GLboolean hide;

int hover=0;
int active=1;
int button[3][2] = { {75,110}, {125,160}, {175,210} };

//
//	Funções ////////////////////////////////////////////////////////////////////
//
void drawMenu();

void init(void) {
	// Clear the window
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winw, 0, winh);
	/*glutCreateMenu(myMenu);
	glutAddMenuEntry("Poligono Regular", 2);
	glutAddMenuEntry("Poligono Irregular", 3);
	glutAddMenuEntry("Circulo", 1);
	glutAddMenuEntry("Limpar Ecra", 4);
	glutAddMenuEntry("Ajuda", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);*/

	hide = 0;
	drawMenu();
}

void menuAnim() {
	if (hide && menuw > 0)
		menuw -= 2;
	if (!hide && menuw < 300)
		menuw += 2;

	glutPostRedisplay();
	glutTimerFunc(10, menuAnim, 0);
}

void keyboardControl(char key, int x, int y) {
	if (key == 'Q' || key == 'q' || key == 27)
		exit(0);
}

void MouseButton(int button, int state, int x, int y) {
	//update the mouse position real position
	//y = winh - y;

	//where was the press
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		if (x > 10 && x < 30 && y > 10 && y < 22) {
			hide = !hide;
		}
		else if (!hide && x < menuw) {
			if (hover != 0) {
				active = hover;
			}
			printf("op %d\n", active);
		}
	}
	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	if (!hide) {
		if (x < menuw) {
			if (y > 75 && y < 110) {
				hover = 1;
			}
			else if(y > 125 && y < 160) {
				hover = 2;
			}
			else if (y > 175 && y < 210) {
				hover = 3;
			}
			else {
				hover = 0;
			}
		}
		else {
			hover = 0;
		}
	}
	glutPostRedisplay();
}

void drawText(char *s, int x, int y) {
	glColor3f(0, 0, 0);
	glRasterPos2i(x, menuh - y);
	for (int c = 0; c <= strlen(s); c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[c]);
	}
}

void menuButton(){
	glBegin(GL_POLYGON);
	glVertex2i(10, menuh - 10);
	glVertex2i(10, menuh - 12);
	glVertex2i(30, menuh - 12);
	glVertex2i(30, menuh - 10);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(10, menuh - 15);
	glVertex2i(10, menuh - 17);
	glVertex2i(30, menuh - 17);
	glVertex2i(30, menuh - 15);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(10, menuh - 20);
	glVertex2i(10, menuh - 22);
	glVertex2i(30, menuh - 22);
	glVertex2i(30, menuh - 20);
	glEnd();
}

void drawMenu() {
	//Fundo Branco
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2i(0, 0);
	glVertex2i(menuw, 0);
	glVertex2i(menuw, 600);
	glVertex2i(0, 600);
	glEnd();

	glColor3f(0, 0, 0);
	menuButton();

	glColor3f(54/255.0, 77/255.0, 226/255.0);
	if (hover != 0) {
		glBegin(GL_POLYGON);
		glVertex2i(0, menuh - button[hover-1][0]);
		glVertex2i(0, menuh - button[hover - 1][1]);
		glVertex2i(menuw, menuh - button[hover - 1][1]);
		glVertex2i(menuw, menuh - button[hover - 1][0]);
		glEnd();
	}

	glColor3f(54 / 255.0, 77 / 255.0, 226 / 255.0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(0, menuh - button[active - 1][0]);
	glVertex2i(0, menuh - button[active - 1][1]);
	glVertex2i(menuw, menuh - button[active - 1][1]);
	glVertex2i(menuw, menuh - button[active - 1][0]);
	glVertex2i(0, menuh - button[active - 1][0]);
	glEnd();

	
	drawText("Dados Gerais", 50, 100);
	drawText("Consumos", 50, 150);
	drawText("Gastos", 50, 200);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	//Menu
	glViewport(0, 0, winw, winh);
	glColor3f(1,1,1);
	menuButton();
	glPushMatrix();
	glTranslatef(menuw - 300, 0, 0);
	drawMenu();
	glPopMatrix();
	//Main view
	glViewport(0, 0, winw, winh);
	// Desencadeia a geração da imagem (rendering)
	glFlush();
}

////////////////////////////////////////////////////////////////////////////////
//	Programa Principal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
	// Inicializa o GLUT
	glutInit(&argc, argv);

	//#pragma
	//FreeConsole();

	glutInitWindowSize(winw, winh);
	glutInitWindowPosition(100, 100);

	// Cria a janela GLUT de visualização
	glutCreateWindow("Gestão de escola");

	init();

	// Obrigatório registar uma "callback function", neste caso de visualização
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardControl);
	glutMouseFunc(MouseButton);
	glutPassiveMotionFunc(MouseMotion);
	glutTimerFunc(0, menuAnim, 0);

	// Esperando por eventos
	glutMainLoop();

	return EXIT_SUCCESS;
}