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

#include "lagrangian/intermediate/submodels/Reacting/CompositionModel/CompositionModel/CompositionModel.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::CompositionModel<CloudType>::CompositionModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner),
    thermo_(owner.thermo()),
    phaseProps_()
{}


template<class CloudType>
Foam::CompositionModel<CloudType>::CompositionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    thermo_(owner.thermo()),
    phaseProps_
    (
        this->coeffDict().lookup("phases"),
        thermo_.carrier().species(),
        thermo_.liquids().components(),
        thermo_.solids().components()
    )
{}


template<class CloudType>
Foam::CompositionModel<CloudType>::CompositionModel
(
    const CompositionModel<CloudType>& cm
)
:
    CloudSubModelBase<CloudType>(cm),
    thermo_(cm.thermo_),
    phaseProps_(cm.phaseProps_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::CompositionModel<CloudType>::~CompositionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const Foam::SLGThermo& Foam::CompositionModel<CloudType>::thermo() const
{
    return thermo_;
}


template<class CloudType>
const Foam::basicSpecieMixture&
Foam::CompositionModel<CloudType>::carrier() const
{
    return thermo_.carrier();
}


template<class CloudType>
const Foam::liquidMixtureProperties&
Foam::CompositionModel<CloudType>::liquids() const
{
    return thermo_.liquids();
}


template<class CloudType>
const Foam::solidMixtureProperties&
Foam::CompositionModel<CloudType>::solids() const
{
    return thermo_.solids();
}


template<class CloudType>
const Foam::phasePropertiesList&
Foam::CompositionModel<CloudType>::phaseProps() const
{
    return phaseProps_;
}


template<class CloudType>
Foam::label Foam::CompositionModel<CloudType>::nPhase() const
{
    return phaseProps_.size();
}


template<class CloudType>
const Foam::wordList& Foam::CompositionModel<CloudType>::phaseTypes() const
{
    // if only 1 phase, return the constituent component names
    if (phaseProps_.size() == 1)
    {
        return phaseProps_[0].names();
    }
    else
    {
        return phaseProps_.phaseTypes();
    }
}


template<class CloudType>
const Foam::wordList& Foam::CompositionModel<CloudType>::stateLabels() const
{
    return phaseProps_.stateLabels();
}


template<class CloudType>
const Foam::wordList&
Foam::CompositionModel<CloudType>::componentNames(const label phasei) const
{
    return phaseProps_[phasei].names();
}


template<class CloudType>
Foam::label Foam::CompositionModel<CloudType>::carrierId
(
    const word& cmptName,
    const bool allowNotFound
) const
{
    label id = thermo_.carrierId(cmptName);

    if (id < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine global id for requested component "
            << cmptName << ". Available components are " << nl
            << thermo_.carrier().species()
            << abort(FatalError);
    }

    return id;
}


template<class CloudType>
Foam::label Foam::CompositionModel<CloudType>::localId
(
    const label phasei,
    const word& cmptName,
    const bool allowNotFound
) const
{
    label id = phaseProps_[phasei].id(cmptName);

    if (id < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine local id for component " << cmptName
            << abort(FatalError);
    }

    return id;
}


template<class CloudType>
Foam::label Foam::CompositionModel<CloudType>::localToCarrierId
(
    const label phasei,
    const label id,
    const bool allowNotFound
) const
{
    label cid = phaseProps_[phasei].carrierIds()[id];

    if (cid < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine global carrier id for phase "
            << phasei << " with local id " << id
            << abort(FatalError);
    }

    return cid;
}


template<class CloudType>
const Foam::scalarField& Foam::CompositionModel<CloudType>::Y0
(
    const label phasei
) const
{
    return phaseProps_[phasei].Y();
}


template<class CloudType>
Foam::scalarField Foam::CompositionModel<CloudType>::X
(
    const label phasei,
    const scalarField& Y
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalarField X(Y.size());
    scalar WInv = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                X[i] = Y[i]/thermo_.carrier().W(cid);
                WInv += X[i];
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                X[i] = Y[i]/thermo_.liquids().properties()[i].W();
                WInv += X[i];
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Only possible to convert gas and liquid mass fractions"
                << abort(FatalError);
        }
    }

    X /= WInv;

    return X;
}


template<class CloudType>
Foam::scalar Foam::CompositionModel<CloudType>::H
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HMixture += Y[i]*thermo_.carrier().Ha(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HMixture += Y[i]*thermo_.liquids().properties()[i].h(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HMixture +=
                     Y[i]
                    *(
                        thermo_.solids().properties()[i].Hf()
                      + thermo_.solids().properties()[i].Cp()*T
                     );
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration" << abort(FatalError);
        }
    }

    return HMixture;
}


template<class CloudType>
Foam::scalar Foam::CompositionModel<CloudType>::Hs
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HsMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HsMixture += Y[i]*thermo_.carrier().Hs(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HsMixture +=
                    Y[i]
                   *(
                       thermo_.liquids().properties()[i].h(p, T)
                     - thermo_.liquids().properties()[i].h(p, 298.15)
                    );
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HsMixture += Y[i]*thermo_.solids().properties()[i].Cp()*T;
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return HsMixture;
}


template<class CloudType>
Foam::scalar Foam::CompositionModel<CloudType>::Hc
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HcMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HcMixture += Y[i]*thermo_.carrier().Hc(cid);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HcMixture +=
                    Y[i]*thermo_.liquids().properties()[i].h(p, 298.15);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HcMixture += Y[i]*thermo_.solids().properties()[i].Hf();
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return HcMixture;
}


template<class CloudType>
Foam::scalar Foam::CompositionModel<CloudType>::Cp
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar CpMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                CpMixture += Y[i]*thermo_.carrier().Cp(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                CpMixture += Y[i]*thermo_.liquids().properties()[i].Cp(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                CpMixture += Y[i]*thermo_.solids().properties()[i].Cp();
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return CpMixture;
}


template<class CloudType>
Foam::scalar Foam::CompositionModel<CloudType>::L
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar LMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            if (debug)
            {
                WarningInFunction
                    << "No support for gaseous components" << endl;
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                LMixture += Y[i]*thermo_.liquids().properties()[i].hl(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            if (debug)
            {
                WarningInFunction
                    << "No support for solid components" << endl;
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return LMixture;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "lagrangian/intermediate/submodels/Reacting/CompositionModel/CompositionModel/CompositionModelNew.cpp"

// ************************************************************************* //
