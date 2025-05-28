#include "src/bap.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace bap;

int main() {
    int n;
    cin >> n;

    vector<vector<int>> custo(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> custo[i][j];

    vector<int> assignment;
    int optimalCost = solveBAP(custo, assignment);

    cout << "========================================================" << endl;
    cout << "CUSTO ÓTIMO(gargalo): " << optimalCost << endl;
    cout << "========================================================" << endl;
    cout << endl;
    for (int j = 0; j < (int)assignment.size(); ++j)
    cout << "Tarefa " << j << " → Agente " << assignment[j] << endl;
    cout << endl;
    
    auto lambda = computeLambda(custo, assignment, optimalCost);
    cout << "========================================================" << endl;
    cout << "MARGENS DE SEGURANÇA λ₍ᵢⱼ₎:" << endl;
    cout << "========================================================" << endl;
    for (int i = 0; i < (int)lambda.size(); ++i)
    for (int j = 0; j < (int)lambda[i].size(); ++j)
    if (assignment[i] == j)
    cout << "λ[" << i << "][" << j << "] = " << lambda[i][j] << endl;
    cout << "========================================================" << endl;

    return 0;
}
