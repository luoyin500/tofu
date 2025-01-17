/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2018 OpenFOAM Foundation
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

#include "mesh/snappyHexMesh/externalDisplacementMeshMover/zeroFixedValue/zeroFixedValuePointPatchField.hpp"

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

template<class Type>
Foam::zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(p, iF)
{}


template<class Type>
Foam::zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    fixedValuePointPatchField<Type>(p, iF, dict, false)
{
    fixedValuePointPatchField<Type>::operator=(Zero);
}


template<class Type>
Foam::zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
    const zeroFixedValuePointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    fixedValuePointPatchField<Type>(ptf, p, iF, mapper)
{
    // For safety re-evaluate
    fixedValuePointPatchField<Type>::operator=(Zero);
}


template<class Type>
Foam::zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
    const zeroFixedValuePointPatchField<Type>& ptf
)
:
    fixedValuePointPatchField<Type>(ptf)
{}


template<class Type>
Foam::zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
    const zeroFixedValuePointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(ptf, iF)
{
    // For safety re-evaluate
    fixedValuePointPatchField<Type>::operator=(Zero);
}


// ************************************************************************* //
