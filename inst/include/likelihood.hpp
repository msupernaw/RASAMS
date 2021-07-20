//  likelihood.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef likelihood_h
#define likelihood_h

#include "Common.hpp"

namespace asams {

template <class T>
class likelihood_base:public model_base<T>{
    
protected:
    
public:
    virtual T evaluate(const std::vector<T>& observed, const std::vector<T>& expected)=0;
};

template <class T>
class lognormal:public likelihood_base<T>{
    
public:
    T cv_m = 1;
    
    lognormal(T cv=1){  // constructor
        this->cv_m=cv;
    }
    
    ~ lognormal(){      //destructor
    }

    
    virtual T evaluate(const std::vector<T>& observed, const std::vector<T>& expected){
        T nll1;
        T nll2;
        for(int i = 0; i < observed.size(); i++){
            T se2 = std::log(cv_m * cv_m + 1.0);
            T se = std::sqrt(se2) / std::sqrt(std::log(M_E));
            nll1 +=  std::log(se);
            // nll2 +=  std::pow((asams::log((observed / expected),2) / se) + 0.5 * se);
            nll2 +=  std::pow(((asams::log(observed) - asams::log(expected)))/se)+ 0.5 * se;
        }
        return nll1 + 0.5 * nll2;
    }
    
};

template <class T>
class multinomial:public likelihood_base<T>{

public:
    

    multinomial(){        // constructor
    }

    ~ multinomial(){      //destructor
    }


    virtual T evaluate(const std::vector<T>& observed, const std::vector<T>& expected, const std::vector<T>& sample_size, int nyears, int nages){
                 this->years = observed->imax;
                 this->ages = observed->kmax;

                 variable nll = static_cast<T> (0.0);
                 this->neff = static_cast<T> (0.0);
                 for (size_t i = 0; i < nyears; i++) {

                         variable temp_sum = 0.0;
                         variable temp1 = static_cast<T> (0.0);
                         variable temp2 = static_cast<T> (0.0);

                         variable temp3 = static_cast<T> (0.0);

                         for (size_t a = 0; a < nages; a++) {
                             size_t index = i * nages + a;
                             temp3 += (expected[index]);
                         }

                         for (size_t a = 0; a < nages; a++) {
                             size_t index = i * nages + a;
                             T obs = observed[index];

                                 //T o = observed->get_error(i, j, a) * obs; * this->ages + a;
                                 variable P_pred = (expected[index]) / temp3;
                                 temp_sum += ((obs) * asams::log(expected[index]));
                                 temp1 += P_pred * (1.0 - P_pred);
                                 temp2 += std::pow((obs - P_pred), 2.0);
                         }

                             nll += static_cast<T> (sample_size[i]) * temp_sum;


                         //this->neff += temp1 / temp2;

                     }
                 nll *= static_cast<T> (-1.0);
                 return nll;
             }

};


}


#endif /* likelihood_h */
