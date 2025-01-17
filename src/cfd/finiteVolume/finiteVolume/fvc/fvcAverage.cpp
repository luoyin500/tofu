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

#include "finiteVolume/finiteVolume/fvc/fvcAverage.hpp"
#include "finiteVolume/finiteVolume/fvc/fvcSurfaceIntegrate.hpp"
#include "finiteVolume/fvMesh/fvMesh.hpp"
#include "finiteVolume/interpolation/surfaceInterpolation/schemes/linear/linear.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
    const fvMesh& mesh = ssf.mesh();

    tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "average("+ssf.name()+')',
                ssf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensioned<Type>("0", ssf.dimensions(), Zero)
        )
    );

    if (!mesh.nGeometricD())
    {
        return taverage;
    }

    GeometricField<Type, fvPatchField, volMesh>& av = taverage.ref();

    av.primitiveFieldRef() =
    (
        surfaceSum(mesh.magSf()*ssf)().primitiveField()
       /surfaceSum(mesh.magSf())().primitiveField()
    );

    typename GeometricField<Type, fvPatchField, volMesh>::
    Boundary& bav = av.boundaryFieldRef();

    forAll(bav, patchi)
    {
        bav[patchi] = ssf.boundaryField()[patchi];
    }

    av.correctBoundaryConditions();

    return taverage;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>& tssf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
    (
        fvc::average(tssf())
    );
    tssf.clear();
    return taverage;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
    const GeometricField<Type, fvPatchField, volMesh>& vtf
)
{
    return fvc::average(linearInterpolate(vtf));
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
    const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvtf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
    (
        fvc::average(tvtf())
    );
    tvtf.clear();
    return taverage;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
