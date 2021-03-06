#pragma once

#include "tools.h"
#include "reduce.h"

#include <functional>

namespace lipaboy_lib::stream_space {

	namespace operators {

		namespace {
			template <class T>
			using TSelfReduce = reduce_impl<std::function<T(T const&, T const&)> >;
		}

		struct min : TReturnSameType, TerminatedOperator
		{
			min() {}
		};

		template <class T>
		struct min_impl :
			public TSelfReduce<T>
		{
			template <class TVal>
			using RetType = typename TSelfReduce<T>::template RetType<TVal>;

		public:
			min_impl(min) :
				TSelfReduce<T>(
					[](T const& first, T const& second) -> T
					{
						return first <= second ? first : second;
					})
			{}

			template <class Stream_>
			auto apply(Stream_& obj) -> typename TSelfReduce<T>::template RetType<T>
			{
				return TSelfReduce<T>::template apply<Stream_>(obj);
			}

		};

		//-------------------------------------------------------------//
		//--------------------------Apply API--------------------------//
		//-------------------------------------------------------------//

	}

	using operators::min;
	using operators::min_impl;

	template <class TStream>
	struct shortening::TerminatedOperatorTypeApply<TStream, min> {
		using type = operators::min_impl<typename TStream::ResultValueType>;
	};

}

