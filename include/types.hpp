/*
 * types.h
 */
#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <complex>
#include <boost/spirit/include/karma.hpp>
#include <boost/exception/all.hpp>

namespace types {

typedef unsigned int Uint;
typedef int Int;
typedef double Real;
typedef std::complex<Real> Complex;
typedef std::string String;

typedef std::vector<char> Binary;
typedef std::string Stream;


struct fileAccessError: virtual boost::exception, virtual std::exception {
    char const * what() const throw() { 
        std::string message = "Error accessing file: ";
        if(const std::string * f = boost::get_error_info<boost::errinfo_file_name>(*this)){
            message += *f;
        }
        else message += "(no file name supplied)";
        return message.c_str();
    }
};

typedef boost::error_info<struct tag_errinfo_parse, std::string> errinfo_parse;
struct parseError: virtual boost::exception, virtual std::exception {
    char const * what() const throw() { 
        std::string message = "Parse error: ";
        if(const std::string * f = boost::get_error_info<types::errinfo_parse>(*this)){
            message += *f;
        }
        else message += "(no further information supplied)";
        return message.c_str();
    }
};

typedef boost::error_info<struct tag_errinfo_runtime, std::string> errinfo_runtime;
struct runtimeError: virtual boost::exception, virtual std::exception {
    char const * what() const throw() { 
        std::string message = "Runtime error: ";
        if(const std::string * f = boost::get_error_info<types::errinfo_runtime>(*this)){
            message += *f;
        }
        else message += "(no further information supplied)";
        return message.c_str();
    }
};


/** 
 * A policy for outputting numbers in scientific format with defined precision
 */
template <typename t, Uint prec> struct scientific_policy : boost::spirit::karma::real_policies<t>
{
    static unsigned int precision(t) { return prec; }
    static bool trailing_zeros(t) { return true; }
    static int floatfield(t) { return boost::spirit::karma::real_policies<t>::fmtflags::scientific; }
};
/** 
 * A policy for outputting numbers with defined precision
 */
template <typename t, Uint prec> struct real_policy : boost::spirit::karma::real_policies<t>
{
    static unsigned int precision(t) { return prec; }
    static bool trailing_zeros(t) { return true; }
    static int floatfield(t) { return boost::spirit::karma::real_policies<t>::fmtflags::fixed; }
};
typedef boost::spirit::karma::real_generator<types::Real, real_policy<types::Real, 6> >
real6_type;
real6_type const real6 = real6_type();
typedef boost::spirit::karma::real_generator<types::Real, scientific_policy<types::Real, 5> >
sci5_type;
sci5_type const sci5 = sci5_type();

}

#endif
