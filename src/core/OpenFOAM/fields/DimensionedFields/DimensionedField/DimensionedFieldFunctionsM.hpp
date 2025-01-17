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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_FUNCTION(ReturnType, Type1, Func, Dfunc)                         \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df                                 \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1                          \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_OPERATOR(ReturnType, Type1, Op, opFunc, Dfunc)                   \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1                          \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_FUNCTION(ReturnType, Type1, Type2, Func)                        \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const Type1& t1,                                                           \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const Type1& t1,                                                           \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);


#define BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)                \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const dimensioned<Type2>& dt2                                              \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const Type2& t2                                                            \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const dimensioned<Type2>& dt2                                              \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf2,                         \
    const Type2& t2                                                            \
);


#define BINARY_TYPE_FUNCTION(ReturnType, Type1, Type2, Func)                   \
    BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                    \
    BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_OPERATOR(ReturnType, Type1, Type2, Op, OpName, OpFunc)          \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpName, OpFunc) \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const Type1& t1,                                                           \
    const DimensionedField<Type2, GeoMesh>& df2                                \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const Type1& t1,                                                           \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
);


#define BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpName, OpFunc) \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const dimensioned<Type2>& dt2                                              \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const Type2& t2                                                            \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const dimensioned<Type2>& dt2                                              \
);                                                                             \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const Type2& t2                                                            \
);


#define BINARY_TYPE_OPERATOR(ReturnType, Type1, Type2, Op, OpName, OpFunc)     \
    BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpName, OpFunc)      \
    BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpName, OpFunc)


// ************************************************************************* //
