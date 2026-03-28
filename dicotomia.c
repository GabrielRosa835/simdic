/*  ============================================================================
    MÉTODOS NUMÉRICOS - ISOLAMENTO DE RAÍZES (MÉTODO DA DICOTOMIA / BISSECÇÃO)
    ============================================================================
    INTEGRANTES:
    - Gabriel Rosa da Silva, RA 237069
    - Rodrigo Avila Ferreira dos Santos Filho, RA 251327
    - Lucas Moraes Mosca, RA 250889

    DESCRIÇÃO DO PROGRAMA:
    Este programa implementa o Método da Dicotomia (ou Bissecção) para encontrar 
    raízes reais de funções polinomiais de grau 2 a 6. O usuário fornece o grau, 
    os coeficientes da equação, o critério de parada (erro) e um intervalo [a, b]. 
    
    O algoritmo realiza uma análise teórica para verificar a existência de raízes 
    no intervalo informado (garantindo que f(a)*f(b) < 0), calcula o número 
    estimado de iterações (K) e itera reduzindo o intervalo pela metade até 
    atingir a precisão especificada pelo erro.

    EXEMPLOS DE POLINÔMIOS PARA TESTE:
    polinômio: f(x) = x² - 4
        grau: 2 
        coeficientes: 1, 0, -4 
        intervalo: [0, 3] 
        raíz: 2
    polinômio: f(x) = x³ - x - 2
        grau: 3 
        coeficientes: 1, 0, -1, -2 
        intervalo: [1, 2] 
        raíz: ~1.521
    polinômio: f(x) = x⁴ - 5x² + 4
        grau: 4 
        coeficientes: 1, 0, -5, 0, 4 
        intervalo: [0.5, 1.5] 
        raíz: 1
    polinômio: f(x) = x⁵ - 3x³ + 1
        grau: 5 
        coeficientes: 1, 0, -3, 0, 0, 1 
        intervalo: [0, 1] 
        raíz: ~0.755
    polinômio: f(x) = x⁶ - x - 1
        grau: 6 
        coeficientes: 1, 0, 0, 0, 0, -1, -1 
        intervalo: [1, 2] 
        raíz: ~1.134
============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Estrutura para agrupar as propriedades da equação, evitando múltiplas variáveis soltas
typedef struct {
    int grau;
    float* coeficientes; // Ponteiro exigido no descritivo para armazenar os fatores
} polinomio;

// Protótipos das funções
void imprimir_cabecalho_tabela();
void imprimir_linha_tabela(int iteracao, float a, float b, float m, float fA, float fB, float fM, char fAfM, char fMfB);
polinomio criar_polinomio();
int calcular_k(float a, float b, float erro);
int verificar_intervalo(polinomio* p, float a, float b, float e);
float calcular(polinomio* p, float valor);
void exibir_polinomio(polinomio* p);

int main()
{
    // ========================================================================
    // ETAPA 1: RECEBER OS DADOS
    // ========================================================================
    printf("\n");
    polinomio p = criar_polinomio(); // Função que isola a lógica de criação
    printf("\n");

    printf("A função recebida foi: \n\t");
    exibir_polinomio(&p);

    // Testes rápidos com valores padrão para confirmar a montagem da equação
    printf("Exemplos: \n\t");
    printf("f(%.2f) = %.2f \n\t", -1.0, calcular(&p, -1.0));
    printf("f(%.2f) = %.2f \n\t", 0.0, calcular(&p, 0.0));
    printf("f(%.2f) = %.2f \n", 1.0, calcular(&p, 1.0));

    float e = 0;
    printf("\n");

    // Recebimento do Erro (critério de parada)
    while(e <= 0) 
    {
        printf("Informe o erro (e > 0): ");
        scanf("%f", &e);
    }

    float a, b;
    int intervalo_invalido;
    printf("\n");

    // ========================================================================
    // ETAPA 2: ANÁLISE TEÓRICA E VALIDAÇÃO DO INTERVALO
    // ========================================================================
    do
    {
        a = b = 0;
        
        while(a >= b) 
        {
            printf("Informe o intervalo [a, b] (a < b):\n");
            printf("a = ");
            scanf("%f", &a);
            printf("b = ");
            scanf("%f", &b);
        }

        // Verifica se f(a) * f(b) < 0 (condição fundamental para existir raiz)
        intervalo_invalido = verificar_intervalo(&p, a, b, e);
        if (intervalo_invalido < 0) {
            // Retorno negativo significa que 'a' ou 'b' já são raízes satisfatórias
            free(p.coeficientes);
            return 0; 
        }
    }
    while(intervalo_invalido); // Repete a pergunta caso f(a)*f(b) > 0

    // ========================================================================
    // ETAPA 3: IMPLEMENTAÇÃO DO MÉTODO DA DICOTOMIA
    // ========================================================================
    
    // Calcula o número máximo estimado de iterações necessárias
    int K = calcular_k(a, b, e);
    printf("K = %i\n\n", K);
    
    int iteracoes = 1;
    float raiz, resultado_raiz;

    imprimir_cabecalho_tabela();

    while(1)
    {
        // Acha o ponto médio
        float m = (a + b) / 2;

        // Calcula os valores da função nos pontos a, b e m
        float resultado_a = calcular(&p, a);
        float resultado_b = calcular(&p, b);
        float resultado_m = calcular(&p, m);
        
        // Define os sinais para a tabela baseados no Teorema de Bolzano
        char fa_fm = resultado_a * resultado_m < 0 ? '-' : '+';
        char fb_fm = resultado_b * resultado_m < 0 ? '-' : '+';

        imprimir_linha_tabela(iteracoes, a, b, m, resultado_a, resultado_b, resultado_m, fa_fm, fb_fm);
        
        // Redefine o intervalo substituindo o limite apropriado pelo ponto médio
        if (resultado_a * resultado_m < 0) 
        {
            b = m;
        }
        else 
        {
            a = m;
        }

        // Critérios de parada: limite de iterações, tamanho do intervalo, ou aproximação de f(m) a zero
        if (iteracoes >= K || fabs(b-a) < e || fabs(resultado_m) < e) {
            raiz = m;
            resultado_raiz = resultado_m;
            break;
        }
        iteracoes += 1;
    }

    // ========================================================================
    // ETAPA 4: RESULTADO FINAL
    // ========================================================================
    printf("----*-----------*-----------*-----------*-----------*-----------*-----------*-----------*-----------+\n");
    printf(" Raíz aproximada encontrada: r = %.5f                                                            |\n", raiz);
    printf(" Valor da função neste ponto: f(r) = %.5f                                                        |\n", resultado_raiz);
    printf(" Iterações realizadas: %2i                                                                           |\n", iteracoes);
    printf("----------------------------------------------------------------------------------------------------*\n\n");

    // Liberação da memória alocada dinamicamente
    free(p.coeficientes);
    return 0;
}

// ----------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
// ----------------------------------------------------------------------------

// Recebe a entrada do usuário, aloca espaço na memória e monta a estrutura do polinômio
polinomio criar_polinomio() 
{
    int grau = 0;
    while(grau < 2 || grau > 6) 
    {
        printf("Informe o grau do polinômio (2 a 6): ");
        scanf("%i", &grau);
    }
    
    // Aloca memória para grau + 1 elementos (ex: grau 2 requer 3 posições: a, b, c)
    float* coeficientes = (float*) malloc((grau + 1) * sizeof(float));
    
    polinomio p = {
        .grau = grau,
        .coeficientes = coeficientes,
    };
    printf("\n");

    for(int i = p.grau; i >= 0; i--)
    {
        printf("Informe o coeficiente de x^%i: ", i);
        scanf("%f", p.coeficientes + i);
    }
    
    return p;
}

// Retorna 0 (válido), 1 (inválido por falta de variação de sinal) ou -1 (já encontrou a raiz nos extremos)
int verificar_intervalo(polinomio* p, float a, float b, float e)
{
    float resultado_a = calcular(p, a);
    float resultado_b = calcular(p, b);

    // Verificações para caso os extremos já sejam as raízes desejadas
    if (resultado_a == 0 || fabs(resultado_a) < e) 
    {
        printf("O limite 'a' já satisfaz o critério de raiz: \n\t f(%.2f) = %.2f \n", a, resultado_a);
        return -1;
    }
    if (resultado_b == 0 || fabs(resultado_b) < e) 
    {
        printf("O limite 'b' já satisfaz o critério de raiz: \n\t f(%.2f) = %.2f \n", b, resultado_b);
        return -1;
    }

    // Análise de Bolzano: Se os sinais forem iguais (multiplicação > 0), não há garantia de raiz
    float verificacao = resultado_a * resultado_b;
    if (verificacao > 0)
    {
        printf("Não é possível utilizar dicotomia no intervalo desejado (sinais iguais): \n\t");
        printf("f(%.2f) * f(%.2f) = %.2f * %.2f = %.2f > 0 \n", a, b, resultado_a, resultado_b, verificacao);
        return 1;
    }
    return 0;
}

// Realiza o cálculo do K usando a fórmula baseada nos logaritmos decimais
int calcular_k(float a, float b, float erro)
{
    float K = (log10(b-a) - log10(erro)) / log10(2);
    return (int) ceil(K); // Arredonda sempre para cima para garantir as iterações necessárias
}

void imprimir_cabecalho_tabela()
{
    printf(" %2s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s |\n", "I", "a", "b", "m", "f(a)", "f(b)", "f(m)", "f(a)*f(m)", "f(m)*f(b)");
    printf("----+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+\n");
}

void imprimir_linha_tabela(int iteracao, float a, float b, float m, float fa, float fb, float fm, char fa_fm, char fm_fb)
{
	printf(" %2i | %9.5f | %9.5f | %9.5f | %9.5f | %9.5f | %9.5f | %9c | %9c |\n", iteracao, a, b, m, fa, fb, fm, fa_fm, fm_fb);
}

// Função coringa (reutilizável) para calcular o valor do polinômio em qualquer ponto (x)
float calcular(polinomio* p, float valor) 
{
    float resultado = 0;
    for(int g = p->grau; g >= 0; g--)
    {
        // O coeficiente armazenado no vetor é multiplicado pelo valor de x elevado ao índice atual
        resultado += *(p->coeficientes + g) * pow(valor, g);
    }
    return resultado;
}

// Lida com a formatação puramente estética de impressão da equação
void exibir_polinomio(polinomio* p) 
{
    printf("f(x) = ");
    int primeiro_termo = 1;

    for(int g = p->grau; g >= 0; g--) 
    {
        float coef = *(p->coeficientes+g);

        // Ignora coeficientes nulos
        if (coef == 0.0f) continue;

        // Formatação de sinais e espaçamentos
        if (coef < 0)
        {
            if (primeiro_termo == 1) printf("-"); 
            else printf(" - ");
            coef = -coef; 
        } 
        else 
        {
            if (primeiro_termo == 0) printf(" + "); 
        }

        // Formatação da incógnita e expoente
        if (g == 0) 
        {
            printf("%.2f", coef);
        }
        else 
        {
            if (coef != 1.0f) printf("%.2f", coef);
            if (g == 1) printf("x");
            else printf("x^%i", g);
        }
        primeiro_termo = 0;
    }

    if (primeiro_termo == 1) printf("0");
    printf("\n");
}
