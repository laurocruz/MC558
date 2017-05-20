#ifndef __MINPATHCPP_H
#define __MINPATHCPP_H

#include <vector>
#include <stdlib.h>

#define INFINITO 1000000001
#define BRANCO 0
#define CINZA 1

using namespace std;

class Grafo {

    friend ostream& operator<<(ostream& output, const Grafo& p);

    public:

    struct Edge {
        int x;
        int y;
        int w;
    };

    // cria um grafo vazio
    Grafo() {
    }

    // cria um novo gravo com n vertices
    Grafo(int n) {
        for (int i = 0; i < n; i++)
            adj.push_back(vector<Edge>());
    }

    // adiciona um vertice ao grafo
    // devolve o indice do vertice adicionado
    int adicionaVertice() {
        adj.push_back(vector<Edge>());
        return adj.size()-1;
    }

    int size(void) {
        return adj.size();
    }

    // adiciona uma aresta de x a y com peso w
    // 0 <= x,y <= n-1
    void adicionaAresta(int x, int y, int w) {
        Edge e;
        e.x = x;
        e.y = y;
        e.w = w;

        adj[x].push_back(e);
    }

    void dfs_visit(int u, vector<int>& cor, vector<int>& ordem) {
        cor[u] = CINZA;


        for (vector<Edge>::iterator it = adj[u].begin(); it != adj[u].end(); it++) {
            int v = it->y;
            if (cor[v] == BRANCO) {
                dfs_visit(v, cor, ordem);
            }
        }

        ordem.push_back(u);
    }

    void dfs(vector<int>& ordem) {
        vector<int> cor(adj.size());

        for (int i = 0; i < adj.size(); i++) {
            cor[i] = BRANCO;
        }

        for (int i = 0; i < adj.size(); i++) {
            if (cor[i] == BRANCO) {
                dfs_visit(i, cor, ordem);
            }
        }
    }

    // calcula caminho minimo de s a t
    // devolve o valor do caminho minimo
    // coloca o caminho em P
    int min_path(int s, int t, vector<Edge>& P) {
        vector<int> d(adj.size());
        Edge **pai = (Edge **) malloc(adj.size()*sizeof(Edge*));

        for (unsigned int i = 0; i < adj.size(); i++) {
            d[i] = INFINITO;
            pai[i] = NULL;
        }
        d[s] = 0;

        vector<int> ordem;
        dfs(ordem);

        while (!ordem.empty()) {
            int u = ordem.back();
            ordem.pop_back();

            for (vector<Edge>::iterator it = adj[u].begin(); it != adj[u].end(); it++) {
                int v = it->y;
                if (d[v] > d[u] + it->w) {
                    pai[v] = &*it;
                    d[v] = d[u] + it->w;
                }
            }
        }

        vector<Edge> Pinv;
        for (Edge *e = pai[t]; e != NULL; e = pai[e->x])
            Pinv.push_back(*e);
        for (vector<Edge>::reverse_iterator it = Pinv.rbegin(); it != Pinv.rend(); it++)
            P.push_back(*it);
        free(pai);
        return d[t];
    }

    private:

    vector<vector<Edge> > adj;
};

ostream& operator<<(ostream& output, const Grafo& g) {
    output << "# vertices: " << g.adj.size() << endl;
    for (unsigned int i = 0; i < g.adj.size(); i++) {
        output << i << ": ";
        vector<Grafo::Edge> l = g.adj[i];
        for (vector<Grafo::Edge>::iterator it = l.begin(); it != l.end(); it++)
            output << "(" << it->x << ", " << it->y << ", " << it->w << ") ";
        output << endl;
    }
    return output;
}


#endif /* __MINPATHCPP_H */
