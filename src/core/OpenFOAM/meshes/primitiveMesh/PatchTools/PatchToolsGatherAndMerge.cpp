/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2018 OpenFOAM Foundation
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

#include "OpenFOAM/meshes/primitiveMesh/PatchTools/PatchTools.hpp"
#include "OpenFOAM/meshes/polyMesh/polyMesh.hpp"
#include "OpenFOAM/meshes/polyMesh/globalMeshData/globalMeshData.hpp"
#include "OpenFOAM/meshes/meshTools/mergePoints.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void Foam::PatchTools::gatherAndMerge
(
    const scalar mergeDist,
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    Field<PointType>& mergedPoints,
    List<Face>& mergedFaces,
    labelList& pointMergeMap
)
{
    // Collect points from all processors
    labelList pointSizes;
    {
        List<Field<PointType>> gatheredPoints(Pstream::nProcs());
        gatheredPoints[Pstream::myProcNo()] = p.points();

        Pstream::gatherList(gatheredPoints);

        if (Pstream::master())
        {
            pointSizes = ListListOps::subSizes
            (
                gatheredPoints,
                accessOp<Field<PointType>>()
            );

            mergedPoints = ListListOps::combine<Field<PointType>>
            (
                gatheredPoints,
                accessOp<Field<PointType>>()
            );
        }
    }

    // Collect faces from all processors and renumber using sizes of
    // gathered points
    {
        List<List<Face>> gatheredFaces(Pstream::nProcs());
        gatheredFaces[Pstream::myProcNo()] = p;
        Pstream::gatherList(gatheredFaces);

        if (Pstream::master())
        {
            mergedFaces = static_cast<const List<Face>&>
            (
                ListListOps::combineOffset<List<Face>>
                (
                    gatheredFaces,
                    pointSizes,
                    accessOp<List<Face>>(),
                    offsetOp<Face>()
                )
            );
        }
    }

    if (Pstream::master())
    {
        Field<PointType> newPoints;
        labelList oldToNew;

        bool hasMerged = mergePoints
        (
            mergedPoints,
            mergeDist,
            false,                  // verbosity
            oldToNew,
            newPoints
        );

        if (hasMerged)
        {
            // Store point mapping
            pointMergeMap.transfer(oldToNew);

            // Copy points
            mergedPoints.transfer(newPoints);

            // Relabel faces
            List<Face>& faces = mergedFaces;

            forAll(faces, facei)
            {
                inplaceRenumber(pointMergeMap, faces[facei]);
            }
        }
    }
}


template<class FaceList>
void Foam::PatchTools::gatherAndMerge
(
    const polyMesh& mesh,
    const FaceList& localFaces,
    const labelList& meshPoints,
    const Map<label>& meshPointMap,

    labelList& pointToGlobal,
    labelList& uniqueMeshPointLabels,
    autoPtr<globalIndex>& globalPointsPtr,
    autoPtr<globalIndex>& globalFacesPtr,
    List<typename FaceList::value_type>& mergedFaces,
    pointField& mergedPoints
)
{
    typedef typename FaceList::value_type FaceType;

    if (Pstream::parRun())
    {
        // Renumber the setPatch points/faces into unique points
        globalPointsPtr = mesh.globalData().mergePoints
        (
            meshPoints,
            meshPointMap,
            pointToGlobal,
            uniqueMeshPointLabels
        );

        globalFacesPtr.reset(new globalIndex(localFaces.size()));

        if (Pstream::master())
        {
            // Get renumbered local data
            pointField myPoints(mesh.points(), uniqueMeshPointLabels);
            List<FaceType> myFaces(localFaces);
            forAll(myFaces, i)
            {
                inplaceRenumber(pointToGlobal, myFaces[i]);
            }


            mergedFaces.setSize(globalFacesPtr().size());
            mergedPoints.setSize(globalPointsPtr().size());

            // Insert master data first
            label pOffset = globalPointsPtr().offset(Pstream::masterNo());
            SubList<point>(mergedPoints, myPoints.size(), pOffset) = myPoints;

            label fOffset = globalFacesPtr().offset(Pstream::masterNo());
            SubList<FaceType>(mergedFaces, myFaces.size(), fOffset) = myFaces;


            // Receive slave ones
            for (int slave=1; slave<Pstream::nProcs(); slave++)
            {
                IPstream fromSlave(Pstream::commsTypes::scheduled, slave);

                pointField slavePoints(fromSlave);
                List<FaceType> slaveFaces(fromSlave);

                label pOffset = globalPointsPtr().offset(slave);
                SubList<point>(mergedPoints, slavePoints.size(), pOffset) =
                    slavePoints;

                label fOffset = globalFacesPtr().offset(slave);
                SubList<FaceType>(mergedFaces, slaveFaces.size(), fOffset) =
                    slaveFaces;
            }
        }
        else
        {
            // Get renumbered local data
            pointField myPoints(mesh.points(), uniqueMeshPointLabels);
            List<FaceType> myFaces(localFaces);
            forAll(myFaces, i)
            {
                inplaceRenumber(pointToGlobal, myFaces[i]);
            }

            // Construct processor stream with estimate of size. Could
            // be improved.
            OPstream toMaster
            (
                Pstream::commsTypes::scheduled,
                Pstream::masterNo(),
                myPoints.byteSize() + 4*sizeof(label)*myFaces.size()
            );
            toMaster << myPoints << myFaces;
        }
    }
    else
    {
        pointToGlobal = identity(meshPoints.size());
        uniqueMeshPointLabels = pointToGlobal;

        globalPointsPtr.reset(new globalIndex(meshPoints.size()));
        globalFacesPtr.reset(new globalIndex(localFaces.size()));

        mergedFaces = localFaces;
        mergedPoints = pointField(mesh.points(), meshPoints);
    }
}


// ************************************************************************* //
