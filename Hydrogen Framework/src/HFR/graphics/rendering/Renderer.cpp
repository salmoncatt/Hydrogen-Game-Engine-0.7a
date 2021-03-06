#include "hfpch.h"

#include "HFR/text/Font.h"
#include "HFR/gui/GuiText.h"
#include HFR_RENDERER2D

namespace HFR {

	Mat4f Renderer::perspectiveMatrix = Mat4f();
	Mat4f Renderer::viewMatrix = Mat4f();

	Camera Renderer::camera = Camera();
	
	Light Renderer::light = Light();

	Shader Renderer::mainShader = HFR::Shader(HFR_RES + "shaders/", "MainVertex.glsl", "MainFragment.glsl");

	Texture Renderer::nullTexture = Texture(HFR_RES + "textures/null.png");
	MeshComponent Renderer::lightObject = MeshComponent();


	bool Renderer::wireframe = false;
	bool Renderer::cull = false;

	Vec2f Renderer::currentWindowSize = Vec2f();

	void Renderer::init(const float& screenWidth, const float& screenHeight) {
		//show opengl stuff
		Debug::newLine();

		std::string vendor((const char*)glGetString(GL_VENDOR));
		std::string version((const char*)glGetString(GL_VERSION));
		std::string gpu((const char*)glGetString(GL_RENDERER));

		Debug::systemLog("OpenGL Vendor: " + vendor, DebugColor::Aqua);
		Debug::systemLog("OpenGL Version: " + version, DebugColor::Aqua);

		Debug::newLine();

		Debug::systemLog("GPU: " + gpu, DebugColor::Aqua);
		Debug::systemLog("CPU: " + Util::getCPU(), DebugColor::Aqua);

		Debug::newLine();

		enableAlphaBlending();

		Debug::systemLog("Creating engine shaders");

		mainShader.create();

		nullTexture.create();
		Debug::setSystemLogMode(HFR_LOG_ON_SUCCESS, HFR_DONT_LOG_ON_FAIL);
		lightObject.meshes = Util::loadMesh(HFR_RES + "models/cube.obj");
		lightObject.meshes[0].create();
		lightObject.clearAllTextureCoordinates();
		lightObject.meshes[0].useLighting = false;
		lightObject.meshes[0].material.diffuseColor = Vec3f(1);

		Debug::setSystemLogMode(HFR_LOG_ON_SUCCESS, HFR_LOG_ON_FAIL);

		createProjectionMatrix(screenWidth, screenHeight);

		Debug::systemSuccess("Created engine shaders");

		Debug::newLine();
	}

