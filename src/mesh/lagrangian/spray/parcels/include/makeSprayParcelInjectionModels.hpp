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

#ifndef makeSprayParcelInjectionModels_H
#define makeSprayParcelInjectionModels_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/CellZoneInjection/CellZoneInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/ConeInjection/ConeInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/ConeNozzleInjection/ConeNozzleInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/FieldActivatedInjection/FieldActivatedInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/InflationInjection/InflationInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/ManualInjection/ManualInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/NoInjection/NoInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/PatchInjection/PatchInjection.hpp"
#include "lagrangian/intermediate/submodels/Kinematic/InjectionModel/PatchFlowRateInjection/PatchFlowRateInjection.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSprayParcelInjectionModels(CloudType)                              \
                                                                               \
    makeInjectionModel(CloudType);                                             \
                                                                               \
    makeInjectionModelType(CellZoneInjection, CloudType);                      \
    makeInjectionModelType(ConeInjection, CloudType);                          \
    makeInjectionModelType(ConeNozzleInjection, CloudType);                    \
    makeInjectionModelType(FieldActivatedInjection, CloudType);                \
    makeInjectionModelType(InflationInjection, CloudType);                     \
    makeInjectionModelType(ManualInjection, CloudType);                        \
    makeInjectionModelType(NoInjection, CloudType);                            \
    makeInjectionModelType(PatchFlowRateInjection, CloudType);                 \
    makeInjectionModelType(PatchInjection, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
