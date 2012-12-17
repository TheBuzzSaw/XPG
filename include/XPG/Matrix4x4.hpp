#ifndef XpgMatrix4x4Hpp
#define XpgMatrix4x4Hpp

#include <cmath>
#include <cstring>

#ifndef M_PI
#define M_PI 3.1415926535898
#endif

#define SCT static_cast<T>
#define DEG2RAD(n) ((n) * SCT(M_PI) / SCT(180))
#define RAD2DEG(n) ((n) * SCT(180) / SCT(M_PI))

namespace XPG
{
    template<typename T>
    class Matrix4x4
    {
        public:
            Matrix4x4(const T* inArray = Identity);
            Matrix4x4(const Matrix4x4<T>& matrix);
            Matrix4x4(const Matrix4x4<T>& inLMatrix,
                const Matrix4x4<T>& inRMatrix);
            inline ~Matrix4x4() {}

            void loadIdentity();

            /// model view
            void Rotate(T degrees, T x, T y, T z);
            void RotateX(T degrees);
            void RotateY(T degrees);
            void RotateZ(T degrees);
            void Scale(T scale);
            void ScaleX(T scale);
            void ScaleY(T scale);
            void ScaleZ(T scale);
            void Scale(T x, T y, T z);
            void Translate(T x, T y, T z);
            void SmartMove(T inRX, T inRY, T inRZ, T inTX, T inTY, T inTZ);

            /// projection
            void Frustum(T left, T right, T bottom, T top, T near,
                T far);
            void Perspective(T fieldOfView, T ratio, T near, T far,
                bool smartAdjustment = false);
            void Orthographic(T left, T right, T bottom, T top,
                T near, T far);
            void Orthographic(T range, T ratio);

            /// matrix operators
            Matrix4x4<T>& operator=(const Matrix4x4<T>& matrix);
            void Multiply(const Matrix4x4<T>& inLMatrix,
                const Matrix4x4<T>& inRMatrix);
            void Inverse();
            void CopyInverseTo(Matrix4x4<T>& matrix) const;
            const Matrix4x4<T> inversed() const;
            void Multiply(const T* inVertex, T* inResult) const;
            void Transform(const T* inVertex, T* inResult) const;
            void Transform(T* inVertex) const;

            /// Allow this object to behave as a simple array.
            inline operator T*() { return _data; }
            inline operator const T*() const { return _data; }

            /// Allow simply access by row and column.
            inline T& operator()(size_t inRow, size_t inCol)
            {
                return _data[inCol * 4 + inRow];
            }

            inline T operator()(size_t inRow, size_t inCol) const
            {
                return _data[inCol * 4 + inRow];
            }

            /// simple comparison operators
            inline bool operator==(const Matrix4x4<T>& matrix) const
            {
                return !memcmp(_data, matrix._data, 16 * sizeof(T));
            }

            inline bool operator!=(const Matrix4x4<T>& matrix) const
            {
                return memcmp(_data, matrix._data, 16 * sizeof(T));
            }

            inline Matrix4x4<T>& operator*=(const Matrix4x4<T>& matrix)
            {
                Multiply(Matrix4x4<T>(_data), matrix);
                return *this;
            }

            inline const Matrix4x4<T> operator*(const Matrix4x4<T>& matrix)
                const
            {
                return Matrix4x4<T>(Matrix4x4<T>(_data), matrix);
            }

        private:
            inline void Copy(const Matrix4x4<T>& matrix)
            {
                memcpy(_data, matrix._data, 16 * sizeof(T));
            }

            inline T& At(size_t inRow, size_t inCol)
            {
                return _data[inCol * 4 + inRow];
            }

            inline T At(size_t inRow, size_t inCol) const
            {
                return _data[inCol * 4 + inRow];
            }

            T _data[16]; // stored in column-major order

            static const T Identity[16];
    };

    /// The identity matrix is pre-built so that it can just be copied to newly
    /// born matrices. This makes it faster than manually filling the identity
    /// matrix and/or having to iterate through a loop.
    template<typename T>
    const T Matrix4x4<T>::Identity[16] = {
        SCT(1), SCT(0), SCT(0), SCT(0),
        SCT(0), SCT(1), SCT(0), SCT(0),
        SCT(0), SCT(0), SCT(1), SCT(0),
        SCT(0), SCT(0), SCT(0), SCT(1)
        };

