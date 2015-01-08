#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include "ex.h"
#include "helper.h"


// LEFT
// Transpose
// Inverse
// Determinant

// TODO
// projection
// rotation
// skew
// composite

// Matrix Class
class Matrix {
    private:
        float* m_matrix;
        unsigned m_row, m_col;
        unsigned m_space;

        // The 4x4 transformation matrix can share memory
        static Matrix transformation;
    public:

        // Returns an identity matrix of size nxn
        static Matrix identity(unsigned n);

        // Returns a zero matrix of size n,m
        static Matrix zero(unsigned n, unsigned m);

        // Returns a translation matrix by (tx,ty,tz)
        static const Matrix& translation(float tx, float ty, float tz);

        // Returns a scaling matrix by sx:sy:sz about (x,y,z)
        static const Matrix& scaling(float sx, float sy, float sz, float x=0, float y=0, float z=0);

        ~Matrix();

        Matrix(unsigned row, unsigned col);

        Matrix(const Matrix& m);

        void operator=(const Matrix& m);

        inline unsigned col() const {
            return m_col;
        }

        inline unsigned row() const {
            return m_row;
        }

        inline unsigned space() const {
            return m_space;
        }

        // Getter in form matrix(x,y)
        inline float& operator()(unsigned row, unsigned col){
            if(row >= m_row || col >= m_col)
                throw ex::OutOfBounds();
            return *(m_matrix+row*m_col+col);
        }

        // Setter in form matrix(x,y)
        inline const float& operator()(unsigned row, unsigned col) const {
            if(row >= m_row || col >= m_col)
                throw ex::OutOfBounds();
            return *(m_matrix+row*m_col+col);
        }

        // Getter in the form matrix(p)
        inline const float& operator()(unsigned place) const {
            if(place >= space())
                throw ex::OutOfBounds();
            return *(m_matrix+place);
        }

        // Setter in the form matrix(p)
        inline float& operator()(unsigned place){
            if(place >= space())
                throw ex::OutOfBounds();
            return *(m_matrix+place);
        }

        // Initialize the whole matrix
        // Some deep shit initializer this is
        // Couldn't use variadic args because it
        // didn't support int and float at the same time
        template<typename... Types>
        void initialize(Types... args) {
            const int size = sizeof...(args);
            if(size!=space())
                throw ex::DimensionMismatch();
            float dummy[] = { static_cast<float>(args)... };
            // memcpy performed for efficiency
            std::memcpy ( m_matrix, dummy , size*sizeof(float) );
        }

        Matrix operator+(const Matrix& m) const;

        void operator+=(const Matrix& m);

        Matrix operator-(const Matrix& m) const;

        void operator-=(const Matrix& m);

        Matrix operator*(const Matrix& m) const;

        // this x m
        void operator*=(const Matrix& m);

        // m x this
        void operator%=(const Matrix& m);

        Matrix operator*(float f) const;

        void operator*=(float f);

        Matrix operator/(float f) const;

        void operator/=(float f);

        void print();
};

#endif
