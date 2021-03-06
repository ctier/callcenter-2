#ifndef BOOST_PP_IS_ITERATING
    ///////////////////////////////////////////////////////////////////////////////
    /// \file lazy.hpp
    /// Contains definition of the lazy<> transform.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #ifndef BOOST_PROTO_TRANSFORM_LAZY_HPP_EAN_12_02_2007
    #define BOOST_PROTO_TRANSFORM_LAZY_HPP_EAN_12_02_2007

    #include <boost/proto/detail/prefix.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_params.hpp>
    #include <boost/proto/proto_fwd.hpp>
    #include <boost/proto/transform/make.hpp>
    #include <boost/proto/transform/call.hpp>
    #include <boost/proto/transform/impl.hpp>
    #include <boost/proto/detail/suffix.hpp>

    namespace boost { namespace proto
    {
        /// \brief A PrimitiveTransform that uses <tt>make\<\></tt> to build
        /// a CallableTransform, and then uses <tt>call\<\></tt> to apply it.
        ///
        /// <tt>lazy\<\></tt> is useful as a higher-order transform, when the
        /// transform to be applied depends on the current state of the
        /// transformation. The invocation of the <tt>make\<\></tt> transform
        /// evaluates any nested transforms, and the resulting type is treated
        /// as a CallableTransform, which is evaluated with <tt>call\<\></tt>.
        template<typename Object>
        struct lazy : transform<lazy<Object> >
        {
            template<typename Expr, typename State, typename Data>
            struct impl
              : call<
                    typename detail::make_<Object, Expr, State, Data>::type
                >::template impl<Expr, State, Data>
            {};
        };

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PROTO_MAX_ARITY, <boost/proto/transform/lazy.hpp>))
    #include BOOST_PP_ITERATE()

        /// INTERNAL ONLY
        ///
        template<typename Object>
        struct is_callable<lazy<Object> >
          : mpl::true_
        {};

    }}

    #endif

#else

    #define N BOOST_PP_ITERATION()

        /// \brief A PrimitiveTransform that uses <tt>make\<\></tt> to build
        /// a CallableTransform, and then uses <tt>call\<\></tt> to apply it.
        ///
        /// <tt>lazy\<\></tt> is useful as a higher-order transform, when the
        /// transform to be applied depends on the current state of the
        /// transformation. The invocation of the <tt>make\<\></tt> transform
        /// evaluates any nested transforms, and the resulting type is treated
        /// as a CallableTransform, which is evaluated with <tt>call\<\></tt>.
        template<typename Object BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct lazy<Object(BOOST_PP_ENUM_PARAMS(N, A))>
          : transform<lazy<Object(BOOST_PP_ENUM_PARAMS(N, A))> >
        {
            template<typename Expr, typename State, typename Data>
            struct impl
              : call<
                    typename detail::make_<Object, Expr, State, Data>::type
                    (BOOST_PP_ENUM_PARAMS(N, A))
                >::template impl<Expr, State, Data>
            {};
        };

    #undef N

#endif
