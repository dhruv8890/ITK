/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDiscreteGaussianDerivativeImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkDiscreteGaussianDerivativeImageFilter_h
#define __itkDiscreteGaussianDerivativeImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkFixedArray.h"
#include "itkImage.h"

namespace itk
{
/**
 * \class DiscreteGaussianDerivativeImageFilter
 * \brief Calculates image derivatives using discrete derivative gaussian kernels.
 * This filter calculates Gaussian derivative by separable convolution of an image
 * and a discrete Gaussian derivative operator (kernel).
 *
 * The Gaussian operators used here were described by Tony Lindeberg (Discrete
 * Scale-Space Theory and the Scale-Space Primal Sketch.  Dissertation. Royal
 * Institute of Technology, Stockholm, Sweden. May 1991.)
 *
 * The variance or standard deviation (sigma) will be evaluated as pixel units
 * if SetUseImageSpacing is off (false) or as physical units if
 * SetUseImageSpacing is on (true, default). The variance can be set
 * independently in each dimension.
 *
 * When the Gaussian kernel is small, this filter tends to run faster than
 * itk::RecursiveGaussianImageFilter.
 *
 * \author Iv�n Mac�a, VICOMTech, Spain, http://www.vicomtech.es
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/179
 *
 * \sa GaussianDerivativeOperator
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 *
 * \ingroup ImageEnhancement
 * \ingroup ImageFeatureExtraction
 */

template <class TInputImage, class TOutputImage >
class ITK_EXPORT DiscreteGaussianDerivativeImageFilter :
    public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef DiscreteGaussianDerivativeImageFilter           Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DiscreteGaussianDerivativeImageFilter, ImageToImageFilter);

  /** Image type information. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same. */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  typedef typename TInputImage::InternalPixelType  InputInternalPixelType;

  /** Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Typedef of double containers */
  typedef FixedArray<double, itkGetStaticConstMacro(ImageDimension)> ArrayType;

  /** Array for storing desired order of derivatives */
  typedef FixedArray<unsigned int, itkGetStaticConstMacro(ImageDimension)> OrderArrayType;

  /** Order of derivatives in each dimension. Sets the derivative order
   * independently for each dimension, but see also
   * SetOrder(const unsigned int v). The default is 1 in each dimension. */
  itkSetMacro(Order, OrderArrayType);
  itkGetMacro(Order, const OrderArrayType);

  /** The variance for the discrete Gaussian kernel.  Sets the variance
   * independently for each dimension, but
   * see also SetVariance(const double v). The default is 0.0 in each
   * dimension. If UseImageSpacing is true, the units are the physical units
   * of your image.  If UseImageSpacing is false then the units are
   * pixels. */
  itkSetMacro(Variance, ArrayType);
  itkGetMacro(Variance, const ArrayType);

  /** The algorithm will size the discrete kernel so that the error
   * resulting from truncation of the kernel is no greater than
   * MaximumError. The default is 0.01 in each dimension. */
  itkSetMacro(MaximumError, ArrayType);
  itkGetMacro(MaximumError, const ArrayType);

  /** Set the kernel to be no wider than MaximumKernelWidth pixels,
   *  even if MaximumError demands it. The default is 32 pixels. */
  itkGetMacro(MaximumKernelWidth, int);
  itkSetMacro(MaximumKernelWidth, int);

  /** Convenience Set methods for setting all dimensional parameters
   *  to the same values. */
  void SetOrder (const typename OrderArrayType::ValueType v)
    {
    m_Order.Fill(v);
    }

  void SetVariance (const typename ArrayType::ValueType v)
    {
    m_Variance.Fill(v);
    }

  void SetMaximumError (const typename ArrayType::ValueType v)
    {
    m_MaximumError.Fill(v);
    }

  void SetOrder (const unsigned int *v)
    {
    OrderArrayType dv;
    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      dv[i] = v[i];
      }
    this->SetOrder(dv);
    }

  void SetVariance (const double *v)
    {
    ArrayType dv;
    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      dv[i] = v[i];
      }
    this->SetVariance(dv);
    }

  void SetVariance (const float *v)
    {
    ArrayType dv;
    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      dv[i] = v[i];
      }
    this->SetVariance(dv);
    }

  void SetMaximumError (const double *v)
    {
    ArrayType dv;
    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      dv[i] = v[i];
      }
    this->SetMaximumError(dv);
    }

  void SetMaximumError (const float *v)
    {
    ArrayType dv;
    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      dv[i] = v[i];
      }
    this->SetMaximumError(dv);
    }

  /** Use the image spacing information in calculations. Use this option if you
   *  want to specify Gaussian variance in real world units.  Default is
   *   ImageSpacingOn. */
  void SetUseImageSpacingOn() { this->SetUseImageSpacing(true); }

  /** Ignore the image spacing. Use this option if you want to specify Gaussian
      variance in pixels.  Default is ImageSpacingOn. */
  void SetUseImageSpacingOff() { this->SetUseImageSpacing(false); }

  /** Set/Get whether or not the filter will use the spacing of the input
      image in its calculations */
  itkSetMacro(UseImageSpacing, bool);
  itkGetMacro(UseImageSpacing, bool);

  /** DiscreteGaussianDerivativeImageFilter needs a larger input requested region
   * than the output requested region (larger by the size of the
   * Gaussian kernel).  As such, DiscreteGaussianDerivativeImageFilter needs to
   * provide an implementation for GenerateInputRequestedRegion() in
   * order to inform the pipeline execution model.
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<OutputPixelType>));
  /** End concept checking */
#endif

protected:

  DiscreteGaussianDerivativeImageFilter()
    {
    m_Order.Fill(1);
    m_Variance.Fill(0.0);
    m_MaximumError.Fill(0.01);
    m_MaximumKernelWidth = 32;
    m_UseImageSpacing = true;
    }
  virtual ~DiscreteGaussianDerivativeImageFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Standard pipeline method. While this class does not implement a
   * ThreadedGenerateData(), its GenerateData() delegates all
   * calculations to an NeighborhoodOperatorImageFilter.  Since the
   * NeighborhoodOperatorImageFilter is multithreaded, this filter is
   * multithreaded by default. */
  void GenerateData();

private:

  DiscreteGaussianDerivativeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The order of the derivatives in each dimensional direction. */
  OrderArrayType m_Order;

  /** The variance of the gaussian blurring kernel in each dimensional direction. */
  ArrayType m_Variance;

  /** The maximum error of the gaussian blurring kernel in each dimensional
   * direction. For definition of maximum error, see GaussianOperator.
   * \sa GaussianOperator */
  ArrayType m_MaximumError;

  /** Maximum allowed kernel width for any dimension of the discrete Gaussian
      approximation */
  int m_MaximumKernelWidth;

  /** Flag to indicate whether to use image spacing */
  bool m_UseImageSpacing;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDiscreteGaussianDerivativeImageFilter.txx"
#endif

#endif
