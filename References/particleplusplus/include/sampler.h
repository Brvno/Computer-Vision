#ifndef SAMPLER_TEMPLATE_H
#define SAMPLER_TEMPLATE_H

#include <functional>

template<class state_type, class obsv_type>
class sampler: public std::binary_function<state_type,obsv_type,state_type>
{
    public:
        sampler();
        sampler(state_type (*f)(state_type,obsv_type));
        virtual ~sampler();
        //sampler(const sampler& other);
        state_type virtual operator() (const state_type a, const obsv_type b) const {
            return f(a,b);
        }
    protected:
        state_type (*f)(state_type,obsv_type);
};


template<class state_type, class obsv_type>
sampler<state_type, obsv_type>::sampler()
{
    //ctor
}

template<class state_type, class obsv_type>
sampler<state_type, obsv_type>::sampler(state_type (*f)(state_type,obsv_type))
: f(f)
{
    //ctor
}

template<class state_type, class obsv_type>
sampler<state_type, obsv_type>::~sampler()
{
    //dtor
}

#endif // SAMPLER_TEMPLATE_H
