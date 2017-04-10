/******************************************************************************/
/**                                                    ,---------------.     **/
/**    COMPUTACAO GRAFICA                              |        .-.    |     **/
/**    MIEEC - FCTUC                                   |        !o|    |     **/
/**                                                    |>-,   ,-' | ,-<|     **/
/**    AUTORES: - André Filipe Gonçalves Fonseca       |  `---'   `-'  |     **/
/**             - Alexandre Filipe Marcela Martins     `---------------'     **/
/******************************************************************************/
/* DESCRICAO: Projeto nº1 - Dashboard de Gestão Escolar                       */
/*            Funcionamento a 100%                                            */
/******************************************************************************/
// 
//	Bibliotecas a incluir //////////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
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

//graph positions
int linex = 400;
int liney = 400;
int centerx = 1050;
int centery = 500;
int radius = 70;
int colx = 400;
int coly = 100;

float ang1, ang2, ang3;

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
float luz[12];
float max_luz, max_agua, max_gas;
float max_cheias, max_vazias, max_ponta;
float sumT, sumC, sumV, sumP;

//vetores com valores presentes nos dados

//contadores para funções
int i, j;

char buff[50];

//
//	Funções ////////////////////////////////////////////////////////////////////
//

//	Leitura e processamento do ficheiro de dados ///////////////////////////////
void leituraFicheiros() {
	if ((dados = fopen("dados.txt", "r+")) == NULL) {
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

	//obtenção dos dados gerais da escola
	//os valores serão armazendados num ficheiro para o propósito, caso não exista
	//é inicializado com valores todos a 0!
	if ((dadosGerais = fopen("info_geral.txt", "r+")) == NULL) {
		for (i = 0; i < 10; i++) {
			tabelaGerais[i] = 0.0;
		}

		dadosGerais = fopen("info_geral.txt", "w+");
		for (i = 0; i < 10; i++) {
			fprintf(dadosGerais, "%f", tabelaGerais[i]);
			fprintf(dadosGerais, "%s", " ");
		}
	}
	else {
		for (i = 0; i < 10; i++) {
			fscanf(dadosGerais, "%f", &tabelaGerais[i]);
		}
	}
	fclose(dadosGerais);

	orcamentoMensal = tabelaGerais[0];
	horasFuncionamento = tabelaGerais[1];
	custoAgua = tabelaGerais[2];
	custoGas = tabelaGerais[3];
	custoEleticidadeCheio = tabelaGerais[4];
	custoEleticidadePonta = tabelaGerais[5];
	custoEleticidadeVazio = tabelaGerais[6];
	numAlunos = tabelaGerais[7];
	numStaff = tabelaGerais[8];
	totOrdenados = tabelaGerais[9];

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

//	Edição de dados, quer nos vetores criados como ficheiros ///////////////////

//Toda esta edição é realizada em terminal (a dash é apenas para apresentar
//valores consomados e finais
void editaValores() { //edição de valores gerais, definidos pelo utilizador
	int opcao;
	char repetir;

	ShowWindow(GetConsoleWindow(), SW_RESTORE);
	
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
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao-1]);
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

		break;
	case 2:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 3:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 4:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 5:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 6:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 7:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 8:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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
		
		break;
	case 9:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 10:
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
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

		break;
	case 11:
		break;
	}

	//atualização do ficheiro
	dadosGerais = fopen("info_geral.txt", "w+");
	for (i = 0; i < 10; i++) {
		fprintf(dadosGerais, "%f", tabelaGerais[i]);
		fprintf(dadosGerais, "%s", " ");
	}
	printf("\nFicheiro atualizado com sucesso!");
	fclose(dadosGerais);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

//funciona de modo análogo com a anterior função, com a diferença que,
//como sabemos que o ficheiro dados.txt existe, caso contrário o programa não 
//iniciava, não temos de controlar a situação de não existência de ficheiro
//e posterior inicialização
void editaDados() {
	FILE *entrada, *saida;
	int m, atualizador;
	float novoAguaMes, novoGasMes, novoCheiaMes, novoPontaMes, novoVazioMes;
	char repetir;

	ShowWindow(GetConsoleWindow(), SW_RESTORE);

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
	while (atualizador < 1 || atualizador > 6) {
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

		aguaMes[m - 1] = novoAguaMes;
		break;
	case 2:
		printf("Valor atual = %f \nNovo: ", tabelaDados[m - 1][1]);
		scanf("%f", &novoGasMes);

		while (novoGasMes < 0) {
			printf("Valor invalido! Tente de novo: ");
			scanf("%f", &novoGasMes);
		}

		tabelaDados[m - 1][1] = novoGasMes;
		printf("Valor atualizado com sucesso! Deseja repetir? (s/n)");
		scanf(" %c", &repetir);

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}

		gasMes[m - 1] = novoGasMes;
		break;
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

		cheiasMes[m - 1] = novoCheiaMes;
		break;
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

		pontaMes[m - 1] = novoPontaMes;
		break;
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
		vaziasMes[m - 1] = novoVazioMes;
		break;
	case 6:
		break;
	}

	//atualização do ficheiro
	saida = fopen("dados.txt", "w");
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fprintf(saida, "%f", tabelaDados[i][j]);
			fprintf(saida, "%s", " ");
		}
		fprintf(saida, "%s", "\n");
	}
	printf("\nFicheiro atualizado com sucesso!");
	fclose(saida);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

