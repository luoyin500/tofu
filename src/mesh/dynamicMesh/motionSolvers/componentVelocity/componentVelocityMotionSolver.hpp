/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2018 OpenFOAM Foundation
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
    Foam::componentVelocityMotionSolver

Description
    Virtual base class for velocity motion solver

    The boundary displacement is set as a boundary condition
    on the pointMotionUX pointScalarField.

SourceFiles
    componentVelocityMotionSolver.C

\*---------------------------------------------------------------------------*/

#ifndef componentVelocityMotionSolver_H
#define componentVelocityMotionSolver_H

#include "dynamicMesh/motionSolvers/motionSolver/motionSolver.hpp"
#include "OpenFOAM/fields/GeometricFields/pointFields/pointFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                   Class componentVelocityMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class componentVelocityMotionSolver
:
    public motionSolver
{
protected:

    // Protected data

        //- The component name to solve for
        word cmptName_;

        //- The component to solve for
        direction cmpt_;

        //- Point motion field
        mutable pointScalarField pointMotionU_;


private:

    // Private Member Functions

        //- Return the component corresponding to the given component name
        direction cmpt(const word& cmptName) const;

        //- Disallow default bitwise copy construct
        componentVelocityMotionSolver
        (
            const componentVelocityMotionSolver&
        );

        //- Disallow default bitwise assignment
        void operator=(const componentVelocityMotionSolver&);


public:

    //- Runtime type information
    TypeName("componentVelocityMotionSolver");


    // Constructors

        //- Construct from mesh and dictionary
        componentVelocityMotionSolver
        (
            const polyMesh&,
            const IOdictionary&,
            const word& type
        );


    //- Destructor
    virtual ~componentVelocityMotionSolver();


    // Member Functions

        //- Non-const access to the pointMotionU in order to allow changes
        //  to the boundary motion
        pointScalarField& pointMotionU()
        {
            return pointMotionU_;
        }

        //- Update local data for geometry changes
        virtual void movePoints(const pointField&);

        //-  Update local data for topology changes
        virtual void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
