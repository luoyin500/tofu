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
    Foam::basicSymmetryPointPatchField

Description
    A Symmetry boundary condition for pointField

SourceFiles
    basicsymmetryPointPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef basicSymmetryPointPatchField_H
#define basicSymmetryPointPatchField_H

#include "OpenFOAM/fields/pointPatchFields/pointPatchField/pointPatchField.hpp"
#include "OpenFOAM/meshes/pointMesh/pointPatches/constraint/symmetry/symmetryPointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class basicSymmetryPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class basicSymmetryPointPatchField
:
    public pointPatchField<Type>
{

public:

    // Constructors

        //- Construct from patch and internal field
        basicSymmetryPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        basicSymmetryPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        basicSymmetryPointPatchField
        (
            const basicSymmetryPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type>> clone() const
        {
            return autoPtr<pointPatchField<Type>>
            (
                new basicSymmetryPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        basicSymmetryPointPatchField
        (
            const basicSymmetryPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type>>
            (
                new basicSymmetryPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::blocking
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "OpenFOAM/fields/pointPatchFields/basic/basicSymmetry/basicSymmetryPointPatchField.cpp"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
