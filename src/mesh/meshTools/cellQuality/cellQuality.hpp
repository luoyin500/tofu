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
    Foam::cellQuality

Description
    Class calculates cell quality measures.

SourceFiles
    cellQuality.C

\*---------------------------------------------------------------------------*/

#ifndef cellQuality_H
#define cellQuality_H

#include "OpenFOAM/meshes/polyMesh/polyMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                           Class cellQuality Declaration
\*---------------------------------------------------------------------------*/

class cellQuality
{
    // Private data

        //- Reference to mesh
        const polyMesh& mesh_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        cellQuality(const cellQuality&);

        //- Disallow default bitwise assignment
        void operator=(const cellQuality&);


public:

    // Constructors

        //- Construct from mesh
        cellQuality(const polyMesh& mesh);


    // Destructor - default


    // Member Functions

        //- Return cell non-orthogonality
        tmp<scalarField> nonOrthogonality() const;

        //- Return cell skewness
        tmp<scalarField> skewness() const;

        //- Return face non-orthogonality
        tmp<scalarField> faceNonOrthogonality() const;

        //- Return face skewness
        tmp<scalarField> faceSkewness() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
