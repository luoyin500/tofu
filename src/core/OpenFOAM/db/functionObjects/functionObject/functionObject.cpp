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

#include "OpenFOAM/db/functionObjects/functionObject/functionObject.hpp"
#include "OpenFOAM/db/dictionary/dictionary.hpp"
#include "OpenFOAM/db/dynamicLibrary/dlLibraryTable/dlLibraryTable.hpp"
#include "OpenFOAM/db/Time/Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineDebugSwitchWithName(functionObject, "functionObject", 0);
    defineRunTimeSelectionTable(functionObject, dictionary);
}

bool Foam::functionObject::postProcess(false);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObject::functionObject(const word& name)
:
    name_(name),
    log(postProcess)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::functionObject> Foam::functionObject::New
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
{
    const word functionType(dict.lookup("type"));

    if (debug)
    {
        Info<< "Selecting function " << functionType << endl;
    }

    if (dict.found("functionObjectLibs"))
    {
        const_cast<Time&>(runTime).libs().open
        (
            dict,
            "functionObjectLibs",
            dictionaryConstructorTablePtr_
        );
    }
    else
    {
        const_cast<Time&>(runTime).libs().open
        (
            dict,
            "libs",
            dictionaryConstructorTablePtr_
        );
    }

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "Unknown function type "
            << functionType << nl << nl
            << "Table of functionObjects is empty" << endl
            << exit(FatalError);
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(functionType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown function type "
            << functionType << nl << nl
            << "Valid functions are : " << nl
            << dictionaryConstructorTablePtr_->sortedToc() << endl
            << exit(FatalError);
    }

    return autoPtr<functionObject>(cstrIter()(name, runTime, dict));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObject::~functionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::word& Foam::functionObject::name() const
{
    return name_;
}


bool Foam::functionObject::read(const dictionary& dict)
{
    if (!postProcess)
    {
        log = dict.lookupOrDefault<Switch>("log", true);
    }

    return true;
}


bool Foam::functionObject::end()
{
    return true;
}


bool Foam::functionObject::setTimeStep()
{
    return false;
}


Foam::scalar Foam::functionObject::timeToNextWrite()
{
    return vGreat;
}


void Foam::functionObject::updateMesh(const mapPolyMesh&)
{}


void Foam::functionObject::movePoints(const polyMesh&)
{}


// ************************************************************************* //
