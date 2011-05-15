/* linbox/matrix/sparse.tcc
 * Copyright 2001-2002 Bradford Hovinen
 *           1999-2001 William J Turner,
 *
 * Written by Bradford Hovinen <hovinen@gmail.com>
 * Based on sparse-base.h by William J Turner <wjturner@math.ncsu.edu>
 *
 * ------------------------------------
 *
 * See COPYING for license information.
 */

#ifndef __LINBOX_matrix_sparse_TCC
#define __LINBOX_matrix_sparse_TCC

#include "linbox/linbox-config.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstring>

#include "linbox/util/debug.h"
#include "linbox/matrix/sparse.h"
#include "linbox/vector/vector-traits.h"
#include "linbox/matrix/submatrix.h"
#include "linbox/util/commentator.h"

namespace LinBox
{

template <class Element, class Row>
void SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>
	::setEntry (size_t i, size_t j, const Element &value) 
{
        typedef typename Row::value_type value_type;
	Row &v = _A[i];
	typename Row::iterator iter;
        
	if (v.size () == 0) {
		v.push_back (value_type (j, value));
	} else {
		iter = std::lower_bound (v.begin (), v.end (), j, VectorWrapper::CompareSparseEntries ());

		if (iter == v.end () || iter->first != j)
			iter = v.insert (iter, value_type (j, value));
                else
                    	iter->second = value;
 	}
}

template <class Element, class Row>
void SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>
	::eraseEntry (size_t i, size_t j) 
{
	Row &v = _A[i];
	typename Row::iterator iter;
	
	iter = std::lower_bound (v.begin (), v.end (), j, VectorWrapper::CompareSparseEntries ());

	if (iter != v.end () && iter->first == j)
		v.erase (iter);
}

template <class Element, class Row>
bool SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>
	::getEntry (Element &x, size_t i, size_t j) const
{
	const Row &v = _A[i];
	typename Row::const_iterator iter;

	if (v.size () == 0)
		return false;
	else {
		iter = std::lower_bound (v.begin (), v.end (), j, VectorWrapper::CompareSparseEntries ());

		if (iter == v.end () || iter->first != j)
			return false;
		else {
			x = iter->second;
			return true;
		}
	}
}

template <class Element, class Row>
template <class Vector>
Vector &SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>
	::columnDensity (Vector &v) const
{
	unsigned int row = 0;

	for (ConstRowIterator i = rowBegin (); i != rowEnd (); ++i, ++row) {
		typename Row::const_iterator j = i.begin ();

		for (; j != i->begin (); ++j)
			++v[j->first];
	}

	return v;
}

template <class Element, class Row>
SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>
	&SparseMatrix<Element, Row, VectorCategories::SparseVectorTag>::transpose (SparseMatrix &AT) const
{
	typename Row::const_iterator j;

	unsigned int row = 0;

	for (ConstRowIterator i = rowBegin (); i != rowEnd (); ++i, ++row)
		for (j = i->begin (); j != i->end (); ++j)
			AT._A[j->first].push_back (std::pair<size_t, Element> (row, j->second));

	return AT;
}

} // namespace LinBox

#endif // __LINBOX_matrix_sparse_TCC

// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: t
// c-basic-offset: 8
// End:

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s:syntax=cpp.doxygen:foldmethod=syntax