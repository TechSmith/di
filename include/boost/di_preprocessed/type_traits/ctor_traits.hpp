#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include "boost/di_preprocessed/inject.hpp"
#include "boost/di_preprocessed/type_traits/parameter_types.hpp"
#include "boost/di_preprocessed/type_traits/has_ctor.hpp"
#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/aux_/yes_no.hpp>











namespace boost {
namespace di {
namespace detail {

struct any_type { };

template<typename T>
struct get_int
{
    typedef mpl::int_<T::value> type;
};

template<typename T>
struct get_longest_ctor
    : mpl::fold<
        mpl::range_c<int, 1, 20 + 1>
      , mpl::int_<0>
      , mpl::if_<
            type_traits::has_ctor<T, get_int<mpl::_2> >
          , mpl::_2
          , mpl::_1
        >
      >::type
{ };

}

template<typename T>
struct ctor_traits
    : mpl::fold<
          mpl::range_c<int, 0, detail::get_longest_ctor<T>::value>
        , mpl::vector0<>
        , mpl::push_back<mpl::_1, detail::any_type>
      >
{ };

template<typename T>
struct ctor_traits<std::basic_string<T> >
{
    BOOST_DI_INJECT_TRAITS();
};

namespace type_traits {

template<typename T>
class has_BOOST_DI_INJECTOR
{
    struct base_impl { void BOOST_DI_INJECTOR(...) { } };
    struct base
        : base_impl
        , mpl::if_<is_class<T>, T, mpl::void_>::type
    { base() { } };

    template<typename U>
    static mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::BOOST_DI_INJECTOR>* = 0
    );

    static mpl::aux::yes_tag test(...);

public:
    typedef has_BOOST_DI_INJECTOR type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<has_BOOST_DI_INJECTOR<di::ctor_traits<T> > >::type>
    : parameter_types<BOOST_TYPEOF_TPL(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<has_BOOST_DI_INJECTOR<T> >::type>
    : parameter_types<BOOST_TYPEOF_TPL(&T::BOOST_DI_INJECTOR)>::type
{ };

}
}
}

#endif

