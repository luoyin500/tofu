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

Application
    Test-volField

\*---------------------------------------------------------------------------*/

#include "finiteVolume/cfdTools/general/include/fvCFD.hpp"
#include "OpenFOAM/fields/GeometricFields/transformGeometricField/transformGeometricField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#include "OpenFOAM/include/setRootCase.hpp"

#include "OpenFOAM/include/createTime.hpp"
#include "OpenFOAM/include/createMesh.hpp"

    Info<< "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< nl
        << "p.v().size(): "
        << p.v().size() << endl;

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

#include "finiteVolume/cfdTools/incompressible/createPhi.hpp"

    GeometricField<symmTensor, fvPatchField, volMesh> st
    (
        IOobject
        (
            "st",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensioned<symmTensor>("st", dimless, symmTensor::one),
        zeroGradientFvPatchSymmTensorField::typeName
    );

    SolverPerformance<symmTensor> sP =
    (
        solve
        (
            fvm::ddt(st)
          + fvm::div(phi, st)
          - fvm::laplacian
            (
                dimensionedScalar("D", sqr(dimLength)/dimTime, 1),
                st
            )
         ==
            dimensioned<symmTensor>
            (
                "source",
                dimless/dimTime,
                symmTensor(0, 2, 0, 1, 1.5, 0)
            )
        )
    );

    Info<< nl
        << "Detailed SolverPerformance<symmTensor>: " << nl
        << "  " << sP << endl;

    Info<< nl
        << "solverPerformanceDict: "
        << mesh.solverPerformanceDict() << endl;

    return 0;
}


// ************************************************************************* //
