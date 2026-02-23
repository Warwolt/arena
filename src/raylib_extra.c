#include "raylib_extra.h"

#include <raymath.h>
#include <rlgl.h>

void DrawTextureRecSheared(Texture2D texture, Rectangle source, Rectangle dest, float angle, Color tint) {
	if (texture.id <= 0) {
		return;
	}

	Vector2 position = (Vector2) { dest.x, dest.y };
	float width = (float)texture.width;
	float height = (float)texture.height;

	bool flipX = false;

	if (source.width < 0) {
		flipX = true;
		source.width *= -1;
	}
	if (source.height < 0)
		source.y -= source.height;

	if (dest.width < 0)
		dest.width *= -1;
	if (dest.height < 0)
		dest.height *= -1;

	float tanAngle = tanf(DEG2RAD * angle);

	// coordinates relative to top left of quad
	Vector2 localTopLeft = { 0.0f, 0.0f };
	Vector2 localTopRight = { dest.width, 0.0f };
	Vector2 localBottomLeft = { 0.0f, dest.height };
	Vector2 localBottomRight = { dest.width, dest.height };

	// world space coordinates of sheared quad
	Vector2 shearedTopLeft = {
		position.x + localTopLeft.x + tanAngle * localTopLeft.y,
		position.y + localTopLeft.y,
	};
	Vector2 shearedTopRight = {
		position.x + localTopRight.x + tanAngle * localTopRight.y,
		position.y + localTopRight.y,
	};
	Vector2 shearedBottomLeft = {
		position.x + localBottomLeft.x + tanAngle * localBottomLeft.y,
		position.y + localBottomLeft.y,
	};
	Vector2 shearedBottomRight = {
		position.x + localBottomRight.x + tanAngle * localBottomRight.y,
		position.y + localBottomRight.y,
	};

	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);

	rlColor4ub(tint.r, tint.g, tint.b, tint.a);
	rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

	// Top-left corner for texture and quad
	if (flipX)
		rlTexCoord2f((source.x + source.width) / width, source.y / height);
	else
		rlTexCoord2f(source.x / width, source.y / height);
	rlVertex2f(shearedTopLeft.x, shearedTopLeft.y);

	// Bottom-left corner for texture and quad
	if (flipX)
		rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
	else
		rlTexCoord2f(source.x / width, (source.y + source.height) / height);
	rlVertex2f(shearedBottomLeft.x, shearedBottomLeft.y);

	// Bottom-right corner for texture and quad
	if (flipX)
		rlTexCoord2f(source.x / width, (source.y + source.height) / height);
	else
		rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
	rlVertex2f(shearedBottomRight.x, shearedBottomRight.y);

	// Top-right corner for texture and quad
	if (flipX)
		rlTexCoord2f(source.x / width, source.y / height);
	else
		rlTexCoord2f((source.x + source.width) / width, source.y / height);
	rlVertex2f(shearedTopRight.x, shearedTopRight.y);

	rlEnd();
	rlSetTexture(0);
}
