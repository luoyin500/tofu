/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2018 OpenFOAM Foundation
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
    Foam::CorrectionLimitingMethod

Description
    Base class for correction limiting methods.

SourceFiles
    CorrectionLimitingMethod.C

\*---------------------------------------------------------------------------*/

#ifndef CorrectionLimitingMethod_H
#define CorrectionLimitingMethod_H

#include "finiteVolume/fields/volFields/volFieldsFwd.hpp"
#include "OpenFOAM/db/dictionary/dictionary.hpp"
#include "OpenFOAM/db/runTimeSelection/construction/runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class CorrectionLimitingMethod Declaration
\*---------------------------------------------------------------------------*/

class CorrectionLimitingMethod
{
private:

    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const CorrectionLimitingMethod&);


public:

    //- Runtime type information
    TypeName("correctionLimitingMethod");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CorrectionLimitingMethod,
        dictionary,
        (const dictionary& dict),
        (dict)
    );


    // Constructors

        //- Construct from components
        CorrectionLimitingMethod(const dictionary& dict);

        //- Construct as copy
        CorrectionLimitingMethod(const CorrectionLimitingMethod& cl);

        //- Construct and return a clone
        virtual autoPtr<CorrectionLimitingMethod> clone() const = 0;


    //- Selector
    static autoPtr<CorrectionLimitingMethod> New
    (
        const dictionary& dict
    );


    //- Destructor
    virtual ~CorrectionLimitingMethod();


    // Member Functions

        //- Return the limited velocity
        virtual vector limitedVelocity
        (
            const vector uP,
            const vector dU,
            const vector uMean
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
