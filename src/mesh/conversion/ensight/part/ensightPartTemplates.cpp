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

Description
    Template to write generalized field components

\*---------------------------------------------------------------------------*/

#include "conversion/ensight/part/ensightPart.hpp"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::ensightPart::writeField
(
    ensightFile& os,
    const Field<Type>& field,
    const bool perNode
) const
{
    if (this->size() && field.size())
    {
        writeHeader(os);

        if (perNode)
        {
            os.writeKeyword("coordinates");
            for
            (
                direction cmpt=0;
                cmpt < pTraits<Type>::nComponents;
                ++cmpt
            )
            {
                writeFieldList(os, field.component(cmpt), labelUList::null());
            }
        }
        else
        {
            forAll(elementTypes(), elemI)
            {
                const labelUList& idList = elemLists_[elemI];

                if (idList.size())
                {
                    os.writeKeyword(elementTypes()[elemI]);

                    for
                    (
                        direction cmpt=0;
                        cmpt < pTraits<Type>::nComponents;
                        ++cmpt
                    )
                    {
                        writeFieldList(os, field.component(cmpt), idList);
                    }
                }
            }
        }
    }
}


// ************************************************************************* //
