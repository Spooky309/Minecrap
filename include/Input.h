#pragma once
#include "OpenGLW.h"
class Input
{
public:
	Input() = default;
	~Input() = default;
	friend class Engine;
	bool GetKey(const int& key);
	bool GetKeyDown(const int& key);
	bool GetKeyUp(const int& key);
	bool GetMouseButton(const int& button);
	bool GetMouseButtonDown(const int& button);
	bool GetMouseButtonUp(const int& button);
	float GetMouseDeltaX();
	float GetMouseDeltaY();
private:
	static void KeyCallback(GLFWwindow* wind, int key, int scancode, int action, int mods);
	static void CurPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	void IntKeyCallback(GLFWwindow* wind, int key, int scancode, int action, int mods);
	void IntCurPosCallback(GLFWwindow* window, double xpos, double ypos);
	void IntMouseCallback(GLFWwindow* window, int button, int action, int mods);
	bool m_keys[1024];
	bool m_lkeys[1024];
	bool m_buts[32];
	bool m_lbuts[32];
	float m_lmx;
	float m_lmy;
	float m_mdx;
	float m_mdy;
	float m_mSensitivity = 0.25f;
	void Init(GLFWwindow* wind);
	void Update(GLFWwindow* wind);
	GLFWwindow* win;
	Input(const Input&) = delete;
};