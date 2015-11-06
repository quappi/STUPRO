/*
 * Globe.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: bbq
 */

#include "Globe.hpp"

#include <vtkAlgorithm.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include "GlobeTile.hpp"
#include "MakeUnique.hpp"

Globe::Globe(vtkRenderer & renderer) :
		myRenderer(renderer), myZoomLevel(0), myDisplayModeInterpolation(0)
{
	myPlaneSource = vtkPlaneSource::New();
	myPlaneSource->SetOrigin(getPlaneSize() / 2.f, -getPlaneSize() / 2.f, 0.f);
	myPlaneSource->SetPoint1(-getPlaneSize() / 2.f, -getPlaneSize() / 2.f, 0.f);
	myPlaneSource->SetPoint2(getPlaneSize() / 2.f, getPlaneSize() / 2.f, 0.f);

	setResolution(Vector2u(128, 128));

	myPlaneMapper = vtkPolyDataMapper::New();
	myPlaneMapper->SetInputConnection(myPlaneSource->GetOutputPort());

	createTiles();
}

Globe::~Globe()
{
}

void Globe::setResolution(Vector2u resolution)
{
	myPlaneSource->SetResolution(resolution.x, resolution.y);
}

Vector2u Globe::getResolution() const
{
	Vector2i ret;
	myPlaneSource->GetResolution(ret.x, ret.y);
	return Vector2u(ret);
}

vtkSmartPointer<vtkPolyDataMapper> Globe::getPlaneMapper() const
{
	return myPlaneMapper;
}

vtkRenderWindow& Globe::getRenderWindow() const
{
	return *myRenderer.GetRenderWindow();
}

vtkRenderer& Globe::getRenderer() const
{
	return myRenderer;
}

void Globe::setZoomLevel(unsigned int zoomLevel)
{
	myZoomLevel = zoomLevel;
}

unsigned int Globe::getZoomLevel() const
{
	return myZoomLevel;
}

GlobeTile & Globe::getTileAt(int lon, int lat) const
{
	return *myTiles[getTileIndex(lon, lat)];
}

unsigned int Globe::getTileIndex(int lon, int lat) const
{
	GlobeTile::Location loc = GlobeTile::Location(myZoomLevel, lon, lat).getNormalized();

	return (1 << myZoomLevel) * loc.latitude * 2 + loc.longitude;
}

float Globe::getPlaneSize() const
{
	return 1.f;
}

void Globe::createTiles()
{
	unsigned int height = 1 << myZoomLevel;
	unsigned int width = height * 2;

	myTiles.resize(width * height);

	for (unsigned int lat = 0; lat < height; ++lat)
	{
		for (unsigned int lon = 0; lon < width; ++lon)
		{
			myTiles[getTileIndex(lon, lat)] = makeUnique<GlobeTile>(*this,
			        GlobeTile::Location(myZoomLevel, lon, lat));
			myTiles[getTileIndex(lon, lat)]->loadTexture();
		}
	}
}

void Globe::setDisplayModeInterpolation(float displayMode)
{
	myDisplayModeInterpolation = displayMode;

	for (const auto & tile : myTiles)
	{
		if (tile)
		{
			tile->updateUniforms();
		}
	}
}

float Globe::getDisplayModeInterpolation() const
{
	return myDisplayModeInterpolation;
}