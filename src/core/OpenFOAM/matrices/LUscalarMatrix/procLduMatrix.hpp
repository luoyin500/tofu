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
    Foam::procLduMatrix

Description
    I/O for lduMatrix and interface values.

SourceFiles
    procLduMatrix.C

\*---------------------------------------------------------------------------*/

#ifndef procLduMatrix_H
#define procLduMatrix_H

#include "OpenFOAM/primitives/ints/lists/labelList.hpp"
#include "OpenFOAM/fields/Fields/scalarField/scalarField.hpp"
#include "OpenFOAM/fields/FieldFields/FieldField/FieldField.hpp"
#include "OpenFOAM/matrices/lduMatrix/lduAddressing/lduInterfaceFields/lduInterfaceField/lduInterfaceFieldPtrsList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class procLduInterface;
class lduMatrix;

// Forward declaration of friend functions and operators

class procLduMatrix;

Ostream& operator<<(Ostream&, const procLduMatrix&);


/*---------------------------------------------------------------------------*\
                           Class procLduMatrix Declaration
\*---------------------------------------------------------------------------*/

class procLduMatrix
{
    // Private data

        labelList upperAddr_;
        labelList lowerAddr_;
        scalarField diag_;
        scalarField upper_;
        scalarField lower_;
        PtrList<procLduInterface> interfaces_;


    // Private Member Functions

        //- Dissallow construction as copy
        procLduMatrix(const procLduMatrix&);


public:

    friend class LUscalarMatrix;


    // Constructors

        procLduMatrix
        (
            const lduMatrix& ldum,
            const FieldField<Field, scalar>& interfaceCoeffs,
            const lduInterfaceFieldPtrsList& interfaces
        );

        procLduMatrix(Istream& is);


    // Member functions

        label size() const
        {
            return diag_.size();
        }


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const procLduMatrix&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
