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

Namespace
    Foam::functionObjects

Description
    Namespace for functionObjects.

    OpenFOAM includes a collection of functionObjects selected by the user at
    run-time to manipulate the simulation and provide mechanisms to extract
    field and derived quantities.  Alternatively, the same actions can be
    executed after the simulation using the \c -postProcess command-line option.

    \subsection secFunctionObjects Using function objects

    FunctionObjects are selected by additional entries in the
    $FOAM_CASE/system/controlDict dictionary.  Each object is listed in the \c
    functions sub-dictionary, e.g. to select the \c functionObjectType
    functionObject the following entry would be specified:

    \verbatim
    functions
    {
        <functionObjectName>
        {
            type                functionObjectType;
            libs                ("libMyFunctionObjectlib.so");
            region              defaultRegion;
            enabled             yes;
            timeStart           0;
            timeEnd             10;
            writeControl        writeTime;
            writeInterval       1;
            ...
        }
    }
    \endverbatim

    Where:
    \table
        Property | Description                       | Required | Default value
        type     | Type of function object               | yes      |
        libs     | Libraries containing implementation   | yes      |
        region   | Name of region for multi-region cases | no       |
        enabled  | On/off switch                         | no       | yes
        log      | Log information to standard output    | no       | yes
        timeStart| Start time                            | no       |
        timeEnd  | End time                              | no       |
        executeControl  | See time controls below        | no       | timeStep
        executeInterval | Steps between each execute phase | no     |
        writeControl    | See time controls below        | no       | timeStep
        writeInterval   | Steps between each write phase | no       |
    \endtable

    Time controls:
    \table
        Option            | Description
        timeStep          | Execute/write every 'Interval' time-steps
        writeTime         | Execute/write every 'Interval' output times
        adjustableRunTime | Execute/write every 'Interval' run time period
        runTime           | Execute/write every 'Interval' run time period
        clockTime         | Execute/write every 'Interval' clock time period
        cpuTime           | Execute/write every 'Interval' CPU time period
        none              | Execute/write every time-step
    \endtable

    The sub-dictionary name \c \<functionObjectName\> is chosen by the user, and
    is typically used as the name of the output directory for any data written
    by the functionObject.  The \c type entry defines the type of function
    object properties that follow.  FunctionObjects are packaged into separate
    libraries and the \c libs entry is used to specify which library should be
    loaded.

    Each function object has two separate run phases:

      - The \c execute phase is meant to be used for updating calculations
        or for management tasks.
      - The \c write phase is meant for writing the calculated data to disk.

    For each phase the respective time controls are provided, as listed above.

Class
    Foam::functionObject

Description
    Abstract base-class for Time/database function objects.

See also
    Foam::functionObjectList
    Foam::functionObjects::timeControl

SourceFiles
    functionObject.C

\*---------------------------------------------------------------------------*/

#ifndef functionObject_H
#define functionObject_H

#include "OpenFOAM/db/typeInfo/typeInfo.hpp"
#include "OpenFOAM/memory/autoPtr/autoPtr.hpp"
#include "OpenFOAM/primitives/bools/Switch/Switch.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Time;
class polyMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class functionObject Declaration
\*---------------------------------------------------------------------------*/

class functionObject
{
    // Private data

        //- Name
        const word name_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        functionObject(const functionObject&);

        //- Disallow default bitwise assignment
        void operator=(const functionObject&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;

    static int debug;

    //- Global post-processing mode switch
    static bool postProcess;

    //- Switch write log to Info
    Switch log;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            functionObject,
            dictionary,
            (const word& name, const Time& runTime, const dictionary& dict),
            (name, runTime, dict)
        );


    // Constructors

        //- Construct from components
        functionObject(const word& name);

        //- Return clone
        autoPtr<functionObject> clone() const
        {
            NotImplemented;
            return autoPtr<functionObject>(nullptr);
        }


    // Selectors

        //- Select from dictionary, based on its "type" entry
        static autoPtr<functionObject> New
        (
            const word& name,
            const Time&,
            const dictionary&
        );


    //- Destructor
    virtual ~functionObject();


    // Member Functions

        //- Return the name of this functionObject
        const word& name() const;

        //- Read and set the function object if its data have changed
        virtual bool read(const dictionary&);

        //- Called at each ++ or += of the time-loop.
        //  postProcess overrides the usual executeControl behaviour and
        //  forces execution (used in post-processing mode)
        virtual bool execute() = 0;

        //- Called at each ++ or += of the time-loop.
        //  postProcess overrides the usual writeControl behaviour and
        //  forces writing always (used in post-processing mode)
        virtual bool write() = 0;

        //- Called when Time::run() determines that the time-loop exits.
        //  By default it simply calls execute().
        virtual bool end();

        //- Called by Time::setDeltaT(). Allows the function object to override
        //  the time-step value.
        //  Returns whether or not the value was overridden.
        virtual bool setTimeStep();

        //- Called by Time::adjustTimeStep(). Allows the function object to
        //  insert a write time earlier than that already in use by the run
        //  time. Returns the write time, or vGreat.
        virtual scalar timeToNextWrite();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh& mpm);

        //- Update for changes of mesh
        virtual void movePoints(const polyMesh& mesh);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
