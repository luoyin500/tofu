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

#include "meshTools/searchableSurfaces/searchablePlane/searchablePlane.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"
#include "OpenFOAM/containers/Lists/SortableList/SortableList.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(searchablePlane, 0);
addToRunTimeSelectionTable(searchableSurface, searchablePlane, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::pointIndexHit Foam::searchablePlane::findLine
(
    const point& start,
    const point& end
) const
{
    pointIndexHit info(true, Zero, 0);

    linePointRef l(start, end);

    scalar t = lineIntersect(l);

    if (t < 0 || t > 1)
    {
        info.setMiss();
        info.setIndex(-1);
    }
    else
    {
        info.setPoint(start+t*l.vec());
    }

    return info;
}


Foam::boundBox Foam::searchablePlane::calcBounds() const
{
    point max(vGreat, vGreat, vGreat);

    for (direction dir = 0; dir < vector::nComponents; dir++)
    {
        if (mag(normal()[dir]) - 1 < small)
        {
            max[dir] = 0;

            break;
        }
    }

    point min = -max;

    return boundBox(min, max);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::searchablePlane::searchablePlane
(
    const IOobject& io,
    const point& basePoint,
    const vector& normal
)
:
    searchableSurface(io),
    plane(basePoint, normal)
{
    bounds() = calcBounds();
}


Foam::searchablePlane::searchablePlane
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    plane(dict)
{
    bounds() = calcBounds();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::searchablePlane::~searchablePlane()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::wordList& Foam::searchablePlane::regions() const
{
    if (regions_.empty())
    {
        regions_.setSize(1);
        regions_[0] = "region0";
    }
    return regions_;
}


void Foam::searchablePlane::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    centres.setSize(1);
    centres[0] = refPoint();

    radiusSqr.setSize(1);
    radiusSqr[0] = Foam::sqr(great);
}


void Foam::searchablePlane::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    info.setSize(samples.size());

    forAll(samples, i)
    {
        info[i].setPoint(nearestPoint(samples[i]));

        if (magSqr(samples[i]-info[i].rawPoint()) > nearestDistSqr[i])
        {
            info[i].setIndex(-1);
            info[i].setMiss();
        }
        else
        {
            info[i].setIndex(0);
            info[i].setHit();
        }
    }
}


void Foam::searchablePlane::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        info[i] = findLine(start[i], end[i]);
    }
}


void Foam::searchablePlane::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    findLine(start, end, info);
}


void Foam::searchablePlane::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit>>& info
) const
{
    List<pointIndexHit> nearestInfo;
    findLine(start, end, nearestInfo);

    info.setSize(start.size());
    forAll(info, pointi)
    {
        if (nearestInfo[pointi].hit())
        {
            info[pointi].setSize(1);
            info[pointi][0] = nearestInfo[pointi];
        }
        else
        {
            info[pointi].clear();
        }
    }
}


void Foam::searchablePlane::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    region = 0;
}


void Foam::searchablePlane::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& n
) const
{
    n.setSize(info.size());
    n = normal();
}


void Foam::searchablePlane::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    FatalErrorInFunction
        << "Volume type not supported for plane."
        << exit(FatalError);
}


// ************************************************************************* //
