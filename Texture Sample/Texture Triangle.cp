/*	File:		Texture Triangle.cp	Contains:	Much like the Gouraud shaded triangles sample, this one demonstrates				texture mapping.	Written by: Timothy Carroll		Copyright:	Copyright � 1998-1999 by Apple Computer, Inc., All Rights Reserved.				You may incorporate this Apple sample source code into your program(s) without				restriction. This Apple sample source code has been provided "AS IS" and the				responsibility for its operation is yours. You are not permitted to redistribute				this Apple sample source code as "Apple sample source code" after having made				changes. If you're going to re-distribute the source, we require that you make				it clear in the source that the code was descended from Apple sample source				code, but that you've made changes.	Change History (most recent first):				7/15/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1				*/#include <RAVE.h>#include "Common Stuff.h"#include "RAVE Utilities.h"#include <Fonts.h>#include <Windows.h>#include <Menus.h>#include <TextEdit.h>#include <Dialogs.h>void main (void);void DoRAVEWindow(void);void main (void){	// do standard mac init	InitGraf(&qd.thePort);	InitFonts();	InitWindows();	InitMenus();	TEInit();	InitDialogs(nil);	InitCursor();	MaxApplZone();	MoreMasters(); MoreMasters(); MoreMasters(); MoreMasters(); MoreMasters();	// Initialize the SIN and COS lookup tables (see RAVE Utilities.cp)	if (InitializeLookups() == noErr)		DoRAVEWindow();}void DoRAVEWindow(void){	// These hold our errors	OSErr			theErr = noErr;	TQAError		theQAErr = kQANoErr;		// These hold the window and device we'll put on the screen	GDHandle		device = NULL;	WindowRef		theWindow = NULL;		// We try to center the window on the device.	Rect			bounds = {0,0,320,320};	Rect			deviceRect;		// We use these parameters to describe our environment to RAVE	TQADevice		qaDevice;	TQARect 		qaBoundsRect;		// we get these items back from RAVE	TQAEngine 		*theEngine = NULL;	TQADrawContext	*theContext = NULL;		// We use these in our drawing loop to create our RAVE animation.	int 			loop = 0;	TQATexture 		*texture = NULL;	TQAVTexture 	theTriangle[3];		RgnHandle		gray;	Rect			grayRect;			// First, grab the first monitor with the deepest display	gray = GetGrayRgn();	grayRect = (**gray).rgnBBox;	device = GetMaxDevice(&grayRect);	FAIL_NIL (device, "\pERROR: No monitors found")			// Center our bounds in the middle of the screen.	deviceRect = (**device).gdRect;	OffsetRect (&bounds,					(deviceRect.right+deviceRect.left-bounds.right)/2 ,					(deviceRect.bottom+deviceRect.top-bounds.bottom)/2);		// Create a Window to hold the RAVE information.			theWindow = NewCWindow(NULL, &bounds, "\pRAVE Window", true, zoomDocProc, (WindowRef) -1, false, 0);	FAIL_NIL (theWindow, "\pERROR: Couldn't create RAVE Window")		SetPortWindowPort (theWindow);		// Next we need to find a RAVE engine and then build a DrawContext to draw into it.  First, we'll find	// an engine that is capable of drawing to the selected GDevice.	qaDevice.deviceType = kQADeviceGDevice;	qaDevice.device.gDevice = device;		// We have to explicitly enable the Apple hardware card because it isn't 100% RAVE compliant.	// If we use this engine, we have to write special case code to deal with it.#if qEnableAppleHardware	QAEngineEnable (kQAVendor_Apple,kQAEngine_AppleHW);#endif		// We'll just grab the first engine that the system offers us.	theEngine = FindTextureMappingEngine (&qaDevice);	FAIL_NIL (theEngine, "\pERROR:  No RAVE engines available that can texture map.")		// Next we need to set up the a QARect with the Window's rectangle in GDevice local coordinates.		qaBoundsRect.left = bounds.left - deviceRect.left;	qaBoundsRect.right = bounds.right  - deviceRect.left;	qaBoundsRect.top = bounds.top - deviceRect.top;	qaBoundsRect.bottom = bounds.bottom - deviceRect.top;	// If we wanted to create a clipping region, we'd initialize a clip here.		// Finally, we're ready to create the draw context!	theQAErr = QADrawContextNew (&qaDevice, &qaBoundsRect, NULL, theEngine, 								 kQAContext_DeepZ | kQAContext_DoubleBuffer, &theContext);	if (theQAErr != kQANoErr) SIGNAL_ERROR ("\pERROR:  Failed to create RAVE draw context")	FAIL_NIL (theContext, "\pERROR:  Failed to create RAVE draw context")		// set the background to black	QASetFloat (theContext, kQATag_ColorBG_a, 1.0);	QASetFloat (theContext, kQATag_ColorBG_r, 0.0);	QASetFloat (theContext, kQATag_ColorBG_g, 0.0);	QASetFloat (theContext, kQATag_ColorBG_b, 0.0);		// Create the texture we'll use to animate our triangle.	texture = LoadTextureFromPictResource (theEngine, 128);	FAIL_NIL (texture, "\pERROR: Failed to load the texture from the resource")		while (!Button())	{	theTriangle[0].x = 160 + 150*gCosArray[loop];	theTriangle[0].y = 160 + 150*gSinArray[loop];	theTriangle[0].z = 0.25;	theTriangle[0].invW = 1.0;	theTriangle[0].a = 1.0;	theTriangle[0].r = 0.0;	theTriangle[0].g = 0.0;	theTriangle[0].b = 0.0;	theTriangle[0].uOverW = 0.5;	theTriangle[0].vOverW = 0;		theTriangle[1].x = 160 + 150*gCosArray[(loop+240) % 720];	theTriangle[1].y = 160 + 150*gSinArray[(loop+240) % 720];	theTriangle[1].z = 0.25;	theTriangle[1].invW = 1.0;	theTriangle[1].a = 1.0;	theTriangle[1].r = 0.0;	theTriangle[1].g = 0.0;	theTriangle[1].b = 0.0;	theTriangle[1].uOverW = 0;	theTriangle[1].vOverW = 1;		theTriangle[2].x = 160 + 150*gCosArray[(loop+480) % 720];	theTriangle[2].y = 160 + 150*gSinArray[(loop+480) % 720];	theTriangle[2].z = 0.25;	theTriangle[2].invW = 1.0;	theTriangle[2].a = 1.0;	theTriangle[2].r = 0.0;	theTriangle[2].g = 0.0;	theTriangle[2].b = 0.0;	theTriangle[2].uOverW = 1;	theTriangle[2].vOverW = 1;	QARenderStart (theContext, NULL, NULL);	QASetPtr (theContext, kQATag_Texture, texture);	QADrawTriTexture (theContext, &theTriangle[0], &theTriangle[1], &theTriangle[2], kQATriFlags_None);	QARenderEnd(theContext, NULL);		// Increment our loop position		loop = (loop+5) % 720;	}	error:	if (texture != NULL)		QATextureDelete (theEngine, texture);	if (theContext != NULL)		QADrawContextDelete (theContext);	if (theWindow != NULL)		DisposeWindow(theWindow);	return;}