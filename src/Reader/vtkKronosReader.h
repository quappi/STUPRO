#ifndef __vtkKronosReader_h
#define __vtkKronosReader_h

#include "vtkTableAlgorithm.h"
#include "vtkPolyDataAlgorithm.h"
#include "Utils/Math/Vector3.hpp"
class vtkKronosReader : public vtkPolyDataAlgorithm {
public:
	static vtkKronosReader* New();
	vtkTypeMacro(vtkKronosReader, vtkPolyDataAlgorithm);
	void SetFileName(std::string name);
    void SetCameraPos(double x,double y,double z);
    void SetUseOffscreenRenderingForScreenshots(int a);
protected:
	vtkKronosReader();
	~vtkKronosReader();

	int RequestData(
		vtkInformation*,
		vtkInformationVector**,
		vtkInformationVector*) override;
    Vector3d cameraPos = Vector3d();
    double distanceToFocalPoint = 0.0;
private:
	vtkKronosReader(const vtkKronosReader&); // Not implemented
	void operator=(const vtkKronosReader&);   // Not implemented

};

#endif