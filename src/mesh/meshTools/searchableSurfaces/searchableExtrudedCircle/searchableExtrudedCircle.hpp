/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2016-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::searchableExtrudedCircle

Description
    Searching on edgemesh with constant radius

SourceFiles
    searchableExtrudedCircle.C

\*---------------------------------------------------------------------------*/

#ifndef searchableExtrudedCircle_H
#define searchableExtrudedCircle_H

#include "OpenFOAM/meshes/treeBoundBox/treeBoundBox.hpp"
#include "meshTools/searchableSurfaces/searchableSurface/searchableSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class edgeMesh;
class treeDataEdge;
template <class Type> class indexedOctree;

/*---------------------------------------------------------------------------*\
                  Class searchableExtrudedCircle Declaration
\*---------------------------------------------------------------------------*/

class searchableExtrudedCircle
:
    public searchableSurface
{
    // Private Member Data

        //- Feature
        autoPtr<edgeMesh> eMeshPtr_;

        //- Search structure
        autoPtr<indexedOctree<treeDataEdge>> edgeTree_;

        //- Radius
        const scalar radius_;

        //- Names of regions
        mutable wordList regions_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        searchableExtrudedCircle(const searchableExtrudedCircle&);

        //- Disallow default bitwise assignment
        void operator=(const searchableExtrudedCircle&);


public:

    //- Runtime type information
    TypeName("searchableExtrudedCircle");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        searchableExtrudedCircle
        (
            const IOobject& io,
            const dictionary& dict
        );


    //- Destructor
    virtual ~searchableExtrudedCircle();


    // Member Functions

        virtual const wordList& regions() const;

        //- Whether supports volume type below
        virtual bool hasVolumeType() const
        {
            return true;
        }

        //- Range of local indices that can be returned.
        virtual label size() const;

        //- Get representative set of element coordinates
        //  Usually the element centres (should be of length size()).
        virtual tmp<pointField> coordinates() const;

        //- Get bounding spheres (centre and radius squared), one per element.
        //  Any point on element is guaranteed to be inside.
        virtual void boundingSpheres
        (
            pointField& centres,
            scalarField& radiusSqr
        ) const;

        //- Get the points that define the surface.
        virtual tmp<pointField> points() const
        {
            return coordinates();
        }

        //- Does any part of the surface overlap the supplied bound box?
        virtual bool overlaps(const boundBox& bb) const
        {
            NotImplemented;
            return false;
        }


        // Multiple point queries.

            virtual void findNearest
            (
                const pointField& sample,
                const scalarField& nearestDistSqr,
                List<pointIndexHit>&
            ) const;

            //- Unique to parametric geometry: given points find
            //  an interpolated (along the curve) point on the surface.
            //  The lambdas[0] is equivalent for start, lambdas.last()
            //  is equivalent for end.
            virtual void findParametricNearest
            (
                const point& start,
                const point& end,
                const scalarField& lambdas,
                const scalarField& nearestDistSqr,
                List<pointIndexHit>&
            ) const;

            virtual void findLine
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const
            {
                NotImplemented;
            }

            virtual void findLineAny
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const
            {
                NotImplemented;
            }

            //- Get all intersections in order from start to end.
            virtual void findLineAll
            (
                const pointField& start,
                const pointField& end,
                List<List<pointIndexHit>>&
            ) const
            {
                NotImplemented;
            }

            //- From a set of points and indices get the region
            virtual void getRegion
            (
                const List<pointIndexHit>&,
                labelList& region
            ) const;

            //- From a set of points and indices get the normal
            virtual void getNormal
            (
                const List<pointIndexHit>&,
                vectorField& normal
            ) const;

            //- Determine type (inside/outside/mixed) for point. unknown if
            //  cannot be determined (e.g. non-manifold surface)
            virtual void getVolumeType
            (
                const pointField&,
                List<volumeType>&
            ) const
            {
                NotImplemented;
            }

            bool writeData(Ostream&) const
            {
                NotImplemented;
                return false;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
