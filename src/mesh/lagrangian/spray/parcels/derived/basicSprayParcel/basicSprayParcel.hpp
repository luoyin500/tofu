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
    Foam::basicSprayParcel

Description
    Definition of spray parcel

SourceFiles
    basicSprayParcel.C
    basicSprayParcelIO.C

\*---------------------------------------------------------------------------*/

#ifndef basicSprayParcel_H
#define basicSprayParcel_H

#include "OpenFOAM/primitives/contiguous/contiguous.hpp"
#include "lagrangian/basic/particle/particle.hpp"
#include "lagrangian/intermediate/parcels/Templates/KinematicParcel/KinematicParcel.hpp"
#include "lagrangian/intermediate/parcels/Templates/ThermoParcel/ThermoParcel.hpp"
#include "lagrangian/intermediate/parcels/Templates/ReactingParcel/ReactingParcel.hpp"
#include "lagrangian/spray/parcels/Templates/SprayParcel/SprayParcel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    typedef SprayParcel
    <
        ReactingParcel
        <
            ThermoParcel
            <
                KinematicParcel
                <
                    particle
                >
            >
        >
    > basicSprayParcel;

    template<>
    inline bool contiguous<basicSprayParcel>()
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
