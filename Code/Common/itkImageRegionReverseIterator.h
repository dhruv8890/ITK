/*==========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageRegionReverseIterator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkImageRegionReverseIterator_h
#define __itkImageRegionReverseIterator_h

#include "itkImageReverseIterator.h"

namespace itk
{

/**
 * \class ImageRegionReverseIterator
 * \brief Multi-dimensional image iterator which only walks a region.
 * 
 * ImageRegionReverseIterator is a templated class to represent a
 * multi-dimensional iterator. ImageRegionReverseIterator is templated
 * over the dimension of the image and the data type of the
 * image. ImageRegionReverseIterator is constrained to walk only
 * within the specified region.
 *
 * ImageRegionReverseIterator is a multi-dimensional iterator,
 * requiring more information be specified before the iterator can be
 * used than conventional iterators. Whereas the std::vector::iterator
 * from the STL only needs to be passed a pointer to establish the
 * iterator, the multi-dimensional image iterator needs a pointer, the
 * size of the buffer, the size of the region, the start index of the
 * buffer, and the start index of the region. To gain access to this
 * information, ImageRegionReverseIterator holds a reference to the
 * image over which it is traversing.
 *
 * ImageRegionReverseIterator assumes a particular layout of the image
 * data. The is arranged in a 1D array as if it were
 * [][][][slice][row][col] with Index[0] = col, Index[1] = row,
 * Index[2] = slice, etc.
 *
 * operator++ provides a simple syntax for walking around a region of
 * a multidimensional image. For a reverse iterator, operator++ moves
 * backwards through the region. operator++ iterates across a row,
 * constraining the movement to within a region of image. When the
 * iterator reaches the boundary of the region along a row, the
 * iterator automatically wraps to the previous row, starting at the last
 * pixel in the row that is part of the region. This allows for simple
 * processing loops of the form:
 *
 * \code
 *
 *      it = it.Begin();
 *      for (; !it.IsAtEnd(); ++it)
 *         {
 *         *it += 100.0;
 *         }
 *
 *  \endcode
 *
 *
 * \ingroup ImageIterators
 *
 *
 * */
