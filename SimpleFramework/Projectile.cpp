#include "Projectile.h"
#include "imgui.h"

Projectile::Projectile()
{
	//Your initialisation code goes here!
}

void Projectile::Update(float delta)
{

	//Your physics (or whatever) code goes here!


	//If you want to use ImGUI to do UI stuff, you can do it here. Check out this function to see what's possible.
	//ImGui::ShowDemoWindow();



	//Example code that draws a coloured circle at the mouse position, whose colour depends on which buttons are down.
	if (leftMouseDown && !down)
	{
		leftDownPos = cursorPos;
		down = true;
	}

	if (!leftMouseDown && down)
	{
		down = false;
		Vec2 launch = leftDownPos - cursorPos;
		launch *= glm::length(launch);
		pos = leftDownPos;
		vel = launch;
	}


	vel += acc * delta;
	pos += vel * delta;

	//Your drawing code goes here!
	if (down)
	{
		lines->DrawLineSegment(leftDownPos, cursorPos);
		lines->DrawCircle(cursorPos, (glm::length(leftDownPos - cursorPos)* glm::length(leftDownPos - cursorPos)) * 0.1f);
	}
	lines->DrawCircle(pos, 0.5f);


}

void Projectile::OnLeftClick()
{
	//You can do something here if you like.
}