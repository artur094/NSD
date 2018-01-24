//
// Created by Luca De Mori on 29/12/2017.
//

#ifndef NSD10_DYNAMICVECTOR_H
#define NSD10_DYNAMICVECTOR_H

#include <iostream>
#include <cmath>

using namespace std;

template <class T>
class DynamicVector {
public:

    DynamicVector(unsigned int _size=0) : size(_size) {
        container = new T[_size]();
    }

    T &operator[](unsigned int i) {
        if (i<0){
            return container[0];
        }

        if (i>=size) {      // array already exist, so realloc
            T *temp = new T[i + 1]();
            std::copy(container, container + size, temp);
            delete[] container;
            container = temp;
            size = i + 1;
            //std::cout << "[DEBUG] - DynamicVector::operator[]: container size=" << size << std::endl;
        }

        return container[i];

    }

    unsigned int getSize() const {
        return size;
    }

    void f_print(std::ostream &out) const {
        for (int j=0; j<size; j++){
            out << j << " " << container[j] << endl;
        }
    }

    float get_avg() {
        float sum = 0;
        for (int j=0; j<size; j++){
            sum += (float) container[j]/size;
        }
        return sum;
    }

    void f_print_nz(std::ostream &out) const {
        for (unsigned int j=0; j<size; j++){
            if (container[j]!=0||j==0||j==size-1) out << j << " " << container[j] << endl;
        }
    }

    void f_print_fill(std::ostream &out) const {
        T last;
        for (unsigned int j=0; j<size; j++){
            out << j << " ";
            if (container[j]!=0||j==0||j==size-1){
                out << container[j];
                last = container[j];
            } else {
                out << last;
            }
            out << endl;
        }
    }

    void insert_order(T e) {
        unsigned int p = 0;
        while (p < size && (*this)[p] < e){
            p++;
        }

        if (p<size) {
            for (unsigned int i = size; i>p; i--) {
                (*this)[i] = (*this)[i-1];
            }
        }

        (*this)[p] = e;
    }


    unsigned int find(T e) {
        unsigned int p = -1;


        while ((*this)[++p]<e);

        if ((*this)[++p] == e ) { return p; }

        return -1;

        /*
         *
         *
    // TODO: add control for loop if number doesn't exist in the middle (now it does a ping pong)
        unsigned int p = size/2;
        unsigned int step = ceil((float)p/2);

        cout << "looking for " << e << endl;
        while(p<size && p>=0 && (*this)[p]!=e) {
            cout << "p=" << p << " el=" << (*this)[p];
            if (e < (*this)[p]) {
                p -= step;
                cout << " [decr]";
            } else {
                p += step;
                cout << " [incr]";
            }
            step = ceil((float)step/2);
            cout << " step=" << step << endl;
        }


        if (p<size && p>=0) { return p; }

        return -1;

         */
    }

    virtual ~DynamicVector() {
        delete[] container;
    }


private:

    unsigned int size;
    T *container;

};


#endif //NSD10_DYNAMICVECTOR_H
