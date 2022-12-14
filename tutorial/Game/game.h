#pragma once

#include "igl/opengl/glfw/Viewer.h"

enum class editorState { DESIGN, ANIMATION };

class Game : public igl::opengl::glfw::Viewer
{
	int m_time; 
public:
	int  pickedShape;

	std::vector<Eigen::Vector3f> uanim; 

	Game();
//	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
	unsigned int Game::CreateTex(int width, int height);
	void setPressControlPoint(float x, float y);
	void updateCurve(float x, float y);
	Eigen::Matrix4d getTranslateRes(Eigen::Vector3d amt, bool preRotation);
	virtual void setCurve();
	virtual void setCameraCurve();
	virtual void removeCameraCurve();

	virtual void removeCurve();

	~Game(void);
	void setState(editorState newState) {
		 state = newState;
	}
	editorState getState() {
		return state;
	}

private:
	editorState state = editorState::DESIGN;
	Eigen::Affine3d ToutCopy;
	Eigen::Affine3d TinCopy;
};

