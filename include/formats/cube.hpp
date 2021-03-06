/*
 * cube.h
 */
#ifndef FORMATS_CUBE_H
#define FORMATS_CUBE_H

#include "atomistic/fundamental.hpp"
#include "types.hpp"


namespace formats {

/**
 * Specialized grid with several member functions for 3d
 */
struct CubeGrid : public la::Grid<3> {

    void sumXY(std::vector<types::Real>& reduced) const;
    void zPlane(types::Uint index, std::vector<types::Real> &plane);
    void plane(types::Uint dir, 
               types::Uint index,
               std::vector<types::Real> &plane);
    void zSurface(std::vector<types::Uint> zIndices, std::vector<types::Real> &plane);
//    void interpolatedZPlane(const std::vector<Real> &zProfile,
//            std::vector<types::Real> &plane);
    void averageXY(std::vector<types::Real>& reduced) const;
    void dirSum(types::Uint dirIndex, std::vector<types::Real>& plane) const;
    void dirAverage(types::Uint dirIndex, std::vector<types::Real>& plane) const;

    void zIsoSurface(types::Real isoValue, std::vector<types::Real> &coordinates) const; 
    void zIsoSurface(types::Real isoValue, std::vector<types::Real> &coordinates, types::Real approachFrom) const; 
    void zIsoSurfaceOnGrid(types::Real isoValue, std::vector<types::Uint> &zIndices) const; 
    void zIsoSurfaceOnGrid(types::Real isoValue, std::vector<types::Uint> &zIndices, types::Real approachFrom) const; 
    void zIsoSurfaceOnGrid(types::Real isoValue, std::vector<types::Uint> &zIndices, std::vector<types::Real> &values) const; 
    /**
     * In lack of a proper resampling method. No new values are calculated
     */
    using la::Grid<3>::stride;
    void stride(types::Uint, types::Uint, types::Uint);
    using la::Grid<3>::resize;
    void resize(types::Uint nX, types::Uint nY, types::Uint nZ);
    types::Uint nX() const  { return directions[0].getNElements(); }
    types::Uint nY() const  { return directions[1].getNElements(); }
    types::Uint nZ() const  { return directions[2].getNElements(); }
    types::Real dX() const  { return directions[0].getIncrementVector()[0]; }
    types::Real dY() const  { return directions[1].getIncrementVector()[1]; }
    types::Real dZ() const  { return directions[2].getIncrementVector()[2]; }
    types::Real volumeElement() const;

    using la::Grid<3>::getNearestDataPoint;
    types::Real getNearestDataPoint(types::Real x, types::Real y, types::Real z) const;
//    types::Real interpolateDataPoint(types::Real x, types::Real y, types::Real z) const;
    using la::Grid<3>::getDataPoint;
    types::Real getDataPoint(types::Uint x, types::Uint y, types::Uint z) const;
    types::Real interpolateZData(types::Uint x, types::Uint y, types::Real z) const;

    void writeDirPlane(types::String fileName, const std::vector<types::Real>& data, types::Uint dir) const;
    void writePlane(types::String fileName, const std::vector<types::Real>& data, types::Uint i, types::Uint j) const;
private:
    void zIsoSurfaceCore(types::Real isoValue, 
                         std::vector<types::Uint> &zIndices, 
                         std::vector<types::Real> &values, 
                         types::Real approachFrom,
                         bool onGrid) const; 
};




struct Cube {
	std::vector<atomistic::Atom> atoms;
	CubeGrid grid;
    types::String title;
    types::String description;
    types::String fileName;

    Cube(types::String filename) { readCubeFile(filename); }
    Cube() {};

    const Cube & operator+=(const Cube &c);

	bool readCubeFile(types::String filename);
	bool readCubeFile();
    bool readDescription(types::String filename);
	bool writeCubeFile(types::String filename) const;
	bool writeZProfile(types::String filename, types::String header) const;
	bool writeZProfile(types::String filename) const;

    void squareValues() { grid.squareValues(); }
    void abs() { grid.abs(); }
    void stride(std::vector<types::Uint> s) { grid.stride(s);}
    void averageXY(std::vector<types::Real> &data) const {grid.averageXY(data);}
    std::vector<types::Real> getZProfile() const;
    void addZProfile(types::Stream &stream, types::String header) const;
    void getZPlane(types::Uint zIndex, std::vector<types::Real> &data){ 
        grid.zPlane(zIndex, data);}
    types::Real topZCoordinate() const;
    void getZIsoSurface(types::Real isoValue, std::vector<types::Real> &data){ 
        grid.zIsoSurface(isoValue, data);}

    void print() const;
	void addHeader(types::Stream &stream) const;
	void addData(types::Stream &stream) const;

    types::Uint countAtoms() const;
	types::Uint countPoints() const;

    void setTitle(types::String t) { title=t;}
    types::String getTitle() const { return title;}
    void setDescription(types::String d) { description=d;}
    types::String getDescription() const { return description;}
    void setGrid(const CubeGrid &g) { grid = g;}
    const CubeGrid & getGrid() { return grid;}
    void setFileName(types::String fileName) { this->fileName = fileName; }
    types::String getFileName() const { return this->fileName; }


    types::Real distance(std::vector<types::Real> coordinates) const;

    types::Uint nX() const  { return grid.nX(); }
    types::Uint nY() const  { return grid.nY(); }
    types::Uint nZ() const  { return grid.nZ(); }
    types::Real dX() const  { return grid.dX(); }
    types::Real dY() const  { return grid.dY(); }
    types::Real dZ() const  { return grid.dZ(); }
};

/**
 * Cube file of a wave function.
 * CP2K will write the level and the spin in the title
 */
struct WfnCube : public Cube {
    types::Uint spin;       /**< as read from CP2K cube file */
    types::Uint wfn;        /**< as read from CP2K cube file */
    types::Real energy;     /**< [a.u.], read from CP2K output */
   
	bool readCubeFile();
    bool readCubeFile(types::String filename);
    bool readDescription(types::String filename);

    void setEnergy(types::Real e){ energy = e; }
    types::Real getEnergy() { return energy; }
    types::Uint getSpin() { return spin; }
    types::Uint getLevel() { return wfn; }

    WfnCube(){}; 
    WfnCube(const Cube& c){ 
        atoms=c.atoms;
        grid=c.grid;
        title=c.title;
        description=c.description;
        fileName=c.fileName;
    }
};

}
#endif
