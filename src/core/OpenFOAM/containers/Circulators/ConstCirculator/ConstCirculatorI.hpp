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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ContainerType>
Foam::ConstCirculator<ContainerType>::ConstCirculator()
:
    CirculatorBase(),
    begin_(0),
    end_(0),
    iter_(0),
    fulcrum_(0)
{}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>::ConstCirculator
(
    const ContainerType& container
)
:
    CirculatorBase(),
    begin_(container.begin()),
    end_(container.end()),
    iter_(begin_),
    fulcrum_(begin_)
{}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>::ConstCirculator
(
    const const_iterator& begin,
    const const_iterator& end
)
:
    CirculatorBase(),
    begin_(begin),
    end_(end),
    iter_(begin),
    fulcrum_(begin)
{}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>::ConstCirculator
(
    const ConstCirculator<ContainerType>& rhs
)
:
    CirculatorBase(),
    begin_(rhs.begin_),
    end_(rhs.end_),
    iter_(rhs.iter_),
    fulcrum_(rhs.fulcrum_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ContainerType>
Foam::ConstCirculator<ContainerType>::~ConstCirculator()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::size_type
Foam::ConstCirculator<ContainerType>::size() const
{
    return end_ - begin_;
}


template<class ContainerType>
bool Foam::ConstCirculator<ContainerType>::circulate
(
    const CirculatorBase::direction dir
)
{
    if (dir == CirculatorBase::CLOCKWISE)
    {
        operator++();
    }
    else if (dir == CirculatorBase::ANTICLOCKWISE)
    {
        operator--();
    }

    return !(iter_ == fulcrum_);
}


template<class ContainerType>
void Foam::ConstCirculator<ContainerType>::setFulcrumToIterator()
{
    fulcrum_ = iter_;
}


template<class ContainerType>
void Foam::ConstCirculator<ContainerType>::setIteratorToFulcrum()
{
    iter_ = fulcrum_;
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::difference_type
Foam::ConstCirculator<ContainerType>::nRotations() const
{
    return (iter_ - fulcrum_);
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::const_reference
Foam::ConstCirculator<ContainerType>::next() const
{
    if (iter_ == end_ - 1)
    {
        return *begin_;
    }

    return *(iter_ + 1);
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::const_reference
Foam::ConstCirculator<ContainerType>::prev() const
{
    if (iter_ == begin_)
    {
        return *(end_ - 1);
    }

    return *(iter_ - 1);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class ContainerType>
void Foam::ConstCirculator<ContainerType>::operator=
(
    const ConstCirculator<ContainerType>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    begin_ = rhs.begin_;
    end_ = rhs.end_;
    iter_ = rhs.iter_;
    fulcrum_ = rhs.fulcrum_;
}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>&
Foam::ConstCirculator<ContainerType>::operator++()
{
    ++iter_;
    if (iter_ == end_)
    {
        iter_ = begin_;
    }

    return *this;
}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>
Foam::ConstCirculator<ContainerType>::operator++(int)
{
    ConstCirculator<ContainerType> tmp = *this;
    ++(*this);
    return tmp;
}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>&
Foam::ConstCirculator<ContainerType>::operator--()
{
    if (iter_ == begin_)
    {
        iter_ = end_;
    }
    --iter_;

    return *this;
}


template<class ContainerType>
Foam::ConstCirculator<ContainerType>
Foam::ConstCirculator<ContainerType>::operator--(int)
{
    ConstCirculator<ContainerType> tmp = *this;
    --(*this);
    return tmp;
}


template<class ContainerType>
bool Foam::ConstCirculator<ContainerType>::operator==
(
    const ConstCirculator<ContainerType>& c
) const
{
    return
    (
        begin_ == c.begin_
     && end_ == c.end_
     && iter_ == c.iter_
     && fulcrum_ == c.fulcrum_
    );
}


template<class ContainerType>
bool Foam::ConstCirculator<ContainerType>::operator!=
(
    const ConstCirculator<ContainerType>& c
) const
{
    return !(*this == c);
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::const_reference
Foam::ConstCirculator<ContainerType>::operator*() const
{
    return *iter_;
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::const_reference
Foam::ConstCirculator<ContainerType>::operator()() const
{
    return operator*();
}


template<class ContainerType>
typename Foam::ConstCirculator<ContainerType>::difference_type
Foam::ConstCirculator<ContainerType>::operator-
(
    const ConstCirculator<ContainerType>& c
) const
{
    return iter_ - c.iter_;
}


// ************************************************************************* //
