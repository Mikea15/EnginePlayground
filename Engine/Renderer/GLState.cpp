#include "GLState.h"


void GLState::SetDepthTest(bool enable)
{
	if (m_depthTest != enable)
	{
		m_depthTest = enable;
		ToggleState(GL_DEPTH_TEST, enable);
	}

}

void GLState::SetDepthFunc(GLenum depthFunc)
{
	if (m_depthFunc != depthFunc)
	{
		m_depthFunc = depthFunc;
		glDepthFunc(depthFunc);
	}
}

void GLState::SetBlend(bool enable)
{
	if (m_blend != enable)
	{
		m_blend = enable;
		ToggleState(GL_BLEND, enable);
	}
}

void GLState::SetBlendFunc(GLenum src, GLenum dst)
{
	if (m_blendSrc != src || m_blendDst != dst)
	{
		m_blendSrc = src;
		m_blendDst = dst;
		glBlendFunc(src, dst);
	}
}

void GLState::SetCull(bool enable)
{
	if (m_cullFace != enable)
	{
		m_cullFace = enable;
		ToggleState(GL_CULL_FACE, enable);
	}
}

void GLState::SetCullFace(GLenum face)
{
	if (m_frontFace != face)
	{
		m_frontFace = face;
		glCullFace(face);
	}
}

void GLState::SetPolygonMode(GLenum mode)
{
	if (m_polygonMode != mode)
	{
		m_polygonMode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

void GLState::SwitchShader(unsigned int ID)
{
	if (m_activeShaderID != ID)
	{
		glUseProgram(ID);
	}
}

void GLState::PushState(GLenum state)
{
	m_stateStack.push(state);
	ToggleState(state, true);
}

void GLState::PopState()
{
	GLenum state = m_stateStack.top();
	m_stateStack.pop();

	ToggleState(state, false);
}

void GLState::ToggleState(GLenum state, bool enable)
{
	if (enable)
	{
		glEnable(state);
	}
	else
	{
		glDisable(state);
	}
}

