#include <iostream>
#include <ctime>
#include "Parameters.h"
#include "Eksperyment.hpp" 

using namespace std;

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL)));
	
	Parameters::readParameters(argc - 1, argv + 1);
    if (Parameters::runMode == Parameters::RunModes::help) {
        Parameters::help();
    }
    else if (Parameters::runMode == Parameters::RunModes::singleFile ||
        Parameters::runMode == Parameters::RunModes::benchmark) {
        przeprowadzEksperyment();
    }
    else {
        cout << "\nNie wybrano poprawnego trybu. Uzyj flagi -h, aby wyswietlic pomoc.\n";
    }
    return 0;
}