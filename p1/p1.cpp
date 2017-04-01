#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>

using namespace std;

class Sheet {
        typedef enum {white, gray, black} node_color;
        struct sheet_node {
            int value;
            vector<int> refs;

        };
        int n;
        vector<struct sheet_node> *vertex;
        int sumRefs(int v);
    public:
        Sheet(int n);
        void setVertexVal(int v, int val);
        int getVertexVal(int v);
        void addRef(int adder, int addee);
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

    for (int i = 0; i < n; i++) {
        if (color[i] == white) {
            v = i;
            do {
                //cout << "INN: Vértice: " << v << " ||| Stack size = " << S.size() << endl;
                if (color[v] == white) {
                    color[v] = gray;
                    S.push(v);
                    vector<int>::iterator it;
                    for(it = (*vertex)[v].refs.begin(); it != (*vertex)[v].refs.end(); it++) {
                        if (color[*it] == gray) return false;
                        else if (color[*it] == white) S.push(*it);
                    }
                } else {
                    color[v] = black;
                    if (!(*vertex)[v].refs.empty()) 
                        (*vertex)[v].value = sumRefs(v);
                    S.pop();
                }
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

