#ifndef STUPRO_VTKOPENGL_HPP
#define STUPRO_VTKOPENGL_HPP

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkShader2.h>
#include <vtkUniformVariables.h>
#include <vtkOpenGLTexture.h>
#include "vtkOBBTree.h"

#include <functional>
#include <cmath>
#include <string>

#include "StuproInteractor.hpp"

#include "Coordinate.hpp"


class VTKOpenGL
{
public:
	
	void run();

private:
	
	enum DisplayMode
	{
		DisplayGlobe, DisplayMap
	};

	void init();
	void initParameters();
	void initGlobe();
	void initRenderer();
	void initShaders();
	void initCallbacks();

	vtkSmartPointer<vtkOpenGLTexture> loadAlphaTexture(std::string rgbFile,
	        std::string alphaFile) const;

	vtkSmartPointer<vtkRenderer> myRenderer;
	vtkSmartPointer<vtkRenderWindow> myRenderWindow;
	vtkSmartPointer<vtkActor> myPlaneActor;

	vtkSmartPointer<vtkShader2> myVertexShader;
	vtkSmartPointer<vtkShader2> myFragmentShader;
    vtkSmartPointer<vtkOBBTree> mySphereTree;
    vtkSmartPointer<vtkOBBTree> myPlaneTree;
    
    DisplayMode myDisplayMode;

    static void cutPlanes(double planes[3][4], double cut [3]);
    static void getIntersectionPoint(double plane1[4], double plane2[4], double plane3[4], double cameraPosition[],vtkSmartPointer<vtkOBBTree> tree, double intersection[3]);

    static Coordinate getCenterGlobeCoordinate(vtkSmartPointer<vtkOBBTree> tree, double cameraPosition[], double globeRadius);

    static std::vector<double *> getIntersectionPoints(double planes[24], double cameraPosition[3], vtkSmartPointer<vtkOBBTree> tree);
    static std::vector<Coordinate> getGlobeCoordinates(std::vector<double*> worldPoints, double radius);
    static std::vector<Coordinate> getPlaneCoordinates(std::vector<double*> worldPoints, double planeWidth, double planeHeight);
    
	float myGlobeRadius;
	float myPlaneSize;
	float myDisplayModeInterpolation;
	float myHeightFactor;
};

#endif