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
#include <windows.h>
#include <time.h>
#include <glut.h>
#include <glu.h>
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

FILE *dados;
FILE *logFile;
FILE *dadosGerais;

//dados presentes no ficheiro de dados (por coluna -> cada linha representa um mês)
//sobre os gastos, no total
float totAgua = 0.0; //metros cubicos
float totGas = 0.0; //metros cubicos
float totCheias = 0.0; //kwH
float totPonta = 0.0; //kwH
float totVazias = 0.0; //kwH
float totalElec = 0.0 ; //kwH -> soma das três anteriores

//definidos pelo utilizador - para efeitos de cálculo
float numAlunos = 0.0; //alunos a frequentar ativamente a escola
float numStaff = 0.0; //professores, funcionários e seguranças a exercer funções
float orcamentoMensal = 0.0; //euros
float horasFuncionamento = 0.0; //período horário
float custoAgua = 0.0; //preço/metro cubico
float custoGas = 0.0; //preço/metro cubico
float custoEleticidadeCheio = 0.0; //preço/kwH
float custoEleticidadePonta = 0.0; //preço/kwH
float custoEleticidadeVazio = 0.0; //preço/kwH
float totOrdenados = 0.0; //euros -> à partida valor fixo

//tratamento/edição de dados
float tabelaDados[12][5]; //12 linhas => meses por 7 colunas => valores de sobra a escola + valores de gastos
float tabelaGerais[10];

float aguaMes[12];
float gasMes[12];
float cheiasMes[12];
float pontaMes[12];
float vaziasMes[12];

//vetores com valores presentes nos dados

//contadores para funções
int i, j;

//
//	Funções ////////////////////////////////////////////////////////////////////
//

//	Leitura e processamento do ficheiro de dados ////////////////////////////////
void leituraFicheiro(char *file) {
	if ((dados = fopen(file, "r+")) == NULL) {
		printf("Erro: ficheiro inexistente na diretoria!\nPrima <ENTER> para sair.");
		getchar();
		exit(0);
	}
	else {
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);

		printf("Leitura efetuada com sucesso! \nData: %s", asctime(tm));

		if ((logFile = fopen("logs.txt", "a")) == NULL) {
			logFile = fopen("logs.txt", "wb+");
		}
		fprintf(logFile, "Leitura efetuada em: %s", asctime(tm));

		fclose(logFile);
	}

	//obtenção do total de gastos e vetores com os dados (para gráficos)
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fscanf(dados, "%f", &tabelaDados[i][j]);

			if (j == 0) {
				totAgua = totAgua + tabelaDados[i][0];
				aguaMes[i] = tabelaDados[i][0];
			}
			if (j == 1) {
				totGas = totGas + tabelaDados[i][1];
				gasMes[i] = tabelaDados[i][1];
			}
			if (j == 2) {
				totCheias = totCheias + tabelaDados[i][2];
				cheiasMes[i] = tabelaDados[i][2];
			}
			if (j == 3) {
				totPonta = totPonta + tabelaDados[i][3];
				pontaMes[i] = tabelaDados[i][3];
			}
			if (j == 4) {
				totVazias = totVazias + tabelaDados[i][3];
				vaziasMes[i] = tabelaDados[i][4];
			}
		}
	}
	totalElec = totCheias + totPonta + totVazias;

	fclose(dados);
}

void editaValores() {
	int opcao;
	char repetir;
	
	if ((dadosGerais = fopen("info_geral.txt", "r+")) == NULL) {
		dadosGerais = fopen("info_geral.txt", "w+");
		for (i = 0; i < 10; i++) {
			tabelaGerais[i] = 0.0;

			fprintf(dadosGerais, "%.2f", tabelaGerais[i]);
			fprintf(dadosGerais, "%s", " ");
		}
		fclose(dadosGerais);
	}
	else {
		for (i = 0; i < 10; i++) {
			fscanf(dadosGerais, "%.2f", tabelaGerais[i]);
		}
		fclose(dadosGerais);
	}
	
	system("cls");
	printf("Que valores deseja editar: \nOrcamento (1); \nNumero de horas de funcionamento (2); ");
	printf("\nCusto da Agua/m3 (3); \nCusto do Gas/m3 (4)");
	printf("\nCusto de horario cheias/KwH (5); \nCusto de horario ponta/KwH (6); \nCusto de horario vazio/KwH (7);");
	printf("\nNumero de alunos (8); \nNumero de membros do staff (9); \nTotal anual de vencimentos (10); \nSair (11)\n");
	scanf("%d", &opcao);
	while (opcao < 1 || opcao > 11) {
		printf("\nValor invalido! Tente de novo:");
		scanf("%d", &opcao);
	}

	switch (opcao) {
	case 1:
		printf("\nValor atual = %f \nNovo: ", orcamentoMensal);
		scanf("%f", &orcamentoMensal);
		while (orcamentoMensal < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &orcamentoMensal);
		}
		tabelaGerais[opcao - 1] = orcamentoMensal;

		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 2:
		printf("\nValor atual = %f \nNovo: ", horasFuncionamento);
		scanf("%f", &horasFuncionamento);
		while (horasFuncionamento < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &horasFuncionamento);
		}
		tabelaGerais[opcao - 1] = horasFuncionamento;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 3:
		printf("\nValor atual = %f \nNovo: ", custoAgua);
		scanf("%f", &custoAgua);
		while (custoAgua < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &custoAgua);
		}
		tabelaGerais[opcao - 1] = custoAgua;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 4:
		printf("\nValor atual = %f \nNovo: ", custoGas);
		scanf("%f", &custoGas);
		while (custoGas < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &custoGas);
		}
		tabelaGerais[opcao - 1] = custoGas;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 5:
		printf("\nValor atual = %f \nNovo: ", custoEleticidadeCheio);
		scanf("%f", &custoEleticidadeCheio);
		while (custoEleticidadeCheio < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &custoEleticidadeCheio);
		}
		tabelaGerais[opcao - 1] = custoEleticidadeCheio;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 6:
		printf("\nValor atual = %f \nNovo: ", custoEleticidadePonta);
		scanf("%f", &custoEleticidadePonta);
		while (custoEleticidadePonta < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &custoEleticidadePonta);
		}
		tabelaGerais[opcao - 1] = custoEleticidadePonta;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 7:
		printf("\nValor atual = %f \nNovo: ", custoEleticidadeVazio);
		scanf("%f", &custoEleticidadeVazio);
		while (custoEleticidadeVazio < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &custoEleticidadeVazio);
		}
		tabelaGerais[opcao - 1] = custoEleticidadeVazio;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 8:
		printf("\nValor atual = %f \nNovo: ", numAlunos);
		scanf("%f", &numAlunos);
		while (numAlunos < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &numAlunos);
		}
		tabelaGerais[opcao - 1] = numAlunos;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 9:
		printf("\nValor atual = %f \nNovo: ", numStaff);
		scanf("%f", &numStaff);
		while (numStaff < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &numStaff);
		}
		tabelaGerais[opcao - 1] = numStaff;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 10:
		printf("\nValor atual = %f \nNovo: ", totOrdenados);
		scanf("%f", &totOrdenados);
		while (totOrdenados < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &totOrdenados);
		}
		tabelaGerais[opcao - 1] = totOrdenados;
		
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaValores();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 11:
		break;
	}

	dadosGerais = fopen("info_geral.txt", "w+");
	for (i = 0; i < 10; i++) {
		fprintf(dadosGerais, "%f", tabelaGerais[i]);
		fprintf(dadosGerais, "%s", " ");
	}
	printf("Ficheiro atualizado com sucesso!");
	fclose(dadosGerais);
}

