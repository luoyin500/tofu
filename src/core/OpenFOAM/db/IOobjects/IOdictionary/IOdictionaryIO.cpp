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

#include "OpenFOAM/db/IOobjects/IOdictionary/IOdictionary.hpp"
#include "OpenFOAM/db/IOstreams/Pstreams/Pstream.hpp"

// * * * * * * * * * * * * * * * Members Functions * * * * * * * * * * * * * //

bool Foam::IOdictionary::readData(Istream& is)
{
    is >> *this;

    if (writeDictionaries && Pstream::master() && !is.bad())
    {
        Sout<< nl
            << "--- IOdictionary " << name()
            << ' ' << objectPath() << ":" << nl;
        writeHeader(Sout);
        writeData(Sout);
        Sout<< "--- End of IOdictionary " << name() << nl << endl;
    }

    return !is.bad();
}


bool Foam::IOdictionary::writeData(Ostream& os) const
{
    dictionary::write(os, false);
    return os.good();
}


// ************************************************************************* //
