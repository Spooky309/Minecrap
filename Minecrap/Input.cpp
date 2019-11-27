#include "Input.h"
#include <cstring>

void Input::Init(GLFWwindow* wind)
{
	win = wind;
	glfwSetKeyCallback(win, &KeyCallback);
	glfwSetCursorPosCallback(win, &CurPosCallback);
	glfwSetMouseButtonCallback(win, &MouseCallback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	double fm, fy;
	glfwGetCursorPos(win, &fm, &fy);
	m_lmx = (float)fm;
	m_lmy = (float)fy;
}

void Input::Update(GLFWwindow* wind)
{
	memcpy(m_lkeys, m_keys, 1024 * sizeof(bool));
	memcpy(m_lbuts, m_buts, 32 * sizeof(bool));
	double fm, fy;
	glfwGetCursorPos(wind, &fm, &fy);
	glfwSetCursorPos(wind, 1024.0f / 2.0f, 768.0f / 2.0f);
	m_mdx = ((float)fm - 1024.0f/2.0f) * m_mSensitivity;
	m_mdy = (768.0f/2.0f - (float)fy) * m_mSensitivity;
	m_lmx = (float)fm;
	m_lmy = (float)fy;
}

bool Input::GetKey(const int& key)
{
	return m_keys[key];
}

bool Input::GetKeyDown(const int& key)
{
	return m_keys[key] && !m_lkeys[key];
}

bool Input::GetKeyUp(const int& key)
{
	return !m_keys[key] && m_lkeys[key];
}

bool Input::GetMouseButton(const int& button)
{
	return m_buts[button];
}

bool Input::GetMouseButtonDown(const int& button)
{
	return m_buts[button] && !m_lbuts[button];
}

bool Input::GetMouseButtonUp(const int& button)
{
	return !m_buts[button] && m_lbuts[button];
}

float Input::GetMouseDeltaX()
{
	return m_mdx;
}

float Input::GetMouseDeltaY()
{
	return m_mdy;
}

void Input::IntKeyCallback(GLFWwindow* wind, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_keys[key] = true;
	if (action == GLFW_RELEASE)
		m_keys[key] = false;
}

void Input::IntCurPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	//m_mdx = ((float)xpos - m_lmx) * m_mSensitivity;
	//m_mdy = (m_lmy - (float)ypos) * m_mSensitivity;
	//m_lmx = (float)xpos;
	//m_lmy = (float)ypos;
}

void Input::IntMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_buts[button] = true;
	if (action == GLFW_RELEASE)
		m_buts[button] = false;
}

void Input::KeyCallback(GLFWwindow* wind, int key, int scancode, int action, int mods)
{
	Instance().IntKeyCallback(wind, key, scancode, action, mods);
}

void Input::CurPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Instance().IntCurPosCallback(window, xpos, ypos);
}

void Input::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	Instance().IntMouseCallback(window, button, action, mods);
}