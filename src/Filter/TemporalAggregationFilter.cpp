#include <Filter/TemporalAggregationFilter.h>

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>

#include <algorithm>
#include <vector>

vtkStandardNewMacro(TemporalAggregationFilter);

const QList<Data::Type> TemporalAggregationFilter::SUPPORTED_DATA_TYPES = QList<Data::Type>() << Data::PRECIPITATION << Data::TEMPERATURE << Data::WIND << Data::CLOUD_COVERAGE;

TemporalAggregationFilter::TemporalAggregationFilter() : currentTimeStep(0), error(false) {
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(1);
}

TemporalAggregationFilter::~TemporalAggregationFilter() { }

void TemporalAggregationFilter::fail(QString message) {
	vtkErrorMacro( << message.toStdString());
	this->error = true;
}

void TemporalAggregationFilter::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Temporal data point aggregation, Kronos Project" << endl;
}

int TemporalAggregationFilter::FillInputPortInformation(int port, vtkInformation* info) {
    if (port == 0) {
		info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
		info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 0);
	}
    
    return 1;
}

int TemporalAggregationFilter::FillOutputPortInformation(int port, vtkInformation* info) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
    return 1;
}

int TemporalAggregationFilter::RequestInformation (
  vtkInformation *request,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector) {
    if (this->error) {
        return 0;
    }

    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    
    if (inInfo->Has(Data::VTK_DATA_TYPE())) {
		this->dataType = static_cast<Data::Type>(inInfo->Get(Data::VTK_DATA_TYPE()));
	} else {
		this->fail("This filter only works with data read by the Kronos reader.");
		return 0;
	}
    
    // Overkill code for creating a comma-separated string with the names of all supported data types
    QString supportedTypes = "";
    int amountOfSupportedDataTypes = TemporalAggregationFilter::SUPPORTED_DATA_TYPES.size();
    if (amountOfSupportedDataTypes == 1) {
        supportedTypes.append(Data::getDataTypeName(TemporalAggregationFilter::SUPPORTED_DATA_TYPES.value(0)));
    } else if (amountOfSupportedDataTypes > 1) {
        for (int i = 0; i < amountOfSupportedDataTypes - 2; i++) {
            supportedTypes.append(Data::getDataTypeName(TemporalAggregationFilter::SUPPORTED_DATA_TYPES.value(i)));
            if (i < amountOfSupportedDataTypes - 3) {
                supportedTypes.append(", ");
            }
        }
        supportedTypes.append(" and ").append(Data::getDataTypeName(TemporalAggregationFilter::SUPPORTED_DATA_TYPES.value(amountOfSupportedDataTypes - 1)));
    }
    
    if (!TemporalAggregationFilter::SUPPORTED_DATA_TYPES.contains(this->dataType)) {
        this->fail(QString("This filter only supports %1 data, but the input contains %2 data.").arg(supportedTypes, Data::getDataTypeName(this->dataType)));
        return 0;
    }
    
    // This filter's output is an aggregation of values over time and therefore has no time information
    outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_RANGE());

    return 1;
}

int TemporalAggregationFilter::RequestData(
  vtkInformation *request,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector) {
    if (this->error) {
        return 0;
    }
    
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtkDataObject *input = vtkDataObject::GetData(inInfo);
    vtkDataObject *output = vtkDataObject::GetData(outInfo);
    
    std::cout << "Current timestep: " << this->currentTimeStep << std::endl;

    if (this->currentTimeStep == 0) {
        // TODO: First execution, initialize stuff
    } else {
        // TODO: Subsequent execution, accumulate new data
    }

    this->currentTimeStep++;

    if (this->currentTimeStep < inInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS())) {
        // There are still time steps left, continue on
        request->Set(vtkStreamingDemandDrivenPipeline::CONTINUE_EXECUTING(), 1);
    } else {
        // Everything has been accumulated
        // TODO: Finish up and output data
        
        request->Remove(vtkStreamingDemandDrivenPipeline::CONTINUE_EXECUTING());
        this->currentTimeStep = 0;
    }
    
    return 1;
}

int TemporalAggregationFilter::RequestUpdateExtent (
  vtkInformation *request,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector) {
    if (this->error) {
        return 0;
    }
    
    // TODO: Update progress
    
    vtkInformation *inputInformation = inputVector[0]->GetInformationObject(0);

    // Make the pipeline executive iterate the upstream pipeline time steps by setting the update time step appropiately
    double *inputTimeSteps = inputInformation->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    if (inputTimeSteps) {
        inputInformation->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), inputTimeSteps[this->currentTimeStep]);
    }

    return 1;
}