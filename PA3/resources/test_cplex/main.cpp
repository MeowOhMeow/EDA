#define IL_STD
#include <iostream>

#include <ilcplex/ilocplex.h>



using namespace std;
ILOSTLBEGIN

int main() {
    
    IloEnv env;
    try {
        IloModel model(env);
        IloCplex cplex(model);

        // Define decision variables
        IloNumVar x(env, 0.0, IloInfinity, ILOFLOAT);
        IloNumVar y(env, 0.0, IloInfinity, ILOFLOAT);

        model.add(IloMaximize(env, 3 * x + 2 * y)); // Objective function

        // Add constraints
        model.add(2 * x + y <= 8);
        model.add(x + 2 * y <= 6);

        // Solve the problem
        cplex.solve();

        // Print solution
        if (cplex.getStatus() == IloAlgorithm::Optimal) {
            std::cout << "Objective Value: " << cplex.getObjValue() << std::endl;
            std::cout << "x = " << cplex.getValue(x) << std::endl;
            std::cout << "y = " << cplex.getValue(y) << std::endl;
        } else {
            std::cout << "No solution found" << std::endl;
        }
    } catch (IloException& ex) {
        std::cerr << "Error: " << ex << std::endl;
    }
    env.end();
    return 0;
}