template<typename TImage>
class ImageRegionReverseIterator : public ImageReverseIterator<TImage>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef ImageRegionReverseIterator Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ImageReverseIterator<TImage>  Superclass;

  /**
   * Dimension of the image the iterator walks.  This enum is needed so that
   * functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks.
   */
  enum { ImageIteratorDimension = Superclass::ImageIteratorDimension };

  /** 
   * Index typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   */
  typedef typename Superclass::IndexType IndexType;

  /** 
   * Size typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   */
  typedef typename Superclass::SizeType SizeType;

  /** 
   * Region typedef support.
   */
  typedef typename Superclass::RegionType   RegionType;

  /**
   * Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   */
  typedef typename Superclass::ImageType ImageType;

  /** 
   * PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   */
  typedef typename Superclass::PixelContainer PixelContainer;
  typedef typename PixelContainer::Pointer PixelContainerPointer;

  /**
   * Internal Pixel Type
   */
  typedef typename Superclass::InternalPixelType   InternalPixelType;

  /**
   * External Pixel Type
   */
  typedef typename Superclass::PixelType   PixelType;

  /** 
   *  Accessor type that convert data between internal and external
   *  representations.
   */
  typedef typename Superclass::AccessorType     AccessorType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(ImageRegionReverseIterator, ImageIterator);

  /**
   * Default constructor. Needed since we provide a cast constructor.
   */
  ImageRegionReverseIterator() : ImageReverseIterator<TImage>()
  {
    m_SpanBeginOffset = 0;
    m_SpanEndOffset = 0;
  }
  
  /**
   * Constructor establishes an iterator to walk a particular image and a
   * particular region of that image.
   */
  ImageRegionReverseIterator(ImageType *ptr,
                      const RegionType &region)
    : ImageReverseIterator<TImage>(ptr, region)
  {
    m_SpanBeginOffset = m_BeginOffset;
    m_SpanEndOffset = m_BeginOffset - m_Region.GetSize()[0];
  }

  /**
   * Constructor that can be used to cast from an ImageIterator to an
   * ImageRegionReverseIterator. Many routines return an ImageIterator
   * but for a particular task, you may want an
   * ImageRegionReverseIterator.  Rather than provide overloaded APIs
   * that return different types of Iterators, itk returns
   * ImageIterators and uses constructors to cast from an
   * ImageIterator to a ImageRegionReverseIterator.
   */
  ImageRegionReverseIterator( const ImageIterator<TImage> &it)
  {
    this->ImageReverseIterator<TImage>::operator=(it);

    IndexType ind = this->GetIndex();
    m_SpanBeginOffset = m_Offset + m_Region.GetSize()[0] 
      - (ind[0] - m_Region.GetIndex()[0]);
    m_SpanEndOffset = m_SpanBeginOffset - m_Region.GetSize()[0];
  }

  /**
   * Constructor that takes in a reverse image iterator.  This can be used
   * to cast between the various types of reverse image iterators.
   */
  ImageRegionReverseIterator( const ImageReverseIterator<TImage> &it)
  {
    this->ImageReverseIterator<TImage>::operator=(it);

    IndexType ind = this->GetIndex();
    m_SpanEndOffset = m_Offset + m_Region.GetSize()[0] 
      - (ind[0] - m_Region.GetIndex()[0]);
    m_SpanBeginOffset = m_SpanEndOffset - m_Region.GetSize()[0];
  }

  
  /**
   * Set the index. No bounds checking is performed. This is overridden
   * from the parent because we have an extra ivar.
   * \sa GetIndex
   */
  void SetIndex(const IndexType &ind)
  { Superclass::SetIndex(ind);
    m_SpanBeginOffset = m_Offset + m_Region.GetSize()[0] 
      - (ind[0] - m_Region.GetIndex()[0]);
    m_SpanEndOffset = m_SpanBeginOffset - m_Region.GetSize()[0];
  }
  
  /**
   * Increment (prefix) the fastest moving dimension of the
   * iterator's index.  For a reverse iterator, this moves backwards
   * through the region.  This operator will constrain the iterator
   * within the region (i.e. the iterator will automatically wrap from
   * the start of the row of the region to the end of the previous row
   * of the region) up until the iterator tries to moves before the
   * first pixel of the region.  Here, the iterator will be set to be
   * one pixel before the start of the region.
   * \sa operator++(int)
   */
  Self &
  operator++()
  {
    if (--m_Offset <= m_SpanEndOffset)
      {
      // We have past the beginning of the span (row), need to wrap around.

      // First move forward one pixel, because we are going to use a different
      // algorithm to compute the next pixel
      m_Offset++;
      
      // Get the index of the first pixel on the span (row)
      ImageIterator<TImage>::IndexType
        ind = m_Image->ComputeIndex( m_Offset );

      const ImageIterator<TImage>::IndexType&
        startIndex = m_Region.GetIndex();
      const ImageIterator<TImage>::SizeType&
        size = m_Region.GetSize();

      // Deccrement along a row, then wrap at the beginning of the region row.
      bool done;
      unsigned int dim;

      // Check to see if we are past the first pixel in the region
      // Note that --ind[0] moves to the previous pixel along the row.
      done = (--ind[0] == startIndex[0] - 1);
      for (unsigned int i=1; done && i < ImageIteratorDimension; i++)
        {
        done = (ind[i] == startIndex[i]);
        }
      
      // if the iterator is outside the region (but not past region begin) then
      // we need to wrap around the region
      dim = 0;
      if (!done)
        {
        while ( (dim < ImageIteratorDimension - 1)
                && (ind[dim] < startIndex[dim]) )
          {
          ind[dim] = startIndex[dim] + size[dim] - 1;
          ind[++dim]--;
          }
        }
      m_Offset = m_Image->ComputeOffset( ind );
      m_SpanBeginOffset = m_Offset;
      m_SpanEndOffset = m_SpanBeginOffset - size[0];
      }
    return *this;
  }

  /**
   * Decrement (prefix) the fastest moving dimension of the iterator's index.
   * For a reverse iterator, this moves forward through the region.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the end of the row of the region
   * to the start of the next row of the region) up until the iterator
   * tries to moves past the last pixel of the region.  Here, the iterator
   * will be set to be one pixel past the end of the region.
   * \sa operator--(int)
   */
  Self & operator--()
  {
    if (++m_Offset >= m_SpanBeginOffset)
      {
      // We have reached the end of the span (row), need to wrap around.

      // First back up one pixel, because we are going to use a different
      // algorithm to compute the next pixel
      --m_Offset;
      
      // Get the index of the last pixel on the span (row)
      ImageIterator<TImage>::IndexType
        ind = m_Image->ComputeIndex( m_Offset );

      const ImageIterator<TImage>::IndexType&
        startIndex = m_Region.GetIndex();
      const ImageIterator<TImage>::SizeType&
        size = m_Region.GetSize();

      // Increment along a row, then wrap at the end of the region row.
      bool done;
      unsigned int dim;

      // Check to see if we are past the last pixel in the region
      // Note that ++ind[0] moves to the next pixel along the row.
      done = (++ind[0] == startIndex[0] + size[0]);
      for (unsigned int i=1; done && i < ImageIteratorDimension; i++)
        {
        done = (ind[i] == startIndex[i] + size[i] - 1);
        }
      
      // if the iterator is outside the region (but not past region end) then
      // we need to wrap around the region
      dim = 0;
      if (!done)
        {
        while ( (dim < ImageIteratorDimension - 1)
                && (ind[dim] > startIndex[dim] + size[dim] - 1) )
          {
          ind[dim] = startIndex[dim];
          ind[++dim]++;
          }
        }
      m_Offset = m_Image->ComputeOffset( ind );
      m_SpanBeginOffset = m_Offset;
      m_SpanEndOffset = m_Offset - size[0];
      }
    return *this;
  }

protected:
  unsigned long m_SpanBeginOffset;  // offset to last pixel in the row
  unsigned long m_SpanEndOffset;  // offset to one pixel before the row
       
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageRegionReverseIterator.txx"
#endif

#endif 
