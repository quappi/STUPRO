#include "vtkCityReader.h"
#include "vtkCommand.h"
#include "vtkDataSetAttributes.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkTable.h"
#include "vtkUnicodeStringArray.h"
#include "vtkStringArray.h"
#include "vtkStringToNumeric.h"

#include "vtkTextCodec.h"
#include "vtkTextCodecFactory.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <set>
#include <vector>
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkGenericCell.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkTensor.h"
#include "vtkPolyData.h"
#include <vtkCellArray.h>
#include "vtkPointData.h"
#include "vtkPoints.h"
#include <math.h>
#include <ctype.h>



/////////////////////////////////////////////////////////////////////////////////////////
// vtkCityReader

vtkStandardNewMacro(vtkCityReader);

vtkCityReader::vtkCityReader()
{
    cout << "Hello" << endl;
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkCityReader::~vtkCityReader()
{

}

void vtkCityReader::PrintSelf(ostream& os, vtkIndent indent)
{

}

int vtkCityReader::ProcessRequest	(vtkInformation * 	request, vtkInformationVector ** 	inInfo,
                 vtkInformationVector * 	outInfo
                 ){
    cout << "test" << endl;
    return 1;
}

void vtkCityReader::SetFileName(std::string name){
    cout<<name<<endl;
}

int vtkCityReader::RequestData(
  vtkInformation*,
  vtkInformationVector**,
  vtkInformationVector* outputVector)
{

    int tempvar=1;
    vtkPolyData* output = vtkPolyData::GetData(outputVector);
    int numberOfQuadsRight=20;
    int numberOfQuadsUp=20;
    
    int nmbPoints=5*numberOfQuadsRight*numberOfQuadsUp;
    
    vtkCellArray *newPolys;
    newPolys = vtkCellArray::New();
    newPolys->Allocate(newPolys->EstimateSize(nmbPoints, 3));
    
    vtkPoints *newPoints;
    newPoints = vtkPoints::New();
    vtkFloatArray *newNormals;
    newPoints->Allocate(nmbPoints);
    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->Allocate(3*nmbPoints);
    newNormals->SetName("Normals");
    
    vtkIdType pts[4];
    for (int i=0; i<numberOfQuadsRight; i++) {
        for (int j=0; j<numberOfQuadsUp; j++) {
            
            
            pts[0]  = newPoints->InsertNextPoint(-1.0+2.0*i, 0.0+2.0*j, sin(-1.0+2.0*i)*2*tempvar);
            pts[1]  = newPoints->InsertNextPoint(1.0+2.0*i, 0.0+2.0*j, sin(1.0+2.0*i)*2*tempvar);
            pts[2]  = newPoints->InsertNextPoint(0.0+2.0*i, 1.0+2.0*j, sin(0.0+2.0*i)*2*tempvar);
            newPolys->InsertNextCell(3, pts);
            newNormals->InsertTuple3(pts[0], -1.0, 0.0, 0.0);
            newNormals->InsertTuple3(pts[1], -1.0, 0.0, 0.0);
            newNormals->InsertTuple3(pts[2], -1.0, 0.0, 0.0);
            
            pts[1]  = newPoints->InsertNextPoint(-1.0+2.0*i, 2.0+2.0*j, sin(-1.0+2.0*i)*2*tempvar);
            newPolys->InsertNextCell(3, pts);
            newNormals->InsertTuple3(pts[1], -1.0, 0.0, 0.0);
            
            pts[0]  = newPoints->InsertNextPoint(1.0+2.0*i, 2.0+2.0*j, sin(1.0+2.0*i)*2*tempvar);
            newPolys->InsertNextCell(3, pts);
            newNormals->InsertTuple3(pts[0], -1.0, 0.0, 0.0);
            
            pts[1]  = newPoints->InsertNextPoint(1.0+2.0*i, 0.0+2.0*j, sin(1.0+2.0*i)*2*tempvar);
            newNormals->InsertTuple3(pts[0], -1.0, 0.0, 0.0);
            newPolys->InsertNextCell(3, pts);
        }
        
    }
    
    // output->CopyStructure( input );
    newPoints->Squeeze();
    output->SetPoints(newPoints);
    newPoints->Delete();
    
    newNormals->Squeeze();
    output->GetPointData()->SetNormals(newNormals);
    newNormals->Delete();
    
    newPolys->Squeeze();
    output->SetPolys(newPolys);
    newPolys->Delete();

  return 1;
}