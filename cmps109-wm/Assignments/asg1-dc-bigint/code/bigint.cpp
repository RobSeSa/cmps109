// $Id: bigint.cpp,v 1.76 2016-06-14 16:34:24-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   /*cout << "addition: " << uvalue << " + " << that.uvalue << endl;
   cout << "is_negative: " << is_negative 
      << " that->is_negative: " << that.is_negative << endl;
   */
   bigint result;
   result.is_negative = false;
   //when both positive
   if (is_negative == false && that.is_negative == false){
      result.uvalue = uvalue + that.uvalue;
   }
   //when this = positive and that = negative
   else if (is_negative == false && that.is_negative == true){
      if ( uvalue < that.uvalue ){
         result.uvalue = that.uvalue - uvalue;
         result.is_negative = true;
      }
      else{
         result.uvalue = uvalue - that.uvalue;
      }
   }
   //when this = negative and that = positive
   else if (is_negative == true  && that.is_negative == false){
      if ( that.uvalue < uvalue ){
         result.uvalue = uvalue - that.uvalue;;
         result.is_negative = true;
      }
      else{
         result.uvalue = that.uvalue - uvalue;
      }
   }
   //when both negative
   else{
      result.uvalue = uvalue + that.uvalue;
      result.is_negative = true;
   }
   return result;
}

bigint bigint::operator- (const bigint& that) const {
   bigint result;
   result.is_negative = false;
   //when both positive
   if (is_negative == false && that.is_negative == false){
      if ( uvalue < that.uvalue ){
         result.uvalue = that.uvalue - uvalue;
         result.is_negative = true;
      }
      else{
         result.uvalue = uvalue - that.uvalue;
      }
   }
   //when this = positive and that = negative
   else if (is_negative == false && that.is_negative == true){
      result.uvalue = uvalue + that.uvalue;
   }
   //when this = negative and that = positive
   else if (is_negative == true  && that.is_negative == false){
      result.uvalue = that.uvalue + uvalue;
      result.is_negative = true;
   }
   //when both negative
   else{
      if ( uvalue < that.uvalue || uvalue == that.uvalue ){
         result.uvalue = that.uvalue - uvalue;
      }
      else{
         result.uvalue = uvalue - that.uvalue;
         result.is_negative = true;
      }
   }
   return result;
}

bigint bigint::operator* (const bigint& that) const {
   bigint result;
   result.is_negative = false;
   result.uvalue = uvalue * that.uvalue;
   if(is_negative != that.is_negative){
      result.is_negative = true;
   }
   return result;
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result;
   result.is_negative = false;
   result.uvalue = uvalue / that.uvalue;
   if(is_negative != that.is_negative){
      result.is_negative = true;
   }
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result;
   result.uvalue = uvalue % that.uvalue;
   result.is_negative = false;
   if(is_negative == true){ 
      result.is_negative = true;
   }
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << "bigint(" << (that.is_negative ? "-" : "+")
              << "," << that.uvalue << ")";
}

