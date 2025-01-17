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

#include "meshTools/coordinateSystems/coordinateRotation/EulerCoordinateRotation.hpp"

#include "OpenFOAM/global/constants/mathematical/mathematicalConstants.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(EulerCoordinateRotation, 0);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        EulerCoordinateRotation,
        dictionary
    );
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        EulerCoordinateRotation,
        objectRegistry
    );
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::vector Foam::EulerCoordinateRotation::transform(const vector& st) const
{
    return (R_ & st);
}


Foam::vector Foam::EulerCoordinateRotation::invTransform
(
    const vector& st
) const
{
    return (Rtr_ & st);
}


Foam::tmp<Foam::vectorField> Foam::EulerCoordinateRotation::transform
(
    const vectorField& st
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


Foam::tmp<Foam::vectorField> Foam::EulerCoordinateRotation::invTransform
(
    const vectorField& st
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


const Foam::tensorField& Foam::EulerCoordinateRotation::Tr() const
{
    NotImplemented;
    return NullObjectRef<tensorField>();
}


Foam::tmp<Foam::tensorField> Foam::EulerCoordinateRotation::transformTensor
(
    const tensorField& st
) const
{
     NotImplemented;
    return tmp<tensorField>(nullptr);
}


Foam::tensor Foam::EulerCoordinateRotation::transformTensor
(
    const tensor& st
) const
{
    return (R_ & st & Rtr_);
}


Foam::tmp<Foam::tensorField> Foam::EulerCoordinateRotation::transformTensor
(
    const tensorField& st,
    const labelList& cellMap
) const
{
    NotImplemented;
    return tmp<tensorField>(nullptr);
}


Foam::tmp<Foam::symmTensorField> Foam::EulerCoordinateRotation::
transformVector
(
    const vectorField& st
) const
{
    tmp<symmTensorField> tfld(new symmTensorField(st.size()));
    symmTensorField& fld = tfld.ref();

    forAll(fld, i)
    {
        fld[i] = transformPrincipal(R_, st[i]);
    }
    return tfld;
}


Foam::symmTensor Foam::EulerCoordinateRotation::transformVector
(
    const vector& st
) const
{
    return transformPrincipal(R_, st);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::EulerCoordinateRotation::calcTransform
(
    const scalar phiAngle,
    const scalar thetaAngle,
    const scalar psiAngle,
    const bool inDegrees
)
{
    scalar phi   = phiAngle;
    scalar theta = thetaAngle;
    scalar psi   = psiAngle;

    if (inDegrees)
    {
        phi   *= constant::mathematical::pi/180.0;
        theta *= constant::mathematical::pi/180.0;
        psi   *= constant::mathematical::pi/180.0;
    }

    R_ =
    (
        tensor
        (
            cos(phi)*cos(psi) - sin(phi)*sin(psi)*cos(theta),
            -sin(phi)*cos(psi)*cos(theta) - cos(phi)*sin(psi),
            sin(phi)*sin(theta),

            cos(phi)*sin(psi)*cos(theta) + sin(phi)*cos(psi),
            cos(phi)*cos(psi)*cos(theta) - sin(phi)*sin(psi),
            -cos(phi)*sin(theta),

            sin(psi)*sin(theta),
            cos(psi)*sin(theta),
            cos(theta)
        )
    );

    Rtr_ = R_.T();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::EulerCoordinateRotation::EulerCoordinateRotation()
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{}


Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const vector& phiThetaPsi,
    const bool inDegrees
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    calcTransform
    (
        phiThetaPsi.component(vector::X),
        phiThetaPsi.component(vector::Y),
        phiThetaPsi.component(vector::Z),
        inDegrees
    );
}


Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const scalar phiAngle,
    const scalar thetaAngle,
    const scalar psiAngle,
    const bool inDegrees
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    calcTransform(phiAngle, thetaAngle, psiAngle, inDegrees);
}


Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const dictionary& dict
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}


Foam::EulerCoordinateRotation::EulerCoordinateRotation
(
    const dictionary& dict,
    const objectRegistry&
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}


void Foam::EulerCoordinateRotation::write(Ostream& os) const
{
     os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
     os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
     os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
