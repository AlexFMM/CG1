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
int linex = 250;
int liney = 300;
int line2x = 500;
int line2y = 200;
int centerx = 1050;
int centery = 500;
int radius = 70;
int colx = 250;
int coly = 100;

float ang1, ang2, ang3;

GLboolean hide;

int hover = 0;
int active = 1;
int button[3][2] = { { 75,110 },{ 125,160 },{ 175,210 } };

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
float totalElec = 0.0; //kwH -> soma das três anteriores

					   //definidos pelo utilizador - para efeitos de cálculo
float numAlunos = 0.0; //alunos a frequentar ativamente a escola
float numStaff = 0.0; //professores, funcionários e seguranças a exercer funções
float orcamentoAnual = 0.0; //euros
float horasFuncionamento = 0.0; //período horário
float custoAgua = 0.0; //preço/metro cubico
float custoGas = 0.0; //preço/metro cubico
float custoEleticidadeCheio = 0.0; //preço/kwH
float custoEleticidadePonta = 0.0; //preço/kwH
float custoEleticidadeVazio = 0.0; //preço/kwH
float totOrdenados = 0.0; //euros -> à partida valor fixo

//consumos por... -> para função de processamento
//consumos anuais -> para dados gerais
float consumoAguaAluno = 0.0;
float consumoAguaHora = 0.0;
float consumoAguaStaff = 0.0;
float consumoGasAluno = 0.0;
float consumoGasHora = 0.0;
float consumoGasStaff = 0.0;
float consumoEletricidadeAluno = 0.0;
float consumoEletricidadeHora = 0.0;
float consumoEletricidadeStaff = 0.0;
float ordenadoMedio = 0.0;
float percentagemAlunos = 0.0;
float percentagemStaff = 0.0;
float balancoContas = 0.0;
float totHorasFuncionamento = 0.0;
float gastosAgua = 0.0;
float gastosGas = 0.0;
float gastosElec = 0.0;
float gastosAguaAluno = 0.0;
float gastosGasAluno = 0.0;
float gastosElecAluno = 0.0;
float gastosAguaStaff = 0.0;
float gastosGasStaff = 0.0;
float gastosElecStaff = 0.0;
float gastosCheio = 0.0;
float gastosPonta = 0.0;
float gastosVazio = 0.0;
float totalGastosAnuais = 0.0;

//consumos manuais -> para gráficos
float gastoEletricidadeMensalTotal[12];
float gastoAguaMensal[12];
float gastoGasMensal[12];
float consumoDiarioAguaMensal[12]; //média mensal, para efeitos de simplificação
float consumoDiarioGasMensal[12]; //média mensal, para efeitos de simplificação
float consumoDiarioEletricidadeMensal[12]; //média mensal, para efeitos de simplificação
float gastoDiarioEletricidadeMensal[12];
float gastoDiarioAguaMensal[12];
float gastoDiarioGasMensal[12];

float aguaMes[12];
float gasMes[12];
float cheiasMes[12];
float pontaMes[12];
float vaziasMes[12];
float luz[12];
float max_luz, max_agua, max_gas, max_gasto;
float max_cheias, max_vazias, max_ponta;
float sumT, sumC, sumV, sumP;

//tratamento/edição de dados
float tabelaDados[12][5]; //12 linhas => meses por 7 colunas => valores de sobra a escola + valores de gastos
float tabelaGerais[10];

//contadores para funções
int i, j;

char buff[255];

//
//	Funções ////////////////////////////////////////////////////////////////////
//

