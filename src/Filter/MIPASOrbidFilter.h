#ifndef KRONOS_ABSTRACT_SELECTION_FILTER_HPP
#define KRONOS_ABSTRACT_SELECTION_FILTER_HPP

#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkDataObjectAlgorithm.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPointData.h>
#include <vtkUnsignedIntArray.h>

#include <qstring.h>
#include <qlist.h>

#include <Reader/DataReader/Data.hpp>
#include <Globe/Coordinate.hpp>


/**
 * Abstract super class for filters that work on a Kronos point data set and extract some of those points.
 */
class MIPASOrbidFilter : public vtkDataObjectAlgorithm {
public:

    vtkTypeMacro(MIPASOrbidFilter, vtkDataObjectAlgorithm)
    static MIPASOrbidFilter *New();

	void PrintSelf(ostream& os, vtkIndent indent) override;

	int RequestData(vtkInformation* info,
	                vtkInformationVector** inputVector,
	                vtkInformationVector* outputVector) override;
	int RequestInformation(vtkInformation* request,
	                       vtkInformationVector** inputVector,
	                       vtkInformationVector* outputVector) override;

	int FillOutputPortInformation(int port, vtkInformation* info) override;
	int FillInputPortInformation(int port, vtkInformation* info) override;
    void SetInputConnection(vtkAlgorithmOutput *input) override;

    void setUpper(int upperLimit);
    void setLower(int lowerLimit);




protected:
	/**
	 * Display an error message and remember that this filter does not hold valid data.
	 * @param message The error message to be shown to the user
	 */
	void fail(QString message);

private:
    MIPASOrbidFilter();
    ~MIPASOrbidFilter();

	MIPASOrbidFilter(const MIPASOrbidFilter&);  // Not implemented.
	void operator=(const MIPASOrbidFilter&);  // Not implemented.

	/**
	 * Boolean flag denoting whether there was an error.
	 */
	bool error;

    int UpperLimit;
    int LowerLimit;


    /**
     * Decide whether a data point should be kept in the selection.
     * @param pointIndex The index of the point to be checked
     * @param coordinate The coordinate of the point
     * @param pointData All scalar point data
     * @return True if the point should be kept, false otherwise
     */
    bool evaluatePoint(int pointIndex, Coordinate coordinate, vtkPointData* pointData);

};

#endif
