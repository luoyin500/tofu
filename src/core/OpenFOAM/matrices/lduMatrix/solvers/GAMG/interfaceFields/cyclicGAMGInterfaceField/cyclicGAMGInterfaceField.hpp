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
    Foam::cyclicGAMGInterfaceField

Description
    GAMG agglomerated cyclic interface field.

SourceFiles
    cyclicGAMGInterfaceField.C

\*---------------------------------------------------------------------------*/

#ifndef cyclicGAMGInterfaceField_H
#define cyclicGAMGInterfaceField_H

#include "OpenFOAM/matrices/lduMatrix/solvers/GAMG/interfaceFields/GAMGInterfaceField/GAMGInterfaceField.hpp"
#include "OpenFOAM/matrices/lduMatrix/solvers/GAMG/interfaces/cyclicGAMGInterface/cyclicGAMGInterface.hpp"
#include "OpenFOAM/matrices/lduMatrix/lduAddressing/lduInterfaceFields/cyclicLduInterfaceField/cyclicLduInterfaceField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class cyclicGAMGInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class cyclicGAMGInterfaceField
:
    public GAMGInterfaceField,
    public cyclicLduInterfaceField
{
    // Private data

        //- Local reference cast into the cyclic interface
        const cyclicGAMGInterface& cyclicInterface_;

        //- Is the transform required
        bool doTransform_;

        //- Rank of component for transformation
        int rank_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        cyclicGAMGInterfaceField(const cyclicGAMGInterfaceField&);

        //- Disallow default bitwise assignment
        void operator=(const cyclicGAMGInterfaceField&);


public:

    //- Runtime type information
    TypeName("cyclic");


    // Constructors

        //- Construct from GAMG interface and fine level interface field
        cyclicGAMGInterfaceField
        (
            const GAMGInterface& GAMGCp,
            const lduInterfaceField& fineInterfaceField
        );

        //- Construct from GAMG interface and fine level interface field
        cyclicGAMGInterfaceField
        (
            const GAMGInterface& GAMGCp,
            const bool doTransform,
            const int rank
        );


    //- Destructor
    virtual ~cyclicGAMGInterfaceField();


    // Member Functions

        // Access

            //- Return size
            label size() const
            {
                return cyclicInterface_.size();
            }


        //- Cyclic interface functions

            //- Does the interface field perform the transformation
            virtual bool doTransform() const
            {
                return doTransform_;
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicInterface_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicInterface_.reverseT();
            }

            //- Return rank of component for transform
            virtual int rank() const
            {
                return rank_;
            }


        // Interface matrix update

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                scalarField& result,
                const scalarField& psiInternal,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
