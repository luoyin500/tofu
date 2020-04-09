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

#include "OpenFOAM/db/functionObjects/timeControl/timeControl.hpp"
#include "OpenFOAM/db/IOstreams/Pstreams/PstreamReduceOps.hpp"
#include "OpenFOAM/db/IOstreams/Pstreams/Pstream.hpp"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char* NamedEnum<timeControl::timeControls, 8>::
    names[] =
    {
        "timeStep",
        "writeTime",
        "outputTime",
        "adjustableRunTime",
        "runTime",
        "clockTime",
        "cpuTime",
        "none"
    };
}

const Foam::NamedEnum<Foam::timeControl::timeControls, 8>
    Foam::timeControl::timeControlNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::timeControl::timeControl
(
    const Time& t,
    const dictionary& dict,
    const word& prefix
)
:
    time_(t),
    prefix_(prefix),
    timeControl_(ocTimeStep),
    intervalSteps_(0),
    interval_(-1),
    executionIndex_(0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::timeControl::~timeControl()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::timeControl::read(const dictionary& dict)
{
    word controlName(prefix_ + "Control");
    word intervalName(prefix_ + "Interval");

    // For backward compatibility support the deprecated 'outputControl' option
    // now superseded by 'writeControl' for compatibility with Time
    if (prefix_ == "write" && dict.found("outputControl"))
    {
        IOWarningInFunction(dict)
            << "Using deprecated 'outputControl'" << nl
            << "    Please use 'writeControl' with 'writeInterval'"
            << endl;

        // Change to the old names for this option
        controlName = "outputControl";
        intervalName = "outputInterval";
    }

    if (dict.found(controlName))
    {
        timeControl_ = timeControlNames_.read(dict.lookup(controlName));
    }
    else
    {
        timeControl_ = ocTimeStep;
    }

    switch (timeControl_)
    {
        case ocTimeStep:
        {
            intervalSteps_ = dict.lookupOrDefault<label>(intervalName, 0);
            break;
        }

        case ocWriteTime:
        case ocOutputTime:
        {
            intervalSteps_ = dict.lookupOrDefault<label>(intervalName, 1);
            break;
        }

        case ocClockTime:
        case ocRunTime:
        case ocCpuTime:
        case ocAdjustableRunTime:
        {
            interval_ = readScalar(dict.lookup(intervalName));
            break;
        }

        default:
        {
            break;
        }
    }
}


bool Foam::timeControl::execute()
{
    switch (timeControl_)
    {
        case ocTimeStep:
        {
            return
            (
                (intervalSteps_ <= 1)
             || !(time_.timeIndex() % intervalSteps_)
            );
            break;
        }

        case ocWriteTime:
        case ocOutputTime:
        {
            if (time_.writeTime())
            {
                executionIndex_++;
                return !(executionIndex_ % intervalSteps_);
            }
            break;
        }

        case ocRunTime:
        case ocAdjustableRunTime:
        {
            label executionIndex = label
            (
                (
                    (time_.value() - time_.startTime().value())
                  + 0.5*time_.deltaTValue()
                )
               /interval_
            );

            if (executionIndex > executionIndex_)
            {
                executionIndex_ = executionIndex;
                return true;
            }
            break;
        }

        case ocCpuTime:
        {
            label executionIndex = label
            (
                returnReduce(time_.elapsedCpuTime(), maxOp<double>())
               /interval_
            );
            if (executionIndex > executionIndex_)
            {
                executionIndex_ = executionIndex;
                return true;
            }
            break;
        }

        case ocClockTime:
        {
            label executionIndex = label
            (
                returnReduce(label(time_.elapsedClockTime()), maxOp<label>())
               /interval_
            );
            if (executionIndex > executionIndex_)
            {
                executionIndex_ = executionIndex;
                return true;
            }
            break;
        }

        case ocNone:
        {
            return false;
        }

        default:
        {
            FatalErrorInFunction
                << "Undefined output control: "
                << timeControlNames_[timeControl_] << nl
                << abort(FatalError);
            break;
        }
    }

    return false;
}


// ************************************************************************* //
