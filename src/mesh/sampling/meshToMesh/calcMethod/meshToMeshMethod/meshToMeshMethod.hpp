/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2018 OpenFOAM Foundation
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
    Foam::meshToMeshMethod

Description
    Base class for mesh-to-mesh calculation methods

SourceFiles
    meshToMeshMethod.C

\*---------------------------------------------------------------------------*/

#ifndef meshToMeshMethod_H
#define meshToMeshMethod_H

#include "OpenFOAM/meshes/polyMesh/polyMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class meshToMeshMethod Declaration
\*---------------------------------------------------------------------------*/

class meshToMeshMethod
{

protected:

    // Protected data

        //- Reference to the source mesh
        const polyMesh& src_;

        //- Reference to the target mesh
        const polyMesh& tgt_;

        //- Cell total volume in overlap region [m3]
        scalar V_;

        //- Tolerance used in volume overlap calculations
        static scalar tolerance_;


    // Protected Member Functions

        //- Return src cell IDs for the overlap region
        labelList maskCells() const;

        //- Return the true if cells intersect
        virtual bool intersect
        (
            const label srcCelli,
            const label tgtCelli
        ) const;

        //- Return the intersection volume between two cells
        virtual scalar interVol
        (
            const label srcCelli,
            const label tgtCelli
        ) const;

        //- Append target cell neihgbour cells to cellIDs list
        virtual void appendNbrCells
        (
            const label tgtCelli,
            const polyMesh& mesh,
            const DynamicList<label>& visitedTgtCells,
            DynamicList<label>& nbrTgtCellIDs
        ) const;

        virtual bool initialise
        (
            labelListList& srcToTgtAddr,
            scalarListList& srcToTgtWght,
            labelListList& tgtToTgtAddr,
            scalarListList& tgtToTgtWght
        ) const;


private:

    // Private member functions

        //- Disallow default bitwise copy construct
        meshToMeshMethod(const meshToMeshMethod&);

        //- Disallow default bitwise assignment
        void operator=(const meshToMeshMethod&);


public:

    //- Run-time type information
    TypeName("meshToMeshMethod");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        meshToMeshMethod,
        components,
        (
            const polyMesh& src,
            const polyMesh& tgt
        ),
        (src, tgt)
    );

    //- Construct from source and target meshes
    meshToMeshMethod(const polyMesh& src, const polyMesh& tgt);

    //- Selector
    static autoPtr<meshToMeshMethod> New
    (
        const word& methodName,
        const polyMesh& src,
        const polyMesh& tgt
    );


    //- Destructor
    virtual ~meshToMeshMethod();


    // Member Functions

        // Evaluate

            //- Calculate addressing and weights
            virtual void calculate
            (
                labelListList& srcToTgtAddr,
                scalarListList& srcToTgtWght,
                labelListList& tgtToTgtAddr,
                scalarListList& tgtToTgtWght
            ) = 0;


        // Access

            //- Return const access to the source mesh
            inline const polyMesh& src() const;

            //- Return const access to the target mesh
            inline const polyMesh& tgt() const;

            //- Return const access to the overlap volume
            inline scalar V() const;


        // Check

            //- Write the connectivity (debugging)
            void writeConnectivity
            (
                const polyMesh& mesh1,
                const polyMesh& mesh2,
                const labelListList& mesh1ToMesh2Addr
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "sampling/meshToMesh/calcMethod/meshToMeshMethod/meshToMeshMethodI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
