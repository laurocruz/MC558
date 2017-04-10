#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Classe que define o grafo que representa a planilha eletrônica
class Graph {
        // Estrutura para arestas
        struct edge {
            int v;
            int peso;
        };

        // Estrutura de cada vértice no grafo (lista de adjacência)
        struct ver {
            bool capital;
            vector<struct edge> estradas;  // Cidades ligadas por estradas
            vector<struct edge> ferrovias; // Cidades ligadas por ferrovias
        };
        
        // Número de vértices
        int n;        

        // Lista de adjacência
        vector<struct ver> *vertex; 

        struct edge make_edge (int v, int peso) {
            struct edge e = {v, peso};
            return e;
        };

    public:
        // Construtor
        Graph(int n);

        // Cria aresta entre vértices i e j
        void addEdge(int i, int j, bool ferrovia, int peso);

};

Graph::Graph(int n) {
    this->n = n;
    this->vertex = new vector<struct ver>(n);

    for (int i = 0; i < n; i++)
        (*vertex)[i].capital = false;
}

void Graph::addEdge(int i, int j, bool ferrovia, int peso) {

    if (ferrovia) {
        (*vertex)[i].ferrovias.push_back(make_edge(j, peso));
        (*vertex)[j].ferrovias.push_back(make_edge(i, peso));

        (*vertex)[i].capital = true;
    } else {
        (*vertex)[i].estradas.push_back(make_edge(j, peso));
        (*vertex)[j].estradas.push_back(make_edge(i, peso));
    }
}

int main() {
    int n, m, p;
    int k, i, j, c;

    scanf("%d %d %d", &n, &m, &p);

    Graph G(n);

    // Estradas
    for (int k = 0; k < m; k++) {
        scanf("%d %d %d", &i, &j, &c);
        G.addEdge(i-1, j-1, false, c);
    }

    // Ferrovias
    for (k = 0; k < p; k++) {
        scanf("%d %d %d", &i, &j, &c);
        G.addEdge(i-1, j-1, true, c);
    }

    return 0;
}
