#include <core/math/Matrix3.h>

namespace Core {

Matrix3::Matrix3() {
    for (int i = 0; i < 9; i++) {
        data[i] = 0;
    }
}

Matrix3 Matrix3::operator*(const Matrix3& other) const {
    Matrix3 result;

    for (int i = 0; i < 9; i++) {
        result.data[i] = 0;
        for (int j = 0; j < 3; j++) {
            result.data[i] += data[j + (i / 3) * 3] * other.data[(i % 3) + j * 3];
        }
    }

    return result;
}

Vector2 Matrix3::operator*(const Vector2& other) const {
    return Vector2(
        data[0] * other.x + data[1] * other.y + data[2],
        data[3] * other.x + data[4] * other.y + data[5]
    );
}

Matrix3 Matrix3::Identity() {
    Matrix3 result;

    result.data[0] = 1;
    result.data[4] = 1;
    result.data[8] = 1;

    return result;
}

Matrix3 Matrix3::Translation(float x, float y) {
    Matrix3 result;

    result.data[0] = 1;
    result.data[4] = 1;
    result.data[8] = 1;

    result.data[2] = x;
    result.data[5] = y;

    return result;
}

Matrix3 Matrix3::Rotation(float angle) {
    Matrix3 result;

    result.data[0] = cos(angle);
    result.data[1] = -sin(angle);
    result.data[3] = sin(angle);
    result.data[4] = cos(angle);
    result.data[8] = 1;

    return result;
}

Matrix3 Matrix3::Scale(float x, float y) {
    Matrix3 result;

    result.data[0] = x;
    result.data[4] = y;
    result.data[8] = 1;

    return result;
}

} // namespace Core