void editaDados() {
	FILE *entrada, *saida;
	int m, atualizador;
	float novoAguaMes, novoGasMes, novoCheiaMes, novoPontaMes, novoVazioMes;
	char repetir;

	entrada = fopen("dados.txt", "r+");
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fscanf(entrada, "%f", &tabelaDados[i][j]);
		}
	}
	fclose(entrada);

	system("cls");
	printf("Em que mes (em valor numerico) pretende alterar dados: ");
	scanf("%d", &m);
	while (m < 1 || m > 12) {
		printf("Valor invalido! Tente de novo: ");
		scanf("%d", &m);
	}
	
	system("cls");
	printf("Que valor pretende atualizar: \nAgua (1); \nGas (2); \nConsumo Cheio (3); \nConsumo Ponta (4); \nConsumo Vazio (5);\nSair (6); \n");
	scanf("%d", &atualizador);
	while (atualizador < 1 || atualizador > 5) {
		printf("Valor invalido! Tente de novo: ");
		scanf("%d", &atualizador);
	}

	switch (atualizador) {
	case 1:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][0]);
		scanf("%f", &novoAguaMes);

		while (novoAguaMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoAguaMes);
		}

		tabelaDados[m - 1][0] = novoAguaMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 2:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][1]);
		scanf("%f", &novoGasMes);

		while (novoGasMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoGasMes);
		}

		tabelaDados[m - 1][1] = novoAguaMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 3:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][2]);
		scanf("%f", &novoCheiaMes);

		while (novoCheiaMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoCheiaMes);
		}

		tabelaDados[m - 1][3] = novoCheiaMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 4:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][3]);
		scanf("%f", &novoPontaMes);

		while (novoPontaMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoPontaMes);
		}

		tabelaDados[m - 1][3] = novoPontaMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 5:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][4]);
		scanf("%f", &novoVazioMes);

		while (novoVazioMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoVazioMes);
		}

		tabelaDados[m - 1][4] = novoVazioMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
	case 6:
		break;
	}

	saida = fopen("dados.txt", "w");
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fprintf(saida, "%f", tabelaDados[i][j]);
			fprintf(saida, "%s", " ");
		}
		fprintf(saida, "%s", "\n");
	}
	printf("Ficheiro atualizado com sucesso!");
	fclose(saida);
}

void menuEdicao(int value) {
	if (value == 1) {
		editaValores();
	}
	if (value == 2) {
		editaDados();
	}
}

//	Funções de desenho /////////////////////////////////////////////////////////

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
	//Lê ficheiros -  se não existir na diretoria o programa não arranca de todo.
	leituraFicheiro("dados.txt");

	// Inicializa o GLUT
	glutInit(&argc, argv);

	//#pragma
	//FreeConsole();

	glutInitWindowSize(winw, winh);
	glutInitWindowPosition(100, 100);

	// Cria a janela GLUT de visualização
	glutCreateWindow("GestEscola");

	init();

	// Obrigatório registar uma "callback function", neste caso de visualização
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardControl);
	glutMouseFunc(MouseButton);
	glutPassiveMotionFunc(MouseMotion);
	glutTimerFunc(0, menuAnim, 0);

	//submenus para edição
	int menu_id = glutCreateMenu(menuEdicao);
	glutAddMenuEntry("Editar Valores Gerais (terminal)", 1);
	glutAddMenuEntry("Editar Valores do Ficheiro de Dados (terminal)", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Esperando por eventos
	glutMainLoop();

	return EXIT_SUCCESS;
}