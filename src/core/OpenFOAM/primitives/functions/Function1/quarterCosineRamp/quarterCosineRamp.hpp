/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2018 OpenFOAM Foundation
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
    Foam::Function1Types::quarterCosineRamp

Description
    Quarter-cosine ramp function starting from 0 and increasing to 1 from \c
    start over the \c duration and remaining at 1 thereafter.

See also
    Foam::Function1Types::ramp

SourceFiles
    quarterCosineRamp.C

\*---------------------------------------------------------------------------*/

#ifndef quarterCosineRamp_H
#define quarterCosineRamp_H

#include "OpenFOAM/primitives/functions/Function1/ramp/ramp.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Function1Types
{

/*---------------------------------------------------------------------------*\
                           Class quarterCosineRamp Declaration
\*---------------------------------------------------------------------------*/

class quarterCosineRamp
:
    public ramp
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const quarterCosineRamp&);


public:

    // Runtime type information
    TypeName("quarterCosineRamp");


    // Constructors

        //- Construct from entry name and dictionary
        quarterCosineRamp
        (
            const word& entryName,
            const dictionary& dict
        );


    //- Destructor
    virtual ~quarterCosineRamp();


    // Member Functions

        //- Return value for time t
        virtual inline scalar value(const scalar t) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Function1Types
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OpenFOAM/primitives/functions/Function1/quarterCosineRamp/quarterCosineRampI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
