# relative paths to lib p3d and media
P3D_PATH = p3d
MEDIA_PATH = ../media

# common QT project configuration
QT += widgets opengl
CONFIG += debug_and_release
CONFIG += c++17
CONFIG(debug,debug|release) {
  TARGET=$$join(TARGET,,,"_d")
}
TEMPLATE = app

DESTDIR = $$_PRO_FILE_PWD_/bin

DEFINES+=GLEW_NO_GLU
DEFINES += MEDIA_PATH=\\\"$$MEDIA_PATH\\\"
DEFINES+= RENDER_SHADER_PATH=\\\"$$MEDIA_PATH/p3dShader\\\"



# mac os config : ** not tested **
macx {
    INCLUDEPATH += /opt/local/include/
    LIBS += -framework OpenGL
}



# win32 config : tested on win10 64bits (qt mingw)
win32 {
  CONFIG += console
  DEFINES+=GLEW_STATIC _STDCALL_SUPPORTED
  LIBS+= -lopengl32
}


INCLUDEPATH += src/p3d src/p3d/GL/ src/application/ src/p3d/algebra/ src/p3d/gui/ src/p3d/render/ src/p3d/scene/ 
DEPENDPATH += src/p3d/GL/ src/application/ src/p3d/algebra/ src/p3d/gui/ src/p3d/render/ src/p3d/scene/ 

SOURCES += \
	src/p3d/GL/glew.c \
	src/application/GLApplication.cpp \
	src/p3d/algebra/Matrix3.cpp \
	src/p3d/algebra/Vector3.cpp \
	src/p3d/algebra/Vector2.cpp \
	src/p3d/algebra/p3helpers.cpp \
	src/p3d/algebra/Matrix4.cpp \
	src/p3d/algebra/Quaternion.cpp \
	src/p3d/algebra/Vector4.cpp \
	src/p3d/algebra/AngleAxis.cpp \
	src/p3d/gui/Tools.cpp \
	src/p3d/render/GLRender.cpp \
	src/p3d/scene/Camera.cpp \
	src/p3d/scene/Object3D.cpp \
	src/p3d/algebra/Line.cpp \
	src/p3d/algebra/Plane.cpp \
	src/p3d/render/Shader.cpp \
	src/p3d/render/DebugTool.cpp \
	src/p3d/gui/GLText.cpp \
	src/p3d/render/GLDraw.cpp \
	src/p3d/render/VertexArray.cpp \
	src/p3d/render/GLTool.cpp \
	src/p3d/render/Texture.cpp \
	src/p3d/render/GLDrawCamera.cpp \
	src/p3d/render/GLPrimitive.cpp \
	src/application/DrawScene.cpp \
	src/p3d/render/FrameBuffer.cpp \
	src/p3d/scene/MeshObject3D.cpp \
	src/p3d/scene/MeshGL.cpp \
	src/p3d/scene/Mesh.cpp \
	src/p3d/gui/GLWidget.cpp \
	src/p3d/gui/main.cpp \
	src/p3d/gui/MainWindow.cpp
HEADERS += \
	src/p3d/GL/eglew.h \
	src/p3d/GL/glxew.h \
	src/p3d/GL/wglew.h \
	src/p3d/GL/glew.h \
	src/p3d/algebra/p3helpers.h \
	src/p3d/algebra/Vector2.h \
	src/p3d/algebra/Vector3.h \
	src/p3d/algebra/Quaternion.h \
	src/p3d/algebra/Vector4.h \
	src/p3d/algebra/AngleAxis.h \
	src/p3d/algebra/Matrix4.h \
	src/p3d/gui/Tools.h \
	src/p3d/gui/glsupport.h \
	src/p3d/algebra/Matrix3.h \
	src/p3d/scene/Camera.h \
	src/p3d/scene/Object3D.h \
	src/p3d/algebra/Plane.h \
	src/p3d/algebra/Line.h \
	src/p3d/render/Shader.h \
	src/p3d/render/GLRender.h \
	src/p3d/gui/GLText.h \
	src/p3d/render/VertexArray.h \
	src/p3d/render/GLDraw.h \
	src/p3d/render/DebugTool.h \
	src/p3d/render/Texture.h \
	src/p3d/render/GLDrawCamera.h \
	src/p3d/render/GLTool.h \
	src/p3d/render/GLPrimitive.h \
	src/application/GLApplication.h \
	src/application/DrawScene.h \
	src/p3d/render/FrameBuffer.h \
	src/p3d/scene/MeshObject3D.h \
	src/p3d/scene/MeshGL.h \
	src/p3d/scene/Mesh.h \
	src/p3d/gui/GLWidget.h \
	src/p3d/gui/MainWindow.h
OTHER_FILES +=\
	media/mosaic_pierre.jpg\
	media/al.obj\
	media/EarthDay512.jpg\
	media/shader/ambientTexture.frag\
	media/shader/perFragmentLighting.frag\
	media/shader/perVertexLighting.frag\
	media/shader/shadowMap.frag\
	media/shader/textureTransform.frag\
	media/shader/ambientTexture.vert\
	media/shader/perFragmentLighting.vert\
	media/shader/perVertexLighting.vert\
	media/shader/shadowMap.vert\
	media/shader/textureTransform.vert\
	media/porsche.obj\
	media/cessna.obj\
	media/Tranquil_Lagoon.jpg\
	media/shader/constantColor.frag\
	media/shader/constantColor.vert\
	media/Tyrol.jpg\
	media/lightmap.png
