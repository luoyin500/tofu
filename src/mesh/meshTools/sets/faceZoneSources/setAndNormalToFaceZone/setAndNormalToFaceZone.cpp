/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2018 OpenFOAM Foundation
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

#include "meshTools/sets/faceZoneSources/setAndNormalToFaceZone/setAndNormalToFaceZone.hpp"
#include "OpenFOAM/meshes/polyMesh/polyMesh.hpp"
#include "meshTools/sets/topoSets/faceZoneSet.hpp"

#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(setAndNormalToFaceZone, 0);

addToRunTimeSelectionTable(topoSetSource, setAndNormalToFaceZone, word);

addToRunTimeSelectionTable(topoSetSource, setAndNormalToFaceZone, istream);

}


Foam::topoSetSource::addToUsageTable Foam::setAndNormalToFaceZone::usage_
(
    setAndNormalToFaceZone::typeName,
    "\n    Usage: setAndNormalToFaceZone <faceSet> <normal>\n\n"
    "    Select all faces in the faceSet and orient using normal.\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::setAndNormalToFaceZone::setAndNormalToFaceZone
(
    const polyMesh& mesh,
    const word& setName,
    const vector& normal
)
:
    topoSetSource(mesh),
    setName_(setName),
    normal_(normal)
{}


Foam::setAndNormalToFaceZone::setAndNormalToFaceZone
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("faceSet")),
    normal_(dict.lookup("normal"))
{}


Foam::setAndNormalToFaceZone::setAndNormalToFaceZone
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is)),
    normal_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::setAndNormalToFaceZone::~setAndNormalToFaceZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::setAndNormalToFaceZone::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if (!isA<faceZoneSet>(set))
    {
        WarningInFunction
            << "Operation only allowed on a faceZoneSet." << endl;
    }
    else
    {
        faceZoneSet& fzSet = refCast<faceZoneSet>(set);

        if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
        {
            Info<< "    Adding all faces from faceSet " << setName_
                << " ..." << endl;

            // Load the sets
            faceSet fSet(mesh_, setName_);

            // Start off from copy
            DynamicList<label> newAddressing(fzSet.addressing());
            DynamicList<bool> newFlipMap(fzSet.flipMap());

            const faceList& faces = mesh_.faces();
            const pointField& points = mesh_.points();

            forAllConstIter(faceSet, fSet, iter)
            {
                label facei = iter.key();

                if (!fzSet.found(facei))
                {
                    newAddressing.append(facei);

                    const vector a = faces[facei].area(points);
                    if ((a & normal_) > 0)
                    {
                        newFlipMap.append(false);
                    }
                    else
                    {
                        newFlipMap.append(true);
                    }
                }
            }

            fzSet.addressing().transfer(newAddressing);
            fzSet.flipMap().transfer(newFlipMap);
            fzSet.updateSet();
        }
        else if (action == topoSetSource::DELETE)
        {
            Info<< "    Removing all faces from faceSet " << setName_
                << " ..." << endl;

            // Load the set
            faceSet loadedSet(mesh_, setName_);

            // Start off empty
            DynamicList<label> newAddressing(fzSet.addressing().size());
            DynamicList<bool> newFlipMap(fzSet.flipMap().size());

            forAll(fzSet.addressing(), i)
            {
                if (!loadedSet.found(fzSet.addressing()[i]))
                {
                    newAddressing.append(fzSet.addressing()[i]);
                    newFlipMap.append(fzSet.flipMap()[i]);
                }
            }
            fzSet.addressing().transfer(newAddressing);
            fzSet.flipMap().transfer(newFlipMap);
            fzSet.updateSet();
        }
    }
}


// ************************************************************************* //
