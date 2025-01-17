/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2016-2018 OpenFOAM Foundation
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
    Foam::functionObjects::timeControl

Description

Note
    Since the timeIndex is used directly from Foam::Time, it is unaffected
    by user-time conversions. For example, Foam::engineTime might cause \a
    writeInterval to be degrees crank angle, but the functionObject
    execution \a interval would still be in timestep.

SourceFiles
    timeControlFunctionObject.C

\*---------------------------------------------------------------------------*/

#ifndef timeControlFunctionObject_H
#define timeControlFunctionObject_H

#include "OpenFOAM/db/functionObjects/functionObject/functionObject.hpp"
#include "OpenFOAM/db/dictionary/dictionary.hpp"
#include "OpenFOAM/db/functionObjects/timeControl/timeControl.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                 Class timeControl Declaration
\*---------------------------------------------------------------------------*/

class timeControl
:
    public functionObject
{
    // Private data

        //- Reference to the time database
        const Time& time_;

        //- Input dictionary
        dictionary dict_;


        // Optional user inputs

            //- Activation time - defaults to -vGreat
            scalar timeStart_;

            //- De-activation time - defaults to vGreat
            scalar timeEnd_;

            //- Number of steps before the dump-time during which deltaT
            //  may be changed (valid for adjustableRunTime)
            label nStepsToStartTimeChange_;


        //- Execute controls
        Foam::timeControl executeControl_;

        //- Write controls
        Foam::timeControl writeControl_;

        //- The functionObject to execute
        autoPtr<functionObject> foPtr_;


    // Private Member Functions

        //- Read relevant dictionary entries
        void readControls();

        //- Returns true if within time bounds
        bool active() const;

        //- Disallow default bitwise copy construct
        timeControl(const timeControl&);

        //- Disallow default bitwise assignment
        void operator=(const timeControl&);


public:

    //- Runtime type information
    TypeName("timeControl");


    // Constructors

        //- Construct from components
        timeControl
        (
            const word& name,
            const Time&,
            const dictionary&
        );


    // Member Functions

        // Access

            //- Return time database
            inline const Time& time() const;

            //- Return the input dictionary
            inline const dictionary& dict() const;

            //- Return the execute control object
            inline const Foam::timeControl& executeControl() const;

            //- Return the write control object
            inline const Foam::timeControl& writeControl() const;

            //- Return the functionObject filter
            inline const functionObject& filter() const;


        // Function object control

            //- Called at each ++ or += of the time-loop.
            //  postProcess overrides the usual executeControl behaviour and
            //  forces execution (used in post-processing mode)
            virtual bool execute();

            //- Called at each ++ or += of the time-loop.
            //  postProcess overrides the usual writeControl behaviour and
            //  forces writing (used in post-processing mode)
            virtual bool write();

            //- Called when Time::run() determines that the time-loop exits
            virtual bool end();

            //- Return the time to the next write
            virtual scalar timeToNextWrite();

            //- Read and set the function object if its data have changed
            virtual bool read(const dictionary&);

            //- Update for changes of mesh
            virtual void updateMesh(const mapPolyMesh& mpm);

            //- Update for changes of mesh
            virtual void movePoints(const polyMesh& mesh);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OpenFOAM/db/functionObjects/timeControl/timeControlFunctionObjectI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
