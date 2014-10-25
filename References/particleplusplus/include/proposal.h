#ifndef PROPOSAL_TEMPLATE_H
#define PROPOSAL_TEMPLATE_H

#include "ternary.h"


template<class state_type,class obsv_type>
class proposal: public ternary_function< state_type,state_type,obsv_type, precision_type >
{
    public:
        proposal();
        proposal(precision_type (*f)(state_type,state_type,obsv_type));
        virtual ~proposal();
        precision_type virtual operator()(const state_type a, const state_type b, const obsv_type c) const {
            return f(a,b,c);
        }
    protected:
        precision_type (*f)(state_type,state_type,obsv_type);
};


template<class state_type, class obsv_type>
proposal<state_type, obsv_type>::proposal()
{
    //ctor
}


template<class state_type, class obsv_type>
proposal<state_type, obsv_type>::proposal(precision_type (*f)(state_type,state_type,obsv_type))
:f(f)
{
    //ctor
}

template<class state_type, class obsv_type>
proposal<state_type, obsv_type>::~proposal()
{
    //dtor
}

#endif // PROPOSAL_TEMPLATE_H
