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

\*---------------------------------------------------------------------------*/

#include "lagrangian/intermediate/submodels/Kinematic/CollisionModel/NoCollision/NoCollision.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::NoCollision<CloudType>::NoCollision
(
    const dictionary& dict,
    CloudType& owner
)
:
    CollisionModel<CloudType>(owner)
{}


template<class CloudType>
Foam::NoCollision<CloudType>::NoCollision
(
    const NoCollision<CloudType>& cm
)
:
    CollisionModel<CloudType>(cm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::NoCollision<CloudType>::~NoCollision()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
Foam::label Foam::NoCollision<CloudType>::nSubCycles() const
{
    return 1;
}


template<class CloudType>
bool Foam::NoCollision<CloudType>::active() const
{
    return false;
}


template<class CloudType>
void Foam::NoCollision<CloudType>::collide()
{}


// ************************************************************************* //
