// Lauro Cruz e Souza - 156175
// laurocruzsouza@gmail.com | lauro.souza@students.ic.unicamp.br

#include <iostream>
#include <cstdio>
#include <climits>
#include <queue>

// Classe que define grafo para o problema de fluxos
class Graph {

    /*
    // Estrutura de cada vértice no grafo (lista de adjacência)
    struct ver {
        int r; // Custo de construir reservatório no distrito
        vector< pair<int,int> > e;  // Possíveis encanamentos entre duas cidades
        // .first:  vértice de destino da aresta 
        // .second: peso da aresta
    };
    */

    struct edge {
        // Vertice de destino, capacidade e fluxo da iteracao atual
        int vd, c, f;
    };

    // Número de vértices
    int n;

    // Lista de adjacência
    vector< vector<struct edge> > *vertex; 

    struct edge make_edge(int v, int c, int f) { 
        struct edge e = {v,c,f};
        return e;
    };
    
    void addVertex() {
        (*vertex).push_back(new vector<struct edge);
        this->n += 1;
    }

    public:
    // Construtor
    Graph(int n);

    // Cria aresta entre vértices i e j
    void addEdge(int i, int j, int c);

    int getVertices() { return this->n; };

    vector<int> Residual_Path(int s, int t, int &min_c);

    // Função que utiliza o algoritmo de Prim para definir a construção dos reservatórios
    // e encanamentos. Retorna o valor mínimo da construção
    int MaxFlux();
};

// Construtor
// n : Numero de vértices
Graph::Graph(int n) {
    this->n = n;
    this->vertex = new vector< vector<struct edge> >(n);

    // Cria vetores de arestas
    for (int i = 0; i < n; i++)
        (*vertex)[i] = new vector<struct edge>;
}

// Adiciona nova aresta
// {i, j}: Aresta
// c: Capacidade da aresta
void Graph::addEdge(int i, int j, int c)  {
    (*vertex)[i].push_back(make_edge(j,c,0));
}

vector<int> Graph::Residual_Path(int s, int t, int &min_c) {
    queue <int> Q;
    //vector<int> d(this->n, INT_MAX);
    vector< pair<int,int> > pi(this->n, make_pair(-1,-1));
    vector<bool> visited(this->n, false);
    vector<int> path;
    int u, v, c, f, i;

    vector< vector<struct edge> > residual(this->n);

    for (i = 0; i < this->n; i++) {
        residual[i] = new vector<struct edge>;
    }

    // Construi grafo residual
    vector< struct egde >::iterator it;
    for (i = 0; i < this->n; i++) {
        for (it = (*vertex)[i].begin(); it != (*vertex)[i].end(); it++) {
            v = (*it).vd; 
            c = (*it).c; 
            f = (*it).f;

            if (f < c)
                residual[i].push_back(make_edge(v,c,c-f));

            if (f > 0) 
                residual[v].push_back(make_edge(i,c,f));
        }
    }
    
    visited[s] = true;
    //d[s] = 0;
    Q.push(s);

    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        for (it = residual[i].begin(); it != residual[i].end(); it++) {
            v = (*it).vd;
            f = (*it).f;
            if (!visited[v]) {
                visited[v] = true;
                //d[v] = d[u]+1;
                pi[v] = make_pair(u,f);
                Q.push(v);
            }
        }
    }

    min_c = -1;
    for (i = t; i != -1; i = pi[i].first) {
        path.insert(path.begin(), i);
        if (pi[i].second < min_c)
            min_c = pi[i].second;
    }

    return path;
}

int Graph::MaxFlux(int s, int t) {
    int fluxo = 0;
    int min_c = -1;
    vector <int> *path;
    bool found;

    path = Residual_Path(s, t, &min_c);

    while (min_c != -1) {
        vector<struct edge>::iterator it;

        for (int i = 0; i < (*path).length()-1; i++) {
            u = (*path)[i];
            v = (*path)[i+1];
            found = false;

            // (u,v) in E
            for (it = (*vertex)[u].begin(); !found && it != (*vertex).end(); it++) {
                if ((*it).vd == v) {
                    found = true;
                    (*it).f += min_c;
                }
            }

            for (it = (*vertex)[v].begin(); !found && it != (*vertex).end(); i++) {
                if ((*it).vd == u) {
                    found == true;
                    (*it).f -= min_c;
                }
            }
        }

        fluxo = 0;
        for (it = (*vertex)[s].begin(); it != (*vertex).end(); i++)
            fluxo += (*it).f;

        path = Residual_Path(s, t, &min_c);
    }

    return fluxo;

}

int main() {
    int n, c;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &c);

            if (c == 1) {

            }
        }
    }

    return 0;
}
