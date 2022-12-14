#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "game.h"
#include "imgui/imgui.h"


	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		auto& io = ImGui::GetIO();
		if (io.WantCaptureMouse) {
			if (rndr->IsPressed())
				rndr->Pressed();
			return;
		}
		if (action == GLFW_PRESS)
		{

			rndr->lastButtonPressed = button;
			Game* scn = (Game*)rndr->GetScene();
			double x2, y2;
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress(x2, y2);
			if (rndr->CheckViewport(x2, y2, 1)) //bezier curve
			{
				//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
					scn->setPressControlPoint(x2, y2);
				scn->pickedViewPort = 1;
				if (rndr->IsPressed())
					rndr->Pressed();
			}
			else
			{
				if (!rndr->tryToZoom && rndr->Picking((int)x2, (int)y2))
				{
					rndr->UpdatePosition(x2, y2);
					rndr->Pick();
					scn->pickedViewPort = 3;


				}
				else
				{
					rndr->Pressed();
					rndr->UnPick(3);
					rndr->setTryToPickMany(true);
					scn->pickedViewPort = 3;

				}

			}
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Game* scn = (Game*)rndr->GetScene();
			if (scn->pickedViewPort != 1) {
				if (rndr->tryToZoom) {
					rndr->ZoomInToArea();
					rndr->Pressed();
					rndr->setTryToPickMany(false);
				}
				else if (rndr->IsTryToPickMany()) {
					rndr->PickMany(3);
					rndr->Pressed();
					rndr->setTryToPickMany(false);

				}
				else {
					if(rndr->IsPressed())
						rndr->Pressed();
					rndr->UnPick(3);

				}

			}
		}

	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Game* scn = (Game*)rndr->GetScene();
		if (rndr->IsPicked()) {
			for (int pShape : scn->pShapes) {
				scn->selected_data_index = pShape;
				scn->ShapeTransformation(scn->zTranslate, (float)-0.4f * yoffset, 1);

			}
		}
		else {
			rndr->MoveCamera(rndr->currentScreenControl, scn->zTranslate, (float)-0.4f * yoffset);
		}
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Game* scn = (Game*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);
	
		if ( rndr->CheckViewport(xpos, ypos, 1)) {
			if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				scn->updateCurve(xpos, ypos);
			}
		}
		if (!rndr->IsPressed()&&rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{ 
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);

	
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (rndr->lastButtonPressed == GLFW_MOUSE_BUTTON_LEFT && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
			{

				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (rndr->lastButtonPressed == GLFW_MOUSE_BUTTON_RIGHT && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}
	
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Game* scn = (Game*)rndr->GetScene();
		auto& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard) {
			return;
		}
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_UP:

				rndr->MoveCamera(rndr->currentScreenControl, scn->xRotate, 0.05f);
				
				break;
			case GLFW_KEY_DOWN:
				
				rndr->MoveCamera(rndr->currentScreenControl, scn->xRotate, -0.05f);
				break;
			case GLFW_KEY_LEFT:
				rndr->MoveCamera(rndr->currentScreenControl, scn->yRotate, 0.05f);
				break;
			case GLFW_KEY_RIGHT:

				rndr->MoveCamera(rndr->currentScreenControl, scn->yRotate, -0.05f);
				break;
			case GLFW_KEY_U:
				rndr->MoveCamera(rndr->currentScreenControl, scn->yTranslate, 0.25f);
				break;
			case GLFW_KEY_D:
				rndr->MoveCamera(rndr->currentScreenControl, scn->yTranslate, -0.25f);
				break;
			case GLFW_KEY_L:
				rndr->MoveCamera(rndr->currentScreenControl, scn->xTranslate, -0.25f);
				break;
			
			case GLFW_KEY_R:
				rndr->MoveCamera(rndr->currentScreenControl, scn->xTranslate, 0.25f);
				break;
			
			case GLFW_KEY_B:
				rndr->MoveCamera(rndr->currentScreenControl, scn->zTranslate, 0.5f);

				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(rndr->currentScreenControl, scn->zTranslate, -0.5f);
				break;
			case GLFW_KEY_S:
				rndr->switchToNextCamera();
				break;
			case GLFW_KEY_C:
				rndr->switchControlForScreen();
				break;
			case GLFW_KEY_1:
				std::cout << "picked 1\n";
				scn->pickedShape = 1;
				break;
			case GLFW_KEY_2:
				std::cout << "picked 2\n";
				scn->pickedShape = 2;
				break;
			case GLFW_KEY_3:
				std::cout << "picked 3\n";
				scn->pickedShape = 3;
				break;
			default:
				break;

			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    menu->init(&display);
}