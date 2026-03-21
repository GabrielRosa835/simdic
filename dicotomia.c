/*
    Exemplos de polinômios para teste:

    Raiz Exata
	polinômio: f(x) = x² - 4
	grau: 2
	coeficientes: 1, 0, -4
        [a, b]: [0, 3]
        raíz: 2

    Raíz Irracional
    	polinômio: f(x) = x³ - x - 2
	grau: 3
	coeficientes: 1, 0, -1, -2
        [a, b]: [1, 2]
        raíz: ~1.521

    Múltiplas Raízes
	polinômio: f(x) = x⁴ - 5x² + 4
	grau: 4
	coeficientes: 1, 0, -5, 0, 4
        [a, b]: [0.5, 1.5]
	raíz: 1

    Variação de Sinais
    	polinômio: f(x) = x⁵ - 3x³ + 1
	grau: 5
	coeficientes: 1, 0, -3, 0, 0, 1
	[a, b]: [0, 1]
	raíz: ~0.755

    Grau 6
	polinômio: f(x) = x⁶ - x - 1
	grau: 6
	coeficientes: 1, 0, 0, 0, 0, -1, -1
	[a, b]: [1, 2]
	raíz: ~1.134
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define true 1

typedef struct {
    int grau;
    float* coeficientes;
} polinomio;

void imprimir_cabecalho_tabela();
void imprimir_linha_tabela(int iteracao, float a, float b, float m, float fA, float fB, float fM, char fAfM, char fMfB);

int calcular_k(float a, float b, float erro);

float calcular(polinomio* p, float valor);
void exibir_polinomio(polinomio* p);

int main()
{
    printf("\n");

    int grau = 0;
    
    while(grau < 2 || grau > 6) {
        printf("Informe o grau do polinômio (2 a 6): ");
        scanf("%i", &grau);
    }
    
    float* coeficientes = (float*) malloc(grau * sizeof(int));
    
    polinomio p = {
        .grau = grau,
        .coeficientes = coeficientes,
    };

    printf("\n");

    // Preenchendo os coeficientes
    for(int i = p.grau; i >= 0; i--)
    {
        printf("Informe o coeficiente de x^%i: ", i);
        scanf("%f", p.coeficientes+i);
    }
    
    printf("\n");

    //Exibindo a funcao recebida
    printf("A função recebida foi: \n\t");
    exibir_polinomio(&p);

    printf("Exemplos: \n\t");
    printf("f(%.2f) = %.2f \n\t", -1.0, calcular(&p, -1));
    printf("f(%.2f) = %.2f \n\t", 0.0, calcular(&p, 0));
    printf("f(%.2f) = %.2f \n", 1.0, calcular(&p, 1));

    float e = 0;
    
    printf("\n");

    while(e <= 0) 
    {
        printf("Informe o erro (e > 0): ");
        scanf("%f", &e);
    }

    float a;
    float b;
    float verificacao;
    
    printf("\n");

    do
    {
        a = 0;
        b = 0;
        verificacao = 0;
        
        while(a >= b) 
        {
            printf("Informe o intervalo [a, b] (a < b):\n");
            printf("a = ");
            scanf("%f", &a);
            printf("b = ");
            scanf("%f", &b);
        }

        float resultado_a = calcular(&p, a);
        float resultado_b = calcular(&p, b);

        if (resultado_a == 0 || fabs(resultado_a) < e) 
        {
            printf("Valor de 'a' já é a raíz ou se encontra dentro da faixa de erro: \n\t");
            printf("f(%.2f) = %.2f \n", a, resultado_a);
            return 0;
        }
        if (resultado_b == 0 || fabs(resultado_b) < e) 
        {
            printf("Valor de 'b' já é a raíz ou se encontra dentro da faixa de erro: \n\t");
            printf("f(%.2f) = %.2f \n", b, resultado_b);
            return 0;
        }

        verificacao = resultado_a * resultado_b;
        if (verificacao > 0)
        {
            printf("Não é possível utilizar dicotomia no intervalo desejado: \n\t");
            printf("f(%.2f) * f(%.2f) = %.2f * %.2f = %.2f > 0 \n", a, b, resultado_a, resultado_b, verificacao);
        }
    }
    while(verificacao > 0);

    int K = calcular_k(a, b, e);
    printf("K = %i\n", K);
    int i = 1;
    float m;

    printf("\n");
    imprimir_cabecalho_tabela();
    do 
    {
        m = (a + b) / 2;

        float resultado_a = calcular(&p, a);
        float resultado_b = calcular(&p, b);
        float resultado_m = calcular(&p, m);
        char fa_fm = resultado_a * resultado_m < 0 ? '-' : '+';
        char fb_fm = resultado_b * resultado_m < 0 ? '-' : '+';

        imprimir_linha_tabela(i, a, b, m, resultado_a, resultado_b, resultado_m, fa_fm, fb_fm);
        
        if (resultado_a * resultado_m < 0) 
        {
            b = m;
        }
        else 
        {
            a = m;
        }
    }
    while (i++ < K && fabs(b-a) > e && fabs(calcular(&p, m)) > e);

    printf("\n");
}

//Realiza o calculo do K (numero de passos)
int calcular_k(float a, float b, float erro)
{
    //Realizar o calculo do valor do K
    float K = (log10(b-a) - log10(erro))/(log10(2));
    return (int) ceil(K);
}

//Imprimindo o cabecalho
void imprimir_cabecalho_tabela()
{
    printf(" %2s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s |\n", "I", "a", "b", "m", "f(a)", "f(b)", "f(m)", "f(a)*f(m)", "f(m)*f(b)");
    printf("----+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+\n");

}

//Imprimindo a tabela com os valores
void imprimir_linha_tabela(int iteracao, float a, float b, float m, float fa, float fb, float fm, char fa_fm, char fm_fb)
{
	printf(" %2i | %9.5f | %9.5f | %9.5f | %9.5f | %9.5f | %9.5f | %9c | %9c |\n", iteracao, a, b, m, fa, fb, fm, fa_fm, fm_fb);
}

float calcular(polinomio* p, float valor) 
{
    float resultado = 0;
    for(int g = p->grau; g >= 0; g--)
    {
        resultado += *(p->coeficientes + g) * pow(valor, g);
    }
    return resultado;
}

void exibir_polinomio(polinomio* p) 
{
    printf("f(x) = ");

    // Flag para saber se estamos imprimindo o primeiríssimo item.
    // Em C antigo não tínhamos o tipo 'bool', então usamos 'int' (1 = true, 0 = false)
    int primeiro_termo = 1;

    for(int g = p->grau; g >= 0; g--) 
    {
        float coef = *(p->coeficientes+g);

        // Regra 1: Se o coeficiente for 0, ignoramos completamente
        if (coef == 0.0f) 
        {
            continue;
        }

        // Regra 2: Tratamento de sinais e espaçamento
        if (coef < 0) 
        {
            if (primeiro_termo == 1) 
            {
                printf("-"); // Ex: "-x^2" (sem espaço no começo)
            } 
            else 
            {
                printf(" - "); // Ex: " ... - x^2" (com espaço no meio)
            }
            coef = -coef; // Transforma o coeficiente em positivo (valor absoluto)
        } 
        else 
        {
            if (primeiro_termo == 0) 
            {
                printf(" + "); // Só imprime '+' se NÃO for o primeiro termo
            }
            // Se for positivo E for o primeiro termo, não imprime sinal nenhum
        }

        // Regra 3: Exibição do número e da incógnita 'x'
        if (g == 0) 
        {
            // Se o grau for 0, é o termo independente. Exibe sempre o número.
            printf("%.2f", coef);
        } 
        else 
        {
            // Se o grau for maior que 0, tem a letra 'x'

            // Regra 4: Ocultar o número se o coeficiente for 1
            if (coef != 1.0f) 
            {
                printf("%.2f", coef);
            }

            // Regra Matemática Extra: Ocultar o "^1" quando o grau for 1
            if (g == 1) 
            {
                printf("x");
            } 
            else 
            {
                printf("x^%i", g);
            }
        }

        // Chegando aqui, garantimos que pelo menos um termo foi impresso.
        // Baixamos a flag para que os próximos termos imprimam os sinais " + " ou " - "
        primeiro_termo = 0;
    }

    // Regra Matemática Extra: Se o for terminou e a flag continua levantada,
    // significa que todos os coeficientes eram zero.
    if (primeiro_termo == 1) 
    {
        printf("0");
    }

    printf("\n");
}
