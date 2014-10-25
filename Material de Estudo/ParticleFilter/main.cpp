/// main.cpp
/// this example shows how to use the C++ particle filter template

# include <iostream>
# include <fstream>
# include <cmath>
# include <chrono>
# include <random>

# include "pfilter.h"  // include the template

// initialize the random seed
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);

const long double PI = 3.14159265359;
const long double alpha = 0.91;
const long double beta = 1.0;

typedef long double statetype;
typedef long double obsvtype;

std::normal_distribution <statetype> distribution(0.0,1.0);

// state pdf
long double f(statetype x1, statetype x2){
    return exp(-0.5*pow((x1-alpha*x2),2));
}

// observation pdf
long double g(statetype x, obsvtype y){
    return 1/exp(x/2)*exp(-0.5*pow(y/beta/exp(x/2),2));
}

// proposal distribution
long double q(statetype x1, statetype x2, obsvtype y){
    return exp(-0.5*pow((x1-alpha*x2),2));
}

// proposal sampling function
statetype q_sam(statetype x, obsvtype y){
    return distribution(generator)+alpha*x;
}


int main(){
    // construct the particle filter object
    pfilter <statetype,obsvtype> A(f,g,q,q_sam);

    std::ifstream in("data_y");     // data input
    std::ofstream on("data_xhat");  // data output
    in >> A;            // input data
    A.initialize(200);  // initialize with
                        // the number of particles we want to use
    A.iterate();        // run
    on << A;            // output data
    return 0;           //  we are done
}
