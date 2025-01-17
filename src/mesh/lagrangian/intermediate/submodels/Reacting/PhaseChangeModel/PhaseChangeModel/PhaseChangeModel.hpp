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
    Foam::PhaseChangeModel

Description
    Templated phase change model class

SourceFiles
    PhaseChangeModel.C
    PhaseChangeModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef PhaseChangeModel_H
#define PhaseChangeModel_H

#include "OpenFOAM/db/IOobjects/IOdictionary/IOdictionary.hpp"
#include "OpenFOAM/memory/autoPtr/autoPtr.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/runTimeSelectionTables.hpp"
#include "lagrangian/intermediate/submodels/CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class PhaseChangeModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PhaseChangeModel
:
    public CloudSubModelBase<CloudType>
{
public:

    // Public enumerations

        //- Enthalpy transfer type
        enum enthalpyTransferType
        {
            etLatentHeat,
            etEnthalpyDifference
        };

        //- Name representations of enthalpy transfer types
        static const Foam::wordList enthalpyTransferTypeNames;


protected:

    // Protected data

        //- Enthalpy transfer type enumeration
        enthalpyTransferType enthalpyTransfer_;


        // Counters

            //- Mass of lagrangian phase converted
            scalar dMass_;


    // Protected Member Functions

        //- Convert word to enthalpy transfer type
        enthalpyTransferType wordToEnthalpyTransfer(const word& etName) const;

        //- Sherwood number
        scalar Sh() const;


public:

    //- Runtime type information
    TypeName("phaseChangeModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        PhaseChangeModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        PhaseChangeModel(CloudType& owner);

        //- Construct from dictionary
        PhaseChangeModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        PhaseChangeModel(const PhaseChangeModel<CloudType>& pcm);

        //- Construct and return a clone
        virtual autoPtr<PhaseChangeModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~PhaseChangeModel();


    //- Selector
    static autoPtr<PhaseChangeModel<CloudType>> New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        //- Return the enthalpy transfer type enumeration
        const enthalpyTransferType& enthalpyTransfer() const;


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const label celli,
            const scalar Re,
            const scalar Pr,
            const scalar d,
            const scalar nu,
            const scalar T,
            const scalar Ts,
            const scalar pc,
            const scalar Tc,
            const scalarField& X,
            scalarField& dMassPC
        ) const = 0;

        //- Return the enthalpy per unit mass
        virtual scalar dh
        (
            const label idc,
            const label idl,
            const scalar p,
            const scalar T
        ) const;

        //- Return vapourisation temperature
        virtual scalar Tvap(const scalarField& X) const;

        //- Return maximum/limiting temperature
        virtual scalar TMax(const scalar p, const scalarField& X) const;

        //- Add to phase change mass
        void addToPhaseChangeMass(const scalar dMass);


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makePhaseChangeModel(CloudType)                                        \
                                                                               \
    typedef Foam::CloudType::reactingCloudType reactingCloudType;              \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        Foam::PhaseChangeModel<reactingCloudType>,                             \
        0                                                                      \
    );                                                                         \
    namespace Foam                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            PhaseChangeModel<reactingCloudType>,                               \
            dictionary                                                         \
        );                                                                     \
    }


#define makePhaseChangeModelType(SS, CloudType)                                \
                                                                               \
    typedef Foam::CloudType::reactingCloudType reactingCloudType;              \
    defineNamedTemplateTypeNameAndDebug(Foam::SS<reactingCloudType>, 0);       \
                                                                               \
    Foam::PhaseChangeModel<reactingCloudType>::                                \
        adddictionaryConstructorToTable<Foam::SS<reactingCloudType>>           \
            add##SS##CloudType##reactingCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "lagrangian/intermediate/submodels/Reacting/PhaseChangeModel/PhaseChangeModel/PhaseChangeModel.cpp"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
