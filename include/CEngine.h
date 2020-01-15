#pragma once

#include <CCommon.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <utils/CImguiUi.h>
#include <utils/CImguiUiDemo.h>
#include <utils/CNoiseGenerator.h>
#include <utils/CDebugDrawer.h>
#include <utils/CObjectPicker.h>

#include <gui/CImGuiManager.h>
#include <gui/CImGuiLayer.h>
#include <gui/CImGuiSceneLayer.h>
#include <gui/CImGuiUtilsLayer.h>

#include <input/CInputCallbacks.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputManager.h>

#include <core/CVertexBuffer.h>
#include <core/CVertexBufferLayout.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>
#include <core/CTexture.h>
#include <core/CTextureCube.h>
#include <core/CFrameBuffer.h>

#include <graphics/CShadowMap.h>
#include <graphics/CFog.h>
#include <graphics/CSkybox.h>
#include <graphics/CMeshBuilder.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <graphics/CScene.h>

#include <materials/CMaterial.h>

#include <shaders/CShaderUniforms.h>
#include <shaders/CShader.h>
#include <shaders/CShaderManager.h>

#include <camera/CICamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>

#include <renderers/CMainRenderer.h>
#include <renderers/CMeshRenderer.h>
#include <renderers/CSkyboxRenderer.h>

#include <assets/CTextureManager.h>

#include <gl/COpenGLWindow.h>
#include <gl/COpenGLContext.h>

#include <app/CApplication.h>


