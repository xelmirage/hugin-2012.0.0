// -*- c-basic-offset: 4 -*-
/** @file nona/RemappedPanoImage.h
 *
 *  Contains functions to transform whole images.
 *  Can use PTools::Transform or PT::SpaceTransform for the calculations
 *
 *  @author Pablo d'Angelo <pablo.dangelo@web.de>
 *
 *  $Id$
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _NONA_IMAGEREMAPPER_H
#define _NONA_IMAGEREMAPPER_H


#include <panodata/PanoramaData.h>
#include <nona/RemappedPanoImage.h>


namespace HuginBase {
namespace Nona {

        
    /** functor to create a remapped image */
    template <typename ImageType, typename AlphaType>
    class SingleImageRemapper
    {
        
        public:
            /** create a remapped pano image.
             *
             *  The image ownership is transferred to the caller.
             */
            virtual RemappedPanoImage<ImageType,AlphaType>* getRemapped(const PanoramaData & pano,
                                                                        const PanoramaOptions & opts,
                                                                        unsigned int imgNr,
                                                                        vigra::Rect2D outputROI,
                                                                        AppBase::MultiProgressDisplay& progress) = 0;
            
            virtual ~SingleImageRemapper() {};
            
            
        public:
            ///
            virtual	void release(RemappedPanoImage<ImageType,AlphaType>* d) = 0;
        
    };


    /** functor to create a remapped image, loads image from disk */
    template <typename ImageType, typename AlphaType>
    class FileRemapper : public SingleImageRemapper<ImageType, AlphaType>
    {
        
    public:
        FileRemapper()
        {
            m_remapped = 0;
        }

        virtual ~FileRemapper() {};

/**        
    #define HUGIN_REMAP_IMGLOAD(TYPE, lut) \
    { \
        vigra::TYPE tmpImg(info.width(), info.height()); \
        if (alpha) { \
            vigra::importImageAlpha(info, vigra::destImage(tmpImg), \
                                    vigra::destImage(srcAlpha)); \
    { \
        vigra::ImageExportInfo exi(DEBUG_FILE_PREFIX "hugin01_original_mask.tif"); \
        vigra::exportImage(vigra::srcImageRange(srcAlpha), exi); \
    } \
    } else { \
            vigra::importImage(info, vigra::destImage(tmpImg)); \
    } \
    { \
        vigra::ImageExportInfo exi(DEBUG_FILE_PREFIX "hugin01_original.tif"); \
        vigra::exportImage(vigra::srcImageRange(tmpImg), exi); \
    } \
    }
*/

    typedef std::vector<float> LUT;


    public:
        ///
        void loadImage(const PanoramaOptions & opts,
                     vigra::ImageImportInfo & info, ImageType & srcImg,
                     AlphaType & srcAlpha)
            {}

        ///
        virtual RemappedPanoImage<ImageType, AlphaType>*
        getRemapped(const PanoramaData & pano, const PanoramaOptions & opts,
                    unsigned int imgNr, vigra::Rect2D outputROI,
                    AppBase::MultiProgressDisplay & progress);

        ///
        virtual void release(RemappedPanoImage<ImageType,AlphaType>* d)
            { delete d; }


    protected:
        RemappedPanoImage<ImageType,AlphaType> * m_remapped;

    };



    /// load a flatfield image and apply the correction
    template <class FFType, class SrcIter, class SrcAccessor, class DestIter, class DestAccessor>
    void applyFlatfield(vigra::triple<SrcIter, SrcIter, SrcAccessor> srcImg,
                        vigra::pair<DestIter, DestAccessor> destImg,
                        vigra::ImageImportInfo & ffInfo,
                        double gamma,
                        double gammaMaxVal,
                        bool division,
                        typename vigra::NumericTraits<typename SrcAccessor::value_type>::RealPromote a,
                        typename vigra::NumericTraits<typename SrcAccessor::value_type>::RealPromote b,
                        bool dither);



}; // namespace
}; // namespace




//==============================================================================
// templated implementations


#include <vigra/functorexpression.hxx>
#include <vigra_ext/VignettingCorrection.h>