//protótipos de funções a usar
void leituraFicheiros();
void editaDados();
void editaValores();
void processamentoDados();
void menuEdicao(int value);

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

		printf("Login efetuado com sucesso! \nData: %s", asctime(tm));

		if ((logFile = fopen("logs.txt", "a")) == NULL) {
			logFile = fopen("logs.txt", "wb+");
		}
		fprintf(logFile, "Login efetuada em: %s", asctime(tm));

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

	orcamentoAnual = tabelaGerais[0];
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

	processamentoDados();
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
		printf("\nValor atual = %f \nNovo: ", tabelaGerais[opcao - 1]);
		scanf("%f", &orcamentoAnual);
		while (orcamentoAnual < 0.0) {
			printf("\nValor invalido! Tente de novo:");
			scanf("%f", &orcamentoAnual);
		}
		tabelaGerais[opcao - 1] = orcamentoAnual;

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

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//atualização do ficheiro
	dadosGerais = fopen("info_geral.txt", "w+");
	for (i = 0; i < 10; i++) {
		fprintf(dadosGerais, "%f", tabelaGerais[i]);
		fprintf(dadosGerais, "%s", " ");
	}
	printf("\nFicheiro atualizado com sucesso!");
	fclose(dadosGerais);

	processamentoDados();
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

	entrada = fopen("dados.txt", "r+");
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fscanf(entrada, "%f", &tabelaDados[i][j]);
		}
	}
	fclose(entrada);

	ShowWindow(GetConsoleWindow(), SW_RESTORE);

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
		
		aguaMes[m - 1] = novoAguaMes;

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}

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

		gasMes[m - 1] = novoGasMes;

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}

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
		cheiasMes[m - 1] = novoCheiaMes;

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}

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
		pontaMes[m - 1] = novoPontaMes;

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}

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
		vaziasMes[m - 1] = novoVazioMes;

		if (repetir == 's' || repetir == 'S') {
			editaDados();
		}
		else if (repetir == 'n' || repetir == 'N') {
			break;
		}
		break;
	case 6:
		break;
	}

	ShowWindow(GetConsoleWindow(), SW_HIDE);

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

	processamentoDados();
}

void processamentoDados() {
	//percentagens
	percentagemAlunos = numAlunos / (numAlunos + numStaff);
	percentagemStaff = numStaff / (numAlunos + numStaff);

	//ordenados
	ordenadoMedio = totOrdenados / numStaff;

	//horas de funcionamento -> vamos considerar que a escola está aberta 365 dias/ano
	totHorasFuncionamento = 365 * horasFuncionamento;

	//consumos por aluno
	consumoAguaAluno = (totAgua / (numAlunos + numStaff)) * percentagemAlunos;
	consumoGasAluno = (totGas / (numAlunos + numStaff)) * percentagemAlunos;
	consumoEletricidadeAluno = (totalElec / (numAlunos + numStaff)) * percentagemAlunos;

	//consumos por membro de staff
	consumoAguaStaff = (totAgua / (numAlunos + numStaff)) * percentagemStaff;
	consumoGasStaff = (totGas / (numAlunos + numStaff)) * percentagemStaff;
	consumoEletricidadeStaff = (totalElec / (numAlunos + numStaff)) * percentagemStaff;

	//consumos por hora
	consumoAguaHora = totAgua / totHorasFuncionamento;
	consumoGasHora = totGas / totHorasFuncionamento;
	consumoEletricidadeHora = totalElec / totHorasFuncionamento;

	//gastos
	//anual
	gastosAgua = totAgua * custoAgua;
	gastosGas = totGas * custoGas;
	gastosCheio = totCheias * custoEleticidadeCheio;
	gastosPonta = totPonta * custoEleticidadePonta;
	gastosVazio = totVazias * custoEleticidadeVazio;
	gastosElec = gastosCheio + gastosPonta + gastosVazio;

	//aluno
	gastosAguaAluno = gastosAgua * percentagemAlunos;
	gastosGasAluno = gastosGas * percentagemAlunos;
	gastosElecAluno = gastosElec * percentagemAlunos;

	//staff
	gastosAguaStaff = gastosAgua * percentagemStaff;
	gastosGasStaff = gastosGas * percentagemStaff;
	gastosElecStaff = gastosElec * percentagemStaff;

	totalGastosAnuais = gastosAgua + gastosGas + gastosElec + totOrdenados;
	balancoContas = orcamentoAnual - totalGastosAnuais;

	//mensal
	for (int i = 0; i < 12; i++) {
		gastoAguaMensal[i] = custoAgua * aguaMes[i];
		gastoGasMensal[i] = custoGas * gasMes[i];
		gastoEletricidadeMensalTotal[i] = (custoEleticidadeCheio * cheiasMes[i]) + (custoEleticidadePonta * pontaMes[i]) + (custoEleticidadeVazio * vaziasMes[i]);
	}

	//diário (a partir de média mensal) -> assumir média de 30 dias por mês (365 dias / 12 meses ~= 30 dias)
	for (i = 0; i < 12; i++) {
		consumoDiarioAguaMensal[i] = aguaMes[i] / 30;
		consumoDiarioGasMensal[i] = gasMes[i] / 30;
		consumoDiarioEletricidadeMensal[i] = (cheiasMes[i] + pontaMes[i] + vaziasMes[i]) / 30;
	}

	//gasto diário
	for (i = 0; i < 12; i++) {
		gastoDiarioEletricidadeMensal[i] = gastoEletricidadeMensalTotal[i] / 30;
		gastoDiarioAguaMensal[i] = gastoAguaMensal[i] / 30;
		gastoDiarioGasMensal[i] = gastoGasMensal[i] / 30;
	}
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
			else if (y > 125 && y < 160) {
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

void reshape(int w, int h) {
	glutReshapeWindow(winw, winh);
}

void drawText(char *s, int x, int y) {
	glColor3f(0, 0, 0);
	glRasterPos2i(x, menuh - y);
	for (int c = 0; c <= strlen(s); c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[c]);
	}
}

void drawTextS(char *s, int x, int y) {
	glColor3f(0, 0, 0);
	glRasterPos2i(x, menuh - y);
	for (int c = 0; c <= strlen(s); c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, s[c]);
	}
}

