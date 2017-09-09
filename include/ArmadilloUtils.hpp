/******************************************
 * An utility class for linear algebra.
 * Last edited: 09.09.2017
 */

#ifndef __ARMADILLOUTILS_HPP_
#define __ARMADILLOUTILS_HPP_

#include <armadillo>
#include <cmath>

namespace ArmaUtils {
    /** \brief Turn a Vector around the X-Axis
     * \param pos arma::vec& the vector to turn
     * \param degree the degree the vector should be turned
     * \return arma::vec the turned vector
     */
    static arma::vec turnVectorXDegree(arma::vec &pos, double degree) {
        arma::mat rotationMatrix = { { 1, 0, 0 },
                                     { 0, std::cos(degree), -std::sin(degree) },
                                     { 0, std::sin(degree), std::cos(degree) } };

        return rotationMatrix * pos;
    }

    /** \brief Turn a Vector around the Y-Axis
     * \param pos arma::vec& the vector to turn
     * \param degree the degree the vector should be turned
     * \return arma::vec the turned vector
     */
    static arma::vec turnVectorYDegree(arma::vec &pos, double degree) {
        arma::mat rotationMatrix = { { std::cos(degree), 0, std::sin(degree) },
                                     { 0, 1, 0 },
                                     { -std::sin(degree), 0, std::cos(degree) } };

        return rotationMatrix * pos;
    }

     /** \brief Turn a Vector around the Z-Axis
     * \param pos arma::vec& the vector to turn
     * \param degree the degree the vector should be turned
     * \return arma::vec the turned vector
     */
    static arma::vec turnVectorZDegree(arma::vec &pos, double degree) {
        arma::mat rotationMatrix = { { std::cos(degree), -std::sin(degree), 0 },
                                     { std::sin(degree), std::cos(degree), 0 },
                                     { 0, 0, 1 } };

        return rotationMatrix * pos;
    }


    /** \brief Get the euclidean length of a given vector.
     * \param vec arma::vec& a reference to the vector
     * \return double the length of the given vector
     */
    static double getLength(arma::vec &vec) {
            // euclidean distance
            double length = 0.f;
            vec.for_each([&length](double x) {length += x*x;});

            return std::sqrt(length);
    }
};

#endif // __ARMADILLOUTILS_HPP_
