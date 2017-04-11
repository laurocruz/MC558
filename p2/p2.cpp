#include <cstdio>
#include <climits>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

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

    //Número de ferrovias
    int f;

    // Lista de adjacência
    vector<struct ver> *vertex; 

    struct edge make_edge (int v, int peso) {
        struct edge e = {v, peso};
        return e;
    };

    /*
       void relax (int * dv, int * du, int u, int wv, pair<int,bool> * piv, bool ferrovia) {
       if (*dv > *du + wv) {
     *dv = *du + wv;
     *piv = make_pair(u, ferrovia);
     }
     }
     */

    public:
    // Construtor
    Graph(int n, int f);

    // Cria aresta entre vértices i e j
    void addEdge(int i, int j, bool ferrovia, int peso);

    int removeFerrovias();

};

Graph::Graph(int n, int f) {
    this->n = n;
    this->f = f;
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

int Graph::removeFerrovias() {
    int u;
    int fers = this->f;
    // Vetor de distâncias
    vector<int> d(this->n, INT_MAX);

    // Vetor de quantidade de ferrovias até cidade
    vector<int> n_fers(this->n, 0);

    //Vetor dos pais. Aresta = (pi[x], x)
    //.first: pai de i
    //.second: ferrovia?
    vector< pair <int,bool> > pi(this->n, make_pair(-1,false));

    // Fila de prioridade dos vértices
    // .first: peso
    // .second: numero do vertice
    set< pair<int,int> > Q;

    // Define se vertice está ou não na fila
    vector< bool > inQ(this->n, true);

    // Distancias das capitais são zeradas, como se houvesse uma capital superior ligando 
    // todas elas com estrada de distancia 0, e a usando de raiz
    for (int i = 0; i < this->n; i++) {
        if ((*vertex)[i].capital) {
            d[i] = 0;
        }
        Q.insert(make_pair(d[i],i));
    }

    /*
    if (this->n < 100) {
        set< pair<int,int> >::iterator is;
        for (is = Q.begin(); is != Q.end(); is++) {
            cout << (*is).second+1 << ": " << (*is).first << endl;
        }
        cout << endl;
    }
    */

    while (!Q.empty()) {
        // u - Vertice a ser avaliado
        u = (*(Q.begin())).second;
        Q.erase(Q.begin());
        inQ[u] = false;

        // Avalia estradas
        vector<struct edge>::iterator it;
        for (it = (*vertex)[u].estradas.begin(); it != (*vertex)[u].estradas.end(); it++) {
            // Obtem cidade v e distância c até ela
            int v = (*it).v;
            int c = (*it).peso;

            // relax
            if (d[v] > d[u] + c) {
                // Atualiza distância
                if (inQ[v])
                    Q.erase(Q.find(make_pair(d[v],v)));

                d[v] = d[u] + c;
                n_fers[v] = n_fers[u];
                pi[v].first = u;
                pi[v].second = false;

                Q.insert(make_pair(d[v],v));
                inQ[v] = true;

            } else if ((d[v] == d[u] + c) && (n_fers[v] > n_fers[u])) {
                if (!inQ[v]) {
                    Q.insert(make_pair(d[v],v));
                    inQ[v] = true;
                }

                n_fers[v] = n_fers[u];
                pi[v].first = u;
                pi[v].second = false;
            }
        }

        // Avalia ferrovias
        for (it = (*vertex)[u].ferrovias.begin(); it != (*vertex)[u].ferrovias.end(); it++) {
            // Obtem cidade v e distância c até ela
            int v = (*it).v;
            int c = (*it).peso;

            // relax

            if (d[v] > d[u] + c) {
                // Atualiza distância
                if (inQ[v])
                    Q.erase(Q.find(make_pair(d[v],v)));

                d[v] = d[u] + c;
                n_fers[v] = n_fers[u] + 1;
                pi[v].first = u;
                pi[v].second = true;

                Q.insert(make_pair(d[v],v));
                inQ[v] = true;

            } else if ((d[v] == d[u] + c) && (n_fers[v] > n_fers[u] + 1)) {
                if (!inQ[v]) {
                    Q.insert(make_pair(d[v],v));
                    inQ[v] = true;
                }

                n_fers[v] = n_fers[u] + 1;
                pi[v].first = u;
                pi[v].second = true;

            }
        }
        /*
        if (this->n < 100) {
            set< pair<int,int> >::iterator is;
            for (is = Q.begin(); is != Q.end(); is++) {
                cout << (*is).second+1 << ": " << (*is).first << endl;
            }
            cout << endl;
        }
        */

    }

    for (int i = 0; i < n; i++) {
        if (pi[i].second)
            fers--;
    }

    return fers;
}

int main() {
    int n, m, p;
    int k, i, j, c;

    scanf("%d %d %d", &n, &m, &p);

    Graph G(n, p);

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

    cout << G.removeFerrovias() << endl;

    return 0;
}
