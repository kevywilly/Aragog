/*
 * customtypes.h
 *
 *  Created on: Jun 14, 2017
 *      Author: kevywilly
 */

#ifndef RTTYPES_H_
#define RTTYPES_H_


template<typename T>
struct tuple4{

    T _1;
    T _2;
    T _3;
    T _4;

    static tuple4<T> make(T a, T b, T c, T d) {
    	return (tuple4<T>) {a,b,c,d};
    }

    T at(int i) {

        switch (i) {
        case 0:
            return _1;
            break;

        case 1:
            return _2;
            break;

        case 2:
            return _3;
            break;

        default:
            return _4;

        }

    }

};

template<typename T>
struct tuple3 {

public:

    T _1;
    T _2;
    T _3;

    static tuple3<T> make(T a, T b, T c) {
        return (tuple3<T>) {a,b,c};
    }

    tuple4< tuple3<T> > toT3_4() {
    	tuple4< tuple3<T> > t = { {_1,_2,_3}, {_1,_2,_3}, {_1,_2,_3}, {_1,_2,_3} };
        return t;
    }

    T at(int i) {

        switch (i) {
        case 0:
            return _1;
            break;

        case 1:
            return _2;
            break;

        default:
            return _3;

        }

    }

};

template<typename T>
struct tuple2 {

    T _1;
    T _2;

    static tuple2<T> make(T a, T b) {
    	return (tuple2<T>) {a,b};
    }

    tuple3< tuple2<T> > toT2_3() {
    	tuple3< tuple2<T> > t = { {_1,_2}, {_1,_2}, {_1,_2}};
    	return t;
    }

    tuple4< tuple2<T> > toT2_4() {
    	 tuple4< tuple2<T> > t =  { {_1,_2}, {_1,_2}, {_1,_2}, {_1,_2}};
    	 return t;
    }

    T at(int i) {

        switch (i) {
        case 0:
            return _1;
            break;

        default:
            return _2;

        }

    }

};

typedef tuple3<int8_t> int8T3;
typedef tuple3<uint8_t> uint8T3;
typedef tuple2<int> intT2;
typedef tuple2<float> floatT2;
typedef tuple3<int> intT3;
typedef tuple3<float> floatT3;
typedef tuple3<intT2> intT2_3;
typedef tuple3<floatT2> floatT2_3;
typedef tuple4<intT2> intT2_4;
typedef tuple4<intT3> intT3_4;
typedef tuple4<floatT2> floatT2_4;
typedef tuple4<floatT3> floatT3_4;
typedef tuple4<int8T3> int8T3_4;
typedef tuple4<uint8T3> uint8T3_4;

#endif /* RTTYPES_H_ */
