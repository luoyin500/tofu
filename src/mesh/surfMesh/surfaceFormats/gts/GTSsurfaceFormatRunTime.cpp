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

#include "surfMesh/surfaceFormats/gts/GTSsurfaceFormat.hpp"

#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"
#include "OpenFOAM/db/runTimeSelection/memberFunctions/addToMemberFunctionSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fileFormats
{

// read UnsortedMeshedSurface
addNamedTemplatedToRunTimeSelectionTable
(
    UnsortedMeshedSurface,
    GTSsurfaceFormat,
    face,
    fileExtension,
    gts
);
addNamedTemplatedToRunTimeSelectionTable
(
    UnsortedMeshedSurface,
    GTSsurfaceFormat,
    triFace,
    fileExtension,
    gts
);

// write MeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurface,
    GTSsurfaceFormat,
    face,
    write,
    fileExtension,
    gts
);
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurface,
    GTSsurfaceFormat,
    triFace,
    write,
    fileExtension,
    gts
);

// write UnsortedMeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
    UnsortedMeshedSurface,
    GTSsurfaceFormat,
    face,
    write,
    fileExtension,
    gts
);
addNamedTemplatedToMemberFunctionSelectionTable
(
    UnsortedMeshedSurface,
    GTSsurfaceFormat,
    triFace,
    write,
    fileExtension,
    gts
);

}
}

// ************************************************************************* //
