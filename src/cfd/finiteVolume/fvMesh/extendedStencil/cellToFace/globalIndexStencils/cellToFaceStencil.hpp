/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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
    Foam::cellToFaceStencil

Description
    Base class for extended cell-to-face stencils (face values from
    neighbouring cells)

SourceFiles
    faceStencil.C

\*---------------------------------------------------------------------------*/

#ifndef cellToFaceStencil_H
#define cellToFaceStencil_H

#include "OpenFOAM/meshes/polyMesh/globalMeshData/globalIndex.hpp"
#include "OpenFOAM/primitives/bools/lists/boolList.hpp"
#include "OpenFOAM/containers/HashTables/HashSet/HashSet.hpp"
#include "OpenFOAM/meshes/primitiveMesh/primitivePatch/indirectPrimitivePatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class polyMesh;

/*---------------------------------------------------------------------------*\
                      Class cellToFaceStencil Declaration
\*---------------------------------------------------------------------------*/

class cellToFaceStencil
:
    public labelListList
{
    // Private data

        const polyMesh& mesh_;

        //- Global numbering for cells and boundary faces
        const globalIndex globalNumbering_;


protected:

        //- Merge two lists.
        //  Guarantee global0, global1 are first.
        static void merge
        (
            const label global0,
            const label global1,
            const labelList& listA,
            labelList& listB
        );

        //- Merge two lists and guarantee globalI is first.
        static void merge
        (
            const label globalI,
            const labelList& pGlobals,
            labelList& cCells
        );

        //- Valid boundary faces (not empty and not coupled)
        void validBoundaryFaces(boolList& isValidBFace) const;

        //- Return patch of all coupled faces.
        autoPtr<indirectPrimitivePatch> allCoupledFacesPatch() const;

        //- Combine operator for labelLists
        class unionEqOp
        {
            public:
            void operator()( labelList& x, const labelList& y ) const;
        };

        //- Collect cell neighbours of faces in global numbering
        void insertFaceCells
        (
            const label exclude0,
            const label exclude1,
            const boolList& nonEmptyFace,
            const labelList& faceLabels,
            labelHashSet& globals
        ) const;

        //- Collect cell neighbours of faces in global numbering
        labelList calcFaceCells
        (
            const boolList& nonEmptyFace,
            const labelList& faceLabels,
            labelHashSet& globals
        ) const;


        //- Collect cell neighbours into extended stencil
        void calcFaceStencil
        (
            const labelListList& globalCellCells,
            labelListList& faceStencil
        ) const;


public:

    // Constructors

        //- Construct from mesh
        explicit cellToFaceStencil(const polyMesh&);


    // Member Functions

        const polyMesh& mesh() const
        {
            return mesh_;
        }

        //- Global numbering for cells and boundary faces
        const globalIndex& globalNumbering() const
        {
            return globalNumbering_;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
