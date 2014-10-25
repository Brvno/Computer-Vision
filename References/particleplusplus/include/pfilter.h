#ifndef PFILTER_H
#define PFILTER_H

#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <random>

#include "statefun.h"
#include "obsvfun.h"
#include "proposal.h"
#include "sampler.h"
#include "resampler.h"
#include "binder3rd.h"
#include "compose3.h"

/// this is the particle filter template
/// To use it, the user should define the state type and observation type
/// where the operator '+', '<<' and '>>' of state type should be predefined.
/// To construct an object, the user should give four function pointers as listed below.
/// The user can also use four function objects to initialize the pfilter object.
/// In that case, automatic type conversion will be called.


template<class state_type, class obsv_type>
class pfilter
{
    public:
        pfilter(precision_type (*fptr)(state_type,state_type),
                precision_type (*gptr)(state_type, obsv_type),
                precision_type (*qptr)(state_type, state_type, obsv_type),
                state_type (*q_sam_ptr)(state_type, obsv_type)); ///< declaration of constructor

        // virtual ~pfilter();

        void iterate();
        void initialize(int pn); ///< initialize with the number of particles we want to use

    protected:
        std::vector<obsv_type>  y; ///< observation data
        std::vector<state_type> x; ///< estimated data
        std::vector<state_type> xi1; ///< particles
        std::vector<state_type> xi2; ///< particles
        std::vector<precision_type> wi; ///< weights of particles

        statefun<state_type> f; ///< pdf for state move
        obsvfun<state_type, obsv_type> g; ///< pdf for observation function
        proposal<state_type,obsv_type> q; ///< pdf of the proposal distribution
        sampler<state_type,obsv_type> q_sampler; ///< sampler of the proposal distribution
        resampler<state_type> resamp; ///< resampler

        int iternum; ///< number of iterations, automaticly determined after loading data
        int particlenum; ///< number of particles

        friend std::istream& operator >> (std::istream &i, pfilter &a){
            obsv_type t;
            while(i>>t){
                ++a.iternum;
                a.y.push_back(t);
            }
            return i;
        } ///< overload operator >> for input

        friend std::ostream& operator << (std::ostream &i, pfilter &a){
            i.precision(15);
            int n = 0;
            for(typename std::vector<state_type>::iterator itr=a.x.begin(); itr!=a.x.end(); itr++, n++){
                i<<n<<"\t"<<*itr<<std::endl;
            }
            return i;
        } ///< overload operator << for output

     private:
        /// hide these functions
        pfilter() = delete;
        pfilter( const pfilter& other) = delete;
        pfilter& operator=(const pfilter& other) = delete;

};

template<class state_type, class obsv_type>
pfilter<state_type, obsv_type>::pfilter (precision_type (*fptr)(state_type, state_type),
                                         precision_type (*gptr)(state_type, obsv_type),
                                         precision_type (*qptr)(state_type, state_type, obsv_type),
                                         state_type (*q_sam_ptr)(state_type, obsv_type)):
    f(fptr),
    g(gptr),
    q(qptr),
    q_sampler(q_sam_ptr),
    resamp(wi,xi2,xi1),
    iternum(0),
    particlenum(0)
{


}


template<class state_type, class obsv_type>
void pfilter<state_type, obsv_type>::iterate(){
    for(int n=0; n<iternum; n++){
        transform ( xi1.begin(), xi1.end(), xi2.begin(), std::bind2nd(q_sampler,y[n]) );
        transform ( xi2.begin(), xi2.end(),
                    xi1.begin(), wi.begin(),
                   bind3rd(compose3<state_type,obsv_type>(f,g,q),y[n]) );
        resamp();
        x[n] = accumulate(xi1.begin(), xi1.end(), 0.0)/particlenum;
        //std::cout.precision(15);
        //std::cout<<x[n]<<std::endl;
    }

}

template<class state_type, class obsv_type>
void pfilter<state_type, obsv_type>::initialize(int pn){
        particlenum = pn;
        x.assign(iternum,0);
        xi1.assign(particlenum,0);
        xi2.assign(particlenum,0);
        wi.assign(particlenum,0);
}

#endif // PFILTER_H
