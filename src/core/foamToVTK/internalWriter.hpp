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
    Foam::internalWriter

Description
    Write fields (internal).

SourceFiles
    internalWriter.C
    internalWriterTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef internalWriter_H
#define internalWriter_H

#include "OpenFOAM/db/IOstreams/Fstreams/OFstream.hpp"
#include "finiteVolume/fields/volFields/volFields.hpp"
#include "OpenFOAM/fields/GeometricFields/pointFields/pointFields.hpp"
#include "foamToVTK/vtkMesh.hpp"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class volPointInterpolation;

/*---------------------------------------------------------------------------*\
                           Class internalWriter Declaration
\*---------------------------------------------------------------------------*/

class internalWriter
{
    const vtkMesh& vMesh_;

    const bool binary_;

    const fileName fName_;

    std::ofstream os_;

public:

    // Constructors

        //- Construct from components
        internalWriter
        (
            const vtkMesh&,
            const bool binary,
            const fileName&
        );


    // Member Functions

        std::ofstream& os()
        {
            return os_;
        }

        //- Write cellIDs
        void writeCellIDs();

        //- Write generic GeometricFields
        template<class Type, template<class> class PatchField, class GeoMesh>
        void write
        (
            const UPtrList<const GeometricField<Type, PatchField, GeoMesh>>&
        );

        //- Interpolate and write volFields
        template<class Type>
        void write
        (
            const volPointInterpolation&,
            const UPtrList<const GeometricField<Type, fvPatchField, volMesh>>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "foamToVTK/internalWriterTemplates.cpp"
#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
