#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <functional>
#include <tuple>

#include <gtest/gtest.h>

#include "stream/stream.h"

namespace stream_tests {

    using std::cout;
    using std::endl;
    using std::vector;
    using std::string;

    using namespace lipaboy_lib;

    using namespace lipaboy_lib::stream_space;
    using namespace lipaboy_lib::stream_space::operators;

    //---------------------------------Tests-------------------------------//

    TEST(StreamTest, paired_stream_simple) {
        auto first = Stream(-1, [](int p) { return p - 1; }) | get(5);
        auto second = Stream(1, 2, 3, 4, 5);

        auto res = (first & second)
                | map([](auto pair) { return pair.first + pair.second; })
                | sum();

        EXPECT_EQ(res, 0);

        //std::pair<int, int> abc = { 2, 4 };
        //auto [fir, sec] = abc;
    }

    TEST(StreamTest, paired_stream_rvalue) {
        int a = 0;
        vector<int> vec = { 1, 2, 3, 4, 5 };

        auto res =
            (Stream([&a]() { return a++; })
                | filter([](auto& a) { return a % 3 == 0; }))
            &
            (Stream(vec)
                | map([](int b) { return b - 3; }))
            | map([](auto pair) { return pair.first * pair.second; })
                //| map([](auto& pair) { return pair.first * pair.second; })        // Error under Linux
            | get(5)
            | to_vector();

        EXPECT_EQ(res, decltype(res)({ 0, -3, 0, 9, 24 }));
    }

    TEST(StreamTest, paired_stream_second_rvalue) {
        auto first = Stream(-1, [](int p) { return p - 1; }) | get(5);

        auto res = (first & Stream(1, 2, 3, 4, 5))
            | map([](auto pair) { return pair.first + pair.second; })
            | sum();

        EXPECT_EQ(res, 0);

        //std::pair<int, int> abc = { 2, 4 };
        //auto [fir, sec] = abc;
    }

    TEST(StreamTest, pseudoinfinite_stream) {
//        auto first = Stream(-1, [](int p) { return p - 1; });
//        auto second = Stream(1, 2, 3, 4, 5);

//        auto res = (first & second)
//                | map([](auto pair) { return pair.first + pair.second; })
//                | sum();

//        EXPECT_EQ(res, 0);
    }

    TEST(StreamTest, paired_stream_diff_types) {
        vector<int> vec = { 4, 5, 6 };
        auto first = Stream({ 1, 2, 3 });
        auto second = Stream(vec);
        auto vec2 = to_pair(first, second) | to_vector();
        int i = 0;
        for (auto& elem : vec2) {
            EXPECT_EQ(elem.first, i + 1);
            EXPECT_EQ(elem.second, i + 4);
            i++;
        }
    }

    TEST(StreamTest, paired_stream_same_types) {
        auto first = Stream({ 1, 2, 3 });
        auto second = Stream({ 4, 5, 6 });
        auto vec2 = to_pair(first, second) | to_vector();
        int i = 0;
        for (auto& elem : vec2) {
            EXPECT_EQ(elem.first, i + 1);
            EXPECT_EQ(elem.second, i + 4);
            i++;
        }
    }

}