    /// For increased compatibility, this matrix can read its data in from an
    /// array. This also doubles as the default constructor: if no array is
    /// specified, the pointer to the static identity matrix is passed in.
    ///
    /// IMPORTANT -- Because the array stores the matrix in column-major order,
    /// it is assumed the incoming data is sorted the same way.
    template<typename T>
    Matrix4x4<T>::Matrix4x4(const T* inArray)
    {
        memcpy(_data, inArray, 16 * sizeof(T));
    }

    /// This is a very simple copy constructor. It does a byte-for-byte copy of
    /// target matrix's data.
    template<typename T>
    Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& matrix)
    {
        copy(matrix);
    }

    /// It is common for matrices to be created on the fly specifically for
    /// capturing the product of two matrices. This constructor defaults its
    /// data to that product (bypasses any other setting of the array).
    template<typename T>
    Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& inLMatrix,
        const Matrix4x4<T>& inRMatrix)
    {
        Multiply(inLMatrix, inRMatrix);
    }

    /// There are many instances where the programmer needs to reset the matrix
    /// back to the identity matrix.
    template<typename T>
    void Matrix4x4<T>::loadIdentity()
    {
        memcpy(_data, Identity, 16 * sizeof(T));
    }

    /// This is a spiritual recreation of glRotatef
    template<typename T>
    void Matrix4x4<T>::Rotate(T degrees, T x, T y, T z)
    {
        /// arbitrary rotation about an axis
        /// http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml

        T r = DEG2RAD(degrees);
        T c = cos(r);
        T ci = SCT(1) - c;
        T s = sin(r);
        Matrix4x4<T> transform;

        transform[0] = x * x * ci + c;
        transform[4] = x * y * ci - (z * s);
        transform[8] = x * z * ci + (y * s);
        transform[1] = y * x * ci + (z * s);
        transform[5] = y * y * ci + c;
        transform[9] = y * z * ci - (x * s);
        transform[2] = x * z * ci - (y * s);
        transform[6] = y * z * ci + (x * s);
        transform[10] = z * z * ci + c;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// Rather than deal with the mathematical nightmare involved with rotating
    /// about an arbitrary axis, it is much simpler and faster to rotate about
    /// X, Y, or Z. This function rotates about X axis.
    template<typename T>
    void Matrix4x4<T>::RotateX(T degrees)
    {
        T r = DEG2RAD(degrees);
        T c = cos(r);
        T s = sin(r);
        Matrix4x4<T> transform;

        transform[5] = c;
        transform[9] = -s;
        transform[6] = s;
        transform[10] = c;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// Rather than deal with the mathematical nightmare involved with rotating
    /// about an arbitrary axis, it is much simpler and faster to rotate about
    /// X, Y, or Z. This function rotates about Y axis.
    template<typename T>
    void Matrix4x4<T>::RotateY(T degrees)
    {
        T r = DEG2RAD(degrees);
        T c = cos(r);
        T s = sin(r);
        Matrix4x4<T> transform;

        transform[0] = c;
        transform[8] = s;
        transform[2] = -s;
        transform[10] = c;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// Rather than deal with the mathematical nightmare involved with rotating
    /// about an arbitrary axis, it is much simpler and faster to rotate about
    /// X, Y, or Z. This function rotates about Z axis.
    template<typename T>
    void Matrix4x4<T>::RotateZ(T degrees)
    {
        T r = DEG2RAD(degrees);
        T c = cos(r);
        T s = sin(r);
        Matrix4x4<T> transform;

        transform[0] = c;
        transform[4] = -s;
        transform[1] = s;
        transform[5] = c;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// This is a uniform scale transformations. All three components are scaled
    /// by the same value specified by scale.
    template<typename T>
    void Matrix4x4<T>::Scale(T scale)
    {
        Matrix4x4<T> transform;

        transform[0] = scale;
        transform[5] = scale;
        transform[10] = scale;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// A common error with scaling along one axis is that it is natural to set
    /// the other scales to zero instead of one. This function mitigates that.
    /// It scales along the X axis only.
    template<typename T>
    void Matrix4x4<T>::ScaleX(T scale)
    {
        Matrix4x4<T> transform;

        transform[0] = scale;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// A common error with scaling along one axis is that it is natural to set
    /// the other scales to zero instead of one. This function mitigates that.
    /// It scales along the Y axis only.
    template<typename T>
    void Matrix4x4<T>::ScaleY(T scale)
    {
        Matrix4x4<T> transform;

        transform[5] = scale;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// A common error with scaling along one axis is that it is natural to set
    /// the other scales to zero instead of one. This function mitigates that.
    /// It scales along the Z axis only.
    template<typename T>
    void Matrix4x4<T>::ScaleZ(T scale)
    {
        Matrix4x4<T> transform;

        transform[10] = scale;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// The standard scale transformation can resize geometry along the X, Y,
    /// and/or the Z axes. It functions like that of glScale.
    template<typename T>
    void Matrix4x4<T>::Scale(T x, T y, T z)
    {
        Matrix4x4<T> transform;

        transform[0] = x;
        transform[5] = y;
        transform[10] = z;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// This transformation serves as a direct offset for vertices. It functions
    /// like that of glTranslate.
    template<typename T>
    void Matrix4x4<T>::Translate(T x, T y, T z)
    {
        Matrix4x4<T> transform;

        transform[12] = x;
        transform[13] = y;
        transform[14] = z;

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// When positioning an object in the scene, there is a particular optimal
    /// order to applying those transformations: rotate Z, rotate X, rotate Y,
    /// translate.
    template<typename T>
    void Matrix4x4<T>::SmartMove(T inRX, T inRY, T inRZ, T inTX, T inTY, T inTZ)
    {
        translate(inTX, inTY, inTZ);
        rotateY(inRY);
        rotateX(inRX);
        rotateZ(inRZ);
    }

    /// This is a spiritual recreation of glFrustum.
    template<typename T>
    void Matrix4x4<T>::Frustum(T left, T right, T bottom, T top,
        T near, T far)
    {
        Matrix4x4<T> transform;

        transform[0] = (SCT(2) * near) / (right - left);
        transform[8] = (right + left) / (right - left);
        transform[5] = (SCT(2) * near) / (top - bottom);
        transform[9] = (top + bottom) / (top - bottom);
        transform[10] = (far + near) / (near - far);
        transform[14] = (SCT(2) * far * near) / (near - far);
        transform[11] = SCT(-1);
        transform[15] = SCT(0);

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// This is a recreation of gluPerspective (more commonly used than
    /// glFrustum).
    template<typename T>
    void Matrix4x4<T>::Perspective(T fieldOfView, T ratio, T near,
        T far, bool smartAdjustment)
    {
        /// adaptation of gluPerspective
        /// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
        T r = DEG2RAD(fieldOfView);
        if (smartAdjustment && ratio < SCT(1))
            r /= ratio;
        T f = SCT(1) / tan(r / SCT(2));

        Matrix4x4<T> transform;

        transform[0] = f / ratio;
        transform[5] = f;
        transform[10] = (far + near) / (near - far);
        transform[14] = (SCT(2) * far * near) / (near - far);
        transform[11] = SCT(-1);
        transform[15] = SCT(0);

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// This is a recreation of glOrtho. It creates a basic orthographic
    /// projection.
    template<typename T>
    void Matrix4x4<T>::Orthographic(T left, T right, T bottom, T top,
        T near, T far)
    {
        Matrix4x4<T> transform;

        transform[0] = SCT(2) / (right - left);
        transform[12] = (right + left) / (right - left);
        transform[5] = SCT(2) / (top - bottom);
        transform[13] = (top + bottom) / (top - bottom);
        transform[10] = SCT(2) / (near - far);
        transform[14] = (far + near) / (far - near);

        Multiply(Matrix4x4<T>(_data), transform);
    }

    /// This is an intelligent orthographic projection. Rather than manually
    /// request all sides of the canonical viewing volume (as in glOrtho), this
    /// function simply takes the aspect ratio and requested object space range.
    /// The range is fixated along the shorter axis of the display. In other
    /// words, in a widescreen display, the top and bottom will exactly reach
    /// the range specified by range. In a tall display, the left and right
    /// will reach that range.
    template<typename T>
    void Matrix4x4<T>::Orthographic(T range, T ratio)
    {
        if (ratio < SCT(1))
        {
            Orthographic(-range, range, -range / ratio,
                range / ratio, -range, range);
        }
        else
        {
            Orthographic(-range * ratio, range * ratio, -range,
                range, -range, range);
        }
    }

    /// This is a rudimentary assignment operator. It just makes a byte-perfect
    /// copy of the target matrix.
    template<typename T>
    Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& matrix)
    {
        if (this != &matrix) copy(matrix);
        return *this;
    }

    /// This function multiplies inLMatrix and inRMatrix together and stores the
    /// result into THIS matrix. In other words, it OVERWRITES its own data with
    /// the product of the two incoming matrices. This is done for performance
    /// reasons as it prevents an extra temporary copy from being made.
    template<typename T>
    void Matrix4x4<T>::Multiply(const Matrix4x4<T>& inLMatrix,
        const Matrix4x4<T>& inRMatrix)
    {
        /// The actual math has been completely unrolled (out of for loops) for
        /// performance improvements.

        _data[0] = (inLMatrix[0] * inRMatrix[0])
            + (inLMatrix[4] * inRMatrix[1])
            + (inLMatrix[8] * inRMatrix[2])
            + (inLMatrix[12] * inRMatrix[3]);

        _data[4] = (inLMatrix[0] * inRMatrix[4])
            + (inLMatrix[4] * inRMatrix[5])
            + (inLMatrix[8] * inRMatrix[6])
            + (inLMatrix[12] * inRMatrix[7]);

        _data[8] = (inLMatrix[0] * inRMatrix[8])
            + (inLMatrix[4] * inRMatrix[9])
            + (inLMatrix[8] * inRMatrix[10])
            + (inLMatrix[12] * inRMatrix[11]);

        _data[12] = (inLMatrix[0] * inRMatrix[12])
            + (inLMatrix[4] * inRMatrix[13])
            + (inLMatrix[8] * inRMatrix[14])
            + (inLMatrix[12] * inRMatrix[15]);

        _data[1] = (inLMatrix[1] * inRMatrix[0])
            + (inLMatrix[5] * inRMatrix[1])
            + (inLMatrix[9] * inRMatrix[2])
            + (inLMatrix[13] * inRMatrix[3]);

        _data[5] = (inLMatrix[1] * inRMatrix[4])
            + (inLMatrix[5] * inRMatrix[5])
            + (inLMatrix[9] * inRMatrix[6])
            + (inLMatrix[13] * inRMatrix[7]);

        _data[9] = (inLMatrix[1] * inRMatrix[8])
            + (inLMatrix[5] * inRMatrix[9])
            + (inLMatrix[9] * inRMatrix[10])
            + (inLMatrix[13] * inRMatrix[11]);

        _data[13] = (inLMatrix[1] * inRMatrix[12])
            + (inLMatrix[5] * inRMatrix[13])
            + (inLMatrix[9] * inRMatrix[14])
            + (inLMatrix[13] * inRMatrix[15]);

        _data[2] = (inLMatrix[2] * inRMatrix[0])
            + (inLMatrix[6] * inRMatrix[1])
            + (inLMatrix[10] * inRMatrix[2])
            + (inLMatrix[14] * inRMatrix[3]);

        _data[6] = (inLMatrix[2] * inRMatrix[4])
            + (inLMatrix[6] * inRMatrix[5])
            + (inLMatrix[10] * inRMatrix[6])
            + (inLMatrix[14] * inRMatrix[7]);

        _data[10] = (inLMatrix[2] * inRMatrix[8])
            + (inLMatrix[6] * inRMatrix[9])
            + (inLMatrix[10] * inRMatrix[10])
            + (inLMatrix[14] * inRMatrix[11]);

        _data[14] = (inLMatrix[2] * inRMatrix[12])
            + (inLMatrix[6] * inRMatrix[13])
            + (inLMatrix[10] * inRMatrix[14])
            + (inLMatrix[14] * inRMatrix[15]);

        _data[3] = (inLMatrix[3] * inRMatrix[0])
            + (inLMatrix[7] * inRMatrix[1])
            + (inLMatrix[11] * inRMatrix[2])
            + (inLMatrix[15] * inRMatrix[3]);

        _data[7] = (inLMatrix[3] * inRMatrix[4])
            + (inLMatrix[7] * inRMatrix[5])
            + (inLMatrix[11] * inRMatrix[6])
            + (inLMatrix[15] * inRMatrix[7]);

        _data[11] = (inLMatrix[3] * inRMatrix[8])
            + (inLMatrix[7] * inRMatrix[9])
            + (inLMatrix[11] * inRMatrix[10])
            + (inLMatrix[15] * inRMatrix[11]);

        _data[15] = (inLMatrix[3] * inRMatrix[12])
            + (inLMatrix[7] * inRMatrix[13])
            + (inLMatrix[11] * inRMatrix[14])
            + (inLMatrix[15] * inRMatrix[15]);
    }

    /// This finds the inverse matrix and stores it into THIS matrix.
    template<typename T>
    void Matrix4x4<T>::Inverse()
    {
        const Matrix4x4<T> m(*this);
        m.copyInverseTo(*this);
    }

    /// This finds the inverse matrix and returns it as a copy.
    template<typename T>
    const Matrix4x4<T> Matrix4x4<T>::inversed() const
    {
        Matrix4x4<T> outMatrix;
        copyInverseTo(outMatrix);
        return outMatrix;
    }

    template<typename T>
    void Matrix4x4<T>::Multiply(const T* inVertex, T* inResult) const
    {
        inResult[0] = At(0, 0) * inVertex[0] + At(0, 1) * inVertex[1]
            + At(0, 2) * inVertex[2] + At(0, 3) * inVertex[3];

        inResult[1] = At(1, 0) * inVertex[0] + At(1, 1) * inVertex[1]
            + At(1, 2) * inVertex[2] + At(1, 3) * inVertex[3];

        inResult[2] = At(2, 0) * inVertex[0] + At(2, 1) * inVertex[1]
            + At(2, 2) * inVertex[2] + At(2, 3) * inVertex[3];

        inResult[3] = At(3, 0) * inVertex[0] + At(3, 1) * inVertex[1]
            + At(3, 2) * inVertex[2] + At(3, 3) * inVertex[3];
    }

    template<typename T>
    void Matrix4x4<T>::Transform(const T* inVertex, T* inResult) const
    {
        T vertex[4];
        memcpy(vertex, inVertex, 3 * sizeof(T));
        vertex[3] = 1.0f;

        T result[4];
        Multiply(vertex, result);

        inResult[0] = result[0] / result[3];
        inResult[1] = result[1] / result[3];
        inResult[2] = result[2] / result[3];
    }

    template<typename T>
    void Matrix4x4<T>::Transform(T* inVertex) const
    {
        T vertex[4];
        memcpy(vertex, inVertex, 4 * sizeof(T));
        transform(vertex, inVertex);
    }

    /// This finds the inverse matrix and stores it into matrix.
    template<typename T>
    void Matrix4x4<T>::CopyInverseTo(Matrix4x4<T>& matrix) const
    {
#define SWAP_ROWS(a, b) { T* _tmp = a; (a) = (b); (b) = _tmp; }

        T wtmp[4][8];
        T m0;
        T m1;
        T m2;
        T m3;
        T s;
        T* r0;
        T* r1;
        T* r2;
        T* r3;

        r0 = wtmp[0];
        r1 = wtmp[1];
        r2 = wtmp[2];
        r3 = wtmp[3];

        r0[0] = At(0, 0);
        r0[1] = At(0, 1);
        r0[2] = At(0, 2);
        r0[3] = At(0, 3);
        r0[4] = 1.0;
        r0[5] = r0[6] = r0[7] = 0.0;
        r1[0] = At(1, 0);
        r1[1] = At(1, 1);
        r1[2] = At(1, 2);
        r1[3] = At(1, 3);
        r1[5] = 1.0;
        r1[4] = r1[6] = r1[7] = 0.0;
        r2[0] = At(2, 0);
        r2[1] = At(2, 1);
        r2[2] = At(2, 2);
        r2[3] = At(2, 3);
        r2[6] = 1.0;
        r2[4] = r2[5] = r2[7] = 0.0;
        r3[0] = At(3, 0);
        r3[1] = At(3, 1);
        r3[2] = At(3, 2);
        r3[3] = At(3, 3);
        r3[7] = 1.0;
        r3[4] = r3[5] = r3[6] = 0.0;

        if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
        if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
        if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
        if (0.0 == r0[0]) return;

        m1 = r1[0] / r0[0];
        m2 = r2[0] / r0[0];
        m3 = r3[0] / r0[0];
        s = r0[1];
        r1[1] -= m1 * s;
        r2[1] -= m2 * s;
        r3[1] -= m3 * s;
        s = r0[2];
        r1[2] -= m1 * s;
        r2[2] -= m2 * s;
        r3[2] -= m3 * s;
        s = r0[3];
        r1[3] -= m1 * s;
        r2[3] -= m2 * s;
        r3[3] -= m3 * s;
        s = r0[4];

        if (s != 0.0)
        {
            r1[4] -= m1 * s;
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r0[5];
        if (s != 0.0)
        {
            r1[5] -= m1 * s;
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r0[6];
        if (s != 0.0)
        {
            r1[6] -= m1 * s;
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r0[7];
        if (s != 0.0)
        {
            r1[7] -= m1 * s;
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }

        if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
        if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
        if (0.0 == r1[1]) return;

        m2 = r2[1] / r1[1];
        m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2];
        r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3];
        r3[3] -= m3 * r1[3];
        s = r1[4];
        if (0.0 != s)
        {
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r1[5];
        if (0.0 != s)
        {
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r1[6];
        if (0.0 != s)
        {
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r1[7];
        if (0.0 != s)
        {
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }

        if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
        if (0.0 == r2[2]) return;

        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3];
        r3[4] -= m3 * r2[4];
        r3[5] -= m3 * r2[5];
        r3[6] -= m3 * r2[6];
        r3[7] -= m3 * r2[7];

        if (0.0 == r3[3]) return;

        s = 1.0 / r3[3];
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;

        m2 = r2[3];
        s = 1.0 / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2);
        r2[5] = s * (r2[5] - r3[5] * m2);
        r2[6] = s * (r2[6] - r3[6] * m2);
        r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1;
        r1[5] -= r3[5] * m1;
        r1[6] -= r3[6] * m1;
        r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0;
        r0[5] -= r3[5] * m0;
        r0[6] -= r3[6] * m0;
        r0[7] -= r3[7] * m0;

        m1 = r1[2];
        s = 1.0 / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1);
        r1[5] = s * (r1[5] - r2[5] * m1);
        r1[6] = s * (r1[6] - r2[6] * m1);
        r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0;
        r0[5] -= r2[5] * m0;
        r0[6] -= r2[6] * m0;
        r0[7] -= r2[7] * m0;

        m0 = r0[1];
        s = 1.0 / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0);
        r0[5] = s * (r0[5] - r1[5] * m0);
        r0[6] = s * (r0[6] - r1[6] * m0);
        r0[7] = s * (r0[7] - r1[7] * m0);

        matrix(0, 0) = r0[4];
        matrix(0, 1) = r0[5];
        matrix(0, 2) = r0[6];
        matrix(0, 3) = r0[7];
        matrix(1, 0) = r1[4];
        matrix(1, 1) = r1[5];
        matrix(1, 2) = r1[6];
        matrix(1, 3) = r1[7];
        matrix(2, 0) = r2[4];
        matrix(2, 1) = r2[5];
        matrix(2, 2) = r2[6];
        matrix(2, 3) = r2[7];
        matrix(3, 0) = r3[4];
        matrix(3, 1) = r3[5];
        matrix(3, 2) = r3[6];
        matrix(3, 3) = r3[7];
    }
}

#endif
