// Lauro Cruz e Souza - 156175
// laurocruzsouza@gmail.com | lauro.souza@students.ic.unicamp.br

#include <iostream>
#include <iomanip>
#include <vector>
#include <lemon/lp.h>

using namespace std;
using namespace lemon;

#define ANO 12

void makePlan(double h, double c, double rp, double tp, double tb, double *rb, double *ra, double *d);

int main() {
    int n;
    double h, c, rp, tp, tb;
    double rb[ANO], ra[ANO], d[ANO];

    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> h >> c >> rp >> tp >> tb;

        for (int j = 0; j < ANO; j++) cin >> rb[j];
        for (int j = 0; j < ANO; j++) cin >> ra[j];
        for (int j = 0; j < ANO; j++) cin >> d[j];

        cout << "Caso " << i+1 << ":" << endl;
        makePlan(h,c,rp,tp,tb,rb,ra,d);
    }

    return 0;

}

void makePlan(double h, double c, double rp, double tp, double tb, double *rb, double *ra, double *d) {
    Lp lp;

    vector<Lp::Col> x(ANO+1), CC(ANO+1), P(ANO+1), CDB(ANO+1), AC(ANO+1);
    vector<Lp::Col> invP(ANO), invCDB(ANO), invAC(ANO);
    vector<Lp::Col> retP(ANO), retCDB(ANO), retAC(ANO);

    lp.addColSet(x);
    lp.addColSet(CC);
    lp.addColSet(P);
    lp.addColSet(CDB);
    lp.addColSet(AC);
    lp.addColSet(invP);
    lp.addColSet(invCDB);
    lp.addColSet(invAC);
    lp.addColSet(retP);
    lp.addColSet(retCDB);
    lp.addColSet(retAC);

    lp.max();

    Lp::Expr obj = h;
    for (int i = 0; i < ANO+1; i++)
        obj += x[i];
    lp.obj(obj);

    lp.addRow(x[0] == (-1)*d[0]);

    lp.addRow(((tp/100)+1)*retP[0] <= ((rp/100)+1)*P[0]);
    lp.addRow(((tb/100)+1)*retCDB[0] <= ((rb[0]/100)+1)*CDB[0]);
    lp.addRow(retAC[0] <= ((ra[0]/100)+1)*AC[0]);

    lp.addRow(invP[0] + invCDB[0] + invAC[0] <= h - d[0]);

    lp.addRow(CC[0] == h - invP[0] - invCDB[0] - invAC[0] - d[0]);
    lp.addRow(P[0] == invP[0]+0);
    lp.addRow(CDB[0] == invCDB[0]+0);
    lp.addRow(AC[0] == invAC[0]+0);

    lp.colLowerBound(x[0],-h);

    for (int i = 1; i < ANO; i++) {

        lp.addRow(x[i] == CC[i]+P[i]+CDB[i]+AC[i]-CC[i-1]-P[i-1]-CDB[i-1]-AC[i-1]);

        lp.addRow(x[i] >= (-1)*(CC[i-1]+P[i-1]+CDB[i-1]+AC[i-1]));

        lp.addRow(((tp/100)+1)*retP[i] <= ((rp/100)+1)*P[i]);
        lp.addRow(((tb/100)+1)*retCDB[i] <= ((rb[i]/100)+1)*CDB[i]);
        lp.addRow(retAC[i] <= ((ra[i]/100)+1)*AC[i]);

        lp.addRow(invP[i] + invCDB[i] + invAC[i] <= CC[i-1] + retP[i-1] + retCDB[i-1] + retAC[i-1] - c - d[i]);

        lp.addRow(CC[i] == CC[i-1] + retP[i-1] + retCDB[i-1] + retAC[i-1] - invP[i] - invCDB[i] - invAC[i] - c - d[i]);
        lp.addRow(P[i] == ((rp/100)+1)*P[i-1] + invP[i] - ((tp/100)+1)*retP[i-1]);
        lp.addRow(CDB[i] == ((rb[i-1]/100)+1)*CDB[i-1] + invCDB[i] - ((tb/100)+1)*retCDB[i-1]);
        lp.addRow(AC[i] == ((ra[i-1]/100)+1)*AC[i-1] + invAC[i] - retAC[i-1]);

    }

    lp.addRow(x[12] == CC[12]+P[12]+CDB[12]+AC[12]-CC[11]-P[11]-CDB[11]-AC[11]);

    lp.addRow(x[12] >= (-1)*(CC[11]+P[11]+CDB[11]+AC[11]));

    lp.addRow(CC[12] == CC[11] + retP[11] + retCDB[11] + retAC[11] - c);
    lp.addRow(P[12] == ((rp/100)+1)*P[11] - ((tp/100)+1)*retP[11]);
    lp.addRow(CDB[12] == ((rb[11]/100)+1)*CDB[11] - ((tb/100)+1)*retCDB[11]);
    lp.addRow(AC[12] == ((ra[11]/100)+1)*AC[11] - retAC[11]);

    lp.colLowerBound(CC,0);
    lp.colLowerBound(P,0);
    lp.colLowerBound(CDB,0);
    lp.colLowerBound(AC,0);
    lp.colLowerBound(invP,0);
    lp.colLowerBound(invCDB,0);
    lp.colLowerBound(invAC,0);
    lp.colLowerBound(retP,0);
    lp.colLowerBound(retCDB,0);
    lp.colLowerBound(retAC,0);

    lp.solve();

    cout << fixed;
    if (lp.primalType() == Lp::OPTIMAL) {
        cout << "Mes 1/2018:" << endl;
        cout << "investir na poupanca " << setprecision(2) << lp.primal(invP[0]) << " dinheiros" << endl;
        cout << "retirar da poupanca 0.00 dinheiros" << endl;
        cout << "investir no CDB " << setprecision(2) << lp.primal(invCDB[0]) << " dinheiros" << endl;
        cout << "retirar do CDB 0.00 dinheiros" << endl;
        cout << "investir nas acoes " << setprecision(2) << lp.primal(invAC[0]) << " dinheiros" << endl;
        cout << "retirar das acoes 0.00 dinheiros" << endl;

        for (int i = 1; i < ANO; i++) {
            cout << "Mes " << i+1 << "/2018:" << endl;
            cout << "investir na poupanca " << setprecision(2) << lp.primal(invP[i]) << " dinheiros" << endl;
            cout << "retirar da poupanca " << setprecision(2) << lp.primal(retP[i-1]) << " dinheiros" << endl;
            cout << "investir no CDB " << setprecision(2) << lp.primal(invCDB[i]) << " dinheiros" << endl;
            cout << "retirar do CDB " << setprecision(2) << lp.primal(retCDB[i-1]) << " dinheiros" << endl;
            cout << "investir nas acoes " << setprecision(2) << lp.primal(invAC[i]) << " dinheiros" << endl;
            cout << "retirar das acoes " << setprecision(2) << lp.primal(retAC[i-1]) << " dinheiros" << endl;
        }

        cout << "Mes 1/2019:" << endl;
        cout << "retirar da poupanca " << setprecision(2) << lp.primal(retP[11]) << " dinheiros" << endl;
        cout << "retirar do CDB " << setprecision(2) << lp.primal(retCDB[11]) << " dinheiros" << endl;
        cout << "retirar das acoes " << setprecision(2) << lp.primal(retAC[11]) << " dinheiros" << endl;

        cout << "saldo da heranca em 2019: " << setprecision(2) << lp.primal() << " dinheiros" << endl;

    } else cout << "Solucao impossivel ou ilimitada" << endl;

}
