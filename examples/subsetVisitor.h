#ifndef PARATREET_SUBSETVISITOR_H
#define PARATREET_SUBSETVISITOR_H_
#pragma once

#include "paratreet.decl.h"
#include "common.h"
#include "Space.h"
#include <cmath>
#include <vector>
#include "CentroidData.h"
#include "subsetCreator.h"
#include "FoF.decl.h"

extern Real linkingLength;
extern Vector3D<Real> fPeriod;

class subsetVisitor {

private:
    Vector3D<Real> offset;

public:

    static constexpr const bool CallSelfLeaf = false;

    subsetVisitor() : offset(0, 0, 0) {}
    subsetVisitor(Vector3D<Real> offseti) : offset(offseti) {}

    void pup(PUP::er& p) {}

    bool open(const SpatialNode<CentroidData>& source, SpatialNode<CentroidData>& target)
    {
        Real r_bucket = target.data.size_sm + linkingLength;
        if (!Space::intersect(source.data.box, target.data.box.center()+offset, r_bucket*r_bucket))
        return false;

        // Check if any of the target balls intersect the source volume
        for (int i = 0; i < target.n_particles; i++) {
        Real ballSq = linkingLength * linkingLength;
        //adding offset to target needs to be the same in leaf
        if(Space::intersect(source.data.box, target.particles()[i].position+offset, ballSq))
            return true;
        }
        return false;
    }


    void node(const SpatialNode<CentroidData>& source, SpatialNode<CentroidData>& target)
    {

    }


    void leaf(const SpatialNode<CentroidData>& source, SpatialNode<CentroidData>& target) 
    {
        for (int i = 0; i < target.n_particles; i++) {
            for (int j = 0; j < source.n_particles; j++) {
                const Particle& sp = source.particles()[j];
                const Particle& tp = target.particles()[i];
                if(sp.group_number == tp.group_number)
                {
                    //if(target.)
                }
            }
        }
    }
};



#endif