//chamadas da função consoante a escolha do utilizador do menu
//que surge com o pressionar da tecla direita do rato
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

void drawTextC(char *s, int x, int y) {
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
	glLineWidth(1);
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
	switch (active)
	{
	case 1://Dados Gerais
		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Numero de alunos:", 350, 100);
		sprintf(buff, "%d", (int)numAlunos);
		drawText(buff, 510, 100);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Numero de funcionarios:", 350, 150);
		sprintf(buff, "%d", (int)numStaff);
		drawText(buff, 550, 150);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Horas de funcionamento:", 350, 200);
		sprintf(buff, "%d h", (int)horasFuncionamento);
		drawText(buff, 560, 200);

		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2i(800, 50);
		glVertex2i(1100, 50);
		glVertex2i(1100, 350);
		glVertex2i(800, 350);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2i(600, 100);
		glVertex2i(800, 50);
		glVertex2i(800, 250);
		glVertex2i(600, 300);
		glEnd();
		glColor3f(0.9, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2i(600, 300);
		glVertex2i(800, 250);
		glVertex2i(800, 295);
		glVertex2i(675, 325);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2i(800, 350);
		glVertex2i(1100, 350);
		glVertex2i(950, 500);
		glEnd();
		glColor3f(0.4, 0.4, 1);
		glBegin(GL_POLYGON);
		glVertex2i(650, 175);
		glVertex2i(750, 150);
		glVertex2i(750, 225);
		glVertex2i(650, 250);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2i(850, 250);
		glVertex2i(1050, 250);
		glVertex2i(1050, 300);
		glVertex2i(850, 300);
		glEnd();
		glColor3f(0.6, 0.2, 0);
		glBegin(GL_POLYGON);
		glVertex2i(925, 50);
		glVertex2i(975, 50);
		glVertex2i(975, 125);
		glVertex2i(925, 125);
		glEnd();
		break;
	case 2://Consumos
		glColor3f(0,0,0);
		glLineWidth(2);
		glPointSize(5);
		/****************************************************/
		/****************Gráfico de linhas*******************/
		/****************************************************/
		glBegin(GL_LINE_STRIP);
		glVertex2i(linex,liney);//origem do gráfico
		glVertex2i(linex,liney+150);
		glVertex2i(linex,liney);
		glVertex2i(linex+470,liney);
		glEnd();
		max_luz = max_agua = max_gas = 0;
		for (i = 0; i < 12; i++) {
			luz[i] = cheiasMes[i] + vaziasMes[i] + pontaMes[i];
			if (luz[i] > max_luz)
				max_luz = luz[i];
			if (gasMes[i] > max_gas)
				max_gas = gasMes[i];
			if (aguaMes[i] > max_agua)
				max_agua = aguaMes[i];
		}
		//lines
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (luz[i] / max_luz) * 125 + liney);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (gasMes[i] / max_gas) * 125 + liney);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (aguaMes[i] / max_agua) * 125 + liney);
		}
		glEnd();
		
		//points
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (luz[i] / max_luz) * 125 + liney);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (gasMes[i] / max_gas) * 125 + liney);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (aguaMes[i] / max_agua) * 125 + liney);
		}
		glEnd();
		/****************************************************/
		/****************Gráfico de barras*******************/
		/****************************************************/
		glColor3f(1, 0, 0);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + i * 40, coly);
			glVertex2f(colx + i * 40 + 10, coly);
			glVertex2f(colx + i * 40 + 10, coly + (luz[i] / max_luz) * 125);
			glVertex2f(colx + i * 40, coly + (luz[i] / max_luz) * 125);
			glEnd();
		}
		glColor3f(0, 1, 0);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + 10 + i * 40, coly);
			glVertex2f(colx + 10 + i * 40 + 10, coly);
			glVertex2f(colx + 10 + i * 40 + 10, coly + (gasMes[i] / max_gas) * 125);
			glVertex2f(colx + 10 + i * 40, coly + (gasMes[i] / max_gas) * 125);
			glEnd();
		}
		glColor3f(0,0,1);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + 20 + i * 40, coly);
			glVertex2f(colx + 20 + i * 40 + 10, coly);
			glVertex2f(colx + 20 + i * 40 + 10, coly + (aguaMes[i] / max_agua) * 125);
			glVertex2f(colx + 20 + i * 40, coly + (aguaMes[i] / max_agua) * 125);
			glEnd();
		}
		//legenda
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2f(450, 50);
		glVertex2f(460, 50);
		glVertex2f(460, 60);
		glVertex2f(450, 60);
		glEnd();
		drawText("Luz", 470, 550);

		glColor3f(0, 1, 0);
		glBegin(GL_POLYGON);
		glVertex2f(530, 50);
		glVertex2f(540, 50);
		glVertex2f(540, 60);
		glVertex2f(530, 60);
		glEnd();
		drawText("Gas", 550, 550);

		glColor3f(0, 0, 1);
		glBegin(GL_POLYGON);
		glVertex2f(600, 50);
		glVertex2f(610, 50);
		glVertex2f(610, 60);
		glVertex2f(600, 60);
		glEnd();
		drawText("Agua", 620, 550);

		/****************************************************/
		/*****************Gráfico circular*******************/
		/****************************************************/
		sumT = sumC = sumV = sumP = 0;
		for (i = 0; i < 12; i++) {
			sumT += cheiasMes[i] + vaziasMes[i] + pontaMes[i];
			sumC += cheiasMes[i];
			sumV += vaziasMes[i];
			sumP += pontaMes[i];
		}
		ang1 = (sumC / sumT) * 2 * PI;
		ang2 = (sumV / sumT) * 2 * PI;
		ang3 = (sumP / sumT) * 2 * PI;

		float n;
		int x, y;

		//CRIA_FATIA_VERMELHA
		glBegin(GL_POLYGON);
		glColor3f(1.0, 0.2, 0.2);
		for (n = ang1 -0.005; n <= ang1 + ang2; n += 0.005)
		{
			x = cos(n) * radius + centerx;
			y = sin(n) * radius + centery;
			glVertex2f(x, y);
		}
		glVertex2f(centerx, centery);
		glEnd();
		//FIM CRIA_FATIA_VERMELHA

		//CRIA_FATIA_AZUL
		glBegin(GL_POLYGON);
		glColor3f(0.2, 0.2, 1);
		for (n = -0.005; n <= ang1; n += 0.005)
		{
			x = cos(n) * radius + centerx;
			y = sin(n) * radius + centery;
			glVertex2f(x, y);
		}
		glVertex2f(centerx, centery);
		glEnd();
		//FIM CRIA_FATIA_AZUL

		//CRIA_FATIA_VERDE
		glBegin(GL_POLYGON);
		glColor3f(0.2, 1, 0.2);

		for (n = ang1 + ang2 - 0.005; n <= ang1 + ang2 + ang3; n += 0.005)
		{
			x = cos(n) * radius + centerx;
			y = sin(n) * radius + centery;
			glVertex2f(x, y);
		}
		glVertex2f(centerx, centery);
		glEnd();
		//FIM CRIA_FATIA_VERDE
		//legenda
		glColor3f(1.0, 0.2, 0.2);
		glBegin(GL_POLYGON);
		glVertex2f(950, 375);
		glVertex2f(960, 375);
		glVertex2f(960, 385);
		glVertex2f(950, 385);
		glEnd();
		float per = (sumV / sumT) * 100;
		sprintf(buff, "Horas Vazias: %d %c", (int)per, '%');
		drawText(buff, 970, 225);
		glColor3f(0.2, 0.2, 1);
		glBegin(GL_POLYGON);
		glVertex2f(950, 350);
		glVertex2f(960, 350);
		glVertex2f(960, 360);
		glVertex2f(950, 360);
		glEnd();
		per = (sumC / sumT) * 100;
		sprintf(buff, "Horas Cheias: %d %c", (int)per, '%');
		drawText(buff, 970, 250);
		glColor3f(0.2, 1, 0.2);
		glBegin(GL_POLYGON);
		glVertex2f(950, 325);
		glVertex2f(960, 325);
		glVertex2f(960, 335);
		glVertex2f(950, 335);
		glEnd();
		per = (sumP / sumT) * 100;
		sprintf(buff, "Horas Ponta: %d %c", (int)per, '%');
		drawText(buff, 970, 275);
		break;
	case 3://Gastos

		break;
	default:
		break;
	}
	// Desencadeia a geração da imagem (rendering)
	glFlush();
}

////////////////////////////////////////////////////////////////////////////////
//	Programa Principal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
	//Lê ficheiros -  se não existir na diretoria o programa não arranca de todo.
	leituraFicheiros();

	// Inicializa o GLUT
	glutInit(&argc, argv);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

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

	//menu com tecla direita do rato para efetuar edições de dados 
	int menu_id = glutCreateMenu(menuEdicao);
	glutAddMenuEntry("Editar Valores Gerais (terminal)", 1);
	glutAddMenuEntry("Editar Valores do Ficheiro de Dados (terminal)", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Esperando por eventos
	glutMainLoop();

	return EXIT_SUCCESS;
}