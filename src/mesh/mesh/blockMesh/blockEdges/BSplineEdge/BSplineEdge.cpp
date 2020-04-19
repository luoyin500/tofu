/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2018 OpenFOAM Foundation
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

#include "mesh/blockMesh/blockEdges/BSplineEdge/BSplineEdge.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace blockEdges
{
    defineTypeNameAndDebug(BSplineEdge, 0);

    addToRunTimeSelectionTable
    (
        blockEdge,
        BSplineEdge,
        Istream
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::blockEdges::BSplineEdge::BSplineEdge
(
    const pointField& points,
    const label start,
    const label end,
    const pointField& internalPoints
)
:
    blockEdge(points, start, end),
    BSpline(appendEndPoints(points, start, end, internalPoints))
{}


Foam::blockEdges::BSplineEdge::BSplineEdge
(
    const dictionary& dict,
    const label index,
    const searchableSurfaces& geometry,
    const pointField& points,
    Istream& is
)
:
    blockEdge(dict, index, points, is),
    BSpline(appendEndPoints(points, start_, end_, pointField(is)))
{
    token t(is);
    is.putBack(t);

    // discard unused start/end tangents
    if (t == token::BEGIN_LIST)
    {
        vector tangent0Ignored(is);
        vector tangent1Ignored(is);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::blockEdges::BSplineEdge::~BSplineEdge()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::point Foam::blockEdges::BSplineEdge::position(const scalar mu) const
{
    return BSpline::position(mu);
}


Foam::scalar Foam::blockEdges::BSplineEdge::length() const
{
    return BSpline::length();
}


// ************************************************************************* //