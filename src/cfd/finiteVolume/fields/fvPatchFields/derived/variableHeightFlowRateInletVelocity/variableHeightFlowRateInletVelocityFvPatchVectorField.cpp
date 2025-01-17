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

#include "finiteVolume/fields/fvPatchFields/derived/variableHeightFlowRateInletVelocity/variableHeightFlowRateInletVelocityFvPatchVectorField.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/addToRunTimeSelectionTable.hpp"
#include "finiteVolume/fields/volFields/volFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    flowRate_(),
    alphaName_("none")
{}


Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict),
    flowRate_(Function1<scalar>::New("flowRate", dict)),
    alphaName_(dict.lookup("alpha"))
{}


Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
    const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    flowRate_(ptf.flowRate_, false),
    alphaName_(ptf.alphaName_)
{}


Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
    const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    flowRate_(ptf.flowRate_, false),
    alphaName_(ptf.alphaName_)
{}


Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
    const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    flowRate_(ptf.flowRate_, false),
    alphaName_(ptf.alphaName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::variableHeightFlowRateInletVelocityFvPatchVectorField
::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    scalarField alphap =
        patch().lookupPatchField<volScalarField, scalar>(alphaName_);

    alphap = max(alphap, scalar(0));
    alphap = min(alphap, scalar(1));

    const scalar t = db().time().timeOutputValue();
    scalar flowRate = flowRate_->value(t);

    // a simpler way of doing this would be nice
    scalar avgU = -flowRate/gSum(patch().magSf()*alphap);

    vectorField n(patch().nf());

    operator==(n*avgU*alphap);

    fixedValueFvPatchField<vector>::updateCoeffs();
}


void Foam::variableHeightFlowRateInletVelocityFvPatchVectorField::write
(
    Ostream& os
) const
{
    fvPatchField<vector>::write(os);
    flowRate_->writeData(os);
    os.writeKeyword("alpha") << alphaName_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       variableHeightFlowRateInletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
