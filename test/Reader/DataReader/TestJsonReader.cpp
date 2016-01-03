#include <gtest/gtest.h>

#include <string>

#include <Reader/DataReader/JsonReaderFactory.hpp>
#include <Reader/DataReader/JsonReader.hpp>
#include <Reader/DataReader/DataType.hpp>
#include <Reader/DataReader/DataPoints/DataPoint.hpp>
#include <Reader/DataReader/DataPoints/NonTemporalDataPoints/CityDataPoint.hpp>
#include <Reader/DataReader/DataPoints/TemporalDataPoints/TweetDataPoint.hpp>

#include <Utils/Config/Configuration.hpp>

#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkStringArray.h>
#include <vtkAbstractArray.h>
#include <vtkDataArray.h>
#include <vtkTypeInt32Array.h>
#include <vtkDoubleArray.h>

TEST(TestJsonReader, ReadCityData) {
	JsonReader cityReader = JsonReaderFactory::createReader("res/test-data/cities.json");
	EXPECT_EQ(
		4,
		cityReader.pointDataSet.getDataPoints().size()
	);
	
	const CityDataPoint* firstDataPoint = dynamic_cast<const CityDataPoint*>(
		cityReader.pointDataSet.getDataPoints().at(1)
	);
	
	EXPECT_EQ(
		"Los Angeles",
		firstDataPoint->getName().toStdString()
	);
	
	EXPECT_EQ(
		34.052223,
		firstDataPoint->getCoordinate().lat()
	);
	
	EXPECT_EQ(
		-118.242775,
		firstDataPoint->getCoordinate().lon()
	);
}

TEST(TestJsonReader, ReadTwitterData) {
	JsonReader twitterReader = JsonReaderFactory::createReader("res/test-data/tweets.json");
	EXPECT_EQ(
		3,
		twitterReader.pointDataSet.getDataPoints().size()
	);
	
	const TweetDataPoint* testDataPoint = dynamic_cast<const TweetDataPoint*>(
		twitterReader.pointDataSet.getDataPoints().at(1)
	);
	
	EXPECT_EQ(
		"elonmusk",
		testDataPoint->getAuthor().toStdString()
	);
	
	EXPECT_EQ(
		"Is this working?",
		testDataPoint->getContent().toStdString()
	);
	
	EXPECT_EQ(
		1439280065,
		testDataPoint->getTimestamp()
	);
	
	EXPECT_EQ(
		34.052223,
		testDataPoint->getCoordinate().lat()
	);
	
	EXPECT_EQ(
		-118.242775,
		testDataPoint->getCoordinate().lon()
	);
}

TEST(TestJsonReader, TestPointCoordinatesInVtkPolyData) {
	JsonReader jsonReader = JsonReaderFactory::createReader("res/test-data/cities.json");
	
    vtkSmartPointer<vtkPolyData> polyData = jsonReader.getVtkDataSet(9);
	vtkSmartPointer<vtkPolyData> lessPolyData = jsonReader.getVtkDataSet(0);
    
	// Test the actual points along with their coordinates
	
	EXPECT_EQ(
		4,
		polyData->GetNumberOfPoints()
	);
	
	EXPECT_EQ(
		2,
		lessPolyData->GetNumberOfPoints()
	);
	
	double testPointCoordinates[3];
	polyData->GetPoint(2, testPointCoordinates);
	
	EXPECT_FLOAT_EQ(
		-122.418335,
		testPointCoordinates[0]
	);
	
	EXPECT_FLOAT_EQ(
		37.775,
		testPointCoordinates[1]
	);
	
	EXPECT_FLOAT_EQ(
		0,
		testPointCoordinates[2]
	);
}

TEST(TestJsonReader, WriteCitiesToVtkPolyData) {
    JsonReader jsonReader = JsonReaderFactory::createReader("res/test-data/cities.json");	
    vtkSmartPointer<vtkPolyData> polyData = jsonReader.getVtkDataSet(
		Configuration::getInstance().getInteger("dataReader.maximumPriority")
	);

	// Test the associated array of city names
	vtkSmartPointer<vtkAbstractArray> abstractCityNameArray = polyData->GetPointData()
		->GetAbstractArray("names");
	ASSERT_TRUE(abstractCityNameArray);
	vtkSmartPointer<vtkStringArray> cityNameArray = vtkStringArray::SafeDownCast(
		abstractCityNameArray
	);
	ASSERT_TRUE(cityNameArray);
	
	EXPECT_EQ(
		4,
		cityNameArray->GetNumberOfValues()
	);
	
	EXPECT_EQ(
		"San Francisco",
		cityNameArray->GetValue(2)
	);
	
	// Test the associated array of data point priorities
	vtkSmartPointer<vtkDataArray> abstractPriorityArray = polyData->GetPointData()
		->GetArray("priorities");
	ASSERT_TRUE(abstractPriorityArray);
	vtkSmartPointer<vtkTypeInt32Array> priorityArray = vtkTypeInt32Array::SafeDownCast(
		abstractPriorityArray
	);
	ASSERT_TRUE(priorityArray);
	
	EXPECT_EQ(
		4,
		priorityArray->GetNumberOfComponents()
	);
	
	EXPECT_EQ(
		Configuration::getInstance().getInteger("dataReader.maximumPriority") - 1,
		priorityArray->GetValue(2)
	);
}

TEST(TestJsonReader, WriteFlightsToVtkPolyData) {
    JsonReader jsonReader = JsonReaderFactory::createReader("res/test-data/flights.json");
    vtkSmartPointer<vtkPolyData> polyData = jsonReader.getVtkDataSet(
		Configuration::getInstance().getInteger("dataReader.maximumPriority")
	);

	// Test the associated array of destination coordinates
	vtkSmartPointer<vtkDataArray> abstractDestinationArray = polyData->GetPointData()
		->GetArray("destinations");
	ASSERT_TRUE(abstractDestinationArray);
	vtkSmartPointer<vtkDoubleArray> destinationArray = vtkDoubleArray::SafeDownCast(
		abstractDestinationArray
	);
	ASSERT_TRUE(destinationArray);
	
	EXPECT_EQ(
		2,
		destinationArray->GetNumberOfComponents()
	);
	
	EXPECT_FLOAT_EQ(
		34.052223,
		destinationArray->GetTuple2(0)[0]
	);
	
	EXPECT_FLOAT_EQ(
		-118.242775,
		destinationArray->GetTuple2(0)[1]
	);
	
	// Test the associated array of data point priorities
	vtkSmartPointer<vtkDataArray> abstractPriorityArray = polyData->GetPointData()
		->GetArray("priorities");
	ASSERT_TRUE(abstractPriorityArray);
	vtkSmartPointer<vtkTypeInt32Array> priorityArray = vtkTypeInt32Array::SafeDownCast(
		abstractPriorityArray
	);
	ASSERT_TRUE(priorityArray);
	
	EXPECT_EQ(
		1,
		priorityArray->GetNumberOfComponents()
	);
	
	EXPECT_EQ(
		Configuration::getInstance().getInteger("dataReader.maximumPriority"),
		priorityArray->GetValue(0)
	);
}