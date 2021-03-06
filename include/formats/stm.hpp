/*
 * cube.h
 */
#ifndef FORMATS_STM_H
#define FORMATS_STM_H

#include "types.hpp"
#include "la.hpp"
#include "atomistic/fundamental.hpp"
#include "formats/cube.hpp"
#include "formats/cp2k.hpp"

namespace formats {
namespace stm {

class StsCube;    

/**
 * STM image
 * List of WfnCubes and bias
 */
class StmCube : public formats::Cube {
    public : enum Mode { CONSTANT_CURRENT = 0, CONSTANT_HEIGHT = 1 };
    private:
        std::vector<WfnCube> levels;
        types::Real bias;
        types::Real setValue;
        Mode mode;
        std::vector<types::Real> stm;

    public:
//        StmImage(types::Real b) : bias(b) {};
        StmCube() {};
//        bool findCubes(std::vector<WfnCube> &list);
        types::Real getBias() { return bias; }
        void setIsoValue(types::Real isoValue); 
        void setZValue(types::Real zValue); 
        bool writeIgorFile(types::String fileName) const;
        bool readIgorFile(types::String fileName);
        std::vector<types::Real> getZProfile() const { return stm; }
        void setZProfile(const std::vector<types::Real> &p) { stm = p; }

        friend class StsCube;
};

/**
 * STS in 2d
 * 
 */
class StsCube : public formats::Cube {
    public : enum ModeFlag { CONSTANT_Z, PROFILE };
    private:
        std::list< formats::WfnCube > levels;
        types::Uint zIndex;
        StmCube stm;
        types::Real eMin;
        types::Real eMax;
        types::Real deltaE;
        types::Real sigma;
        ModeFlag modeFlag;
        types::String modeParameter;
        types::Real cubeZ;
        bool psiSquared;
       
        bool initialize(); 
        void addLevel(
                const std::vector<types::Real> &plane,
                types::Real energy);

    public:
        StsCube(
            const std::list< formats::WfnCube > &levels,
            types::Real eMin,
            types::Real eMax,
            types::Real deltaE,
            types::Real Fwhm,
            ModeFlag modeFlag,
            types::String modeParameter,
            types::Real cubeZ,
            bool psiSquared) :
            levels(levels),
            eMin(eMin), 
            eMax(eMax), 
            deltaE(deltaE), 
            sigma(Fwhm/2.355),
            modeFlag(modeFlag),
            modeParameter(modeParameter),
            cubeZ(cubeZ),
            psiSquared(psiSquared)
    {       calculate();             }
        StsCube(){};

        bool calculate();
        types::Real getEMin();
        types::Real getEMax();
        types::Real getDeltaE();
        void interpolateOnZProfile(const formats::CubeGrid &grid, 
                std::vector<types::Real> &result) const;
        void setStm(const StmCube &s){ stm = s; }
        const StmCube & getStm(){ return stm; }
};
    
class WfnExtrapolation {
public:
    enum Mode { plane = 0, isoSurface = 1, rollingBall = 2 };
    void onPlane(WfnCube& wfn);
    void onSurface(WfnCube& wfn, types::Uint nKX, types::Uint nKY);
    void execute();
    void determineRange();
    void setSurfacePotential();
    WfnExtrapolation(){};
    WfnExtrapolation(std::vector<types::String> fileNames,
                     const cp2k::Spectrum &spectrum,
                     Cube&   hartree,
                     Mode          mode    ,
                     types::Real   var1    ,
                     types::Real   isoLevel,
                     types::Real   approachFrom,
                     types::Real   kPlaneMax,
                     types::Uint   nLayers
                     );
private:
    std::vector<WfnCube> wfns;
    cp2k::Spectrum     spectrum;
    Cube               hartree;
    Mode               mode;
    types::Real        zStart;
    types::Uint        zStartIndex;
    types::Real        zWidth;
    types::Uint        zEndIndex;
    types::Uint        zSurfEndIndex;
    types::Real        isoValue;
    types::Real        approachFrom;
    types::Real        kPlaneMax;
    types::Uint        nLayers;
    std::vector<types::Real> surface;
    std::vector<types::Uint> zIndices;
    types::Real         surfacePotential;
    types::Real        ballRadius;




};
    

}
}
#endif
