/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2018 OpenFOAM Foundation
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

#include "finiteVolume/cfdTools/general/solutionControl/solutionControl/singleRegionSolutionControl/singleRegionSolutionControl.hpp"
#include "finiteVolume/fields/volFields/volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(singleRegionSolutionControl, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::singleRegionSolutionControl::singleRegionSolutionControl
(
    fvMesh& mesh,
    const word& algorithmName
)
:
    solutionControl(mesh, mesh.time(), algorithmName),
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::singleRegionSolutionControl::~singleRegionSolutionControl()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::dictionary& Foam::singleRegionSolutionControl::dict() const
{
    return mesh_.solutionDict().subDict(algorithmName());
}


void Foam::singleRegionSolutionControl::storePrevIterFields()
{
    storePrevIterTypeFields<scalar>();
    storePrevIterTypeFields<vector>();
    storePrevIterTypeFields<sphericalTensor>();
    storePrevIterTypeFields<symmTensor>();
    storePrevIterTypeFields<tensor>();
}


// ************************************************************************* //
