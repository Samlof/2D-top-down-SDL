#include "Graphics\Sprite.h"

#include <cstddef>

#include <GL\glew.h>

#include "Graphics\Graphics.h"
#include "Graphics\Vertex.h"

#include "Rectangle.h"
#include "units.h"
#include "ResourceManager.h"

namespace Graphics {
	Sprite::Sprite(const std::string & pFilepath,
		const float pX, const float pY, const float pWidth, const float pHeight)
		:
		mX(pX), mY(pY), mWidth(pWidth), mHeight(pHeight), mVboId(0),
		mTexture(ResourceManager::getTexture(pFilepath))
	{
		init();
	}

	Sprite::~Sprite()
	{
		if (mVboId != 0) {
			glDeleteBuffers(1, &mVboId);
			mVboId = 0;
		}
	}

	void Sprite::init()
	{
		if (mVboId == 0) {
			glGenBuffers(1, &mVboId);
		}

		Vertex vertexData[6];
		//First Triangle
		vertexData[0].setPosition(mX + mWidth, mY + mHeight);
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPosition(mX, mY + mHeight);
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPosition(mX, mY);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second Triangle
		vertexData[3].setPosition(mX, mY);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(mX + mWidth, mY);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(mX + mWidth, mY + mHeight);
		vertexData[5].setUV(1.0f, 1.0f);

		for (size_t i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 255, 255, 255);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mVboId);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw(const int pX, const int pY)
	{
		glBindTexture(GL_TEXTURE_2D, mTexture.id);

		glBindBuffer(GL_ARRAY_BUFFER, mVboId);
		glEnableVertexAttribArray(0);

		// Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw(const int pX, const int pY, const Rectangle & pSourceRect)
	{
		/*
		Graphics::renderTexture(
			mSpriteSheet,
			pSourceRect,
			Rectangle(pX, pY, units::kTileSize, units::kTileSize)
		);
		*/
	}
}