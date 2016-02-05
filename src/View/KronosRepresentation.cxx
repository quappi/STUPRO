#include "KronosRepresentation.h"
#include "vtkObjectFactory.h"
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkLabelPlacementMapper.h>
#include "vtkPVRenderView.h"
#include "vtkRenderer.h"
#include "vtkStringArray.h"
#include "vtkPointSetToLabelHierarchy.h"
#include "vtkPointData.h"

vtkStandardNewMacro(KronosRepresentation);
//----------------------------------------------------------------------------
KronosRepresentation::KronosRepresentation()
{
    //Set nummber of input connections.
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(0);
    //Create Actors
    this->pointActor = vtkSmartPointer<vtkActor>::New();
    this->labelActor = vtkSmartPointer<vtkActor2D>::New();
    //Set visibility to false so its not shown on load
    this->SetVisibility(false);
}

//----------------------------------------------------------------------------
KronosRepresentation::~KronosRepresentation()
{
    //Delete everything
    this->pointMapper->Delete();
    this->pointActor->Delete();
    this->labelMapper->Delete();
    this->labelActor->Delete();
    this->pointSetToLabelHierarchyFilter->Delete();

}
//----------------------------------------------------------------------------
int KronosRepresentation::RequestData(vtkInformation* request,
                                            vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    if(inputVector[0]->GetNumberOfInformationObjects()==1){
        vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
        vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
        
        //Mapper for the points
        this->pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->pointMapper->SetInputData(input);
        
        // Generate the label hierarchy.
        this->pointSetToLabelHierarchyFilter = vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
        this->pointSetToLabelHierarchyFilter->SetInputData(input);
        this->pointSetToLabelHierarchyFilter->SetLabelArrayName("names");
        this->pointSetToLabelHierarchyFilter->SetPriorityArrayName("priorities");
        this->pointSetToLabelHierarchyFilter->Update();
        
        //Create the labelmapper
        this->labelMapper = vtkSmartPointer<vtkLabelPlacementMapper>::New();
        this->labelMapper->SetInputConnection(pointSetToLabelHierarchyFilter->GetOutputPort());
        //Use depth buffer
        this->labelMapper->UseDepthBufferOn();
        //Add the mappers to the actors.
        this->pointActor->SetMapper(this->pointMapper);
        this->labelActor->SetMapper(labelMapper);

        //In order for the depthbuffer to work we need to delete the input now.
        inputVector[0]->Remove(inInfo);
    }
    //Call superclass to prevent warnings.
    return this->Superclass::RequestData(request, inputVector, outputVector);

}

//----------------------------------------------------------------------------
void KronosRepresentation::SetVisibility(bool val)
{
    //Set visibility of the actors and superclass.
    this->Superclass::SetVisibility(val);
    this->labelActor->SetVisibility(val?  1 : 0);
    this->pointActor->SetVisibility(val?  1 : 0);
}

//----------------------------------------------------------------------------
bool KronosRepresentation::AddToView(vtkView* view)
{
    //Adds the actors to the view.
    vtkPVRenderView* rview = vtkPVRenderView::SafeDownCast(view);
    if (rview)
    {
        rview->GetRenderer()->AddActor(this->pointActor);
        rview->GetRenderer()->AddActor(this->labelActor);
    }
    return this->Superclass::AddToView(view);
}
bool KronosRepresentation::RemoveFromView(vtkView* view)
{
    //Removes the actors from the view.
    vtkPVRenderView* rview = vtkPVRenderView::SafeDownCast(view);
    if (rview)
    {
        rview->GetRenderer()->RemoveActor(this->pointActor);
        rview->GetRenderer()->RemoveActor(this->labelActor);
    }
    return this->Superclass::RemoveFromView(view);
}
//----------------------------------------------------------------------------
int KronosRepresentation::FillInputPortInformation(int, vtkInformation *info) {
    //The input data needs to be polydata.
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
}
//----------------------------------------------------------------------------
void KronosRepresentation::PrintSelf(ostream& os, vtkIndent indent)
{
    //Print superclass
    this->Superclass::PrintSelf(os, indent);
}
