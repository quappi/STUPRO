#ifndef SPHERICAL_COORDINATE_FUNCTIONS
#define SPHERICAL_COORDINATE_FUNCTIONS

#include"Vector3.hpp"
#include"Utils/Misc/Macros.hpp"

#define BASE_HEIGHT 100

/**
 * Convert the spherical (gps) to the cartesian representation
 * @param gps the gps position to convert
 * @return the cartesian position of gps
 */
template<typename T> Vector3<T> sphericalToCartesian(const Vector3<T>& gps) {
    const T lon = gps.x * KRONOS_PI / 180;
    const T lat = gps.y * KRONOS_PI / 180;

    Vector3<T> retVal;
    retVal.z = (BASE_HEIGHT + gps.z) * cos(lat) * cos(lon);
    retVal.x = (BASE_HEIGHT + gps.z) * cos(lat) * sin(lon);
    retVal.y = (BASE_HEIGHT + gps.z) * sin(lat);
    return retVal;
}

/**
 * Convert the cartesian to the spherical (gps) representation
 * @param point the cartesian position
 * @return the gps position of point
 */
template<typename T> Vector3<T> cartesianToSpherical(const Vector3<T>& point) {
    Vector3<T> retVal;
    retVal.z = point.length() - BASE_HEIGHT;
    retVal.x = atan2(point.x, point.z) * 180 / KRONOS_PI;
    retVal.y = asin(point.y / point.length()) * 180 / KRONOS_PI;
    return point;
}

/**
 * Get the (cartesian) abs of a spherical coordinate
 * @param gps the position to get the abs from
 * @return the abs of gps
 */
template<typename T> T abs(const Vector3<T>& gps) {
    return gps.z + BASE_HEIGHT;
}

/**
 * Scale a spherical coordinate to a given (cartesian) length
 * @param gps the position to scale
 * @return the scaled position
 */
template<typename T> Vector3<T> scaleTo(const Vector3<T>& gps, const T targetLength) {
    return Vector3<T>(gps.x, gps.y, targetLength - BASE_HEIGHT);
}

/**
 * Get the point right between two other points (in spherical sense, so the result has the middle abs of gps1 and gps2)
 */
template<typename T> Vector3<T> calculateCenter(const Vector3<T>& gps1, const Vector3<T>& gps2) {
    Vector3<T> cartesian1 = sphericalToCartesian(gps1) / 2;
    Vector3<T> cartesian2 = sphericalToCartesian(gps2) / 2;
    return scaleTo(cartesianToSpherical(cartesian1 + cartesian2), (abs(gps1) + abs(gps2)) / 2);
}

#endif // SPHERICAL_COORDINATE_FUNCTIONS

