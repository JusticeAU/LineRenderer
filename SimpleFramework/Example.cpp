#include "Example.h"
#include "imgui.h"

Example::Example()
{
	//Your initialisation code goes here!
}

void Example::Update(float delta)
{

	//Your physics (or whatever) code goes here!


	//If you want to use ImGUI to do UI stuff, you can do it here. Check out this function to see what's possible.
	//ImGui::ShowDemoWindow();



	//Example code that draws a coloured circle at the mouse position, whose colour depends on which buttons are down.
	if (leftMouseDown)
	{
		totalTime -= delta;
		lines->DrawCircle(cursorPos, 0.2f, { 1, 0, 0 });
	}
	else if (rightMouseDown)
	{
		totalTime += delta;
		lines->DrawCircle(cursorPos + Vec2(1, 1), 0.2f, {0, 1, 0});
	}
	else
	{
		lines->DrawCircle(cursorPos, 0.2f, { 0, 0, 1 });
	}

	//Your drawing code goes here!
	Vec2 position = Vec2(3, 3);
	position.x = position.x * glm::sin(totalTime);
	position.y = position.y * glm::cos(totalTime);

	lines->DrawCircle(cursorPos + position, 0.5f);


}

void Example::OnLeftClick()
{
	//You can do something here if you like.
}
