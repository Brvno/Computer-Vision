#ifndef OBSVFUN_TEMPLATE_H
#define OBSVFUN_TEMPLATE_H

#include <functional>

template<class state_type, class obsv_type>
class obsvfun :
    public std::binary_function<state_type, obsv_type, precision_type>
{
    public:
        obsvfun();
        obsvfun(precision_type (*f)(state_type,obsv_type));
        virtual ~obsvfun();
        precision_type virtual operator() (const state_type a, const obsv_type b) const {  return f(a,b) ;}

    protected:
        precision_type (*f)(state_type, obsv_type );


};




template<class state_type, class obsv_type>
obsvfun<state_type,obsv_type>::obsvfun()
{
    //ctor
}

template<class state_type, class obsv_type>
obsvfun<state_type,obsv_type>::obsvfun(precision_type (*f)(state_type,obsv_type))
:f(f)
{

}

template<class state_type, class obsv_type>
obsvfun<state_type,obsv_type>::~obsvfun()
{
    //dtor
}

#endif // OBSVFUN_TEMPLATE_H
