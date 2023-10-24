/*******************************************************************************
* Author: Daniel
* Date: 02/10/2023
* Version: 1
* Description: COMPLEX
*******************************************************************************/
#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <iostream>

/******************************************************************************/
using namespace std;

namespace ilrd //we need to wrap everything in namespace
{

class Complex 
{
public:
    explicit Complex(double m_real_ = 0, double m_imag_ = 0); 

    //generated CCtor and assignment

    inline double GetReal() const;
    inline double GetImag() const; 

    inline void SetReal(double new_real_);
    inline void SetImag(double new_imag_);  

private:
    double m_real;
    double m_imag;
};

/************************       Implementation          ***********************/
Complex::Complex(double m_real_, double m_imag_)
: m_real(m_real_), m_imag(m_imag_) {}

inline double Complex::GetReal() const
{
    return m_real;
}

inline double Complex::GetImag() const 
{
    return m_imag;
}

inline void Complex::SetReal(double new_real_)
{
    m_real = new_real_;
}

inline void Complex::SetImag(double new_imag_)
{
    m_imag = new_imag_;
} 

// Operator+b_y value
inline const Complex operator+(Complex a_, const Complex&b_)
{
    return Complex(a_.GetReal() +b_.GetReal(), a_.GetImag() +b_.GetImag());
}

// (+=) Operator+b_y reference (just for the test)
inline const Complex& operator+=(Complex& a_, const Complex&b_)
{
    a_ = a_ +b_;
    return a_;
}

// (-=)
inline const Complex operator-=(Complex& a_, const Complex&b_) 
{
    a_.SetReal(a_.GetReal()-b_.GetReal());
    a_.SetImag(a_.GetImag()-b_.GetImag());

    return a_;
}

// (-)
inline const Complex operator-(const Complex& a_, const Complex&b_) 
{
    Complex ans = a_;
    ans -= b_;
    return ans;
}

// (*=)
inline const Complex operator*=(Complex& a_, const Complex&b_) 
{
    a_.SetReal(a_.GetReal()*b_.GetReal());
    a_.SetImag(a_.GetImag()*b_.GetImag());
    
    return a_;
}

// (*)
inline const Complex operator*(const Complex& a_, const Complex&b_) 
{
    Complex ans = a_;
    ans *= b_;
    return ans;
}

// (/=)
inline const Complex operator/=(Complex& a_, const Complex&b_) 
{
    a_.SetReal(a_.GetReal()/b_.GetReal());
    a_.SetImag(a_.GetImag()/b_.GetImag());
    
    return a_;
}

// (/)
inline const Complex operator/(const Complex& a_, const Complex&b_) 
{
    Complex ans = a_;
    ans /= b_;
    return ans;
}

// (==)
inline bool operator==(const Complex& a, const Complex&b_)
{
    return (a.GetReal() ==b_.GetReal()) && (a.GetImag() ==b_.GetImag());
}

// (!=)
inline bool operator!=(const Complex& a, const Complex&b_)
{
    return (a.GetReal() !=b_.GetReal()) && (a.GetImag() !=b_.GetImag());
}

std::istream& operator>>(std::istream& is_, Complex& num_)
{
    double real = 0, imag = 0;
    char open = 0, comma = 0, close = 0;

    std::cout << "Enter the complex num: (real,imag) ";
    is_ >> open >> real >> comma >> imag >> close;
    if(!is_ || open!='(' || comma!=',' || close!=')')
    {
        return is_;
    }

    num_.SetReal(real);
    num_.SetImag(imag);

    return is_;
}

std::ostream& operator<<(std::ostream& os_, const Complex& complex) 
{
    os_ << complex.GetReal() << ", " << complex.GetImag();
    return os_;
}

} // namespace ilrd


#endif    /*__COMPLEX_H__*/

