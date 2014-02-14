#ifndef BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP
#define BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include <boost/mpl/count.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>










namespace boost {
namespace di {
namespace policies {
class circular_dependencies
{
    template<typename TCallStack>
    struct is_unique_call_stack
        : mpl::bool_<
              static_cast<std::size_t>(mpl::accumulate<
                  typename mpl::transform<
                      TCallStack
                    , mpl::count<TCallStack, mpl::_>
                  >::type
                , mpl::int_<0>
                , mpl::plus<mpl::_1, mpl::_2>
              >::type::value
            ) == mpl::size<TCallStack>::value
          >
    { };

public:
    template<
        typename
      , typename T
    >
    static void assert_policy() {
       BOOST_MPL_ASSERT_MSG(is_unique_call_stack<typename T::call_stack>::value, CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED, (typename T::call_stack))



         ;
    }
};

}
}
}

#endif

