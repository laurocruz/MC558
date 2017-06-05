// Lauro Cruz e Souza - 156175
// laurocruzsouza@gmail.com | lauro.souza@students.ic.unicamp.br

#include <iostream>
#include <cstdio>
#include <climits>
#include <vector>
#include <queue>

#define S 0
#define T (2*n + 1)

using namespace std;

// Classe que define grafo para o problema de fluxos
class Graph {

    struct edge {
        // Vertice de destino, capacidade e fluxo da iteracao atual
        int vd, c, f;
    };

    // Número de vértices
    int n;

    // Lista de adjacência
    vector< vector<struct edge> > *vertex;

    // Cria instância de aresta
    struct edge make_edge(int v, int c, int f) { 
        struct edge e = {v,c,f};
        return e;
    };

    // Busca binária para encontrar aresta (u,v)
    int searchEdge(int u, int v);

    public:
    // Construtor
    Graph(int n);

    // Cria aresta entre vértices i e j
    void addEdge(int i, int j, int c);

    // Retorna numero de vértices
    int getVertices() { return this->n; };

    // Imprime o grafo
    void printGraph();

    // Descobre se há caminho no grafo residual
    void Residual_Path(int s, int t, vector<int> & path, int & min_c);

    // Encontra fluxo máximo com o algoritmo de Edmonds-Karp
    int MaxFlux(int s, int t);
};


// Função de busca binária para encontrar aresta (u,v)
int Graph::searchEdge(int u, int v) {
    int i = 0, f = (*vertex)[u].size();

    while (i <= f) {
        int m = i + (f-i)/2;

        if ((*vertex)[u][m].vd == v)
            return m;

        if ((*vertex)[u][m].vd < v)
            i = m + 1;
        else
            f = m - 1;
    }
    return -1;
}

// Construtor
// n : Numero de vértices
Graph::Graph(int n) {
    this->n = n;
    this->vertex = new vector< vector<struct edge> >(n);
}

// Adiciona nova aresta
// {i, j}: Aresta
// c: Capacidade da aresta
void Graph::addEdge(int i, int j, int c)  {
    (*vertex)[i].push_back(make_edge(j,c,0));
}

void Graph::printGraph() {
    for (int i = 0; i < this->n; i++) {
        cout << "(" << i << ") ->";

        vector<struct edge>::iterator it;

        for (it = (*vertex)[i].begin(); it != (*vertex)[i].end(); it++)
            cout << " " << (*it).vd;
        cout << endl;

    }
}

void Graph::Residual_Path(int s, int t, vector<int> & path, int & min_c) {
    queue <int> Q;
    //vector<int> d(this->n, INT_MAX);
    vector<int> pi_e(this->n, INT_MAX);
    vector<bool> visited(this->n, false);
    int u, v, c, f, i;

    path.assign(this->n, -1);

    vector< vector<struct edge> > residual(this->n);

    // Construi grafo residual
    vector<struct edge>::iterator it;
    for (i = 0; i < this->n; i++) {
        for (it = (*vertex)[i].begin(); it != (*vertex)[i].end(); it++) {
            v = (*it).vd; 
            c = (*it).c; 
            f = (*it).f;

            // Utiliza o setor de f para o valor da aresta
            if (f < c)
                residual[i].push_back(make_edge(v,c,c-f));

            if (f > 0) 
                residual[v].push_back(make_edge(i,c,f));
        }
    }
    
    visited[s] = true;
    //d[s] = 0;
    Q.push(s);

    // BFS
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        for (it = residual[u].begin(); it != residual[u].end(); it++) {
            v = (*it).vd;
            f = (*it).f;
            if (!visited[v]) {
                visited[v] = true;
                //d[v] = d[u]+1;
                path[v] = u;
                pi_e[v] = f;
                Q.push(v);
            }
        }
    }

    // Obtem o menor valor de aresta no caminho
    min_c = INT_MAX;
    for (i = t; i != -1; i = path[i]) {
        if (pi_e[i] < min_c)
            min_c = pi_e[i];
    }
}

int Graph::MaxFlux(int s, int t) {
    int fluxo, u, v, min_c = -1, pos;
    vector<int> path(this->n);
    vector<struct edge>::iterator it;

    Residual_Path(s, t, path, min_c);

    while (min_c != INT_MAX) {

        for (int i = t; path[i] != -1; i = path[i]) {
            u = path[i];
            v = i;

            // (u,v) in E ?
            if ((pos = searchEdge(u,v)) != -1) {
                (*vertex)[u][pos].f += min_c;
            // (v,u) in E ?
            } else if ((pos = searchEdge(v,u)) != -1) {
                (*vertex)[v][pos].f -= min_c;
            }
        }

        Residual_Path(s, t, path, min_c);
    }

    // Fluxo maximo final
    fluxo = 0;
    for (it = (*vertex)[s].begin(); it != (*vertex)[s].end(); it++)
        fluxo += (*it).f;

    return fluxo;

}

int main() {
    int n, c;
    scanf("%d", &n);

    // 0       -> source
    // 2*n + 1 -> terminal
    Graph G(2*n + 2);

    for (int i = 0; i < n; i++) {
        // Liga a fonte e o sorvedouro ao vértice i de cada lado do grafo bipartido
        G.addEdge(S, i+1, 1);
        G.addEdge(i+1+n, T, 1);

        for (int j = 0; j < n; j++) {
            scanf("%d", &c);

            // Liga o vertice de um lado do grafo bipartido ao outro lado
            if (c == 1)
                G.addEdge(i+1, j+1+n, 1);
        }
    }

    //G.printGraph();

    if (G.MaxFlux(S,T) == n)
        cout  << "SIM" << endl;
    else cout << "NAO" << endl;

    return 0;
}
