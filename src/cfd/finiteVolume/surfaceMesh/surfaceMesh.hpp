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

Class
    Foam::surfaceMesh

Description
    Mesh data needed to do the Finite Volume discretisation.

\*---------------------------------------------------------------------------*/

#ifndef surfaceMesh_H
#define surfaceMesh_H

#include "OpenFOAM/meshes/GeoMesh/GeoMesh.hpp"
#include "finiteVolume/fvMesh/fvMesh.hpp"
#include "OpenFOAM/meshes/primitiveMesh/primitiveMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class surfaceMesh Declaration
\*---------------------------------------------------------------------------*/

class surfaceMesh
:
    public GeoMesh<fvMesh>
{

public:

    explicit surfaceMesh(const fvMesh& mesh)
    :
        GeoMesh<fvMesh>(mesh)
    {}

    label size() const
    {
        return size(mesh_);
    }

    static label size(const Mesh& mesh)
    {
        return mesh.nInternalFaces();
    }

    const surfaceVectorField& C()
    {
        return mesh_.Cf();
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
