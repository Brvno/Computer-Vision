#ifndef STATEFUN_TEMPLATE_H
#define STATEFUN_TEMPLATE_H

#include <functional>

template<class state_type>
class statefun :
    public std::binary_function<state_type, state_type, precision_type>
{
    public:
        statefun();
        statefun(precision_type (*f)(state_type,state_type)); //!< automatic type conversion
        virtual ~statefun();
        precision_type virtual operator()(const state_type a,const state_type b) const {
            return f(a,b);
        }
    protected:
        precision_type (*f)(state_type,state_type);
};


template<class state_type>
statefun<state_type>::statefun()
{
    //ctor
}


template<class state_type>
statefun<state_type>::statefun(precision_type (*f)(state_type,state_type))
:f(f)
{
    //ctor
}

template<class state_type>
statefun<state_type>::~statefun()
{
    //dtor
}

#endif // STATEFUN_TEMPLATE_H
