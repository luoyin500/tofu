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
    Foam::fileFormats::NASsurfaceFormat

Description
    Nastran surface reader.

    - Uses the Ansa "$ANSA_NAME" or the Hypermesh "$HMNAME COMP" extensions
      to obtain zone names.
    - Handles Nastran short and long formats, but not free format.
    - Properly handles the Nastran compact floating point notation: \n
    \verbatim
        GRID          28        10.20269-.030265-2.358-8
    \endverbatim

SourceFiles
    NASsurfaceFormat.C

\*---------------------------------------------------------------------------*/

#ifndef NASsurfaceFormat_H
#define NASsurfaceFormat_H

#include "surfMesh/MeshedSurface/MeshedSurface.hpp"
#include "surfMesh/MeshedSurfaceProxy/MeshedSurfaceProxy.hpp"
#include "surfMesh/UnsortedMeshedSurface/UnsortedMeshedSurface.hpp"
#include "fileFormats/nas/NASCore.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class NASsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class NASsurfaceFormat
:
    public MeshedSurface<Face>,
    public NASCore
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        NASsurfaceFormat(const NASsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const NASsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct from file name
        NASsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<MeshedSurface<Face>> New(const fileName& name)
        {
            return autoPtr<MeshedSurface<Face>>
            (
                new NASsurfaceFormat<Face>(name)
            );
        }


    //- Destructor
    virtual ~NASsurfaceFormat()
    {}


    // Member Functions

        //- Read from a file
        virtual bool read(const fileName&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "surfMesh/surfaceFormats/nas/NASsurfaceFormat.cpp"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
