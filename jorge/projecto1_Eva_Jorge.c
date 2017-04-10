/******************************************************************************/
/**                                                    ,---------------.     **/
/**    COMPUTACAO GRAFICA                              |        .-.    |     **/
/**    MIEEC - FCTUC                                   |        !o|    |     **/
/**                                                    |>-,   ,-' | ,-<|     **/
/**    AUTORES: - Eva Raquel Belém Curto               |  `---'   `-'  |     **/
/**             - Jorge Miguel de Carvalho Monteiro    `---------------'     **/
/******************************************************************************/
/* DESCRICAO: Trabalho 1 -Dashboard de Gestão Familiar                        */
/*            - gcc test.c -o test -lGL -lGLU -lglut    -lm                   */
/******************************************************************************/

// 
//    Bibliotecas a incluir ////////////////////////////////////////////////////
//

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//
// Definições ///////////////////////////////////////////////////////////////////
//

#define PI 3.14159265
#define GL_COLOR_CLEAR
#define P_GAS 0.07
#define P_AGUA 0.7
#define P_ELECTRICIDADE 0.158
#define round(fp) (int)((fp) >= 0 ? (fp) + 0.5 : (fp) - 0.5)

//
//    Funções ////////////////////////////////////////////////////////////////////
//

void init(){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 710, 0, 500);
    glPointSize(4.0);
}