	void Renderer::createProjectionMatrix(const float& screenWidth, const float& screenHeight) {
		perspectiveMatrix = Mat4f::createPerspectiveMatrix(screenWidth, screenHeight, 90, 0.1f, 1000);

		currentWindowSize.x = screenWidth;
		currentWindowSize.y = screenHeight;
		
		glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);
	}

	void Renderer::close() {
		mainShader.close();
		nullTexture.destroy();
	}

	float Renderer::getAspectRatio() {
		return currentWindowSize.x / currentWindowSize.y;
	}
	
	Vec2f Renderer::getCurrentWindowSize() {
		return currentWindowSize;
	}

	void Renderer::setCamera(const Camera& _camera) {
		viewMatrix = Mat4f::createViewMatrix(_camera.position, _camera.rotation);
		camera = _camera;
	}

	void Renderer::setWireFrameMode(const bool& in) {
		wireframe = in;
		glPolygonMode(GL_FRONT_AND_BACK, in ? GL_LINE : GL_FILL);
	}

	bool Renderer::getWireFrameMode() {
		return wireframe;
	}

	void Renderer::toggleWireFrameMode() {
		setWireFrameMode(!wireframe);
	}

	void Renderer::setCullingMode(const bool& in) {
		cull = in;
		if (cull) {
			glEnable(GL_BACK);
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_BACK);
			glDisable(GL_CULL_FACE);
		}
	}

	bool Renderer::getCullingMode() {
		return cull;
	}

	void Renderer::toggleCullingMode() {
		setWireFrameMode(!cull);
	}

	void Renderer::enableAlphaBlending() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::disableAlphaBlending() {
		glDisable(GL_BLEND);
		//enableDepthTest();
	}

	void Renderer::enableDepthTest() {
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::disableDepthTest() {
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::render(Mesh& mesh, const Vec3f& position, const Vec3f& rotation, const Vec3f& scale, const Texture& texture) {
		render(mesh, mainShader, position, rotation, scale, texture);
	}

	void Renderer::render(Mesh& mesh, const Shader& shader, const Transform& transform, const Texture& texture) {
		render(mesh, shader, transform.position, transform.rotation, transform.scale, texture);
	}

	void Renderer::render(Mesh& mesh, const Transform& transform, const Texture& texture) {
		render(mesh, mainShader, transform.position, transform.rotation, transform.scale, texture);
	}

	void Renderer::loadLight(const Light& _light) {
		light = _light;
	}

	//position and size in pixels
	//void Renderer::renderRectangle(const Vec2f& position, const Vec2f& size, const Texture& texture) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiShader.bind();
	//	glActiveTexture(GL_TEXTURE0);

	//	if (texture.image.hasData())
	//		glBindTexture(GL_TEXTURE_2D, texture.textureID);
	//	else
	//		glBindTexture(GL_TEXTURE_2D, nullTexture.textureID);

	//	//matrix stuff
	//	Mat4f transform = Mat4f::createTransformationMatrix(
	//		Vec2f((((position.x + (size.x / 2)) / currentWindowSize.x) * 2) - 1, (((currentWindowSize.y - position.y - (size.y / 2)) / currentWindowSize.y) * 2) - 1),
	//		Vec3f(),
	//		Vec2f(size.x / currentWindowSize.x, size.y / currentWindowSize.y));

	//	guiShader.setUniform("transform", transform);
	//	guiShader.setUniform("ortho", orthoMatrix);
	//	guiShader.setUniform("hasTexture", true);
	//	guiShader.setUniform("uiPosition", position);
	//	guiShader.setUniform("uiWidth", size.x / currentWindowSize.x);
	//	guiShader.setUniform("uiHeight", size.y / currentWindowSize.y);
	//	guiShader.setUniform("drawmode", HFR_RECTANGLE);


	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	////position and size in pixels
	//void Renderer::renderRectangle(const Vec2f& position, const Vec2f& size, const Vec3f& color) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiShader.bind();

	//	//matrix stuff
	//	Mat4f transform = Mat4f::createTransformationMatrix(
	//		Vec2f((((position.x + (size.x / 2)) / currentWindowSize.x) * 2) - 1, (((currentWindowSize.y - position.y - (size.y / 2)) / currentWindowSize.y) * 2) - 1),
	//		Vec3f(), 
	//		Vec2f(size.x / currentWindowSize.x, size.y / currentWindowSize.y));

	//	guiShader.setUniform("transform", transform);
	//	guiShader.setUniform("ortho", orthoMatrix);
	//	guiShader.setUniform("hasTexture", false);
	//	guiShader.setUniform("color", color);
	//	guiShader.setUniform("uiPosition", position);
	//	guiShader.setUniform("uiWidth", size.x / currentWindowSize.x);
	//	guiShader.setUniform("uiHeight", size.y / currentWindowSize.y);
	//	guiShader.setUniform("drawmode", HFR_RECTANGLE);


	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	//void Renderer::renderRoundedRectangle(const Vec2f& position, const Vec2f& size, const float& radius, const Texture& texture) {

	//}

	//void Renderer::renderRoundedRectangle(const Vec2f& position, const Vec2f& size, const float& radius, const Vec3f& color) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiShader.bind();

	//	Vec2f uiposition = Vec2f((((position.x + (size.x / 2)) / currentWindowSize.x) * 2) - 1, (((currentWindowSize.y - position.y - (size.y / 2)) / currentWindowSize.y) * 2) - 1);
	//	Vec2f uiSize = Vec2f(size.x / currentWindowSize.x, size.y / currentWindowSize.y);

	//	Mat4f rotationMat = Mat4f::createTransformationMatrix(Vec2f(), Vec3f(0, 0, 0), Vec2f());

	//	//matrix stuff
	//	Mat4f transform = Mat4f::createTransformationMatrix(
	//		uiposition,
	//		Vec3f(0, 0, 0),
	//		uiSize);

	//	guiShader.setUniform("transform", transform);
	//	guiShader.setUniform("rotationMat", rotationMat);
	//	guiShader.setUniform("ortho", orthoMatrix);
	//	guiShader.setUniform("uiSize", size);
	//	guiShader.setUniform("hasTexture", false);
	//	guiShader.setUniform("color", color);
	//	guiShader.setUniform("radius", radius);
	//	guiShader.setUniform("smoothness", 0.7f);
	//	guiShader.setUniform("drawmode", HFR_RECTANGLE_ROUNDED);


	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	//void Renderer::renderRectangle(const Vec2f& position, const Vec2f& size, const float& rotation, const Vec3f& color) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiShader.bind();

	//	Vec2f uiPosition = Vec2f(position.x - (size.x / 2), -position.y);

	//	//Vec2f uiSize = Vec2f(size.x / x, size.y / y);

	//	//matrix stuff
	//	Mat4f transform = orthoMatrix * Mat4f::createTransformationMatrix(
	//		position,
	//		Vec3f(0, 0, rotation),
	//		size);

	//	guiShader.setUniform("transform", transform);
	//	guiShader.setUniform("uiSize", Vec2f(size.x * currentWindowSize.x, size.y * currentWindowSize.y));
	//	guiShader.setUniform("hasTexture", false);
	//	guiShader.setUniform("color", color);
	//	//guiShader.setUniform("radius", radius);
	//	guiShader.setUniform("smoothness", 0.7f);
	//	guiShader.setUniform("drawmode", HFR_RECTANGLE);


	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	////position and size in pixels, angle in degrees, also draws from middle
	//void Renderer::radialRevealRectangle(const Vec2f& position, const Vec2f& size, const float& angle, const float& offset, const bool& flipped, const Texture& texture) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiShader.bind();

	//	glActiveTexture(GL_TEXTURE0);

	//	if (texture.image.hasData())
	//		glBindTexture(GL_TEXTURE_2D, texture.textureID);
	//	else
	//		glBindTexture(GL_TEXTURE_2D, nullTexture.textureID);

	//	Mat4f rotationMat = Mat4f::createTransformationMatrix(Vec2f(), Vec3f(0, 0, 0), Vec2f());

	//	//matrix stuff
	//	Mat4f transform = Mat4f::createTransformationMatrix(
	//		Vec2f((((position.x + (size.x / 2)) / currentWindowSize.x) * 2) - 1, (((currentWindowSize.y - position.y - (size.y / 2)) / currentWindowSize.y) * 2) - 1),
	//		Vec3f(0, 0, offset),
	//		Vec2f(size.x / currentWindowSize.x, size.y / currentWindowSize.y));
	//	
	//	guiShader.setUniform("transform", transform);
	//	guiShader.setUniform("rotationMat", rotationMat);
	//	guiShader.setUniform("ortho", orthoMatrix);
	//	guiShader.setUniform("hasTexture", true);
	//	guiShader.setUniform("angle", (float)HMath::toRadians(angle));
	//	guiShader.setUniform("flipped", flipped);
	//	guiShader.setUniform("drawmode", HFR_RADIAL_REVEAL_RENDER);

	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	void Renderer::render(Mesh& mesh, const Shader& shader, const Vec3f& position, const Vec3f& rotation, const Vec3f& scale, const Texture& texture) {
		glBindVertexArray(mesh.VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);

		shader.bind();
		//Debug::log("test");
		//mesh.material.bindLightMaps(shader);
		
		shader.bindLightMaps(mesh.material);

		//glActiveTexture(GL_TEXTURE0);

		/*if (mesh.material.diffuseMap.image.hasData()) {
			shader.setUniform("material.diffuseMap", (int)0);
			shader.setUniform("material.useDiffuseMap", true);
			glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseMap.textureID);
		}
		else
			glBindTexture(GL_TEXTURE_2D, nullTexture.textureID);*/

		//matrix stuff
		Mat4f transform = Mat4f::createTransformationMatrix(position, rotation, scale);
		Mat4f projectionViewTransform = perspectiveMatrix * viewMatrix * transform;
		//Mat4f projectionTransform = perspectiveMatrix * transform;

		shader.setUniform("transformMatrix", transform);
		shader.setUniform("projectionViewTransformMatrix", projectionViewTransform);
		shader.setUniform("hasTextureCoords", !mesh.texturecoords.empty());

		//light things
		//shader.setUniform("lightMode", light.usePerPixelLighting);
		shader.setUniform("useLighting", mesh.useLighting);
		shader.setUniform("cameraPosition", camera.position);

		//light parameters
		shader.setUniform("light.position", light.position);
		shader.setUniform("light.diffuseColor", light.diffuseColor);
		shader.setUniform("light.ambientColor", light.ambientColor);
		shader.setUniform("light.specularColor", light.specularColor);

		//stupid cast size_t to GLsizei warning
		if (!mesh.indices.empty())
			glDrawElements(GL_TRIANGLES, (int)mesh.indices.size(), GL_UNSIGNED_INT, 0);
		else if(!mesh.vertices.empty())
			glDrawArrays(GL_TRIANGLES, 0, (int)mesh.vertices.size() / mesh.type);

		shader.unbind();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	//void Renderer::render(const GuiFrame& frame) {
	//	disableDepthTest();

	//	glBindVertexArray(quad.VAO);
	//	glEnableVertexAttribArray(0);

	//	guiFrameShader.bind();

	//	Vec2f uiPosition;

	//	Mat4f transform;
	//	Mat4f outlineTransform;

	//	//matrix stuff
	//	if (frame.sizeType == HFR_SCREEN_SPACE_SIZE) {

	//		uiPosition = Vec2f(frame.position.x * 2 * Renderer::getAspectRatio(), -frame.position.y * 2) + Vec2f(frame.size.x - frame.size.x * frame.anchorPoint.x * 2, -frame.size.y + frame.size.y * frame.anchorPoint.y * 2);

	//		transform = orthoMatrix * Mat4f::createTransformationMatrix(uiPosition, Vec3f(0, 0, frame.rotation), frame.size);

	//		//because dank aspect ratio madness fuck me
	//		if (currentWindowSize.x < currentWindowSize.y)
	//			guiFrameShader.setUniform("uiSize", Vec2f(frame.size.x * currentWindowSize.x, frame.size.y * currentWindowSize.x));
	//		else
	//			guiFrameShader.setUniform("uiSize", Vec2f(frame.size.x * currentWindowSize.y, frame.size.y * currentWindowSize.y));

	//	}
	//	else if (frame.sizeType == HFR_PIXEL_SIZE) {

	//		transform = pixelOrthoMatrix * Mat4f::createTransformationMatrix(Vec2f(frame.position.x * 2, -frame.position.y * 2) + Vec2f(frame.size.x - frame.size.x * frame.anchorPoint.x * 2, -frame.size.y + frame.size.y * frame.anchorPoint.y * 2), Vec3f(0, 0, frame.rotation), frame.size);
	//		
	//		guiFrameShader.setUniform("uiSize", frame.size);
	//	}

	//	guiFrameShader.setUniform("transform", transform);
	//	guiFrameShader.setUniform("color", frame.backgroundColor);
	//	guiFrameShader.setUniform("borderColor", frame.borderColor);
	//	guiFrameShader.setUniform("borderSize", (float)frame.borderSize);
	//	guiFrameShader.setUniform("cornerRadius", (float)frame.roundedCornerRadius);
	//	guiFrameShader.setUniform("aspectRatio", getAspectRatio());


	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)quad.vertices.size());

	//	guiFrameShader.unbind();

	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	enableDepthTest();
	//}

	//void Renderer::renderGuis() {
	//	for (size_t i = 0; i < Engine::guiFrames.size(); ++i) {
	//		auto& frame = Engine::guiFrames[i];
	//		if (frame->active && frame->draggable && frame->isSelected())
	//			frame->position += frame->sizeType == HFR_PIXEL_SIZE ? Vec2f(Input::getMouseMovement().x / 2, Input::getMouseMovement().y / 2) : Vec2f(Input::getMouseMovementScreenSpace().x / 2, Input::getMouseMovementScreenSpace().y / 2);

	//		if (frame->visible)
	//			Renderer2D::render(*frame);
	//	}
	//}

	//void Renderer::render(const GuiText& text) {
	//	disableDepthTest();
	//	enableAlphaBlending();

	//	glBindVertexArray(text.mesh.VAO);
	//	glEnableVertexAttribArray(0);
	//	glEnableVertexAttribArray(1);

	//	textShader.bind();

	//	glActiveTexture(GL_TEXTURE0);

	//	/*if (text.mesh.material.albedoTexture.image.hasData())
	//		glBindTexture(GL_TEXTURE_2D, text.mesh.material.albedoTexture.textureID);
	//	else
	//		glBindTexture(GL_TEXTURE_2D, nullTexture.textureID);*/

	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, text.font.texture.textureID);


	//	textShader.setUniform("color", text.color);

	//	if (!text.mesh.vertices.empty())
	//		glDrawArrays(GL_TRIANGLES, 0, (int)text.mesh.vertices.size() / text.mesh.type);
	///*	else
	//		Debug::log("aweg");*/

	//	
	//	glDisableVertexAttribArray(0);
	//	glDisableVertexAttribArray(1);
	//	glBindVertexArray(0);

	//	textShader.unbind();

	//	disableAlphaBlending();
	//	enableDepthTest();
	//}

	void Renderer::update() {
		lightObject.meshes[0].material.diffuseColor = light.diffuseColor;
		render(lightObject.meshes[0], Transform(light.position, Vec3f(), Vec3f(1, 1, 1)), Texture());
	}

}
