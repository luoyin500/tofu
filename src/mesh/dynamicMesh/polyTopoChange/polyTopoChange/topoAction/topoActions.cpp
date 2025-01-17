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

#include "dynamicMesh/polyTopoChange/polyTopoChange/topoAction/topoAction.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/addObject/polyAddPoint.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/addObject/polyAddFace.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/addObject/polyAddCell.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/modifyObject/polyModifyPoint.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/modifyObject/polyModifyFace.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/modifyObject/polyModifyCell.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/removeObject/polyRemovePoint.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/removeObject/polyRemoveFace.hpp"
#include "dynamicMesh/polyTopoChange/polyTopoChange/removeObject/polyRemoveCell.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(topoAction, 0);

    defineTypeNameAndDebug(polyAddPoint, 0);
    defineTypeNameAndDebug(polyModifyPoint, 0);
    defineTypeNameAndDebug(polyRemovePoint, 0);

    defineTypeNameAndDebug(polyAddFace, 0);
    defineTypeNameAndDebug(polyModifyFace, 0);
    defineTypeNameAndDebug(polyRemoveFace, 0);

    defineTypeNameAndDebug(polyAddCell, 0);
    defineTypeNameAndDebug(polyModifyCell, 0);
    defineTypeNameAndDebug(polyRemoveCell, 0);
}


// ************************************************************************* //