void display(void){

    FILE *fp;

    float v1,v2,v3; //variaveis auxiliares para pie chart
    float r1, r2, r3; //variaveis auxiliares para graficos de barras
    float n; //contador para radianos

    int x,y; //coordenadas x e y num eixo xOy

    float sum; // soma de diversos valores
    float vencimento=0; //soma dos vencimentos
    float aux1=0; // soma do consumo de horas cheias
    float aux2=0; // soma do consumo de horas vazias
    float aux3=0; //soma do consumo de horas de ponta
    float tGas=0;//soma dos consumos de gas
    float tAgua=0;//soma dos consumos de agua
    float tabela[12][6]; //tabela para alocar os valores lidos do ficheiro
    int i, j, mes, c; //usados como contadores
    float gas, luz, agua;
    float pos_x=5.0, pos_y=70.0; //cindicadores de posição
    char cheias[] = "HORAS CHEIAS";
    char vazias[] = "HORAS DE VAZIO";
    char ponta[]  = "HORAS DE PONTA";
	char str[10]; // variavel auxiliar para imprimir no ecra os valores numericos
	double floor(double); //define a função floor. (caso contrario dava erro)
	char gas_[]= "GAS (KWh)";
	char luz_[]="ELECTRICIDADE (KWh)"; 
	char agua_[]="AGUA (m3)";
	char vencimento_[]="VENCIMENTO (euros)";
	char legenda1_[]="VENCIMENTO/DESPESAS - Mensal";
	char hchvhp_[]="HORAS CHEIAS, HORAS VAZIAS, HORAS DE PONTA [%] - Anual";

	char energia_[]="DESPESA (euros)";
	float maximo;

    char meses[12][4]={"JAN", "FEV", "MAR", "ABR", "MAI", "JUN", "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"};    


    glClearColor(1.0, 1.0, 1.0, 0.0); //para poder aumentar o tamanho da janela 
                                      //sem interferir com os graficos
    glClear(GL_COLOR_BUFFER_BIT);

    //********LE FICHEIRO**************************************************/
    fp=fopen("dados.txt", "r");  
    
    if(fp==NULL) // caso o ficheiro não exista, o programa sai automaticamente
    {
        printf("ficheiro inexistente. SAIR \n");
        getchar();
        exit(-1);
    }
    else
    {
        for(i=0; i<=11; i++)
        { 
                for(j=0; j<=5; j++) 
                {
                    fscanf(fp," %f", &tabela[i][j]); // le os valores de dados.txt
					if(j==0)	vencimento=vencimento+tabela[i][j];
					if(j==1)	aux1=aux1+tabela[i][j];
					if(j==2)	aux2=aux2+tabela[i][j];
					if(j==3)    aux3=aux3+tabela[i][j];
					if(j==4)	tGas=tGas+tabela[i][j];
					if(j==5)	tAgua=tAgua+tabela[i][j];
                }
        }
    }
    //****************FIM LE FICHEIRO********************************/

    //*************DESENHA GRAFICO CIRCULAR*************************/
    sum=aux1+aux2+aux3;
    v1=(aux1*2*PI)/sum;
    v2=(aux2*2*PI)/sum;
    v3=(aux3*2*PI)/sum;

    //CRIA_FATIA_AZUL
    glBegin(GL_POLYGON); 
    glColor3f(0.2, 0.2, 1);
    for (n = v1+v2-0.005; n <= v3+v2+v1; n+=0.005)
    {
        x = cos(n)*70+100;
        y = sin(n)*70+400;
        glVertex2f(x, y);
    }
    glVertex2f(100,400); //para fazer um poligono, é necessário que o último
                         //vertice esteja conectado com a origem
    glEnd();
    //FIM CRIA_FATIA_AZUL
    
    //CRIA_FATIA_VERMELHA
    glBegin(GL_POLYGON); 
    glColor3f(1.0, 0.2, 0.2);
    for (n = -0.005; n <=v1; n+=0.005)
    {
        x = cos(n)*70+100;
        y = sin(n)*70+400;
        glVertex2f(x, y);
    }
    glVertex2f(100,400); //para fazer um poligono, é necessário que o último
                     //vertice esteja conectado com a origem
    glEnd();
    //FIM CRIA_FATIA_VERMELHA

    //CRIA_FATIA_VERDE
    glBegin(GL_POLYGON); 
    glColor3f(0.2, 1, 0.2);
    
    for (n=v1-0.005; n<=v2 + v1; n+=0.005)
    {
        x = cos(n)*70+100;
        y = sin(n)*70+400;
        glVertex2f(x, y);
    }
    glVertex2f(100,400); //para fazer um poligono, é necessário que o último
                         //vertice esteja conectado com a origem
    glEnd();
    //FIM CRIA_FATIA_VERDE

    //******** FIM DESENHA GRAFICO CIRULAR**********************/


	//LEGENDA GRÁFICO CIRCULAR
	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(20, 482);
	for (i = 0; i < strlen (hchvhp_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, hchvhp_[i]); 
	}
	//FIM LEGENDA GRÁFICO CIRCULAR

	//***** DESENHA GRAFICO ENERGIA ANUAL***********/

    r1=(150*aux1)/sum;
    r2=(150*aux2)/sum;
    r3=(150*aux3)/sum;

    glColor3f(1, 0.2, 0.2);//DESENHA GRAFICO VERMELHO
    glBegin(GL_POLYGON);
        glVertex2f(200,325);
        glVertex2f(250,325);
        glVertex2f(250,325+r1);
        glVertex2f(200,325+r1);
    glEnd();

	glColor3f(0.0,0.0,0.0); 
	sprintf(str, "%d", round((100*aux1)/sum)); // arredonda o valor da % para depois escrever
    glRasterPos2f(210, 325+r1/2 ); 
	for (i = 0; i < strlen(str); i++) { //escreve o valor em % correspondente à barra
    		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[i]); 
	}
	glRasterPos2f(225, 325+r1/2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '%');

    glColor3f(0.2, 1, 0.2);//DESENHA GRAFICO VERDE
    glBegin(GL_POLYGON);
        glVertex2f(200,325+r1);
        glVertex2f(250,325+r1);
        glVertex2f(250,325+r1+r2);
        glVertex2f(200,325+r1+r2);
    glEnd();

	glColor3f(0.0,0.0,0.0); 
	sprintf(str, "%d", round((100*aux2)/sum)); //arredonda o valor da % para depois escrever
    glRasterPos2f(210, 325+r1+r2/2 ); //escreve o valor em % correspondente à barra
	for (i = 0; i < strlen(str); i++) { 
    		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[i]); 
	}
	glRasterPos2f(225, 325+r1+r2/2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '%');

    glColor3f(0.2, 0.2, 1.0);//DESENHA GRAFICO AZUL
    glBegin(GL_POLYGON);
        glVertex2f(200,325+r1+r2);
        glVertex2f(250,325+r1+r2);
        glVertex2f(250,325+r1+r2+r3);
        glVertex2f(200,325+r1+r2+r3);
    glEnd();

    glColor3f(0.0,0.0,0.0); 
	sprintf(str, "%d", round((100*aux3)/sum)); //arredonda o valor da % para depois escrever
    glRasterPos2f(210, 325+r1+r2+r3/2 ); 
	for (i = 0; i < strlen(str); i++) { //escreve o valor em % correspondente à barra
    		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[i]); 
	}
	glRasterPos2f(225, 325+r1+r2+r3/2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '%');

	glBegin(GL_LINE_STRIP); //LINHA VAZIO
		glVertex2f(250,325+r1/2);
		glVertex2f(285,355);
		glVertex2f(400,355);
	glEnd();

	glBegin(GL_LINE_STRIP); //LINHA CHEIA
		glVertex2f(250,325+r1+r2/2);
		glVertex2f(285,405);
		glVertex2f(400,405);
	glEnd();

	glBegin(GL_LINE_STRIP); //LINHA PONTA
		glVertex2f(250,325+r2+r1+r3/2);
		glVertex2f(285,455);
		glVertex2f(400,455);
	glEnd();
   
    glColor3f(0.0,0.0,0.0);  // HORAS DE PONTA
    glRasterPos2f(295, 460); //tem que ser as coordenadas de acordo com os quadrados das legendas
    for (i = 0; i < strlen (ponta); i++) {
    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, ponta[i]); 
    }
    
    glColor3f(0.0,0.0,0.0); // HORAS CHEIAS
    glRasterPos2f(295, 410); //tem que ser as coordenadas de acordo com os quadrados das legendas
    for (i = 0; i < strlen (cheias); i++) {
    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, cheias[i]); 
    }
    
    glColor3f(0.0,0.0,0.0); // HORAS DE VAZIO
    glRasterPos2f(295,360); //tem que ser as coordenadas de acordo com os quadrados das legendas
    for (i = 0; i < strlen (vazias); i++) {
    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, vazias[i]); 
    }
	//*********** FIM DESENHAR GRAFICO ENERGIA ANUAL*****/


	//******** DESENHA GRAFICOS MENSAIS**********///
    for(i=0; i <=11; i++){ //percorre os 12 meses do ano

	luz= tabela[i][1]+tabela[i][2]+tabela[i][3]; //o valor da electricidade é a soma de horas
	//cheias com horas vazias e horas de ponta
        gas= tabela[i][4];        
        agua=tabela[i][5];
        sum=gas+luz+agua;
        gas=(200*gas)/sum;
        luz=(200*luz)/sum;
        agua=(200*agua)/sum;

        glColor3f(1.0, 0.4, 0.4);//GRAFICO DE BARRAS VERMELHO - GAS
        glBegin(GL_POLYGON);
            glVertex2f(pos_x , pos_y);
            glVertex2f(pos_x + 12 , pos_y);
            glVertex2f(pos_x + 12 , pos_y + gas);
            glVertex2f(pos_x , pos_y + gas);
        glEnd();	



        glColor3f(0.7, 1.0, 0.4);//GRAFICO DE BARRAS VERDE - LUZ
        glBegin(GL_POLYGON);
            glVertex2f(pos_x + 12 , pos_y);
            glVertex2f(pos_x + 24 , pos_y);
            glVertex2f(pos_x + 24 , pos_y + luz);
            glVertex2f(pos_x + 12 , pos_y + luz);
        glEnd();

		

        glColor3f(0.4, 0.7, 1.0);//GRAFICO DE BARRAS AZUL - AGUA
        glBegin(GL_POLYGON);
            glVertex2f(pos_x + 24 , pos_y);
            glVertex2f(pos_x + 36 , pos_y);
            glVertex2f(pos_x + 36 , pos_y + agua);
            glVertex2f(pos_x + 24 , pos_y + agua);
        glEnd();

	/*glColor3f(0.0,0.0,0.0); 
	sprintf(str, "%d", (int)round(tabela[i][5]));
    	glRasterPos2f(pos_x+28, pos_y+agua+10 ); //tem que ser as coordenadas de acordo com os quadrados das legendas
	for (c = 0; c < strlen(str); c++) {
    		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
	}*/

		glColor3f(0.0,0.0,0.0);
		for(c=0; c<=2; c++){ //escreve os meses
		glRasterPos2f(pos_x+14+4*c, pos_y-10-11*c );
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, meses[i][c]);
		}

        pos_x=pos_x+41; //incrementa a posição para os graficos não se sobreporem
    }

	glColor3f(1.0, 0.4, 0.4); // LEGENDA GAS
	glBegin(GL_POLYGON); //cria o quadrado identificador da cor
	glVertex2f(89,15);
	glVertex2f(99,15);
	glVertex2f(99,25);
	glVertex2f(89,25);
	glEnd();
	glColor3f(0.0,0.0,0.0); 
	glRasterPos2f(100, 15); 
	for (i = 0; i < strlen (gas_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, gas_[i]); 
	}

	glColor3f(0.7, 1.0, 0.4);// LEGENDA LUZ
	glBegin(GL_POLYGON); //cria o quadrado identificador da cor
	glVertex2f(189,15);
	glVertex2f(199,15);
	glVertex2f(199,25);
	glVertex2f(189,25);
	glEnd();
	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(200, 15);
	for (i = 0; i < strlen (luz_); i++) {
	   	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, luz_[i]); 
	}

	glColor3f(0.4, 0.7, 1.0);//LEGENDA AGUA
	glBegin(GL_POLYGON); //cria o quadrado identificador da cor
	glVertex2f(339,15);
	glVertex2f(349,15);
	glVertex2f(349,25);
	glVertex2f(339,25);
	glEnd();
	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(350, 15); 
	for (i = 0; i < strlen (agua_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, agua_[i]); 
	}

	glColor3f(0.7,0.4,1);//LEGENDA VENCIMENTO
	glBegin(GL_POLYGON); //cria o quadrado identificador da cor
	glVertex2f(419,15);
	glVertex2f(429,15);
	glVertex2f(429,25);
	glVertex2f(419,25);
	glEnd();
	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(430, 15); 
	for (i = 0; i < strlen (vencimento_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, vencimento_[i]); 
	}

	/*****************FIM DESENHA GRAFICOS MENSAIS******************/

	//DESENHA RECTANGULO DE VALORES

	glBegin(GL_LINE_STRIP);
	glVertex2f(550,15);
	glVertex2f(690,15);
	glVertex2f(690,50);
	glVertex2f(550,50);
	glVertex2f(550,15);
	glEnd();

	/*DESENHA GRAFICO DOS VENCIMENOS*/
	
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(450,450);
	glVertex2f(450,300);
	glVertex2f(700,300);
	glVertex2f(700,450);
	glVertex2f(450,450);
	glEnd();

	/*TITULO GRAFICO VENCIMENTO E DESPESAS MENSAIS*/
	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(470, 475);
	for (i = 0; i < strlen (legenda1_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, legenda1_[i]); 
	}

	/**/

	/*LEGENDA DO VENCIMENTO*/
	glColor3f(0,1,0);
	glBegin(GL_POINTS);
	glVertex2f(455,460);
	glEnd();

	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(460, 455);
	for (i = 0; i < strlen (vencimento_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, vencimento_[i]); 
	}
	/***********************/

	/*LEGENDA DAS DESPESAS*/
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	glVertex2f(590,460);
	glEnd();

	glColor3f(0.0,0.0,0.0);  
	glRasterPos2f(595, 455);
	for (i = 0; i < strlen (energia_); i++) {
	    	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, energia_[i]); 
	}
	/***********************/

	pos_x=445; // inicializa a posição em x novamente, para desenhar o grafico
	pos_y=300; // inicializa a posição em y novamente, para desenhar o grafico

	glColor3f(0.0,0.0,0.0);
	
	aux1=0; //variavel para o maximo dos vencimentos
	aux2=0; //variavel para o maximo das despesas

	for(i=0; i<=11; i++){ // calcula o maximo entre vencimento e maximo das despesas, para efeitos de normalização
		if(aux1 < tabela[i][0]){
			aux1=tabela[i][0];	//calcula o maximo dos vencimentos	
		}
		if(aux2 < P_ELECTRICIDADE*(tabela[i][1]+tabela[i][2]+tabela[i][3])+P_GAS*tabela[i][4]+P_AGUA*tabela[i][5]){
			//calcula o maximo das despesas
			aux2=P_ELECTRICIDADE*(tabela[i][1]+tabela[i][2]+tabela[i][3])+P_GAS*tabela[i][4]+P_AGUA*tabela[i][5];
		}
	}

	for(i=0;i<=11; i++){ //grafico dos pontos do vencimento
		
		glColor3f(0,1,0);
		glBegin(GL_POINTS);
		glVertex2f(pos_x+16,300+((140*tabela[i][0])/aux1));
		glEnd();

		sum=(tabela[i][1]+tabela[i][2]+tabela[i][3])*P_ELECTRICIDADE + tabela[i][4]*P_GAS + tabela[i][5]*P_AGUA;
		aux3=(tabela[i-1][1]+tabela[i-1][2]+tabela[i-1][3])*P_ELECTRICIDADE + tabela[i-1][4]*P_GAS + tabela[i-1][5]*P_AGUA;

		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex2f(pos_x+16,300+((140*sum)/aux2));
		glEnd();

		glColor3f(0,1,0);
		glBegin(GL_POINTS);
		glVertex2f(450,300+((140*tabela[i][0])/aux1));
		glEnd();
		
		glColor3f(0.0,0.0,0.0); 		

		sprintf(str, "%d", (int)round(tabela[i][0]));
    		glRasterPos2f(420,300+((140*tabela[i][0])/aux1));
		for (c = 0; c < strlen(str); c++) {
    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
		}

		sprintf(str, "%d", (int)round(sum));
    		glRasterPos2f(pos_x +16,290+((140*sum)/aux2) );
		for (c = 0; c < strlen(str); c++) {
    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
		}

		if(i>0 && i<=11){ //FAZ A LINHA A LIGAR OS DIFERENTES PONTOS
			glColor3f(0,1,0);
			glBegin(GL_LINE_STRIP);
			glVertex2f(pos_x-4,300+((140*tabela[i-1][0])/aux1));
			glVertex2f(pos_x+16,300+((140*tabela[i][0])/aux1));
			glEnd();

			glColor3f(1,0,0);
			glBegin(GL_LINE_STRIP);
			glVertex2f(pos_x-4,300+((140*aux3)/aux2));
			glVertex2f(pos_x+16,300+((140*sum)/aux2));
			glEnd();

		}

		for(c=0; c<=2; c++){ //ESCREVE OS MESES
			glColor3f(0,0,0);
			glRasterPos2f(pos_x+12+4*c, pos_y-10-11*c );
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, meses[i][c]);
		}

		pos_x=pos_x+20; // intervalo entre pontos é de 20 pixeis
	}
	
	/***********************************************************/
	

	/*****DESENHA GRAFICO DO VENCIMENTO EM FUNÇAO DE GASTOS*****/
	if(vencimento>=P_GAS*tGas+P_AGUA*tAgua+P_ELECTRICIDADE*(aux1+aux2+aux3)){maximo=vencimento;}
	else{maximo=P_GAS*tGas+P_AGUA*tAgua+P_ELECTRICIDADE*(aux1+aux2+aux3);}
	
	glColor3f(0.7,0.4,1);
	glBegin(GL_POLYGON);
	glVertex2f(620,70);
	glVertex2f(660,70);
	glVertex2f(660,70+((190*vencimento)/maximo));
	glVertex2f(620,70+((190*vencimento)/maximo));
	glEnd();

	glColor3f(1.0, 0.4, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(580,70);
	glVertex2f(620,70);
	glVertex2f(620,70+((190*P_GAS*tGas)/maximo));
	glVertex2f(580,70+((190*P_GAS*tGas)/maximo));
	glEnd();

	glColor3f(0.4, 0.7, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(580,70+((190*P_GAS*tGas)/maximo));
	glVertex2f(620,70+((190*P_GAS*tGas)/maximo));
	glVertex2f(620,70+((190*P_GAS*tGas)/maximo)+((190*P_AGUA*tAgua)/maximo));
	glVertex2f(580,70+((190*P_GAS*tGas)/maximo)+((190*P_AGUA*tAgua)/maximo));
	glEnd();

	glColor3f(0.7, 1.0, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(580,70+((190*P_GAS*tGas)/maximo)+((190*P_AGUA*tAgua)/maximo));
	glVertex2f(620,70+((190*P_GAS*tGas)/maximo)+((190*P_AGUA*tAgua)/maximo));
	glVertex2f(620,70+((190*P_GAS*tGas)/maximo)+((190*P_ELECTRICIDADE*(aux1+aux2+aux3))/maximo)+((190*P_AGUA*tAgua)/maximo));
	glVertex2f(580,70+((190*P_GAS*tGas)/maximo)+((190*P_ELECTRICIDADE*(aux1+aux2+aux3))/maximo)+((190*P_AGUA*tAgua)/maximo));
	glEnd();

	/*****************************************************/

    fclose(fp);
    glFlush();
    
}

void ControloTeclado(unsigned char key, int x, int y){
	if(key=='q' || key== 'Q'){ // sai do programa
		exit(0);
	}
	if(key=='r' || key== 'R'){
		init();
		display();
	}
}

void ControloRato(int button, int state, int x, int y){

	FILE* fp;

	float tabela[12][6];

	float cj; //variavel auxiliar

	char str[10];

	char strConsumosMensais[]="CONSUMO MENSAL";

	int c,i,j;

	char legenda_1[]="VENCIMENTO / ANO (euros)";
	char legenda_2[]="DESPESAS / ANO (euros)";
	char m3_[]="[m3]";
	char kwh_[]="[Kwh]";


   /**********LE FICHEIRO**********************************************************/

   fp=fopen("dados.txt", "r");
    
    if(fp==NULL) // caso o ficheiro não exista, o programa sai automaticamente
    {
        printf("ficheiro inexistente. SAIR \n");
        getchar();
        exit(-1);
    }
    else
    {
        for(i=0; i<=11; i++) // ficheiro: salario, cheias, vazias, ponta, gas, agua
        { 
                for(j=0; j<=5; j++) 
                {
                    fscanf(fp," %f", &tabela[i][j]); // le os valores no ficheiro de texto
                }
        }
    }
	/************ACABA DE LER DO FICHEIRO**************************************/

	/**FUNçAO QUE DETECTA CLIQUES NAS BARRAS DOS GRAFICOS**********************/
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && y<430 && y>=230 && x>=0 && x<492){ //grafico consumos mensais
		
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(551,16);
		glVertex2f(689,16);
		glVertex2f(689,49);
		glVertex2f(551,49);
		glEnd();

		glColor3f(0.0,0.0,0.0);
		glRasterPos2f(551, 34);
		for (c = 0; c < strlen(strConsumosMensais); c++) {
	    		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, strConsumosMensais[c]); 
		}

		i=floor((float)(x)/41); 
		cj=(x-41*floor((float)(x)/41))/41;

		if(cj>0.12 && cj<=0.41)
		{
			glColor3f(0.0,0.0,0.0);
		    	glRasterPos2f(591, 18);
			sprintf(str, "%.2f", tabela[i][4]);
			for (c = 0; c < strlen(str); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]);
			} 
			glRasterPos2f(650, 20);
			for (c = 0; c < strlen(kwh_); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, kwh_[c]); 
			}
		}

		if(cj>0.41 && cj<=0.71)
		{
			glColor3f(0.0,0.0,0.0);
		    	glRasterPos2f(591, 18);
			sprintf(str, "%.2f", (tabela[i][1]+tabela[i][2]+tabela[i][3]));
			for (c = 0; c < strlen(str); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
			}
			glRasterPos2f(650, 20);
			for (c = 0; c < strlen(kwh_); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, kwh_[c]); 
			}
		}

		if(cj>0.71 && cj<=1)
		{

			glColor3f(0.0,0.0,0.0);
			glRasterPos2f(591, 18);
			sprintf(str, "%.2f", tabela[i][5]);
			for (c = 0; c < strlen(str); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
			}
			glRasterPos2f(650, 20);
			for (c = 0; c < strlen(m3_); c++) {
	    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, m3_[c]); 
			}
		}

	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x>580 && x<=620){ //clica no grafico da soma das energias

		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(551,16);
		glVertex2f(689,16);
		glVertex2f(689,49);
		glVertex2f(551,49);
		glEnd();

		cj=0;

		for(i=0; i<=11; i++){
			cj=cj+(tabela[i][1]+tabela[i][2]+tabela[i][3])*P_ELECTRICIDADE+tabela[i][4]*P_GAS+tabela[i][5]*P_AGUA;
		}

		glColor3f(0.0,0.0,0.0);
		glRasterPos2f(591, 18);
		sprintf(str, "%.2f", cj);
		for (c = 0; c < strlen(str); c++) {
    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
		}

		glRasterPos2f(551, 34); 
		for(c=0; c<=strlen(legenda_2);c++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, legenda_2[c]);
		}

		

	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (x>620 && x<=660)){ //clica no grafico dos vencimentos
		
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(551,16);
		glVertex2f(689,16);
		glVertex2f(689,49);
		glVertex2f(551,49);
		glEnd();
		
		cj=0; 
		for(i=0; i<=11; i++){
			cj=cj+tabela[i][0]; //soma os vencimentos de todos os meses
		}

		glColor3f(0.0,0.0,0.0);
		glRasterPos2f(591, 18);
		sprintf(str, "%.2f", cj);
		for (c = 0; c < strlen(str); c++) {
    			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, str[c]); 
		}
		glRasterPos2f(551, 34); 
		for(c=0; c<=strlen(legenda_1);c++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, legenda_1[c]);
		}

	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (y>=430 || y<230 || (x>492 && x<=580) || x>660) ){
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(551,16);
		glVertex2f(689,16);
		glVertex2f(689,49);
		glVertex2f(551,49);
		glEnd();
	}
	/*********************************************************************************/
	glFlush();
}