void drawTextC(char *s, int x, int y) {
	glRasterPos2i(x, menuh - y);
	for (int c = 0; c <= strlen(s); c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[c]);
	}
}

void menuButton() {
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

	glColor3f(54 / 255.0, 77 / 255.0, 226 / 255.0);
	if (hover != 0) {
		glBegin(GL_POLYGON);
		glVertex2i(0, menuh - button[hover - 1][0]);
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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
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
		drawTextC("Horas de funcionamento diarias:", 350, 200);
		sprintf(buff, "%d h", (int)horasFuncionamento);
		drawText(buff, 620, 200);

		//logo escola
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
		/****************************************************/
		/****************Gráfico de barras*******************/
		/****************************************************/
		glColor3f(0.8, 0.2, 0.2);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + i * 40, coly);
			glVertex2f(colx + i * 40 + 10, coly);
			glVertex2f(colx + i * 40 + 10, coly + (luz[i] / max_luz) * 125);
			glVertex2f(colx + i * 40, coly + (luz[i] / max_luz) * 125);
			glEnd();
		}
		glColor3f(0.2, 0.8, 0.2);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + 10 + i * 40, coly);
			glVertex2f(colx + 10 + i * 40 + 10, coly);
			glVertex2f(colx + 10 + i * 40 + 10, coly + (gasMes[i] / max_gas) * 125);
			glVertex2f(colx + 10 + i * 40, coly + (gasMes[i] / max_gas) * 125);
			glEnd();
		}
		glColor3f(0.2, 0.2, 0.8);
		for (i = 0; i < 12; i++) {
			glBegin(GL_POLYGON);
			glVertex2f(colx + 20 + i * 40, coly);
			glVertex2f(colx + 20 + i * 40 + 10, coly);
			glVertex2f(colx + 20 + i * 40 + 10, coly + (aguaMes[i] / max_agua) * 125);
			glVertex2f(colx + 20 + i * 40, coly + (aguaMes[i] / max_agua) * 125);
			glEnd();
		}
		//legenda
		glColor3f(0.8, 0.2, 0.2);
		glBegin(GL_POLYGON);
		glVertex2f(450, 50);
		glVertex2f(460, 50);
		glVertex2f(460, 60);
		glVertex2f(450, 60);
		glEnd();
		drawText("Luz", 470, 550);

		glColor3f(0.2, 0.8, 0.2);
		glBegin(GL_POLYGON);
		glVertex2f(530, 50);
		glVertex2f(540, 50);
		glVertex2f(540, 60);
		glVertex2f(530, 60);
		glEnd();
		drawText("Gas", 550, 550);

		glColor3f(0.2, 0.2, 0.8);
		glBegin(GL_POLYGON);
		glVertex2f(600, 50);
		glVertex2f(610, 50);
		glVertex2f(610, 60);
		glVertex2f(600, 60);
		glEnd();
		drawText("Agua", 620, 550);

		/****************************************************/
		/****************Gráfico de linhas 1*****************/
		/****************************************************/
		max_luz = max_agua = max_gas = 0;
		for (i = 0; i < 12; i++) {
			if (consumoDiarioEletricidadeMensal[i] > max_luz)
				max_luz = consumoDiarioEletricidadeMensal[i];
			if (consumoDiarioGasMensal[i] > max_gas)
				max_gas = consumoDiarioGasMensal[i];
			if (consumoDiarioAguaMensal[i] > max_agua)
				max_agua = consumoDiarioAguaMensal[i];
		}

		glColor3f(0, 0, 0);
		glLineWidth(2);
		glPointSize(5);
		glBegin(GL_LINE_STRIP);
		glVertex2i(linex, liney);//origem do gráfico
		glVertex2i(linex, liney + 150);
		glVertex2i(linex, liney);
		glVertex2i(linex + 470, liney);
		glEnd();

		drawText("Media de consumo diario", 400, 100);

		//lines
		glLineWidth(1);
		glColor3f(0.8, 0.2, 0.2);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioEletricidadeMensal[i] / max_luz) * 125 + liney);
		}
		glEnd();
		glColor3f(0.2, 0.8, 0.2);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioGasMensal[i] / max_gas) * 125 + liney);
		}
		glEnd();
		glColor3f(0.2, 0.2, 0.8);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioAguaMensal[i] / max_agua) * 125 + liney);
		}
		glEnd();

		//points
		glColor3f(0, 0, 0);
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioEletricidadeMensal[i] / max_luz) * 125 + liney);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioGasMensal[i] / max_gas) * 125 + liney);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + linex, (consumoDiarioAguaMensal[i] / max_agua) * 125 + liney);
		}
		glEnd();

		//labels
		drawTextS("Eletricidade", linex - 58, 600 - (consumoDiarioEletricidadeMensal[0] / max_luz * 125 + liney));
		drawTextS("Gas", linex - 21, 600 - (consumoDiarioGasMensal[0] / max_gas * 125 + liney));
		drawTextS("Agua", linex - 30, 600 - (consumoDiarioAguaMensal[0] / max_agua * 125 + liney));
		for (i = 0; i < 12; i++) {
			sprintf(buff, "%.1fkWh", consumoDiarioEletricidadeMensal[i]);
			drawTextS(buff, i * 42 + linex + 5, 600 - (consumoDiarioEletricidadeMensal[i] / max_luz * 125 + liney));

			sprintf(buff, "%.1fkWh", consumoDiarioGasMensal[i]);
			drawTextS(buff, i * 42 + linex + 5, 600 - (consumoDiarioGasMensal[i] / max_gas * 125 + liney));

			sprintf(buff, "%.1fm3", consumoDiarioAguaMensal[i]);
			drawTextS(buff, i * 42 + linex + 5, 600 - (consumoDiarioAguaMensal[i] / max_agua * 125 + liney));
		}

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
		for (n = ang1 - 0.005; n <= ang1 + ang2; n += 0.005)
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

		//consumos detalhados
		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Consumos:", 800, 400);
		drawTextC("Aluno", 900, 400);
		drawTextC("Funcionario", 1000, 400);

		drawTextC("Gas", 800, 425);
		drawTextC("Agua", 800, 450);
		drawTextC("Eletricidade", 780, 475);

		sprintf(buff, "%.2fkwh", consumoGasAluno);
		drawText(buff, 900, 425);
		sprintf(buff, "%.2fm3", consumoAguaAluno);
		drawText(buff, 900, 450);
		sprintf(buff, "%.2fkwh", consumoEletricidadeAluno);
		drawText(buff, 900, 475);

		sprintf(buff, "%.2fkwh", consumoGasStaff);
		drawText(buff, 1000, 425);
		sprintf(buff, "%.2fm3", consumoAguaStaff);
		drawText(buff, 1000, 450);
		sprintf(buff, "%.2fkwh", consumoEletricidadeStaff);
		drawText(buff, 1000, 475);

		break;
	case 3://Gastos
		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Orcamento:", 400, 50);
		sprintf(buff, "%d e", (int)orcamentoAnual);
		drawText(buff, 500, 50);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Ordenados:", 400, 100);
		sprintf(buff, "%d e", (int)totOrdenados);
		drawText(buff, 500, 100);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Ordenado medio:", 400, 150);
		sprintf(buff, "%d e", (int)ordenadoMedio);
		drawText(buff, 550, 150);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Gastos:", 800, 50);
		drawTextC("Aluno", 900, 50);
		drawTextC("Funcionario", 1000, 50);

		drawTextC("Gas", 800, 75);
		drawTextC("Agua", 800, 100);
		drawTextC("Eletricidade", 780, 125);

		sprintf(buff, "%.2fe", gastosGasAluno);
		drawText(buff, 900, 75);
		sprintf(buff, "%.2fe", gastosAguaAluno);
		drawText(buff, 900, 100);
		sprintf(buff, "%.2fe", gastosElecAluno);
		drawText(buff, 900, 125);

		sprintf(buff, "%.2fe", gastosGasStaff);
		drawText(buff, 1000, 75);
		sprintf(buff, "%.2fe", gastosAguaStaff);
		drawText(buff, 1000, 100);
		sprintf(buff, "%.2fe", gastosElecStaff);
		drawText(buff, 1000, 125);

		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Total de gastos:", 500, 500);
		sprintf(buff, "%.2fe", totalGastosAnuais);
		drawText(buff, 650, 500);
		glColor3f(0.2, 0.2, 0.8);
		drawTextC("Balanco anual:", 500, 550);
		sprintf(buff, "%.2fe", balancoContas);
		drawText(buff, 625, 550);

		/****************************************************/
		/****************Gráfico de linhas 2*****************/
		/****************************************************/
		max_gasto = 0;
		for (i = 0; i < 12; i++) {
			if (gastoEletricidadeMensalTotal[i] > max_gasto)
				max_gasto = gastoEletricidadeMensalTotal[i];
			if (gastoGasMensal[i] > max_gasto)
				max_gasto = gastoGasMensal[i];
			if (gastoAguaMensal[i] > max_gasto)
				max_gasto = gastoAguaMensal[i];
		}
		glColor3f(0, 0, 0);
		glLineWidth(2);
		glPointSize(5);
		glBegin(GL_LINE_STRIP);
		glVertex2i(line2x, line2y);//origem do gráfico
		glVertex2i(line2x, line2y + 150);
		glVertex2i(line2x, line2y);
		glVertex2i(line2x + 470, line2y);
		glEnd();

		drawText("Gastos mensais", 500, 225);

		//lines
		glLineWidth(1);
		glColor3f(0.8, 0.2, 0.2);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoEletricidadeMensalTotal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();
		glColor3f(0.2, 0.8, 0.2);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoGasMensal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();
		glColor3f(0.2, 0.2, 0.8);
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoAguaMensal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();

		//points
		glColor3f(0, 0, 0);
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoEletricidadeMensalTotal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoGasMensal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();
		glBegin(GL_POINTS);
		for (i = 0; i < 12; i++) {
			glVertex2f(i * 42 + line2x, (gastoAguaMensal[i] / max_gasto) * 125 + line2y);
		}
		glEnd();

		//labels
		drawTextS("Eletricidade", line2x - 58, 600 - (gastoEletricidadeMensalTotal[0] / max_gasto * 125 + line2y));
		drawTextS("Gas", line2x - 21, 600 - (gastoGasMensal[0] / max_gasto * 125 + line2y));
		drawTextS("Agua", line2x - 30, 600 - (gastoAguaMensal[0] / max_gasto * 125 + line2y));
		for (i = 0; i < 12; i++) {
			sprintf(buff, "%.1fe", gastoEletricidadeMensalTotal[i]);
			drawTextS(buff, i * 42 + line2x + 5, 600 - (gastoEletricidadeMensalTotal[i] / max_gasto * 125 + line2y));

			sprintf(buff, "%.1fe", gastoGasMensal[i]);
			drawTextS(buff, i * 42 + line2x + 5, 600 - (gastoGasMensal[i] / max_gasto * 125 + line2y));

			sprintf(buff, "%.1fe", gastoAguaMensal[i]);
			drawTextS(buff, i * 42 + line2x + 5, 600 - (gastoAguaMensal[i] / max_gasto * 125 + line2y));
		}
		break;
	default:
		break;
	}
	//Menu
	glViewport(0, 0, winw, winh);
	glColor3f(1, 1, 1);
	menuButton();
	glPushMatrix();
	glTranslatef(menuw - 300, 0, 0);
	drawMenu();
	glPopMatrix();
	// Desencadeia a geração da imagem (rendering)
	glFlush();
}

////////////////////////////////////////////////////////////////////////////////
//	Programa Principal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

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
	glutReshapeFunc(reshape);
	glutTimerFunc(0, menuAnim, 0);

	//Lê ficheiros -  se não existir na diretoria o programa não arranca de todo.
	leituraFicheiros();

	//menu com tecla direita do rato para efetuar edições de dados 
	int menu_id = glutCreateMenu(menuEdicao);
	glutAddMenuEntry("Editar Valores Gerais (terminal)", 1);
	glutAddMenuEntry("Editar Valores do Ficheiro de Dados (terminal)", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Esperando por eventos
	glutMainLoop();

	return EXIT_SUCCESS;
}