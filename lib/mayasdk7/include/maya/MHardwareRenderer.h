#ifndef _MHardwareRenderer
#define _MHardwareRenderer
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MHardwareRenderer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MHardwareRenderer)
//
// MHardwareRenderer is a wrapper class to access the hardware renderer.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MDrawProcedureBase.h>
#include <maya/MColor.h>

class MStringArray;
class MGeometryData;
class MImage;

// *****************************************************************************

// CLASS DECLARATION (MHardwareRenderer)

/// Static hardware renderer interface class
/**
This class is a wrapper class to access the hardware renderer
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYARENDER_EXPORT MHardwareRenderer
{
public:
	/// Draw Procedure status codes
	enum DrawProcedureStatusCode {
		/// Success
		kSuccess = 0,
		/// Failure
		kFailure,
		/// Item already exists
		kItemExists,
		/// Item is not found
		kItemNotFound,
		/// Location not found
		kLocationNotFound
	};

	/// Specify the pixel format of the current buffer  
	enum BufferPixelFormat {
		/// 8 bit Red, Green, Blue, and Alpha channel
		kRGBA_Fix8,
		/// 16 bit Red, Green, Blue, and Alpha channel
		kRGBA_Float16,
		/// 32 bit floating point depth buffer
		kDepth_Float32
	};

	/// Draw Procedure call locations
	enum CallLocation {
		/// Before rendering one frame. No model or view matrices
		// can be assumed to have been set up at this time, 
		// nor an assumption on the background.
		kPreRendering = 0,
		/// Before rendering one "exposure". If multiple exposures are
		/// are required to render one frame. After the frame buffer is cleared
		/// and model and view matrices are set up for the current exposure.
		kPreExposure = 1,
		/// After rendering one "exposure".
		kPostExposure = 2,
		/// After rendering one frame. Before a possible fame buffer swap.
		kPostRendering = 3
	};

	/// Default geometry shapes 
	enum GeometricShape {
		/// Sphere with radius 1, centered at 0,0,0.
		kDefaultSphere = 0,
		/// Plane with width and height of 1, centered at 0,0,0.
		/// Assuming "Y-Up" orientation: width = x-axis, and height = y-axis.
		kDefaultPlane,
		/// Cube with width, height and depth of 1, centered at 0,0,0.
		kDefaultCube
	};

	///
	static MHardwareRenderer*	theRenderer();

	///
	const MString &				backEndString() const;


	// Draw procedure methods
	///
	DrawProcedureStatusCode		addDrawProcedure( const MString & backEndString,
									MDrawProcedureBase *drawProcedureptr,
									CallLocation location);
	///
	DrawProcedureStatusCode		removeDrawProcedure( const MString & backEndString,
									MDrawProcedureBase *drawProcedureptr,
									CallLocation location );
	///
	DrawProcedureStatusCode		insertDrawProcedure( const MString & backEndString,
									   MDrawProcedureBase *drawProcedurePtr,
									   CallLocation location,
									   unsigned int listIndex,
									   bool moveExistingItem = false);
	///
	DrawProcedureStatusCode		getDrawProcedureCount( const MString & backEndString,
									  CallLocation location,
									  unsigned int &count ) const;
	///
	DrawProcedureStatusCode		getDrawProcedureListNames( const MString & backEndString,
												CallLocation location,
												MStringArray &namesOfProcs ) const;
	///
	MDrawProcedureBase * findDrawProcedure( const MString & backEndString,
									CallLocation location,
									const MString &procName ) const;

	// Context handling methods
	///
	MStatus						makeResourceContextCurrent(const MString &backEndString);

	///
	MStatus						getBufferSize( const MString& backEndString,
												unsigned int &width,
												unsigned int &height ) const;

	///
	MStatus						getColorBufferPixelFormat( const MString& backEndString,
														BufferPixelFormat &fmt ) const;

	///
	MStatus						getDepthBufferPixelFormat( const MString& backEndString,
														BufferPixelFormat &fmt ) const;

	///
	MStatus						getCurrentExposureNumber( const MString& backEndString,
														  unsigned int &number ) const;
	///
	MStatus						getTotalExposureCount( const MString& backEndString,
														  unsigned int &number ) const;

	///
	MStatus restoreCurrent(const MString& backEndString) const;


	// Swatch rendering methods
	///
	MStatus						makeSwatchContextCurrent( const MString& backEndString,
														  unsigned int & width, 
														  unsigned int & height );

	///
	MStatus						readSwatchContextPixels( const MString& backEndString,
												   MImage & image );

	///
	MGeometryData *				referenceDefaultGeometry(
									MHardwareRenderer::GeometricShape geomShape,
									unsigned int & numberOfData,
									unsigned int* & pIndexing,
									unsigned int & indexLength);
	///
	MStatus						dereferenceGeometry(MGeometryData * pGeomData,
											unsigned int numberOfData);

	///
	void getSwatchOrthoCameraSetting( double& l, double& r,
									  double& b, double& t,
									  double& n, double& f ) const;

	///
	void getSwatchPerspectiveCameraSetting( double& l, double& r,
									  double& b, double& t,
									  double& n, double& f ) const;
	///
	void getSwatchPerspectiveCameraTranslation( float& x,  float& y, float& z, float& w ) const;

	///
	void getSwatchLightDirection( float& x,  float& y, float& z, float& w ) const;

	///
	void drawSwatchBackGroundQuads( const MColor & quadColor = MColor(0.5f, 0.5f, 0.5f, 1.0f), 
									bool textured = false,
									unsigned int numberOfRepeats = 8) const;

protected:
	// Destructor and constructor are protected
	MHardwareRenderer();
	~MHardwareRenderer();

	// Static global instance of wrapper class
	//
	static	MHardwareRenderer *	fsHardwareRenderer;

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MHardwareRenderer */


