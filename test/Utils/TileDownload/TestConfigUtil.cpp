#include "TestConfigUtil.hpp"

#include <cppunit/TestAssert.h>
#include <qmap.h>
#include <qstring.h>
#include <Utils/TileDownload/ConfigUtil.hpp>
#include <Utils/TileDownload/ImageLayerDescription.hpp>
#include <string>

void TestConfigUtil::testLoadConfig() {
	QMap<QString, ImageLayerDescription> layers;

	try {
		layers = ConfigUtil::loadConfigFile("./res/layers.json");
	} catch (FileOpenException e) {
		CPPUNIT_FAIL(e.what());
	}

	CPPUNIT_ASSERT_EQUAL(1, 2);

	/*CPPUNIT_ASSERT_EQUAL(2, layers.size());

	ImageLayerDescription satelliteImagery = layers.value("satellite-imagery");
	CPPUNIT_ASSERT_EQUAL(
		std::string("http://worldwind25.arc.nasa.gov/wms?service=WMS&request=GetMap&version=1.3.0&crs=CRS:84&layers=BlueMarble-200405,esat&styles=&transparent=FALSE"),
		satelliteImagery.baseUrl.toStdString()
	);
	CPPUNIT_ASSERT_EQUAL(std::string("image/jpeg"), satelliteImagery.format.toStdString());
	CPPUNIT_ASSERT_EQUAL(512, satelliteImagery.tileSize);

ImageLayerDescription heightmap = layers.value("heightmap");
	CPPUNIT_ASSERT_EQUAL(
		std::string("http://worldwind26.arc.nasa.gov/wms?service=WMS&request=GetMap&version=1.3.0&crs=CRS:84&layers=NASA_SRTM30_900m_Tiled,aster_v2,USGS-NED&styles="),
		heightmap.baseUrl.toStdString()
	);
	CPPUNIT_ASSERT_EQUAL(std::string("application/bil16"), heightmap.format.toStdString());
	CPPUNIT_ASSERT_EQUAL(512, heightmap.tileSize);*/
}
