#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace lipaboy_lib {

	//-----------------------------------------------------------------------//
	//----------------SEQUENCE PRODUCING ITERATOR BY FUNCTION----------------//
	//-----------------------------------------------------------------------//

	template <class T>
	class SequenceProducingIterator {
	public:
		using GeneratorType = std::function<T(T)>;
		using value_type = T;
		using CurrentValueType = value_type;
		using CurrentValueTypePtr = std::shared_ptr<CurrentValueType>;
		using reference = T & ;
        using const_reference = const T & ;
		using pointer = T * ;
		using const_pointer = const pointer;
		using iterator_category = std::input_iterator_tag;
		// It is type of measuring the distance between iterators (nothing else).
		// It is commented because ProducingIterator is not a real iterator. You can't count the distance
		// between iterators
        using difference_type = std::ptrdiff_t;

	public:
		SequenceProducingIterator()
			: generator_(nullptr), pCurrentElem_(nullptr)
		{}
		SequenceProducingIterator(T const & initValue, GeneratorType gen)
			: generator_(gen), pCurrentElem_(std::make_shared<CurrentValueType>(initValue))
		{}
		SequenceProducingIterator(SequenceProducingIterator const & obj)
			: generator_(obj.generator_),
			pCurrentElem_(obj.pCurrentElem_)
		{}

		const_reference operator*() { return *pCurrentElem_; }
		const_pointer operator->() { return &(*pCurrentElem_); }

		// Note: not strong condition (maybe add counter to distinguishing the different iterators)
		bool operator== (SequenceProducingIterator& other) {
            return generator_.template target<T(*)(T)>()
                    == other.generator_.template target<T(*)(T)>()
				&& pCurrentElem_ == other.pCurrentElem_
				&& *pCurrentElem_ == *(other.pCurrentElem_);
		}
		bool operator!= (SequenceProducingIterator& other) { return !((*this) == other); }

		SequenceProducingIterator operator++() {
			*pCurrentElem_ = generator_(*pCurrentElem_);
			return *this;
		}
		// Info: Return type is void because you cannot return previous iterator.
		//		 You cannot return previous iterator because pCurrentElem_ of different copies [iterators]
		//		 point to the same variable. If we don't have pointer to current elem then
		//		 we must have storage it directly (as a field). But copy constructor of iterator will be expensive.
		void operator++(int) {
			*pCurrentElem_ = generator_(*pCurrentElem_);
		}

	private:
		GeneratorType generator_;
		CurrentValueTypePtr pCurrentElem_;
	};

}