void MenuPrincipal(int value)
{
	if(value == 0) // altera valores do ficheiro
	{
		FILE *fr, *fw;

		int mes, i,j;

		float tabela[12][6];

		float V, HC, HV, HP, G, A;
	
		fr=fopen("dados.txt", "r");
	
		if(fr==NULL){	
			printf("ficheiro não existe. Sair \n");
			exit(0);
		}
		else{
			for(i=0; i<=11; i++){
				for(j=0; j<=5; j++){
					fscanf(fr,"%f", &tabela[i][j]);
				}
			}
			fclose(fr);

			printf("Qual o mes a que deseja aceder? (1-Janeiro, 2-Fevereiro,...)\t");
			scanf("%d", &mes);

			if(mes<=0 || mes>=13){
				printf("inválido. SAIR \n");
				exit(0);
			}
			printf("introduza os valores dos consumos\n");
			printf("Ordenado(euros): ");
			scanf("%f", &V); 
			printf("Horas Cheias(kwh):  ");
			scanf("%f", &HV);
			printf("Horas de Vazio(kwh): ");
			scanf("%f", &HC);
			printf("Horas de Ponta(kwh); ");
			scanf("%f", &HP);
			printf("Gas(kwh):");
			scanf("%f", &G); 
			printf("Agua(m3): ");
			scanf("%f", &A);

			tabela[mes-1][0]=V;
			tabela[mes-1][1]=HV;
			tabela[mes-1][2]=HC;
			tabela[mes-1][3]=HP;
			tabela[mes-1][4]=G;
			tabela[mes-1][5]=A;

			fw=fopen("dados.txt", "w");

			for(i=0; i<=11;i++){
				for(j=0; j<=5; j++){
					fprintf(fw, "%.2f", tabela[i][j]);
					fprintf(fw, "%s", " ");
				}
				fprintf(fw, "%s", "\n");
			}
			printf("\nFicheiro actualizado.\n\n");
			fclose(fw);
		}
		init();
		display();	
	}

	if(value == 1) // sai do programa
	{
		exit(0);
	}
}



////////////////////////////////////////////////////////////////////////////////
//  Programa Principal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

	glutInit(&argc, argv);
    glutInitWindowSize(710, 500);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Dashboard Gestao Familiar");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(ControloTeclado);
	glutMouseFunc(ControloRato);
	glutCreateMenu(MenuPrincipal);
	glutAddMenuEntry("Alterar valores", 0);
	glutAddMenuEntry("Sair", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return EXIT_SUCCESS;
}
