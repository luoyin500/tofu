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
    Foam::cellToFace

Description
    A topoSetSource to select a faceSet from cells.

    Either all faces of cell or some other criterion.
    See implementation.
    Note: when picking up coupled faces uses cells on neighbouring processors.

SourceFiles
    cellToFace.C

\*---------------------------------------------------------------------------*/

#ifndef cellToFace_H
#define cellToFace_H

#include "meshTools/sets/topoSetSource/topoSetSource.hpp"
#include "OpenFOAM/containers/NamedEnum/NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class cellToFace Declaration
\*---------------------------------------------------------------------------*/

class cellToFace
:
    public topoSetSource
{
public:
        //- Enumeration defining the valid options
        enum cellAction
        {
            ALL,
            BOTH
        };


private:

        //- Add usage string
        static addToUsageTable usage_;

        static const NamedEnum<cellAction, 2> cellActionNames_;

        //- Name of set to use
        word setName_;

        //- Option
        cellAction option_;


    // Private Member Functions

        //- Depending on face to cell option add to or delete from cellSet.
        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("cellToFace");

    // Constructors

        //- Construct from components
        cellToFace
        (
            const polyMesh& mesh,
            const word& setName,
            const cellAction option
        );

        //- Construct from dictionary
        cellToFace
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        cellToFace
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~cellToFace();


    // Member Functions

        virtual sourceType setType() const
        {
            return FACESETSOURCE;
        }

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
