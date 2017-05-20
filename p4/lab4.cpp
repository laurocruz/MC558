// Lauro Cruz e Souza - 156175
// laurocruzsouza@gmail.com | lauro.souza@students.ic.unicamp.br

/* No algoritmo abaixo é feita a redução do problema apresentado no enunciado
 * para um problema de caminho mínimo entre dois vértices.
 *
 * Precisávamos encontrar uma lista de reservas que não tenham conflito 
 * maximizando o lucro. 
 *
 * Para tal, na transformação 1, foram criados um vértice para cada reserva (com
 * os vértices com os mesmos números das reservas) e criado dois vértices
 * auxiliares, chamados abaixo de CABEÇA e PÉ. Cada vértice real u aponta para 
 * outro vértice real v quando o horário de término de u é menor ou igual ao
 * horário de início de v, implicando que as duas reservas podem ser realizadas,
 * uma atrás da outra, sem ocorrer conflito. A aresta que aponta de u para v 
 * é o oposto do valor da reserva de v, implicando que, no caminho, estando em u
 * a reserva de u já foi feita e partindo para v, a reserva de v será realizada.
 * Abaixo será explicado porque esse peso é o **oposto** do valor da reserva.
 *
 * Quanto aos vértices CABEÇA e PÉ, CABEÇA se liga a todas os vértices reais
 * (com o peso das arestas sendo o oposto do valor de reserva do vértice de
 * destino) e PÉ é ligado por todas os vértices reais (com arestas nulas, pois
 * não fazemos um reserva de PÉ).
 *
 * Com esse grafo montado, se for realizada uma busca pelo caminho mínimo entre
 * CABEÇA e PÉ, o caminho conterá exatamente todas as reservas necessárias para
 * a maximização do lucro pois da forma como o grafo foi montado não temos 
 * reservas conflitantes em um caminho (o final do vértice anterior sempre é menor
 * ou igual ao início do próximo) e como os pesos das arestas são os opostos dos
 * valores das reservas, o caminho mínimo é aquele com a menor soma dos pesos, e por
 * isso foram usados os opostos, pois a adição de uma nova reserva (aumentando o 
 * lucro) diminui o peso do caminho
 *
 * Assim, na transformação 2, obtemos o dinheiro recebido fazendo o oposto da 
 * distância obtida em min_path e temos a sequência de reservas em ordenação 
 * crescente como o vértice de origem de da segunda aresta do caminho até a última 
 * (na primeira, o vértice de origem é CABEÇA).
 *
 * As reservas necessariamente estarão ordenadas assim pois, se para uma aresta (u,v),
 * temos u.f <= v.i --> u.i < u.f <= v.i < v.f, ou seja, u.i < v.i.
 * */
#include <iostream>
#include <cstdio>

#include "minpathcpp.h"

using namespace std;

#define CABECA 0

// Estrutura com os dados de uma reserva
// i: Horário de início
// f: Horário de término
// v: Valor do aluguel
struct res {
    int i, f, v;
};

// Cria estrutura de reserva
struct res make_res (int f, int i, int v) {
    struct res r = {f,i,v};
    return r;
}

int main(void) {
    int n, in, fi, v;
    vector <struct res> R;

    scanf("%d", &n);
    const int PE = n+1;

    Grafo g(n+2);

/************************* T1 *********************/
    // Cabeça
    R.push_back(make_res(0,0,0));

    for (int i = 1; i <= n; i++) {
        scanf("%d %d %d", &in, &fi, &v);
        R.push_back(make_res(in,fi,v));

        // Liga cabeça à i
        g.adicionaAresta(CABECA, i, -R[i].v);

        // Liga i ao pé
        g.adicionaAresta(i, PE, 0);

        // Liga j's ao i quando este começa depois de j terminar e vice-versa
        for (int j = i-1; j > CABECA; j--) {
            if (R[j].f <= R[i].i)
                g.adicionaAresta(j, i, -R[i].v);
            else if (R[i].f <= R[j].i)
                g.adicionaAresta(i, j, -R[j].v);
        }
    }

/************ Black Box ****************************/
    vector<Grafo::Edge> P;

    int dist = -g.min_path(CABECA, PE, P);

/************************* T2 *********************/
    cout << dist << endl;

    vector<Grafo::Edge>::iterator it;
    for (it = P.begin()+1; it != P.end()-1; it++)
        cout << it->x << " ";
    cout << it->x << " " << endl;

    return 0;
}
