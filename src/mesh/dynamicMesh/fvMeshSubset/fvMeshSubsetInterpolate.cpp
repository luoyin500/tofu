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

#include "dynamicMesh/fvMeshSubset/fvMeshSubset.hpp"
#include "finiteVolume/fields/fvsPatchFields/constraint/empty/emptyFvsPatchField.hpp"
#include "OpenFOAM/fields/pointPatchFields/constraint/empty/emptyPointPatchField.hpp"
#include "finiteVolume/fields/fvPatchFields/constraint/empty/emptyFvPatchFields.hpp"
#include "finiteVolume/fields/fvPatchFields/fvPatchField/directFvPatchFieldMapper.hpp"
#include "OpenFOAM/fields/pointPatchFields/pointPatchField/directPointPatchFieldMapper.hpp"
#include "OpenFOAM/primitives/ops/flipOp.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> fvMeshSubset::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const fvMesh& sMesh,
    const labelList& patchMap,
    const labelList& cellMap,
    const labelList& faceMap
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<fvPatchField<Type>> patchFields(patchMap.size());

    forAll(patchFields, patchi)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces. Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchi] == -1)
        {
            patchFields.set
            (
                patchi,
                new emptyFvPatchField<Type>
                (
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchi,
                fvPatchField<Type>::New
                (
                    calculatedFvPatchField<Type>::typeName,
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
    }

    tmp<GeometricField<Type, fvPatchField, volMesh>> tresF
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>(vf.primitiveField(), cellMap),
            patchFields
        )
    );
    GeometricField<Type, fvPatchField, volMesh>& resF = tresF.ref();


    // 2. Change the fvPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, fvPatchField, volMesh>::
        Boundary& bf = resF.boundaryFieldRef();

    forAll(bf, patchi)
    {
        if (patchMap[patchi] != -1)
        {
            // Construct addressing
            const fvPatch& subPatch = sMesh.boundary()[patchi];
            const fvPatch& basePatch = vf.mesh().boundary()[patchMap[patchi]];
            const label baseStart = basePatch.start();
            const label baseSize = basePatch.size();

            labelList directAddressing(subPatch.size());

            forAll(directAddressing, i)
            {
                label baseFacei = faceMap[subPatch.start()+i];

                if (baseFacei >= baseStart && baseFacei < baseStart+baseSize)
                {
                    directAddressing[i] = baseFacei-baseStart;
                }
                else
                {
                    // Mapped from internal face. Do what? Leave up to
                    // fvPatchField
                    directAddressing[i] = -1;
                }
            }

            bf.set
            (
                patchi,
                fvPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchi]],
                    subPatch,
                    resF(),
                    directFvPatchFieldMapper(directAddressing)
                )
            );
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> fvMeshSubset::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return interpolate
    (
        vf,
        subMesh(),
        patchMap(),
        cellMap(),
        faceMap()
    );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& vf,
    const fvMesh& sMesh,
    const labelList& patchMap,
    const labelList& cellMap,
    const labelList& faceMap,
    const bool negateIfFlipped
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<fvsPatchField<Type>> patchFields(patchMap.size());

    forAll(patchFields, patchi)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces. Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchi] == -1)
        {
            patchFields.set
            (
                patchi,
                new emptyFvsPatchField<Type>
                (
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchi,
                fvsPatchField<Type>::New
                (
                    calculatedFvsPatchField<Type>::typeName,
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
    }

    // Create the complete field from the pieces
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> tresF
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>
            (
                vf.primitiveField(),
                SubList<label>
                (
                    faceMap,
                    sMesh.nInternalFaces()
                )
            ),
            patchFields
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& resF = tresF.ref();


    // 2. Change the fvsPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        Boundary& bf = resF.boundaryFieldRef();

    forAll(bf, patchi)
    {
        if (patchMap[patchi] != -1)
        {
            // Construct addressing
            const fvPatch& subPatch = sMesh.boundary()[patchi];
            const fvPatch& basePatch = vf.mesh().boundary()[patchMap[patchi]];
            const label baseStart = basePatch.start();
            const label baseSize = basePatch.size();

            labelList directAddressing(subPatch.size());

            forAll(directAddressing, i)
            {
                label baseFacei = faceMap[subPatch.start()+i];

                if (baseFacei >= baseStart && baseFacei < baseStart+baseSize)
                {
                    directAddressing[i] = baseFacei-baseStart;
                }
                else
                {
                    // Mapped from internal face. Do what? Leave up to
                    // patchField. This would require also to pass in
                    // original internal field so for now do as postprocessing
                    directAddressing[i] = -1;
                }
            }

            bf.set
            (
                patchi,
                fvsPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchi]],
                    subPatch,
                    resF(),
                    directFvPatchFieldMapper(directAddressing)
                )
            );


            // Postprocess patch field for exposed faces

            fvsPatchField<Type>& pfld = bf[patchi];
            const labelUList& fc = bf[patchi].patch().faceCells();
            const labelList& own = vf.mesh().faceOwner();

            forAll(pfld, i)
            {
                label baseFacei = faceMap[subPatch.start()+i];
                if (baseFacei < vf.primitiveField().size())
                {
                    Type val = vf.internalField()[baseFacei];

                    if (cellMap[fc[i]] == own[baseFacei] || !negateIfFlipped)
                    {
                        pfld[i] = val;
                    }
                    else
                    {
                        pfld[i] = flipOp()(val);
                    }
                }
                else
                {
                    // Exposed face from other patch.
                    // Only possible in case of a coupled boundary
                    label patchi = vf.mesh().boundaryMesh().whichPatch
                    (
                        baseFacei
                    );
                    const fvPatch& otherPatch = vf.mesh().boundary()[patchi];
                    label patchFacei = otherPatch.patch().whichFace(baseFacei);
                    pfld[i] = vf.boundaryField()[patchi][patchFacei];
                }
            }
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf,
    const bool negateIfFlipped
) const
{
    return interpolate
    (
        sf,
        subMesh(),
        patchMap(),
        cellMap(),
        faceMap(),
        negateIfFlipped
    );
}


template<class Type>
tmp<GeometricField<Type, pointPatchField, pointMesh>>
fvMeshSubset::interpolate
(
    const GeometricField<Type, pointPatchField, pointMesh>& vf,
    const pointMesh& sMesh,
    const labelList& patchMap,
    const labelList& pointMap
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<pointPatchField<Type>> patchFields(patchMap.size());

    forAll(patchFields, patchi)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces.  Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchi] == -1)
        {
            patchFields.set
            (
                patchi,
                new emptyPointPatchField<Type>
                (
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, pointMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchi,
                pointPatchField<Type>::New
                (
                    calculatedPointPatchField<Type>::typeName,
                    sMesh.boundary()[patchi],
                    DimensionedField<Type, pointMesh>::null()
                )
            );
        }
    }

    // Create the complete field from the pieces
    tmp<GeometricField<Type, pointPatchField, pointMesh>> tresF
    (
        new GeometricField<Type, pointPatchField, pointMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh.thisDb(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>(vf.primitiveField(), pointMap),
            patchFields
        )
    );
    GeometricField<Type, pointPatchField, pointMesh>& resF = tresF.ref();


    // 2. Change the pointPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, pointPatchField, pointMesh>::
        Boundary& bf = resF.boundaryFieldRef();

    forAll(bf, patchi)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces.  Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchi] != -1)
        {
            // Construct addressing
            const pointPatch& basePatch =
                vf.mesh().boundary()[patchMap[patchi]];

            const labelList& meshPoints = basePatch.meshPoints();

            // Make addressing from mesh to patch point
            Map<label> meshPointMap(2*meshPoints.size());
            forAll(meshPoints, localI)
            {
                meshPointMap.insert(meshPoints[localI], localI);
            }

            // Find which subpatch points originate from which patch point
            const pointPatch& subPatch = sMesh.boundary()[patchi];
            const labelList& subMeshPoints = subPatch.meshPoints();

            // If mapped from outside patch leave handling up to patchField
            labelList directAddressing(subPatch.size(), -1);

            forAll(subMeshPoints, localI)
            {
                // Get mesh point on original mesh.
                label meshPointi = pointMap[subMeshPoints[localI]];

                Map<label>::const_iterator iter = meshPointMap.find(meshPointi);

                if (iter != meshPointMap.end())
                {
                    directAddressing[localI] = iter();
                }
            }

            bf.set
            (
                patchi,
                pointPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchi]],
                    subPatch,
                    resF(),
                    directPointPatchFieldMapper(directAddressing)
                )
            );
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, pointPatchField, pointMesh>> fvMeshSubset::interpolate
(
    const GeometricField<Type, pointPatchField, pointMesh>& sf
) const
{
    return interpolate
    (
        sf,
        pointMesh::New(subMesh()),     // subsetted point mesh
        patchMap(),
        pointMap()
    );
}


template<class Type>
tmp<DimensionedField<Type, volMesh>> fvMeshSubset::interpolate
(
    const DimensionedField<Type, volMesh>& df,
    const fvMesh& sMesh,
    const labelList& cellMap
)
{
    // Create the complete field from the pieces
    tmp<DimensionedField<Type, volMesh>> tresF
    (
        new DimensionedField<Type, volMesh>
        (
            IOobject
            (
                "subset"+df.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            df.dimensions(),
            Field<Type>(df, cellMap)
        )
    );

    return tresF;
}


template<class Type>
tmp<DimensionedField<Type, volMesh>> fvMeshSubset::interpolate
(
    const DimensionedField<Type, volMesh>& df
) const
{
    return interpolate(df, subMesh(), cellMap());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
