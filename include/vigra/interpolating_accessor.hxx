/************************************************************************/
/*                                                                      */
/*               Copyright 1998-2002 by Ullrich Koethe                  */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    You may use, modify, and distribute this software according       */
/*    to the terms stated in the LICENSE file included in               */
/*    the VIGRA distribution.                                           */
/*                                                                      */
/*    The VIGRA Website is                                              */
/*        http://kogs-www.informatik.uni-hamburg.de/~koethe/vigra/      */
/*    Please direct questions, bug reports, and contributions to        */
/*        koethe@informatik.uni-hamburg.de                              */
/*                                                                      */
/*  THIS SOFTWARE IS PROVIDED AS IS AND WITHOUT ANY EXPRESS OR          */
/*  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. */
/*                                                                      */
/************************************************************************/
 
#ifndef VIGRA_INTERPOLATING_ACCESSOR_HXX
#define VIGRA_INTERPOLATING_ACCESSOR_HXX


#include "vigra/accessor.hxx"
#include "vigra/diff2d.hxx"

namespace vigra {

/** \addtogroup DataAccessors 
*/
//@{

/********************************************************/
/*                                                      */
/*                  InterpolatingAccessor               */
/*                                                      */
/********************************************************/

/** \brief Bilinear interpolation at non-integer positions.

    This accessor allows an image be accessed at arbitrary non-integer
    coordinates and performs an bi-linear interpolation to
    obtain a pixel value.
    It uses the given ACCESSOR (which is usually the
    accessor originally associated with the iterator)
    to access data.
    
    <b>\#include</b> "<a href="accessor_8hxx-source.html">vigra/accessor.hxx</a>"
    Namespace: vigra
    
    <b> Required Interface:</b>
    
    \code
    ITERATOR iter;
    ACCESSOR a;
    VALUETYPE destvalue;
    float s;
    int x, y;
    
    destvalue = s * a(iter, x, y) + s * a(iter, x, y);
    
    \endcode
*/
template <class ACCESSOR, class VALUETYPE>
class BilinearInterpolatingAccessor
{
  public:
    /** the iterators' pixel type
    */
    typedef VALUETYPE value_type;
    
    /** init from given accessor
    */
    BilinearInterpolatingAccessor(ACCESSOR a)
    : a_(a)
    {}
    
    /** Interpolate the data item at a non-integer position.
        Ensure that no outside pixels are accessed if 
        (x, y) is near the image border (as long as
        0 <= x <= width-1, 0 <= y <= height-1).
    */
    template <class ITERATOR>
    value_type operator()(ITERATOR const & i, float x, float y) const 
    { 
        int ix = int(x);
        int iy = int(y);
        float dx = x - ix;
        float dy = y - iy;
        
        value_type ret;
        
        // avoid dereferencing the iterator outside its range
        if(dx == 0.0)
        {
            if(dy == 0.0)
            {
                ret = a_(i, Diff2D(ix, iy));
            }
            else
            {
                ret = detail::RequiresExplicitCast<value_type>::cast(
                  (1.0 - dy) * a_(i, Diff2D(ix, iy)) +
                  dy * a_(i, Diff2D(ix, iy + 1)));
            }
        }
        else
        {
            if(dy == 0.0)
            {
                ret = detail::RequiresExplicitCast<value_type>::cast(
                  (1.0 - dx) * a_(i, Diff2D(ix, iy)) + 
                  dx * a_(i, Diff2D(ix + 1, iy)));
            }
            else
            {
                ret = detail::RequiresExplicitCast<value_type>::cast(
                  (1.0 - dx) * (1.0 - dy) * a_(i, Diff2D(ix, iy)) +
                  dx * (1.0 - dy) * a_(i, Diff2D(ix + 1, iy)) +
                  (1.0 - dx) * dy * a_(i, Diff2D(ix, iy + 1)) +
                  dx * dy * a_(i, Diff2D(ix + 1, iy + 1)));
            }
        }
            
        return ret;
    }

    /** Interpolate the data item at a non-integer position.
        This function works as long as 0 <= x < width-1, 
        0 <= y < height-1. It is slightly faster than <TT>operator()</TT>.
    */
    template <class ITERATOR>
    value_type unchecked(ITERATOR const & i, float x, float y) const 
    { 
        int ix = int(x);
        int iy = int(y);
        float dx = x - ix;
        float dy = y - iy;
        return detail::RequiresExplicitCast<value_type>::cast(
               (1.0 - dx) * (1.0 - dy) * a_(i, Diff2D(ix, iy)) +
               dx * (1.0 - dy) * a_(i, Diff2D(ix + 1, iy)) +
               (1.0 - dx) * dy * a_(i, Diff2D(ix, iy + 1)) +
               dx * dy * a_(i, Diff2D(ix + 1, iy + 1)));
    }
    
  private:
    ACCESSOR a_;
};

//@}

} // namespace vigra

#endif /* VIGRA_INTERPOLATING_ACCESSOR_HXX */