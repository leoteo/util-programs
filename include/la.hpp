/*
 * la.h
 */
#ifndef LA_H
#define LA_H

#include <vector>
#include "types.hpp"

namespace formats {class Cube; }

namespace la {


class Direction {
    std::vector<types::Real> incrementVector;
    types::Uint nElements;
    bool periodic;

public:
    Direction() {};
    Direction(std::vector<types::Real> v, types::Uint c) :
        incrementVector(v), nElements(c) {};
    /**
     * Adapts Direction for given stride.
     */
    void stride(types::Uint s);
    void scaleVector(types::Real factor);
    types::Uint getNElements() const { 
        return nElements;}
    const std::vector<types::Real> & getIncrementVector() const { 
        return incrementVector;}
    bool checkRange(types::Uint index) const;
    bool isPeriodic() const { return periodic; }
    
    friend bool operator==(const Direction& d1, const Direction& d2);
    friend class formats::Cube;
};

class Cell {
    std::vector< std::vector<types::Real> > vectors;
public:
    Cell() {};
    Cell(const std::vector<Direction> &directions);
    std::vector< types::Real > vector(types::Uint n) {
        return vectors[n];
    }
    types::Real getExtent(types::Uint index) const;
    std::vector<types::Real> extents() const;
};

template <unsigned int dim>
struct Grid {
    std::vector<Direction> directions;
    std::vector<types::Real> originVector;
    std::vector<types::Real> data;

   
    Grid(){};

    std::vector<Direction> getDirections() const {
        return directions;
    }
    std::vector<types::Real> getOriginVector() const {
        return originVector;
    }
    const std::vector<types::Real> & getData() const {
        return data;
    }
    Cell cell() const;

    const Grid & operator+=(const Grid &g);
    const Grid & operator*=(types::Real x);
    void squareValues();
    void signsqrt();
    void sqrt();
    void abs();
    types::Real sum() const;
    types::Uint countPoints() const;

    /**
     * In lack of a proper resampling method. No new values are calculated
     */
    void stride(std::vector<types::Uint> stride);
    void resize(const std::vector<types::Uint>& nElements);

    bool getNearestIndices(std::vector<types::Real>& coordinates, std::vector<types::Uint>& indices) const;
    bool getFractionalIndices(std::vector<types::Real>& coordinates, std::vector<types::Real>& fractionalIndices) const;
    types::Real getNearestDataPoint(std::vector<types::Real>& coordinates) const;
    types::Real getDataPoint(std::vector<types::Uint> indices) const;
    types::Real interpolateDataPoint(std::vector<types::Real> coordinates) const;

    bool checkRange(const std::vector<types::Uint>& indices) const;
    bool checkDimension(types::Uint size) const;
    bool hasSameGrid(const Grid &g) const;
    void wrapAround(std::vector<types::Uint> &indices) const;

private:
    void copyRecursive(
        types::Uint directionIndex,
        std::vector<types::Real>::const_iterator &oldIt,
        std::vector<types::Real>::iterator &newIt,
        const std::vector<types::Uint> &newCounts);
    void strideRecursive(
        types::Uint directionIndex,
        std::vector<types::Real>::const_iterator &oldIt,
        std::vector<types::Real>::iterator &newIt,
        const std::vector<types::Uint> &newCounts,
        const std::vector<types::Uint> &strides,
        const std::vector<types::Uint> &rests);
};

template<unsigned int dim>
types::Real  operator*(const Grid<dim>& g1, const Grid<dim>& g2);


}

#endif