namespace HuginBase {
namespace Nona {
    
    
template <typename ImageType, typename AlphaType>
RemappedPanoImage<ImageType, AlphaType>*
    FileRemapper<ImageType,AlphaType>::getRemapped(const PanoramaData & pano, const PanoramaOptions & opts,
                              unsigned int imgNr, vigra::Rect2D outputROI,
                               AppBase::MultiProgressDisplay & progress)
{
    typedef typename ImageType::value_type PixelType;
    
    //typedef typename vigra::NumericTraits<PixelType>::RealPromote RPixelType;
    //        typedef typename vigra::BasicImage<RPixelType> RImportImageType;
    typedef typename vigra::BasicImage<float> FlatImgType;
    
    FlatImgType ffImg;
    AlphaType srcAlpha;
    
    // choose image type...
    const SrcPanoImage & img = pano.getImage(imgNr);
    
    vigra::Size2D origSrcSize = img.getSize();
    // DGSW FIXME - Unreferenced
    //		const PT::VariableMap & srcVars = pano.getImageVariables(imgNr);
    //		const Lens & lens = pano.getLens(img.getLensNr());
    
    vigra::Size2D destSize(opts.getWidth(), opts.getHeight());
    
    m_remapped = new RemappedPanoImage<ImageType, AlphaType>;
    
    // load image
    
    vigra::ImageImportInfo info(img.getFilename().c_str());

    int width = info.width();
    int height = info.height();

    if (opts.remapUsingGPU) {
        // Extend image width to multiple of 8 for fast GPU transfers.
        const int r = width % 8;
        if (r != 0) width += 8 - r;
    }

    ImageType srcImg(width, height);
    m_remapped->m_ICCProfile = info.getICCProfile();
    
    if (info.numExtraBands() > 0) {
        srcAlpha.resize(width, height);
    }
    //int nb = info.numBands() - info.numExtraBands();
    bool alpha = info.numExtraBands() > 0;
    std::string type = info.getPixelType();
    
    SrcPanoImage src = pano.getSrcImage(imgNr);
    
    // import the image
    progress.setMessage(std::string("loading ") + hugin_utils::stripPath(img.getFilename()));
    
    if (alpha) {
        vigra::importImageAlpha(info, vigra::destImage(srcImg),
                                vigra::destImage(srcAlpha));
    } else {
        vigra::importImage(info, vigra::destImage(srcImg));
    }
    // check if the image needs to be scaled to 0 .. 1,
    // this only works for int -> float, since the image
    // has already been loaded into the output container
    double maxv = vigra_ext::getMaxValForPixelType(info.getPixelType());
    if (maxv != vigra_ext::LUTTraits<PixelType>::max()) {
        double scale = ((double)vigra_ext::LUTTraits<PixelType>::max()) /  maxv;
        //std::cout << "Scaling input image (pixel type: " << info.getPixelType() << " with: " << scale << std::endl;
        transformImage(vigra::srcImageRange(srcImg), destImage(srcImg),
                       vigra::functor::Arg1()*vigra::functor::Param(scale));
    }
    
    // load flatfield, if needed.
    if (img.getVigCorrMode() & SrcPanoImage::VIGCORR_FLATFIELD) {
        // load flatfield image.
        vigra::ImageImportInfo ffInfo(img.getFlatfieldFilename().c_str());
        progress.setMessage(std::string("flatfield vignetting correction ") + hugin_utils::stripPath(img.getFilename()));
        vigra_precondition(( ffInfo.numBands() == 1),
                           "flatfield vignetting correction: "
                           "Only single channel flatfield images are supported\n");
        ffImg.resize(ffInfo.width(), ffInfo.height());
        vigra::importImage(ffInfo, vigra::destImage(ffImg));
    }
    // remap the image
	std::cout<<imgNr<<",";
    remapImage(srcImg, srcAlpha, ffImg,
               pano.getSrcImage(imgNr), opts,
               outputROI,
               *m_remapped,
               progress);
	std::cout<<endl;
    return m_remapped;
}


/// load a flatfield image and apply the correction
template <class FFType, class SrcIter, class SrcAccessor, class DestIter, class DestAccessor>
void applyFlatfield(vigra::triple<SrcIter, SrcIter, SrcAccessor> srcImg,
                    vigra::pair<DestIter, DestAccessor> destImg,
                    vigra::ImageImportInfo & ffInfo,
                    double gamma,
                    double gammaMaxVal,
                    bool division,
                    typename vigra::NumericTraits<typename SrcAccessor::value_type>::RealPromote a,
                    typename vigra::NumericTraits<typename SrcAccessor::value_type>::RealPromote b,
                    bool dither)
{
    FFType ffImg(ffInfo.width(), ffInfo.height());
    vigra::importImage(ffInfo, vigra::destImage(ffImg));
    vigra_ext::flatfieldVigCorrection(srcImg, vigra::srcImage(ffImg), 
                                      destImg, gamma, gammaMaxVal, division, a, b, dither);
}


}; // namespace
}; // namespace

#endif // _H
