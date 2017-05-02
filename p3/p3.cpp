// Lauro Cruz e Souza - 156175
// lauro.souza@students.ic.unicamp.br | laurocruzsouza@gmail.com

/* O problema descrito pode ser resolvido encontrado a árvore geradora mínima dos
 * distritos, ligados por arestas que representam os possíveis encanamentos entre estes
 * distritos. Para representar os custos da construção de reservatórios nos distritos, 
 * podemos pensar que existe um único distrito superior que será o único a possuir um 
 * reservatório com custo nulo e pode-se construir encanamentos entre este distrito e
 * todos os outros, sendo seus custos o custo de construir um reservatório no distrito
 * inferior.
 *
 * Uma forma de representar isso sem realmente ter que adicionar esse vértice é, no 
 * algortimo de Prim, iniciar os valores de cada key[i], i sendo um distrito, com o custo
 * do reservatório, pois seria o que aconteceria no primeiro passo do algoritmo inciando-se
 * no vértice superior.
 *
 * A partir daí podemos apenas continuar a aplicar o algortimo de Prim e obteremos a
 * árvore mínima, com o vértice global imaginário como a raiz.
 *
 * A partir daí, obtemos o custo total sumando os valores das keys, que representam os valores
 * de todas as arestas presentes na árvore (ou o custo de construção do reservatórios, sendo 
 * assim um aresta que se liga no vértice superior imaginário).
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <set>

using namespace std;

// Classe que define o grafo que representa o pais com suas cidades e distritos
class Graph {

    // Estrutura de cada vértice no grafo (lista de adjacência)
    struct ver {
        int r; // Custo de construir reservatório no distrito
        vector< pair<int,int> > e;  // Possíveis encanamentos entre duas cidades
        // .first:  vértice de destino da aresta 
        // .second: peso da aresta
    };

    // Número de vértices
    int n;

    // Lista de adjacência
    vector<struct ver> *vertex; 

    public:
    // Construtor
    Graph(int n);

    // Adiciona o valor de criação do reservatório no vértice
    void updateVertex(int v, int value);

    // Cria aresta entre vértices i e j
    void addEdge(int i, int j, int w);

    // Função que utiliza o algoritmo de Prim para definir a construção dos reservatórios
    // e encanamentos. Retorna o valor mínimo da construção
    int Prim();
};

// Construtor
// n : Numero de vértices
Graph::Graph(int n) {
    this->n = n;
    this->vertex = new vector<struct ver>(n);

    // Inicialmente todos os vértices tem custo 0
    // (não é necessário no caso dos testes controlados usados no lab))
    for (int i = 0; i < n; i++)
        (*vertex)[i].r = 0;
}

// Atualiza valor de construção do reservatório no distrito
// v: Número do vértice
// value: Valor de construção
void Graph::updateVertex(int v, int value) {
    (*vertex)[v].r = value;
}


// Adiciona nova aresta (encanamento)
// {i, j}: Aresta
// w: Custo de construção do encanamento
void Graph::addEdge(int i, int j, int w)  {
    (*vertex)[i].e.push_back(make_pair(j,w));
    (*vertex)[j].e.push_back(make_pair(i,w));
}

int Graph::Prim() {
    int u;

    // Vetor da aresta de menor peso que se conecta ao vértice
    vector <int> key(this->n); 

    // Fila de prioriadade dos vértices
    // .first:  key value
    // .second: número do vértice
    set< pair<int,int> > Q;

    // Define se vértice está ou não na fila
    vector<bool> inQ(this->n, true);

    // Todos os vértices são adicionados na fila de prioridade
    // Vetor de keys é iniciado
    for (int i = 0; i < this->n; i++) {
        key[i] = (*vertex)[i].r;
        Q.insert(make_pair(key[i],i));
    }

    while (!Q.empty()) {
        // u - vértice a ser avaliado
        u = (*(Q.begin())).second;
        // retira o vértice da fila
        Q.erase(Q.begin());
        inQ[u] = false;

        // Avalia vértices adjacentes
        // Basicamente aplica o algoritmo de Prim padrão
        vector< pair<int,int> >::iterator it;
        for (it = (*vertex)[u].e.begin(); it != (*vertex)[u].e.end(); it++) {
            int v = (*it).first;
            int w = (*it).second;

            // Se o vértice adjacente está na fila de prioridade e a aresta ligada a ele é mais
            // custosa que a avaliada
            if (inQ[v] && w < key[v]) {
                // Atualiza sues valores no vetor de na fila
                Q.erase(Q.find(make_pair(key[v],v)));
                key[v] = w;
                Q.insert(make_pair(key[v],v));
            }
        }
    }

    // Obtem o valor total de construção do sistema hidríco do país
    int sum = 0;
    for (int i = 0; i < this->n; i++)
        sum += key[i];

    return sum;
}

int main() {
    int n, m;
    int w, i, j;

    // Obtem quantidade de distritos de arestas
    scanf("%d %d", &n, &m);

    // Instancia o grafo
    Graph G(n);

    // Obtem os valores de contrução dos resevatórios
    for (int k = 0; k < n; k++) {
        scanf("%d", &w);
        G.updateVertex(k, w);
    }

    // Obtem os dados dos encanamentos que podem ser construídos
    for (int k = 0; k < m; k++) {
        scanf("%d %d %d", &i, &j, &w);
        G.addEdge(i-1, j-1, w);
    }

    // Encontra a árvore geradora mínima
    cout << G.Prim() << endl;

    return 0;
}
