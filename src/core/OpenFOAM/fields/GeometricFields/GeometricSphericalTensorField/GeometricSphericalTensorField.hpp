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

InClass
    Foam::GeometricSphericalTensorField

Description
    SphericalTensor specific part of the implementation of GeometricField.

SourceFiles
    GeometricSphericalTensorField.C

\*---------------------------------------------------------------------------*/

#ifndef GeometricSphericalTensorField_H
#define GeometricSphericalTensorField_H

#include "OpenFOAM/fields/GeometricFields/GeometricField/GeometricField.hpp"
#include "OpenFOAM/primitives/SphericalTensor/sphericalTensor/sphericalTensor.hpp"

#define TEMPLATE template<template<class> class PatchField, class GeoMesh>
#include "OpenFOAM/fields/GeometricFields/GeometricField/GeometricFieldFunctionsM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

UNARY_FUNCTION(scalar, sphericalTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, sph, transform)
UNARY_FUNCTION(scalar, sphericalTensor, det, transform)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, inv, inv)

BINARY_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, '|', divide)
BINARY_TYPE_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, '|', divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OpenFOAM/fields/Fields/Field/undefFieldFunctionsM.hpp"

#ifdef NoRepository
#include "OpenFOAM/fields/GeometricFields/GeometricSphericalTensorField/GeometricSphericalTensorField.cpp"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //