// Lauro Cruz e Souza - 156175
// lauro.souza@students.ic.unicamp.br | laurocruzsouza@gmail.com

#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>

using namespace std;

// Classe que define o grafo que representa a planilha eletrônica
class Sheet {
        // Tipo de estado de cada vértice do grafo durante DFS (resolveSheet)
        typedef enum {white, gray, black} node_color;
        // Estrutura de cada vértice no grafo (lista de adjacência)
        struct sheet_node {
            int value; // Valor do nó
            vector<int> refs; // Nós utilizados na equação deste nó

        };
        // Número de vértices
        int n;        
        // Lista de adjacência
        vector<struct sheet_node> *vertex; 
        // Soma os valores dos vértices referenciados pelo vértice v
        int sumRefs(int v); 
    public:
        // Csnstrutor
        Sheet(int n); 
        // Define o valor do vérrtice v
        void setVertexVal(int v, int val); 
        // Obtém o valor do vértice v
        int getVertexVal(int v);
        // Cria aresta adder -> addee
        void addRef(int adder, int addee);
        // Realiza o DFS que obtem os valores das equações dos nós e encontra referências circulares
        // true se não há referência circular, false caso contrário
        bool resolveSheet();
        
};

Sheet::Sheet(int n) {
    this->n = n;
    this->vertex = new vector<struct sheet_node>(n);
}

int Sheet::sumRefs(int v) {
    int sum = 0;
    vector<int>::iterator it;

    for(it = (*vertex)[v].refs.begin(); it != (*vertex)[v].refs.end(); it++) {
        sum += (*vertex)[*it].value;
    }
    return sum;
}

void Sheet::setVertexVal(int v, int val) {
    (*vertex)[v].value = val;
}
int Sheet::getVertexVal(int v) {
    return (*vertex)[v].value;
}

void Sheet::addRef(int adder, int addee) {
    (*vertex)[adder].refs.push_back(addee);
}

bool Sheet::resolveSheet() {
    stack<int> S;
    int v;
    node_color *color = new node_color[n];

    for (int i = 0; i < n; i++) color[i] = white;

    // DFS
    for (int i = 0; i < n; i++) {
        if (color[i] == white) {
            v = i;
            do {
                if (color[v] == white) {
                    // Adiciona nó da pilha e so o retira após percorrer todos os descedentes
                    color[v] = gray;
                    S.push(v);
                    vector<int>::iterator it;
                    for(it = (*vertex)[v].refs.begin(); it != (*vertex)[v].refs.end(); it++) {
                        // Se o nó já está na pilha ocorreu referência circular
                        if (color[*it] == gray) return false;
                        // Adiciona os filhos ainda não visitados
                        else if (color[*it] == white) S.push(*it);
                    }
                } else {
                    // Retira o nó da pilha pois já percorreu todos seus descendentes
                    color[v] = black;
                    // Obtem o valor do nó formado por uma equacao
                    if (!(*vertex)[v].refs.empty()) 
                        (*vertex)[v].value = sumRefs(v);
                    S.pop();
                }
                // Se a pilha não está vazia, realiza o S.top, que com +1 sempre será verdadeiro
                // Se a pilha está vazia, não realiza o S.top, evitando tentar obter o valor da
                // pilha vazia
            } while (!S.empty() && (v = S.top())+1 );
        }
    }
    return true;
}


int main() {
    int n, v, value;
    scanf("%d", &n);
    Sheet G(n);
    char c;

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &value)) {
            G.setVertexVal(i,value);
        } else {
            // Obtem valores referenciados pela equação e cria as arestas no grafo
            do {
                scanf("A%d%c", &v, &c);
                G.addRef(i,v-1);
            } while(c != '\n');
        }
    }

    if (G.resolveSheet()) {
        for (int i = 0; i < n; i++)
            printf("%d\n", G.getVertexVal(i));
    } else printf("Erro! Referencia circular.\n");

}

