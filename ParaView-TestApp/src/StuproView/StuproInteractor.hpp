#ifndef STUPRO_STUPROINTERACTOR_HPP
#define STUPRO_STUPROINTERACTOR_HPP

#include <vtkInteractorStyleTerrain.h>
#include <cstring>

class StuproInteractor : public vtkInteractorStyleTerrain
{
public:
	static StuproInteractor *New();

	vtkTypeMacro(StuproInteractor, vtkInteractorStyleTerrain);

	void OnTimer() override
	{
	}

	/*
	void OnMiddleButtonDown() override {}

	void OnMiddleButtonUp() override {}

	void OnRightButtonDown() override {}

	void OnRightButtonUp() override {}
	*/

	void OnMouseWheelForward() override;

	void OnMouseWheelBackward() override;

	void zoomWithFactor(float factor);
};

#endif
