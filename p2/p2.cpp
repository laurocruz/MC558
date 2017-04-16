// Lauro Cruz e Souza - 156175
// lauro.souza@students.ic.unicamp.br | laurocruzsouza@gmail.com

/* A ideia do laboratório é encontrar a capital mais próxima de cada cidade comum 
 * e o caminho entre elas. Assim, tendo esses caminhos mínimos, podemos definir quais 
 * são as ferrovias essenciais para que esses caminhos sejam mantidos, como foi
 * pedido no enunciado.
 *
 * A ideia da resolução, como foi sugerida no enunciado, pode ser pensada inicialmente 
 * para uma única capital e extrapolada para várias. No caso de uma capital, podemos
 * aplicar normalmente o algoritmo de dijkstra com a raiz da árvore sendo a capital,
 * assim qualquer ferrovia não presente na árvore de distâncias mínimas pode ser 
 * desativada.
 *
 * Para várias capitais, podemos imaginar a existência de uma única capital superior
 * que é conectada apenas a todas as outras capitais com estradas de distância 0 e
 * chamar a função de dijkstra com essa capital como raiz. Assim podemos garantir 
 * que cada caminho de uma cidade à capital é à capital mais próxima, pois, se 
 * chamarmor a capital superior de CF e as inferiores de CEi, sendo i o número da
 * capital, o caminho CF--C é mínimo, ou seja, CF-CEi--C é mínimo. Assim, se existe j
 * para o qual d(CEi--C) > d(CEj--C), então d(CF-CEi--C) > d(CF-CEj--C), tendo assim 
 * uma contradição.
 *
 * Para simular a existência desta capital superior, podemos apenas colocar as 
 * distâncias das capitais como 0 e rodar o algoritmo de dijkstra começando por uma
 * delas.
 *
 * Outras condição a ser verificada é se, entre dois caminhos de mesma distância a duas 
 * capitais diferente, fazer o relax de v (u-v) se a aresta entre u e v for uma estrada.
 * Isso é importante pois, entre uma cidade e sua capital mais próxima pode existir 
 * apenas uma ferrovia, pois pelo menos uma das pontas de cada ferrovia é uma capital.
 * Assim, é importante fazer o relax, pois caso o pai de v fosse uma ferrovia, estaremos
 * retirando uma das ferrovias da árvore de caminhos mínimos.
*/

#include <cstdio>
#include <climits>
#include <iostream>
#include <vector>
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

    // Cria aresta
    struct edge make_edge (int v, int peso) {
        struct edge e = {v, peso};
        return e;
    };

    public:
    // Construtor
    Graph(int n, int f);

    // Cria aresta entre vértices i e j
    void addEdge(int i, int j, bool ferrovia, int peso);

    // Função que utiliza Dijkstra para definir ferrovias a serem desativadas
    int removeFerrovias();

};

// Construtor
// n : Numero de vértices
// f: Número de ferrovias
Graph::Graph(int n, int f) {
    this->n = n;
    this->f = f;
    this->vertex = new vector<struct ver>(n);

    // Inicialmente nenhuma das cidades é capital
    for (int i = 0; i < n; i++)
        (*vertex)[i].capital = false;
}

// Adiciona nova aresta (estrada ou ferrovia
// (i, j): aresta
// ferrovia: true se sim, false caso contrário
// peso: distância (peso) da estrada/ferrovia (aresta)
void Graph::addEdge(int i, int j, bool ferrovia, int peso) {
    if (ferrovia) {
        (*vertex)[i].ferrovias.push_back(make_edge(j, peso));
        (*vertex)[j].ferrovias.push_back(make_edge(i, peso));

        // primeiro vértice (i) da aresta da ferrovia é capital
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
            if (d[v] >= d[u] + c && inQ[v]) {
                int dv = d[v];

                Q.erase(Q.find(make_pair(dv,v)));

                // Atualiza distância
                d[v] = d[u] + c;
                // Atualiza pai de v (u)
                pi[v].first = u;
                // v e u são ligador por estrada
                pi[v].second = false;

                Q.insert(make_pair(d[v],v));
            }
        }

        // Avalia ferrovias
        for (it = (*vertex)[u].ferrovias.begin(); it != (*vertex)[u].ferrovias.end(); it++) {
            // Obtem cidade v e distância c até ela
            int v = (*it).v;
            int c = (*it).peso;

            // relax
            if (d[v] > d[u] + c) {
                
                Q.erase(Q.find(make_pair(d[v],v)));
                // Atualiza distância
                d[v] = d[u] + c;
                // Atualiza pai de v (u)
                pi[v].first = u;
                // v e u são ligados por ferrovia
                pi[v].second = true;

                Q.insert(make_pair(d[v],v));
            }
        }
    }

    // Todas as ferrovias não presentes na árvore de distâncias
    // mínimas podem ser retiradas
    for (int i = 0; i < this->n; i++) {
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
