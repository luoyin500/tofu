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

\*---------------------------------------------------------------------------*/

#include "lagrangian/intermediate/submodels/MPPIC/CorrectionLimitingMethods/relative/relative.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"

namespace Foam
{
namespace CorrectionLimitingMethods
{
    defineTypeNameAndDebug(relative, 0);

    addToRunTimeSelectionTable
    (
        CorrectionLimitingMethod,
        relative,
        dictionary
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::CorrectionLimitingMethods::relative::relative(const dictionary& dict)
:
    CorrectionLimitingMethod(dict),
    e_(readScalar(dict.lookup("e")))
{}


Foam::CorrectionLimitingMethods::relative::relative(const relative& cl)
:
    CorrectionLimitingMethod(cl),
    e_(cl.e_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::CorrectionLimitingMethods::relative::~relative()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::vector Foam::CorrectionLimitingMethods::relative::limitedVelocity
(
    const vector uP,
    const vector dU,
    const vector uMean
) const
{
    const vector uRelative = uP - uMean;

    return minMod
    (
        dU,
      - (1.0 + this->e_)*uRelative
    );
}


// ************************************************************************* //
