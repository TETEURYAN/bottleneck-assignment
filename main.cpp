#include "src/bap.hpp"
#include "src/meta-heuristic/ils.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace bap;
using namespace ils;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <opção>\n"
             << "Onde <opção> é:\n"
             << "1 - para executar BAP\n"
             << "2 - para executar ILS\n";
        return 1;
    }

    int mode = atoi(argv[1]); 
    
    int n;
    cin >> n;

    
    CostMatrix custo(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
    cin >> custo[i][j];
    
    vector<int> assignment;
    int optimalCost = solveBAP(custo, assignment);

    if (mode == 1) {
        auto start = chrono::high_resolution_clock::now();

        cout << "========================================================" << endl;
        cout << "CUSTO ÓTIMO (gargalo): " << optimalCost << endl;
        cout << "========================================================\n\n";

        for (int j = 0; j < (int)assignment.size(); ++j)
            cout << "Tarefa " << j << " → Agente " << assignment[j] << endl;

        cout << "\n========================================================" << endl;
        cout << "MARGENS DE SEGURANÇA λ₍ᵢⱼ₎:" << endl;
        cout << "========================================================" << endl;

        auto lambda = computeLambda(custo, assignment, optimalCost);
        for (int i = 0; i < (int)lambda.size(); ++i)
            for (int j = 0; j < (int)lambda[i].size(); ++j)
                if (assignment[i] == j)
                    cout << "λ[" << i << "][" << j << "] = " << lambda[i][j] << endl;

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << "\n========================================================" << endl;
        cout << "TEMPO DE EXECUÇÃO DO BAP: " << duration.count() << " segundos" << endl;
        cout << "========================================================\n\n";

    } else if (mode == 2) {
        auto start = chrono::high_resolution_clock::now();

        // Etapa 1: solução inicial exata

        // Etapa 2: margens de robustez estimadas com VND
        auto lambdaInit = vnd_heuristica(custo, assignment, optimalCost);

        // Etapa 3: aplicação do ILS
        ILSSolver solver;
        int finalGargalo;
        vector<vector<int>> finalLambda;
        vector<int> improved = solver.solve_ils(custo, 100, optimalCost, assignment, lambdaInit, finalGargalo, finalLambda);

        cout << "========================================================" << endl;
        cout << "CUSTO ÓTIMO COM ILS (gargalo): " << finalGargalo << endl;
        cout << "========================================================\n\n";

        for (int j = 0; j < (int)improved.size(); ++j)
            cout << "Tarefa " << j << " → Agente " << improved[j] << endl;

        cout << "\n========================================================" << endl;
        cout << "MARGENS DE SEGURANÇA λ₍ᵢⱼ₎:" << endl;
        cout << "========================================================" << endl;

        for (int i = 0; i < (int)finalLambda.size(); ++i)
            for (int j = 0; j < (int)finalLambda[i].size(); ++j)
                if (improved[i] == j)
                    cout << "λ[" << i << "][" << j << "] = " << finalLambda[i][j] << endl;

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << "\n========================================================" << endl;
        cout << "TEMPO DE EXECUÇÃO DO ILS: " << duration.count() << " segundos" << endl;
        cout << "========================================================\n\n";
    } else if (mode == 3) {
        // ============================== MODO VND PURO ==============================
        auto start = chrono::high_resolution_clock::now();

        cout << "========================================================" << endl;
        cout << "EXECUTANDO VND COM BASE NA SOLUÇÃO ÓTIMA" << endl;
        cout << "========================================================\n\n";

        auto lambda = vnd_heuristica(custo, assignment, optimalCost);

        cout << "CUSTO BASE (gargalo): " << optimalCost << endl;

        cout << "\n========================================================" << endl;
        cout << "MARGENS DE SEGURANÇA λ₍ᵢⱼ₎ (VND):" << endl;
        cout << "========================================================" << endl;

        for (int i = 0; i < (int)lambda.size(); ++i)
            for (int j = 0; j < (int)lambda[i].size(); ++j)
                if (assignment[i] == j)
                    cout << "λ[" << i << "][" << j << "] = " << lambda[i][j] << endl;

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << "\n========================================================" << endl;
        cout << "TEMPO DE EXECUÇÃO DO VND: " << duration.count() << " segundos" << endl;
        cout << "========================================================\n\n";
    }

    return 0;
}
