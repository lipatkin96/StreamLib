#pragma once

#include "stream_extended.h"
#include "extra_tools/producing_iterator.h"
#include "extra_tools/initializer_list_iterator.h"
#include "extra_tools/sequence_producing_iterator.h"

#include <functional>
#include <iterator>
#include <type_traits>

namespace lipaboy_lib::stream_space {

	using lipaboy_lib::ProducingIterator;
	using lipaboy_lib::InitializerListIterator;
    using lipaboy_lib::SequenceProducingIterator;

	//--------------------------Stream Base (specialization class)----------------------//

	template <class TIterator>
	class StreamBase<TIterator> {
	public:
		using T = typename std::iterator_traits<TIterator>::value_type;
		using ValueType = T;
		using size_type = size_t;
		using outside_iterator = TIterator;
		using GeneratorType = std::function<ValueType(void)>;
		using SequenceGeneratorType = std::function<ValueType(ValueType)>;

	public:
		template <class Functor>
		using ExtendedStreamType = StreamBase<Functor, TIterator>;

		using ResultValueType = ValueType;

	public:
		// INFO: this friendship means that all the Streams, which is extended from current,
		//		 can have access to current class method API.
		template <typename, typename...> friend class StreamBase;

	public:
		//----------------------Constructors----------------------//

		template <class OuterIterator>
		explicit
			StreamBase(OuterIterator begin, OuterIterator end)
				: begin_(begin),
				end_(end)
		{}
		explicit
			StreamBase(std::initializer_list<T> init)
				: begin_(init)
		{
			if constexpr (StreamBase::isInitializingListCreation())
				end_ = begin_.endIter();
		}
		explicit
			StreamBase(GeneratorType generator)
				: begin_(generator),
				end_()
		{}
		explicit
			StreamBase(ValueType initValue, SequenceGeneratorType generator)
				: begin_(initValue, generator),
				end_()
		{}

		//----------------------Methods API-----------------------//

	protected:
		static constexpr bool isNoFixSizeOperatorBefore() { return true; }
		static constexpr bool isGeneratorProducing() {
            return std::is_same_v<TIterator, ProducingIterator<ValueType> >
                    || std::is_same_v<TIterator, SequenceProducingIterator<ValueType> >;
		}
		static constexpr bool isInitializingListCreation() {
			return std::is_same_v<TIterator, InitializerListIterator<ValueType>
				//typename std::initializer_list<ValueType>::iterator
			>;
		}
		static constexpr bool isOutsideIteratorsRefer() {
			return !isGeneratorProducing() && !isInitializingListCreation();
		}

	public:
		inline static constexpr bool isInfinite() {
			return isGeneratorProducing() && isNoFixSizeOperatorBefore();
		}
		template <class TStream_>
		inline static constexpr void assertOnInfinite() {
			static_assert(!TStream_::isInfinite(),
				"Stream error: attempt to work with infinite stream");
		}

	protected:
		// Info:
		// illusion of protected (it means that can be replace on private)
		// (because all the variadic templates are friends
		// from current Stream to first specialization) (it is not a real inheritance)


		//-----------------Slider API--------------//
	public:
		ResultValueType nextElem() {
			auto elem = //std::forward<T>(
				*begin_;
				//);
			begin_++;
			return elem;
		}
		bool hasNext() { return begin_ != end_; }
		void incrementSlider() { begin_++; }

		//-----------------Slider API Ends--------------//

	public:
		bool operator==(StreamBase const & other) const { return equals(other); }
		bool operator!=(StreamBase const & other) const { return !((*this) == other); }
	private:
		bool equals(StreamBase const & other) const {
			return begin_ == other.begin_
				&& end_ == other.end_;
		}

	private:
		TIterator begin_;
		TIterator end_;
	};

}
