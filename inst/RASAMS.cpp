#include <RcppCommon.h>
#include <Rcpp.h>

class parameter{
public:
  parameter(){

  }
  parameter(double v){
   this->value = v;
  }
  double value;
  bool estimated;
};

RCPP_EXPOSED_CLASS(parameter)

    RCPP_MODULE(rasams) {
    class_<parameter>("parameter")
      .constructor()
      .constructor<double>()
      .constructor<parameter>()
      .field("value", &parameter::value);
}

